#pragma once

#include <sc-memory/sc_agent.hpp>

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
private:
    ScAddrVector pages_;
};