#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>

class ScAgentCreateGraph : public ScActionInitiatedAgent
{
public:
  ScAgentCreateGraph();
  ScAddr GetActionClass() const override;
  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
  void GetOrCreatePage(ScAddr & pageNode, std::string const & pageName);
  void CreateMainIdntf(ScAddr const & node, std::string const & idtf);

private:
  int next_page{0};
  std::map<std::string, std::string> pagesIdentificators_;
};