#include "rrModule.hpp"
#include "agents/graph_agent.hpp"
#include "agents/half_degree_agent.hpp"

SC_MODULE_REGISTER(RRModule)->Agent<ScAgentCreateGraph>()
                            ->Agent<ScHalfDegreeAgent>()
;