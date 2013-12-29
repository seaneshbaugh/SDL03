#include "GameMap.h"

SDL_Renderer* GameMap::renderer;

const char LuaGameMap::className[] = "GameMap";

Lunar<LuaGameMap>::RegType LuaGameMap::methods[] = {
    {"getWidth", &LuaGameMap::getWidth},
    {"getHeight", &LuaGameMap::getHeight},
    {"render", &LuaGameMap::render},
    {0, 0}
};

GameMap::GameMap() {
    this->width = 0;

    this->height = 0;

    this->tilewidth = 0;

    this->tileheight = 0;
}

GameMap::GameMap(std::string filename) {
    this->Load(filename);
}

GameMap::~GameMap() {
    for (std::map<int, GameMapTile*>::iterator tile = this->tiles.begin(); tile != this->tiles.end(); tile++) {
        if (tile->second) {
            delete tile->second;
        }
    }
}

bool GameMap::ReadFile(std::string filename, std::string &contents) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);

    try {
        if (in) {
            in.seekg(0, std::ios::end);

            contents.resize(in.tellg());

            in.seekg(0, std::ios::beg);

            in.read(&contents[0], contents.size());

            in.close();

            return true;
        } else {
            return false;
        }
    } catch (int exception) {
        return false;
    }
}

// This is kind of a mess. But I guess it works.
bool GameMap::ParseMapFile(std::string jsonString) {
    JSONNode mapNode = libjson::parse(jsonString);

    JSONNode layersNode;

    JSONNode tilesetsNode;

    std::function<void(const JSONNode&, GameMapLayer* layer)> parseLayerData = [&] (const JSONNode &node, GameMapLayer* layer) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            layer->tiles.push_back((int)i->as_int());

            i++;
        }
    };

    std::function<void(const JSONNode&, GameMapLayer*)> parseLayers = [&] (const JSONNode &node, GameMapLayer* layer) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (layer == nullptr && i->type() == JSON_NODE) {
                std::cout << "NEW LAYER" << std::endl;

                GameMapLayer* newLayer = new GameMapLayer();

                parseLayers(*i, newLayer);

                this->layers.push_back(newLayer);
            }

            if (i->name() == "data" && i->type() == JSON_ARRAY) {
                parseLayerData(*i, layer);
            }

            i++;
        }
    };

    std::function<void(const JSONNode&)> parseTileset = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = node.begin();

        int firstGID = 1;

        std::vector<GameMapTile*> tiles;

        while (i != node.end()) {
            if (i->name() == "tileproperties" && i->type() == JSON_NODE) {
                JSONNode::const_iterator j = i->begin();

                while (j != i->end()) {
                    if (j->type() == JSON_NODE) {
                        JSONNode::const_iterator k = j->begin();

                        GameMapTile* tile = new GameMapTile();

                        while (k != j->end()) {
                            if (k->name() == "filename") {
                                tile->filename = k->as_string();
                            }

                            k++;
                        }

                        tiles.push_back(tile);
                    }

                    j++;
                }
            } else {
                if (i->name() == "firstgid" && i->type() == JSON_NODE) {
                    firstGID = (int)i->as_int();
                }
            }

            i++;
        }

        // I'm putting this out here instead of trying to make the loop use it since it's
        // not really part of the conditions for the loop. Whatever, I have no idea what
        // I'm doing.
        int GID = firstGID;

        for (std::vector<GameMapTile*>::iterator it = tiles.begin(); it != tiles.end(); it++) {
            this->tiles[GID] = *it;

            GID++;
        }
    };

    std::function<void(const JSONNode&)> parseTilesets = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                parseTileset(*i);
            }

            i++;
        }
    };

    std::function<void(const JSONNode&)> parseMap = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (i->name() == "layers" && i->type() == JSON_ARRAY) {
                layersNode = *i;

                parseLayers(*i, nullptr);
            } else {
                if (i->name() == "tilesets" && i->type() == JSON_ARRAY) {
                    tilesetsNode = *i;
                } else {
                    if (i->name() == "width" && i->type() == JSON_NUMBER) {
                        this->width = (int)i->as_int();
                    } else {
                        if (i->name() == "height" && i->type() == JSON_NUMBER) {
                            this->height = (int)i->as_int();
                        } else {
                            if (i->name() == "tilewidth" && i->type() == JSON_NUMBER) {
                                this->tilewidth = (int)i->as_int();
                            } else {
                                if (i->name() == "tileheight" && i->type() == JSON_NUMBER) {
                                    this->tileheight = (int)i->as_int();
                                } else {
                                    if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                                        parseMap(*i);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            i++;
        }
    };

    parseMap(mapNode);

    parseTilesets(tilesetsNode);

    return true;
}

bool GameMap::Load(std::string filename) {
    std::string jsonString;

    if (!this->ReadFile(filename, jsonString)) {
        return false;
    }

    if (!this->ParseMapFile(jsonString)) {
        return false;
    }

    return true;
}

void GameMap::Render(int xOffset, int yOffset, int xMovementOffset, int yMovementOffset) {
    if (xOffset < 0) {
        xOffset = 0;
    } else {
        if (xOffset > this->width - 20) {
            xOffset = this->width - 20;
        }
    }

    if (yOffset < 0) {
        yOffset = 0;
    } else {
        if (yOffset > this->height - 15) {
            yOffset = this->height - 15;
        }
    }

    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
        int x = 0;

        int y = 0;

        for (std::vector<int>::iterator tile = (*layer)->tiles.begin(); tile != (*layer)->tiles.end(); tile++) {
            if (x >= this->width) {
                x = 0;

                y++;
            }

//            if (xMovementOffset != 0) {
//                std::cout << "x movement offset = " << xMovementOffset << std::endl;
//            }
//
//            if (yMovementOffset != 0) {
//                std::cout << "y movement offset = " << yMovementOffset << std::endl;
//            }

            SDL_Rect tilePosition = {((x - xOffset) * 32) + xMovementOffset, ((y - yOffset) * 32) + yMovementOffset, 32, 32};

            SDL_RenderCopy(this->renderer, this->tiles[*tile]->texture->texture, NULL, &tilePosition);

            x++;
        }
    }
}
