#include "graph_agent.hpp"
#include <sc-memory/sc_memory_headers.hpp>
#include "keynodes/keynodes.hpp"
#include "utils/utils.hpp"
#include <string>

#define DEFAULT_PAGE_NAME "page_"

ScAgentCreateGraph::ScAgentCreateGraph(){
    m_logger = utils::ScLogger(utils::ScLogger::ScLogType::File, "logs/ScAgentCreateGraph.log", utils::ScLogLevel::Debug);
}

void ScAgentCreateGraph::GetOrCreatePage(ScAddr & pageNode, const std::string& pageName){
    if (this->pagesIdentificators_.find(pageName) == this->pagesIdentificators_.end()){
        pageNode = m_context.GenerateNode(ScType::ConstNode);
        std::string pageIdentificator = DEFAULT_PAGE_NAME + std::to_string(this->next_page);
        m_context.SetElementSystemIdentifier(pageIdentificator, pageNode);
        CreateMainIdntf(pageNode, pageName);
        ScAddr const & pageClassConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::concept_page, pageNode);
        this->pagesIdentificators_[pageName] = pageIdentificator;
        this->next_page++;
    }
    else{
        std::string pageIdentificator = this->pagesIdentificators_[pageName];
        pageNode = m_context.SearchElementBySystemIdentifier(pageIdentificator);
    }
}

void ScAgentCreateGraph::CreateMainIdntf(ScAddr const & node, const std::string& idtf){
    ScAddr const & mainIdenificator = m_context.GenerateLink();
    m_context.SetLinkContent(mainIdenificator, idtf);
    ScAddr const & idtfConnector = m_context.GenerateConnector(ScType::ConstCommonArc, node, mainIdenificator);
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScKeynodes::nrel_main_idtf, idtfConnector);
}

ScAddr ScAgentCreateGraph::GetActionClass() const{
    return ScGraphKeynodes::action_create_graph;
}

ScResult ScAgentCreateGraph::DoProgram(ScActionInitiatedEvent const & event, ScAction& action){
    std::string file_data;
    m_logger.Debug("Start action");
    ScIterator5Ptr const it5 = m_context.CreateIterator5(
        action,
        ScType::ConstCommonArc,
        ScType::ConstNodeLink,
        ScType::ConstPermPosArc,
        ScGraphKeynodes::nrel_file_path);
    it5->Next();
    ScAddr const & elementAddr = it5->Get(2);
    m_logger.Debug("Find link");

    m_context.GetLinkContent(elementAddr, file_data);
    m_logger.Debug("Ready to create graph");
    
    std::vector<std::string> pagesData = Utils::splitData(file_data, "\n");
    for (auto& page : pagesData){
        m_logger.Debug(page);
        std::vector<std::string> pageData = Utils::splitData(page, ";");
        m_logger.Debug(pageData);
        std::string pageName = pageData[0];
        std::string visitorsCount = pageData[1];
        std::string spentTime = pageData[2];
        std::vector<std::string> links;
        if(pageData.size() == 4)
            links = Utils::splitData(pageData[3], ",");
            m_logger.Debug(links);

        ScAddr pageNode;
        GetOrCreatePage(pageNode, pageName);
        m_logger.Debug("Create page node");
        m_logger.Debug("Create page class connector");
        ScAddr const & visitorsNode = m_context.GenerateNode(ScType::ConstNode);
        CreateMainIdntf(visitorsNode, visitorsCount);
        m_logger.Debug("Create visitors node");
        ScAddr const & visitorsClassConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::concept_number, visitorsNode);
        m_logger.Debug("Create number connector(visitors)");
        ScAddr const & visitorsConnector = m_context.GenerateConnector(ScType::ConstCommonArc, pageNode, visitorsNode);
        ScAddr const & nrel_visitorsConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_visitors_count, visitorsConnector);
        m_logger.Debug("Create visitors connector");
        ScAddr const & spentTimeNode = m_context.GenerateNode(ScType::ConstNode);
        CreateMainIdntf(spentTimeNode, spentTime);
        m_logger.Debug("Create time node");
        ScAddr const & spentTimeClassConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::concept_number, spentTimeNode);
        m_logger.Debug("Create number connector(time)");
        ScAddr const & spentTimeConnector = m_context.GenerateConnector(ScType::ConstCommonArc, pageNode, spentTimeNode);
        ScAddr const & nrel_spent_timeConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_spent_time, spentTimeConnector);
        m_logger.Debug("Create time connector");


        if (!links.empty()){
            for(auto& link : links){
                ScAddr nextPageNode;
                GetOrCreatePage(nextPageNode, link);
                m_logger.Debug("Create next page");
                ScAddr const & linkConnector = m_context.GenerateConnector(ScType::ConstCommonArc, pageNode, nextPageNode);
                m_logger.Debug("Create link between current page and " + link);
                ScAddr const & nrel_linkConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_link, linkConnector);
            }
        }
    }
    
    return action.FinishSuccessfully();
}