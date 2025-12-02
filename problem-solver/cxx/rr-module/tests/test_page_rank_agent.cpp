#include <sc-memory/test/sc_test.hpp>

#include <sc-memory/sc_memory.hpp>

#include "agents/graph_agent.hpp"
#include "agents/page_rank_agent.hpp"
#include "keynodes/keynodes.hpp"
#include <thread>
#include <chrono>


using AgentTest = ScMemoryTest;

class PageRankAgentTest : public AgentTest
{
protected:
  void SetUp() override
  {
    ScMemoryTest::SetUp();
    m_ctx->SubscribeAgent<ScAgentCreateGraph>();
    m_ctx->SubscribeAgent<ScPageRankAgent>();
  }

  void TearDown() override
  {
    m_ctx->UnsubscribeAgent<ScPageRankAgent>();
    m_ctx->UnsubscribeAgent<ScAgentCreateGraph>();
    ScMemoryTest::TearDown();
  }
};

TEST_F(PageRankAgentTest, ActionFinishSuccessfully){
    ScAction action_create_graph = m_ctx->GenerateAction(ScGraphKeynodes::action_create_graph);

    std::string linkContent = "Вакансии;1200;300;Техподдержка,Политика конфиденциальности\nТехподдержка;780;100;Политика конфиденциальности\nПолитика конфиденциальности;400;120";

    ScAddr const fileLink = m_ctx->GenerateLink();
    m_ctx->SetLinkContent(fileLink, linkContent);

    ScAddr const linkConnector = m_ctx->GenerateConnector(ScType::ConstCommonArc, action_create_graph, fileLink);
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_file_path, linkConnector);


    EXPECT_TRUE(action_create_graph.InitiateAndWait());
    EXPECT_TRUE(action_create_graph.IsFinishedSuccessfully());
    ScWaiter waiter;
    waiter.Wait(500);

    int pageRanksCount = 0;
    double sum = 0;
    ScIterator5Ptr it5 = m_ctx->CreateIterator5(
        ScType::ConstNode,
        ScType::ConstCommonArc,
        ScType::ConstNodeLink,
        ScType::ConstPermPosArc,
        ScGraphKeynodes::nrel_page_rank
    );

    while(it5->Next()){
        std::string pageRankContent;
        ScAddr const & link = it5->Get(2);
        m_ctx->GetLinkContent(link, pageRankContent);
        sum += std::stod(pageRankContent.substr(11));
        pageRanksCount++;
    }

    EXPECT_EQ(pageRanksCount, 3);
    EXPECT_TRUE((1 - sum) <= 0.00001);
}