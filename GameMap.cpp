#include "GameMap.h"

SDL_Renderer* GameMap::renderer;

const char LuaGameMap::className[] = "GameMap";

Lunar<LuaGameMap>::RegType LuaGameMap::methods[] = {
    {"getWidth", &LuaGameMap::getWidth},
    {"getHeight", &LuaGameMap::getHeight},
    {"render", &LuaGameMap::render},
    {"getWalkability", &LuaGameMap::getWalkability},
    {"getObjects", &LuaGameMap::getObjects},
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

    std::function<void(const JSONNode&, GameMapLayer* layer)> parseLayerObjects = [&] (const JSONNode &node, GameMapLayer* layer) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            JSONNode::const_iterator j = i->begin();

            GameMapObject* object = new GameMapObject();

            while (j != i->end()) {
                // For some reason Tiled exports the coordinates as pixels. Divide by 32, for now, to get the coordinates.
                if (j->name() == "x" && j->type() == JSON_NUMBER) {
                    object->x = (int)j->as_int() / 32;
                }

                if (j->name() == "y" && j->type() == JSON_NUMBER) {
                    object->y = (int)j->as_int() / 32;
                }

                if (j->name() == "width" && j->type() == JSON_NUMBER) {
                    object->width = (int)j->as_int() / 32;
                }

                if (j->name() == "height" && j->type() == JSON_NUMBER) {
                    object->height = (int)j->as_int() / 32;
                }

                if (j->name() == "properties" && j->type() == JSON_NODE) {
                    JSONNode::const_iterator k = j->begin();

                    while (k != j->end()) {
                        object->properties[k->name()] = k->as_string();

                        k++;
                    }
                }

                if (j->name() == "type" && j->type() == JSON_STRING) {
                    object->type = std::string(j->as_string());
                }

                j++;
            }

            layer->objects.push_back(object);

            i++;
        }
    };

    std::function<void(const JSONNode&)> parseLayer = [&] (const JSONNode &node) {
        GameMapLayer* layer = new GameMapLayer();


        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (i->name() == "data" && i->type() == JSON_ARRAY) {
                parseLayerData(*i, layer);
            }

            if (i->name() == "objects" && i->type() == JSON_ARRAY) {
                parseLayerObjects(*i, layer);
            }

            if (i->name() == "name" && i->type() == JSON_STRING) {
                layer->name = i->as_string();
            }

            i++;
        }

        this->layers.push_back(layer);
    };

    std::function<void(const JSONNode&, GameMapLayer*)> parseLayers = [&] (const JSONNode &node, GameMapLayer* layer) {
        JSONNode::const_iterator i = node.begin();

        while (i != node.end()) {
            if (i->type() == JSON_NODE) {
                parseLayer(*i);
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
        if ((*layer)->name != "walkability") {
            int x = 0;

            int y = 0;

            for (std::vector<int>::iterator tile = (*layer)->tiles.begin(); tile != (*layer)->tiles.end(); tile++) {
                if (x >= this->width) {
                    x = 0;

                    y++;
                }

                if (*tile != 0) {
                    SDL_Rect tilePosition = {((x - xOffset) * 32) + xMovementOffset, ((y - yOffset) * 32) + yMovementOffset, 32, 32};

                    SDL_RenderCopy(this->renderer, this->tiles[*tile]->texture->texture, NULL, &tilePosition);
                }
                
                x++;
            }
        }
    }
}

// This whole thing is a horrible hack for right now.
bool GameMap::GetWalkability(int x, int y) {
    if (x < 0 || y < 0 || x > this->width || y > this->height) {
        return false;
    }

    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
        if ((*layer)->name == "walkability") {
            // omg what am I even doing here.
            return this->tiles[(*layer)->tiles[(y * this->width) + x]]->filename == "walkable.png";
        }
    }

    return false;
}

std::vector<GameMapObject*> GameMap::GetObjects(int x, int y) {
    std::vector<GameMapObject*> result;

    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
        for (std::vector<GameMapObject*>::iterator object = (*layer)->objects.begin(); object != (*layer)->objects.end(); object++) {
            if (x >= (*object)->x && x < ((*object)->x + (*object)->width) <= x && y >= (*object)->y && y < ((*object)->y + (*object)->height)) {
                result.push_back(*object);
            }
        }
    }

    return result;
}
