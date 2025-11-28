#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>
#include <algorithm>

using SuccessfulyGraphCreationEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

class ScHalfDegreeAgent : public ScAgent<SuccessfulyGraphCreationEvent>
{
public:
  ScHalfDegreeAgent();
  ScAddr GetActionClass() const override;
  ScAddr GetEventSubscriptionElement() const override;
  ScTemplate GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const override;
  ScResult DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action) override;
  ScAddr GetStructConnector(ScAddr const & page, ScStructure & graphStruct);
  void GetPagesVector();
  void CreateDegreeLinks(ScAddr const & page, int number, std::string const & type);
  void CreateDeadEndRelations(ScAddr const & page, ScStructure & graphStruct);
  void CreateSourceAndPopularRelations(int const & popularMax, int const & sourceMax, ScStructure & graphStruct);

private:
  ScAddrVector pages_;
  std::map<int, ScAddrUnorderedSet> popularPages_;
  std::map<int, ScAddrUnorderedSet> sourcePages_;
};