#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>
#include <algorithm>
#include <string>

using SuccessfulyGraphCreationEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

class ScPageRankAgent : public ScAgent<SuccessfulyGraphCreationEvent>
{
public:
  ScPageRankAgent();
  ScAddr GetActionClass() const override;
  ScAddr GetEventSubscriptionElement() const override;
  ScTemplate GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const override;
  ScResult DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action) override;
  void GetPagesVector();
  void CalculatePageRank();
  void GeneratePageRankLinks(int place, double pageRank, ScAddr const & page);

private:
  ScAddrVector pages_;
  std::map<double, std::string> pageRanks_;
};