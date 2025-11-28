#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>
#include <algorithm>

using SuccessfulyGraphCreationEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

class ScHalfDegreeAgent : public ScAgent<SuccessfulyGraphCreationEvent> {
public:
    ScHalfDegreeAgent();
    ScAddr GetActionClass() const override;
    ScAddr GetEventSubscriptionElement() const override;
    ScTemplate GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const override;
    ScResult DoProgram(SuccessfulyGraphCreationEvent const &event, ScAction& action) override;
    void GetPagesVector();
    void CreateDegreeLinks(ScAddr const & page, int number, const std::string & type);
    void CreateDeadEndLinks(ScAddr const & page);
    void CreateSourceAndPopularLinks(const int & popularMax, const int & sourceMax);
private:
    ScAddrVector pages_;
    std::map<int, ScAddrUnorderedSet> popularPages_;
    std::map<int, ScAddrUnorderedSet> sourcePages_;
};