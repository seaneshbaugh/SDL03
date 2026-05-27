#ifndef SDL03_Game_Scene_Dialogue_Graph
#define SDL03_Game_Scene_Dialogue_Graph

#include "../../../../lib/nlohmann/json.hpp"

#include "../../services/locator.hpp"
#include "dialogue_node.hpp"
#include "../../../helpers/file_system.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Scene {
        namespace Dialogue {
            class DialogueGraph {
            public:
                DialogueGraph(const std::string& graphName);
                ~DialogueGraph();

                bool Load(const std::string& graphName);

                std::shared_ptr<DialogueNode> root;
                std::shared_ptr<DialogueNode> current;

            private:
                std::string graphName;

                bool ParseFile(const std::string& jsonString);

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, DialogueGraph* graph);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                };
            };
        }
    }
}

#endif
