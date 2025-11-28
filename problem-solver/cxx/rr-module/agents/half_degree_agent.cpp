#include "half_degree_agent.hpp"
#include <sc-memory/sc_memory_headers.hpp>
#include "keynodes/keynodes.hpp"
#include <string>

ScHalfDegreeAgent::ScHalfDegreeAgent()
{
  m_logger =
      utils::ScLogger(utils::ScLogger::ScLogType::File, "logs/ScHalfDegreeAgentLog.log", utils::ScLogLevel::Debug);
}

ScAddr ScHalfDegreeAgent::GetActionClass() const
{
  return ScGraphKeynodes::action_calculate_half_degree;
}

ScAddr ScHalfDegreeAgent::GetEventSubscriptionElement() const
{
  return ScGraphKeynodes::concept_successfuly_created_graph;
}

ScTemplate ScHalfDegreeAgent::GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const
{
  ScTemplate templ;
  templ.Triple(ScGraphKeynodes::concept_successfuly_created_graph, ScType::VarPermPosArc, ScType::VarNode);

  return templ;
}

ScAddr ScHalfDegreeAgent::GetStructConnector(ScAddr const & page, ScStructure & graphStruct){
  ScTemplate findConnectorTempl;
  findConnectorTempl.Quintuple(
    graphStruct,
    ScType::VarPermPosArc,
    page,
    ScType::VarPermPosArc,
    ScType::VarNodeRole
  );
  ScTemplateSearchResult connectorRes;
  ScAddr rrelConnector;
  if (m_context.SearchByTemplate(findConnectorTempl, connectorRes)){
    rrelConnector = m_context.GenerateConnector(ScType::ConstPermPosArc, graphStruct, page);
  }
  else{
    ScIterator3Ptr it3 = m_context.CreateIterator3(
      graphStruct,
      ScType::ConstPermPosArc,
      page
    );
    it3->Next();
    rrelConnector = it3->Get(1);

  }

  return rrelConnector;
}

ScResult ScHalfDegreeAgent::DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action)
{
  m_logger.Debug("Started collecting pages.");
  GetPagesVector();
  m_logger.Debug("Collected pages successfuly.");
  ScAddr const & graphStructAddr = event.GetArcTargetElement();
  ScStructure graphStruct = m_context.ConvertToStructure(graphStructAddr);

  int sourceMax = 0, popularMax = 0;

  for (auto const & page : pages_)
  {
    m_logger.Debug(m_context.GetElementSystemIdentifier(page));
    int inPages = 0, outPages = 0;

    ScIterator5Ptr const outPagesIterator = m_context.CreateIterator5(
        page, ScType::ConstCommonArc, ScType::ConstNode, ScType::ConstPermPosArc, ScGraphKeynodes::nrel_link);

    ScIterator5Ptr const inPagesIterator = m_context.CreateIterator5(
        ScType::ConstNode, ScType::ConstCommonArc, page, ScType::ConstPermPosArc, ScGraphKeynodes::nrel_link);

    while (inPagesIterator->Next())
    {
      inPages++;
    }
    m_logger.Debug(inPages);

    popularPages_[inPages].insert(page);

    if (inPages > popularMax)
    {
      popularMax = inPages;
    }

    while (outPagesIterator->Next())
    {
      outPages++;
    }
    m_logger.Debug(outPages);

    sourcePages_[outPages].insert(page);

    if (outPages > sourceMax)
    {
      sourceMax = outPages;
    }

    CreateDegreeLinks(page, outPages, "out");
    CreateDegreeLinks(page, inPages, "in");

    if (outPages == 0)
    {
      CreateDeadEndRelations(page, graphStruct);
    }
  }

  CreateSourceAndPopularRelations(popularMax, sourceMax, graphStruct);

  return action.FinishSuccessfully();
}

void ScHalfDegreeAgent::CreateSourceAndPopularRelations(
    int const & popularMax,
    int const & sourceMax,
    ScStructure & graphStruct)
{
  auto const & sourceSet = sourcePages_[sourceMax];
  m_logger.Debug(sourceSet);
  auto const & popularSet = popularPages_[popularMax];
  m_logger.Debug(popularSet);

  for (auto const & sourcePage : sourceSet)
  {
    ScAddr const & rrelSourcePageConnector = GetStructConnector(sourcePage, graphStruct);
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::rrel_source_page, rrelSourcePageConnector);
    m_logger.Debug("Successfuly create source page for " + m_context.GetElementSystemIdentifier(sourcePage));
  }

  for (auto const & popularPage : popularSet)
  {
    ScAddr const & rrelPopularPageConnector = GetStructConnector(popularPage, graphStruct);
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::rrel_popular_page, rrelPopularPageConnector);
    m_logger.Debug("Successfuly create popular page for " + m_context.GetElementSystemIdentifier(popularPage));
  }
}

void ScHalfDegreeAgent::CreateDeadEndRelations(ScAddr const & page, ScStructure & graphStruct)
{
  ScAddr const & rrelConnector = GetStructConnector(page, graphStruct);
  m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::rrel_dead_end_page, rrelConnector);
  m_logger.Debug("Successfuly found dead end page.");
}

void ScHalfDegreeAgent::CreateDegreeLinks(ScAddr const & page, int number, std::string const & type)
{
  ScAddr const & degreeLink = m_context.GenerateLink();
  m_context.SetLinkContent(degreeLink, std::to_string(number));
  ScAddr degreeConnector = m_context.GenerateConnector(ScType::ConstCommonArc, page, degreeLink);

  if (type == "in")
  {
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_in_degree, degreeConnector);
    m_logger.Debug("Successfuly create in-link.");
  }
  else if (type == "out")
  {
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_out_degree, degreeConnector);
    m_logger.Debug("Successfuly create out-link.");
  }
}

void ScHalfDegreeAgent::GetPagesVector()
{
  ScIterator3Ptr const it3 =
      m_context.CreateIterator3(ScGraphKeynodes::concept_page, ScType::ConstPermPosArc, ScType::ConstNode);

  while (it3->Next())
  {
    pages_.push_back(it3->Get(2));
  }
}