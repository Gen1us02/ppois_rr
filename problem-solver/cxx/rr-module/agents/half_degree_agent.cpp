#include "half_degree_agent.hpp"
#include <sc-memory/sc_memory_headers.hpp>
#include "keynodes/keynodes.hpp"
#include <string>

ScHalfDegreeAgent::ScHalfDegreeAgent() {
    m_logger = utils::ScLogger(utils::ScLogger::ScLogType::File, "logs/ScHalfDegreeAgentLog.log", utils::ScLogLevel::Debug);
}

ScAddr ScHalfDegreeAgent::GetActionClass() const {
    return ScGraphKeynodes::action_calculate_half_degree;
}

ScAddr ScHalfDegreeAgent::GetEventSubscriptionElement() const {
    return ScGraphKeynodes::concept_successfuly_created_graph;
}

ScTemplate ScHalfDegreeAgent::GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const {
    ScTemplate templ;
    templ.Triple(
        ScGraphKeynodes::concept_successfuly_created_graph,
        ScType::VarPermPosArc,
        ScType::VarNode
    );

    return templ;
}

ScResult ScHalfDegreeAgent::DoProgram(SuccessfulyGraphCreationEvent const &event, ScAction& action) {
    m_logger.Debug("Started collecting pages.");
    GetPagesVector();
    m_logger.Debug("Collected pages successfuly.");
    
    for(const auto & page: pages_) {
        m_logger.Debug(m_context.GetElementSystemIdentifier(page));
        int inPages = 0, outPages = 0;

        ScIterator5Ptr const outPagesIterator = m_context.CreateIterator5(
            page,
            ScType::ConstCommonArc,
            ScType::ConstNode,
            ScType::ConstPermPosArc,
            ScGraphKeynodes::nrel_link
        );

        ScIterator5Ptr const inPagesIterator = m_context.CreateIterator5(
            ScType::ConstNode,
            ScType::ConstCommonArc,
            page,
            ScType::ConstPermPosArc,
            ScGraphKeynodes::nrel_link
        );

        while(inPagesIterator->Next()) {
            inPages++;
        }
        m_logger.Debug(inPages);

        while(outPagesIterator->Next()) {
            outPages++;
        }
        m_logger.Debug(outPages);

        CreateDegreeLinks(page, outPages, "out");
        CreateDegreeLinks(page, inPages, "in");

        if(outPages == 0) {
            CreateDeadEndLinks(page);
        }
    }

    return action.FinishSuccessfully();
}

void ScHalfDegreeAgent::CreateDeadEndLinks(ScAddr const & page) {
    ScAddr const & deadEndLink = m_context.GenerateLink();
    m_context.SetLinkContent(deadEndLink, "This page is a dead-end page which means it doesn't have any outgoing links to other pages.");
    ScAddr deadEndConnector = m_context.GenerateConnector(ScType::ConstCommonArc, page, deadEndLink);

    m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_dead_end, deadEndConnector);
    m_logger.Debug("Successfuly found dead end page.");
}

void ScHalfDegreeAgent::CreateDegreeLinks(ScAddr const & page, int number, const std::string &type) {
    ScAddr const & degreeLink = m_context.GenerateLink();
    m_context.SetLinkContent(degreeLink, std::to_string(number));
    ScAddr degreeConnector = m_context.GenerateConnector(ScType::ConstCommonArc, page, degreeLink);
    
    if (type == "in") {
        m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_in_degree, degreeConnector);
        m_logger.Debug("Successfuly create in-link.");
    }
    else if(type == "out") {
        m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_out_degree, degreeConnector);
        m_logger.Debug("Successfuly create out-link.");
    }
}

void ScHalfDegreeAgent::GetPagesVector() {
    ScIterator3Ptr const it3 = m_context.CreateIterator3(
        ScGraphKeynodes::concept_page,
        ScType::ConstPermPosArc,
        ScType::ConstNode);

    while (it3->Next()) {
        pages_.push_back(it3->Get(2));
    }
}