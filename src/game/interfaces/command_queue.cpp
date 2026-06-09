#include "command_queue.hpp"

namespace Game {
    namespace Interfaces {
        void CommandQueue::QueueCommand(States::StateCommand command) {
            this->pendingCommands.push(command);
        }

        void CommandQueue::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            luaState->new_usertype<States::StartDialogueCommand>("StartDialogueCommand",
                                                                 sol::constructors<States::StartDialogueCommand(const std::string&)>(),
                                                                 "dialogue_id", &States::StartDialogueCommand::dialogueId);

            luaState->new_usertype<States::StartCutsceneCommand>("StartCutsceneCommand",
                                                                 sol::constructors<States::StartCutsceneCommand(const std::string&)>(),
                                                                 "cutscene_id", &States::StartCutsceneCommand::cutsceneId);

            luaState->new_usertype<CommandQueue>("CommandQueue",
                                                 sol::no_constructor,
                                                 "queue_command", &CommandQueue::QueueCommand);
        }
    }
}
