#ifndef SDL03_Game_Interfaces_CommandQueue
#define SDL03_Game_Interfaces_CommandQueue

#include <queue>

#include "../../../lib/sol/sol.hpp"

// This is a smell. Commands should probably be more generalized and not so tightly
// coupled to states. Eventually I'll probably want some sort of generic interface
// for commands themselves rather than a std::variant. For now this is fine since
// the only thing that consumes commands is the Map state and soon to be other
// states. But if anything other than a state needs this interface then I'll for
// sure want to refactor this.
#include "../states/commands.hpp"

namespace Game {
    namespace Interfaces {
        class CommandQueue {
        public:
            virtual ~CommandQueue() = default;
            virtual void QueueCommand(States::StateCommand command);

        protected:
            std::queue<States::StateCommand> pendingCommands;

            virtual void ProcessPendingCommands() = 0;

        public:
            class LuaInterface {
            public:
                static void Bind(std::shared_ptr<sol::state> luaState);
            };
        };
    }
}

#endif
