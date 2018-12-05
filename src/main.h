#ifndef SDL03_main
#define SDL03_main

#include <map>
#include <memory>
#include <vector>

#include "LuaIntf.h"

namespace LuaIntf {
    LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
    LUA_USING_LIST_TYPE(std::vector)
    LUA_USING_MAP_TYPE(std::map)
}

#include "game/engine.hpp"

#endif
