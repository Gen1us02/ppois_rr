#include <sc-memory/sc_keynodes.hpp>

class ScGraphKeynodes : public ScKeynodes{
public:
    static inline ScKeynode const action_create_graph{"action_create_graph", ScType::ConstNodeClass};
    static inline ScKeynode const concept_number {"concept_number", ScType::ConstNodeClass};
    static inline ScKeynode const concept_page {"concept_page", ScType::ConstNodeClass};
    static inline ScKeynode const nrel_file_path{"nrel_file_path", ScType::ConstNodeNonRole};
    static inline ScKeynode const nrel_visitors_count{"nrel_visitors_count", ScType::ConstNodeNonRole};
    static inline ScKeynode const nrel_spent_time{"nrel_spent_time", ScType::ConstNodeNonRole};
    static inline ScKeynode const nrel_link{"nrel_link", ScType::ConstNodeNonRole};
};