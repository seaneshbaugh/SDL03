#include "MapParser.h"

MapParser::MapParser() {
    this->logger = new Log::Logger("json");
}

// I will be shocked if there are no memory leaks here. The user of pointers here is sloppy.
bool MapParser::Parse(std::string json, GameMap* map) {
    this->logger->debug() << "Parsing map file.";

    JSONNode mapNode = libjson::parse(json);

    std::function<void(const JSONNode&)> parseMap = [&] (const JSONNode &node) {
        JSONNode::const_iterator i = mapNode.begin();

        while (i != mapNode.end()) {
            if (i->name() == "layers" && i->type() == JSON_ARRAY) {
                this->logger->debug() << "About to parse map layers.";

                map->layers = this->ParseLayers(*i);
            } else {
                if (i->name() == "tilesets" && i->type() == JSON_ARRAY) {
                    this->logger->debug() << "About to parse map tilesets.";
                    std::map<int, GameMapTile*> tiles = this->ParseTilesets(*i);

                    map->tiles = tiles;
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

    this->logger->debug() << "Parsing map layer...";

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

    this->logger->debug() << "Parsed map layer \"" << layer->name << "\".";
    
    return layer;
}

std::vector<GameMapLayer*> MapParser::ParseLayers(const JSONNode &node) {
    std::vector<GameMapLayer*> layers;

    this->logger->debug() << "Parsing map layers.";
    
    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        if (i->type() == JSON_NODE) {
            layers.push_back(this->ParseLayer(*i));
        }
        
        i++;
    }

    this->logger->debug() << "Parsed map layers";
    
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
    this->logger->debug() << "Parsing layer objects.";

    JSONNode::const_iterator i = node.begin();
    
    while (i != node.end()) {
        if (layer->name == "encounter_areas") {
            this->logger->debug() << "Creating encounter area.";

            // TODO: Figure out a better way to handle this. Obviously hard coding the map name is bogus.
            std::string filename = "resources/encounter_areas/world01-" + i->at("properties").at("name").as_string() + ".json";

            GameMapEncounterArea* encounterArea = new GameMapEncounterArea(filename);

            // For some reason Tiled exports the coordinates as pixels. Divide by 32, for now, to get the coordinates.
            // TODO: Use map tileheight and tilewidth.
            encounterArea->type = i->at("type").as_string();
            encounterArea->x = static_cast<int>(i->at("x").as_int()) / 32;
            encounterArea->y = static_cast<int>(i->at("y").as_int()) / 32;
            encounterArea->width = static_cast<int>(i->at("width").as_int()) / 32;
            encounterArea->height = static_cast<int>(i->at("height").as_int()) / 32;
            std::map<std::string, std::string> properties;
            JSONNode::const_iterator k = i->at("properties").begin();
            while (k != i->at("properties").end()) {
                properties[k->name()] = k->as_string();

                k++;
            }
            encounterArea->properties = properties;

            layer->objects.push_back(encounterArea);
        } else if (layer->name == "load_points") {
            this->logger->debug() << "Creating load point.";

            GameMapLoadPoint* loadPoint = new GameMapLoadPoint();

            // For some reason Tiled exports the coordinates as pixels. Divide by 32, for now, to get the coordinates.
            // TODO: Use map tileheight and tilewidth.
            loadPoint->type = i->at("type").as_string();
            loadPoint->x = static_cast<int>(i->at("x").as_int()) / 32;
            loadPoint->y = static_cast<int>(i->at("y").as_int()) / 32;
            loadPoint->width = static_cast<int>(i->at("width").as_int()) / 32;
            loadPoint->height = static_cast<int>(i->at("height").as_int()) / 32;
            std::map<std::string, std::string> properties;
            JSONNode::const_iterator k = i->at("properties").begin();
            while (k != i->at("properties").end()) {
                properties[k->name()] = k->as_string();

                k++;
            }
            loadPoint->properties = properties;

            layer->objects.push_back(loadPoint);
        } else {
            throw;
        }


//        this->logger->debug() << "Parsing object attributes.";
//        JSONNode::const_iterator j = i->begin();
//
//        std::map<std::string, JSONNode> objectAttributes;
//
//        while (j != i->end()) {
//            if (j->type() == JSON_STRING) {
//                JSONNode::const_iterator n = j;
//
//                this->logger->debug() << n->name() << "=" << n->as_string();
//
//                //this->logger->debug() << j->name() << "=" << j->as_string();
//                objectAttributes[j->name()] = j->as_string();
//            } else if (j->type() == JSON_NUMBER) {
//                this->logger->debug() << j->name() << "=" << j->as_int();
//                objectAttributes[j->name()] = j->as_int();
//            } else if (j->type() == JSON_BOOL) {
//                this->logger->debug() << j->name() << "=" << j->as_bool();
//                objectAttributes[j->name()] = j->as_bool();
//            } else {
//                this->logger->debug() << j->name() << "=" << j->write_formatted();
//                objectAttributes[j->name()] = j->as_node();
//            }
//
//            j++;
//        }

        // Leaving this here for when std::variant makes it to XCode (should be when XCode 10 is out of beta).
//        std::map<std::string, std::variant<int, std::string, std::map<std::string, std::string>> objectAttributes;
//
//        while (j != i->end()) {
//            switch (j-type()) {
//                case JSON_NUMBER:
//                    objectAttributes[j->name()] = j->as_int();
//                    break;
//                case JSON_STRING:
//                    objectAttributes[j->name()] = j->as_string();
//                    break;
//                case JSON_NODE:
//                    JSONNode::const_iterator k = j->begin();
//
//                    std::map<std::string, std::string> properties;
//
//                    // For now just assume any nodes at this level will all have string values.
//                    while (k != j->end()) {
//                        properties[k->name()] = k->as_string();
//
//                        k++;
//                    }
//
//                    objectAttributes[j->name()] = properties;
//                    break;
//            }
//        }

        // This might actually be worse than what I had before. At least each block here
        // can probably be extracted to a constructor or some sort of object factory class.
//        if (std::get<std::string>(objectAttributes["type"]) == "encounter_area") {
//        if (objectAttributes["type"] == "encounter_area") {
//            this->logger->debug() << "Creating encounter Area.";
//            // TODO: Figure out a better way to handle this. Obviously hard coding the file name is bogus.
//            std::string filename = "resources/maps/world01-" + objectAttributes["properties"]["name"].as_string() + ".json";
//
//            GameMapEncounterArea* encounterArea = new GameMapEncounterArea(filename);
//
//            // For some reason Tiled exports the coordinates as pixels. Divide by 32, for now, to get the coordinates.
//            encounterArea->type = std::string(objectAttributes["type"].as_string());
//            encounterArea->x = static_cast<int>(objectAttributes["x"].as_int()) / 32;
//            encounterArea->y = static_cast<int>(objectAttributes["y"].as_int()) / 32;
//            encounterArea->width = static_cast<int>(objectAttributes["width"].as_int()) / 32;
//            encounterArea->height = static_cast<int>(objectAttributes["height"].as_int()) / 32;
//            std::map<std::string, std::string> properties;
//            JSONNode::const_iterator k = objectAttributes["properties"].begin();
//            while (k != objectAttributes["properties"].end()) {
//                properties[k->name()] = k->as_string();
//
//                k++;
//            }
//            encounterArea->properties = properties;
//
//            layer->objects.push_back(encounterArea);
//        } else {
//            this->logger->debug() << "Creating object with type " << objectAttributes["type"].as_string() << ".";
//
//            GameMapObject* object = new GameMapObject();
//
//            object->type = std::string(objectAttributes["type"].as_string());
//            object->x = static_cast<int>(objectAttributes["x"].as_int()) / 32;
//            object->y = static_cast<int>(objectAttributes["y"].as_int()) / 32;
//            object->width = static_cast<int>(objectAttributes["width"].as_int()) / 32;
//            object->height = static_cast<int>(objectAttributes["height"].as_int()) / 32;
//            std::map<std::string, std::string> properties;
//            JSONNode::const_iterator k = objectAttributes["properties"].begin();
//            while (k != objectAttributes["properties"].end()) {
//                properties[k->name()] = k->as_string();
//
//                k++;
//            }
//            object->properties = properties;
//
//            layer->objects.push_back(object);
//        }

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

// For now this just sort of dumps all tilesets into one list of tiles.
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
