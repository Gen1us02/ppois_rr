#include "page_rank_agent.hpp"
#include <sc-memory/sc_memory_headers.hpp>
#include "keynodes/keynodes.hpp"
#include "utils/utils.hpp"
#include "basevars/basevars.hpp"


ScPageRankAgent::ScPageRankAgent()
{
  m_logger = utils::ScLogger(utils::ScLogger::ScLogType::File, "logs/ScPageRankAgentLog.log", utils::ScLogLevel::Debug);
}

ScAddr ScPageRankAgent::GetActionClass() const
{
  return ScGraphKeynodes::action_calculate_page_rank;
}

ScAddr ScPageRankAgent::GetEventSubscriptionElement() const
{
  return ScGraphKeynodes::concept_successfuly_created_graph;
}

ScTemplate ScPageRankAgent::GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const
{
  ScTemplate templ;
  templ.Triple(ScGraphKeynodes::concept_successfuly_created_graph, ScType::VarPermPosArc, ScType::VarNode);

  return templ;
}

ScResult ScPageRankAgent::DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action)
{
  m_logger.Debug("Starting getting site pages");
  GetPagesVector();
  m_logger.Debug("Get all pages. Ready to calcualte page ranks");
  m_logger.Debug("Starting calculating page ranks");
  try
  {
    CalculatePageRank();
  }
  catch (std::invalid_argument ex)
  {
    m_logger.Warning(ex.what());
    return action.FinishUnsuccessfully();
  }
  m_logger.Debug("Finish calculate page ranks");
  return action.FinishSuccessfully();
}

void ScPageRankAgent::GetPagesVector()
{
  ScIterator3Ptr const it3 =
      m_context.CreateIterator3(ScGraphKeynodes::concept_page, ScType::ConstPermPosArc, ScType::ConstNode);

  while (it3->Next())
  {
    pages_.push_back(it3->Get(2));
  }
}

void ScPageRankAgent::CalculatePageRank()
{
  int N = pages_.size();
  std::vector<double> pr(N, 1.0 / N);
  m_logger.Debug(N);

  std::vector<int> out_degree(N, 0);
  for (auto const & page : pages_)
  {
    int outPages = 0;
    ScIterator5Ptr const outPagesIterator = m_context.CreateIterator5(
      page, ScType::ConstCommonArc, ScType::ConstNode, ScType::ConstPermPosArc, ScGraphKeynodes::nrel_link);
    std::string pageIdtf = m_context.GetElementSystemIdentifier(page);
    m_logger.Debug(pageIdtf);
    int pageIndex = Utils::getPageIndex(pageIdtf);
    m_logger.Debug(pageIndex);
    while(outPagesIterator->Next()){
      outPages++;
    }
    out_degree[pageIndex] = outPages;
    m_logger.Debug("Succesfuly write out pages to vector");
  }

  m_logger.Debug("Succesfully create out degree vector");

  for (int iter = 0; iter < MAX_ITERATIONS; iter++)
  {
    std::vector<double> new_pr(N, 0.0);

    double dangling_sum = 0.0;
    for (int i = 0; i < N; i++)
    {
      if (out_degree[i] == 0)
      {
        dangling_sum += pr[i];
      }
    }
    double dangling_factor = dangling_sum / N;

    for (int i = 0; i < N; i++)
    {
      double sum_incoming = 0.0;

      ScIterator5Ptr const inPages = m_context.CreateIterator5(
          ScType::ConstNode, ScType::ConstCommonArc, pages_[i], ScType::ConstPermPosArc, ScGraphKeynodes::nrel_link);

      while (inPages->Next())
      {
        int index = Utils::getPageIndex(m_context.GetElementSystemIdentifier(inPages->Get(0)));
        if (out_degree[index] > 0)
        {
          sum_incoming += pr[index] / out_degree[index];
        }
      }

      new_pr[i] = (1.0 - DAMPING) / N + DAMPING * (sum_incoming + dangling_factor);
    }

    pr = new_pr;
  }

  for (int i = 0; i < N; i++)
  {
    pageRanks_[pr[N]] = DEFAULT_PAGE_NAME + std::to_string(i);
  }

  std::sort(pr.begin(), pr.end());

  for (auto const & page : pages_)
  {
    std::string systemIdtf = m_context.GetElementSystemIdentifier(page);
    int index = Utils::getPageIndex(systemIdtf);
    GeneratePageRankLinks(index + 1, pr[index], page);
  }
}

void ScPageRankAgent::GeneratePageRankLinks(int place, double pageRank, ScAddr const & page)
{
  std::string linkContent = std::to_string(place) + " place - " + std::to_string(pageRank);
  ScAddr const & pageRankLink = m_context.GenerateLink();
  m_context.SetLinkContent(pageRankLink, linkContent);

  ScAddr const & pageRankConnector = m_context.GenerateConnector(ScType::ConstCommonArc, page, pageRankLink);
  m_context.GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_page_rank, pageRankConnector);
}