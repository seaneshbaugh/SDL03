#ifndef __SDL03__GameResource__
#define __SDL03__GameResource__

#include <string>

class GameResource {
public:
    GameResource();
    ~GameResource();
    virtual bool Load(std::string filename);
};

#endif
