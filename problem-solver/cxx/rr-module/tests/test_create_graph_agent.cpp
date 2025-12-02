#include <sc-memory/test/sc_test.hpp>

#include <sc-memory/sc_memory.hpp>

#include "agents/graph_agent.hpp"
#include "keynodes/keynodes.hpp"


using AgentTest = ScMemoryTest;

TEST_F(AgentTest, CreateGraphFinishSuccessfully){
    m_ctx->SubscribeAgent<ScAgentCreateGraph>();

    ScAction action = m_ctx->GenerateAction(ScGraphKeynodes::action_create_graph);

    std::string linkContent = "Вакансии;1200;300;Политика конфиденциальности\nТехподдержка;780;100;Политика конфиденциальности\nПолитика конфиденциальности;400;120";

    ScAddr const fileLink = m_ctx->GenerateLink();
    m_ctx->SetLinkContent(fileLink, linkContent);

    ScAddr const linkConnector = m_ctx->GenerateConnector(ScType::ConstCommonArc, action, fileLink);
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_file_path, linkConnector);


    EXPECT_TRUE(action.InitiateAndWait());
    EXPECT_TRUE(action.IsFinishedSuccessfully());

    ScIterator3Ptr const it3 = m_ctx->CreateIterator3(
        ScGraphKeynodes::concept_page,
        ScType::ConstPermPosArc,
        ScType::ConstNode
    );

    ScStructure const siteStruct = action.GetResult();
    EXPECT_FALSE(siteStruct.IsEmpty());

    int pagesCount = 0;
    while(it3->Next()){
        pagesCount++;
    }

    EXPECT_EQ(pagesCount, 3);

    m_ctx->UnsubscribeAgent<ScAgentCreateGraph>();
}

TEST_F(AgentTest, CreateGraphFinishUnsuccessfully){
    m_ctx->SubscribeAgent<ScAgentCreateGraph>();

    ScAction action = m_ctx->GenerateAction(ScGraphKeynodes::action_create_graph);

    ScAddr const fileLink = m_ctx->GenerateNode(ScType::ConstNode);

    ScAddr const linkConnector = m_ctx->GenerateConnector(ScType::ConstCommonArc, action, fileLink);
    m_ctx->GenerateConnector(ScType::ConstPermPosArc, ScGraphKeynodes::nrel_file_path, linkConnector);


    EXPECT_TRUE(action.InitiateAndWait());
    EXPECT_TRUE(action.IsFinishedUnsuccessfully());

    ScIterator3Ptr const it3 = m_ctx->CreateIterator3(
        ScGraphKeynodes::concept_page,
        ScType::ConstPermPosArc,
        ScType::ConstNode
    );

    ScStructure const siteStruct = action.GetResult();
    EXPECT_TRUE(siteStruct.IsEmpty());

    int pagesCount = 0;
    while(it3->Next()){
        pagesCount++;
    }

    EXPECT_EQ(pagesCount, 0);

    m_ctx->UnsubscribeAgent<ScAgentCreateGraph>();
}