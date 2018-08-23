#include "MapParser.h"

// I will be shocked if there are no memory leaks here. The user of pointers here is sloppy.
bool MapParser::Parse(std::string json, GameMap* map) {
    JSONNode mapNode = libjson::parse(json);

    std::function<void(const JSONNode&)> parseMap = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = mapNode.begin();

        while (i != mapNode.end()) {
            if (i->name() == "layers" && i->type() == JSON_ARRAY) {
                map->layers = this->ParseLayers(*i);
                
                //std::copy(layers.begin(), layers.end(), std::back_inserter(map->layers));
            } else {
                if (i->name() == "tilesets" && i->type() == JSON_ARRAY) {
                    std::map<int, GameMapTile*> tiles = this->ParseTilesets(*i);

                    map->tiles = tiles;
                    
    //                int id = 0;
    //                for (std::vector<GameMapTile*>::iterator it = tiles.begin(); it != tiles.end(); it++) {
    //                    map->tiles[id] = *it;
    //                    id += 1;
    //                }
                    
                    //std::copy(tiles.begin(), tiles.end(), std::back_inserter(map->tiles));
                } else {
                    if (i->name() == "width" && i->type() == JSON_NUMBER) {
                        map->width = (int)i->as_int();
                    } else {
                        if (i->name() == "height" && i->type() == JSON_NUMBER) {
                            map->height = (int)i->as_int();
                        } else {
                            if (i->name() == "tilewidth" && i->type() == JSON_NUMBER) {
                                map->tilewidth = (int)i->as_int();
                            } else {
                                if (i->name() == "tileheight" && i->type() == JSON_NUMBER) {
                                    map->tileheight = (int)i->as_int();
                                } else {
                                    if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                                        std::cout << i->write_formatted() << std::endl;
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

    return true;
}

// Probably should use unique_ptr
// See https://stackoverflow.com/questions/6876751/differences-between-unique-ptr-and-shared-ptr
GameMapLayer* MapParser::ParseLayer(const JSONNode &node) {
    GameMapLayer* layer = new GameMapLayer();

    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        if (i->name() == "data" && i->type() == JSON_ARRAY) {
            this->ParseLayerData(*i, layer);
        }
        
        if (i->name() == "objects" && i->type() == JSON_ARRAY) {
            this->ParseLayerObjects(*i, layer);
        }
        
        if (i->name() == "name" && i->type() == JSON_STRING) {
            layer->name = i->as_string();
        }
        
        i++;
    }
    
    return layer;
    
    //map->layers.push_back(layer);
}

std::vector<GameMapLayer*> MapParser::ParseLayers(const JSONNode &node) {
    std::vector<GameMapLayer*> layers;
    
    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        if (i->type() == JSON_NODE) {
            layers.push_back(this->ParseLayer(*i));
        }
        
        i++;
    }
    
    return layers;
}

void MapParser::ParseLayerData(const JSONNode &node, GameMapLayer* layer) {
    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        layer->tiles.push_back((int)i->as_int());
        
        i++;
    }
}

void MapParser::ParseLayerObjects(const JSONNode &node, GameMapLayer* layer) {
    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        JSONNode::const_iterator j = i->begin();
        
        GameMapObject* object = new GameMapObject();
        
        // TODO: Consider using a std::map<std::string, std::variant<int, std::string> to hold the parsed values in generic way and then just pull the properties from that.
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
}

std::map<int, GameMapTile*> MapParser::ParseTileset(const JSONNode &node) {
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
                            std::cout << "filename = " << k->as_string() << std::endl;
                        } else {
                            if (k->name() == "name") {
                                tile->name = k->as_string();
                            }
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

    int GID = firstGID;
    std::map <int, GameMapTile*> result;

    for (std::vector<GameMapTile*>::iterator it = tiles.begin(); it != tiles.end(); it++) {
        result[GID] = *it;

        GID++;
    }

    return result;
}

// For now this just sort of dumps all tilesets into one list of times.
std::map<int, GameMapTile*> MapParser::ParseTilesets(const JSONNode &node) {
    std::map<int, GameMapTile*> tiles;
    
    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
            std::map<int, GameMapTile*> t = this->ParseTileset(*i);
            
            tiles.insert(t.begin(), t.end());
        }
        
        i++;
    }
    
    return tiles;
}
