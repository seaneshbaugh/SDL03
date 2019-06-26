#include "base.hpp"

namespace Game {
    namespace Objects {
        Base::Base() {
        }

        Base::~Base() {
        }

        void Base::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
            LuaIntf::LuaBinding(luaContext->state())
            .beginModule("objects")
                .beginClass<Base>("Base")
                    .addConstructor(LUA_ARGS())
                .endClass()
            .endModule();
        }
    }
}
