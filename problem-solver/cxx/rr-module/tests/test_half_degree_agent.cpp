#include <sc-memory/test/sc_test.hpp>

#include <sc-memory/sc_memory.hpp>

#include "agents/graph_agent.hpp"
#include "agents/half_degree_agent.hpp"
#include "keynodes/keynodes.hpp"
#include <thread>
#include <chrono>


using AgentTest = ScMemoryTest;

class HalfDegreeAgentTest : public AgentTest
{
protected:
  void SetUp() override
  {
    ScMemoryTest::SetUp();
    m_ctx->SubscribeAgent<ScAgentCreateGraph>();
    m_ctx->SubscribeAgent<ScHalfDegreeAgent>();
  }

  void TearDown() override
  {
    m_ctx->UnsubscribeAgent<ScHalfDegreeAgent>();
    m_ctx->UnsubscribeAgent<ScAgentCreateGraph>();
    ScMemoryTest::TearDown();
  }
};

TEST_F(HalfDegreeAgentTest, ActionFinishSuccesfully){
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

    int deadEndCount = 0, sourceCount = 0, popularCount = 0;

    ScIterator5Ptr const deadEndIterator = m_ctx->CreateIterator5(
        ScType::ConstNodeStructure,
        ScType::ConstPermPosArc,
        ScType::ConstNode,
        ScType::ConstPermPosArc,
        ScGraphKeynodes::rrel_dead_end_page
    );

    ScIterator5Ptr const sourcePageIterator = m_ctx->CreateIterator5(
        ScType::ConstNodeStructure,
        ScType::ConstPermPosArc,
        ScType::ConstNode,
        ScType::ConstPermPosArc,
        ScGraphKeynodes::rrel_source_page
    );

    ScIterator5Ptr const popularPageIterator = m_ctx->CreateIterator5(
        ScType::ConstNodeStructure,
        ScType::ConstPermPosArc,
        ScType::ConstNode,
        ScType::ConstPermPosArc,
        ScGraphKeynodes::rrel_popular_page
    );

    while(deadEndIterator->Next()){
        deadEndCount++;
    }

    while(sourcePageIterator->Next()){
        sourceCount++;
    }

    while(popularPageIterator->Next()){
        popularCount++;
    }

    EXPECT_EQ(deadEndCount, 1);
    EXPECT_EQ(popularCount, 1);
    EXPECT_EQ(sourceCount, 1);
}