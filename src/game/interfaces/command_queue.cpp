#include "command_queue.hpp"
#include "../scenes/actor.hpp"

namespace Game {
    namespace Interfaces {
        void CommandQueue::QueueCommand(States::StateCommand command) {
            this->pendingCommands.push(command);
        }

        void CommandQueue::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
            luaState->new_usertype<States::StartDialogueCommand>("StartDialogueCommand",
                                                                 sol::constructors<States::StartDialogueCommand(Scenes::Actor*)>(),
                                                                 "actor", &States::StartDialogueCommand::actor);

            luaState->new_usertype<States::StartInteractionCommand>("StartInteractionCommand",
                                                                    sol::constructors<States::StartInteractionCommand(Scenes::Actor*)>(),
                                                                    "actor", &States::StartInteractionCommand::actor);

            luaState->new_usertype<States::StartScriptCommand>("StartScriptCommand",
                                                               sol::constructors<States::StartScriptCommand(const std::string&)>(),
                                                               "script_id", &States::StartScriptCommand::scriptId);

            luaState->new_usertype<CommandQueue>("CommandQueue",
                                                 sol::no_constructor,
                                                 "queue_command", &CommandQueue::QueueCommand);
        }
    }
}
