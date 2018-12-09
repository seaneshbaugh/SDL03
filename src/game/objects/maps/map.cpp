#include "map.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            const std::string Map::logChannel = "map";

            Map::Map() {
                this->logger = Services::Locator::LoggerService()->GetLogger(Map::logChannel);
                this->name = "";
                this->filename = "";
                this->width = 0;
                this->height = 0;
                this->tilewidth = 0;
                this->tileheight = 0;
            }

            Map::Map(const std::string& filename) : Map() {
                this->Load(filename);
            }

            Map::~Map() {
            }

            bool Map::ParseMapFile(const std::string& jsonString) {
                Map::Parser mapParser = Map::Parser();

                this->logger->debug() << "About to parse map file";

                // See https://forum.libcinder.org/topic/solution-calling-shared-from-this-in-the-constructor
                auto wptr = std::shared_ptr<Map>(this, [](Map*){});

                mapParser.Parse(jsonString, std::static_pointer_cast<Map>(this->shared_from_this()));

                return true;
            }

            // Maybe this should be put in some sort of texture loading class. Same with other resources.
            bool Map::LoadTextures() {
                for (auto tile = this->tiles.begin(); tile != this->tiles.end(); tile++) {
                    std::shared_ptr<Resources::Texture> texture = std::make_shared<Resources::Texture>();

                    this->logger->debug() << "Loading map tile texture \"assets/images/map_tiles/" << tile->second->filename << "\".";

                    texture->Load("assets/images/map_tiles/" + tile->second->filename);

                    this->textures[tile->second->name] = texture;

                    tile->second->texture = texture;
                }

                return true;
            }

            bool Map::Load(const std::string& filename) {
                this->logger->info() << "Loading map \"" << filename << "\".";

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile(filename, jsonString)) {
                    return false;
                }

                this->logger->info() << "Read " << jsonString.size() << " bytes";

                if (!this->ParseMapFile(jsonString)) {
                    this->logger->error() << "Failed to parse map";

                    return false;
                }

                for (auto layer = this->layers.begin(); layer != this->layers.end(); layer++) {
                    if ((*layer)->name == "walkability") {
                        this->walkabilityLayer = *layer;
                    }
                }

                if (!this->LoadTextures()) {
                    this->logger->error() << "Failed to load textures";

                    return false;
                }

                this->SetNameFromFilename();

                this->logger->info() << "Map loaded";

                return true;
            }

            void Map::Render(int xOffset, int yOffset, int xMovementOffset, int yMovementOffset) {
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

                for (auto layer = this->layers.begin(); layer != this->layers.end(); layer++) {
                    if ((*layer)->name != "walkability") {
                        int x = 0;

                        int y = 0;

                        for (auto tile = (*layer)->tiles.begin(); tile != (*layer)->tiles.end(); tile++) {
                            if (x >= this->width) {
                                x = 0;

                                y++;
                            }

                            if (*tile != 0) {
                                SDL_Rect tilePosition = {((x - xOffset) * 32) + xMovementOffset, ((y - yOffset) * 32) + yMovementOffset, 32, 32};

                                Services::Locator::VideoService()->Render(this->tiles[*tile]->texture, nullptr, &tilePosition);
                                // SDL_RenderCopy(Services::Locator::VideoService()->GetRenderer().get(), this->tiles[*tile]->texture, nullptr, &tilePosition);
                            }

                            x++;
                        }
                    }
                }
            }

            bool Map::GetWalkability(int x, int y) {
                if (x < 0 || y < 0 || x > this->width - 1 || y > this->height - 1) {
                    return false;
                }

                return this->tiles[this->walkabilityLayer->tiles[(y * this->width) + x]]->name == "walkable";
            }

            std::vector<std::shared_ptr<MapObject>> Map::GetObjects(int x, int y) {
                std::vector<std::shared_ptr<MapObject>> result;

                for (auto layer = this->layers.begin(); layer != this->layers.end(); layer++) {
                    this->logger->debug() << "Getting objects for layer \"" << (*layer)->name << "\" at (" << x << ", " << y << ").";

                    for (auto object = (*layer)->objects.begin(); object != (*layer)->objects.end(); object++) {
                        if (x >= (*object)->x && x < ((*object)->x + (*object)->width) && y >= (*object)->y && y < ((*object)->y + (*object)->height)) {
                            this->logger->debug() << "Object with type " << (*object)->type << " (" << (*object)->x << ", " << (*object)->y << ") - (" << ((*object)->x + (*object)->width) << ", " << ((*object)->y + (*object)->height) << ") stepped on";

                            result.push_back(*object);
                        }
                    }
                }

                return result;
            }

            void Map::SetNameFromFilename() {
                this->name = Helpers::Path::BaseName(this->filename, true);
            }

            const std::string Map::Parser::logChannel = "json";

            Map::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(Map::Parser::logChannel);
            }

            Map::Parser::~Parser() {
            }

            void Map::Parser::Parse(const std::string& jsonString, std::shared_ptr<Map> map) {
                this->logger->debug() << "Parsing map file.";

                JSONNode mapNode = libjson::parse(jsonString);

                std::function<void(const JSONNode&)> parseJSON = [&] (const JSONNode &node) {
                    JSONNode::const_iterator i = mapNode.begin();

                    while (i != mapNode.end()) {
                        if (i->name() == "layers" && i->type() == JSON_ARRAY) {
                            this->logger->debug() << "About to parse map layers.";

                            map->layers = this->ParseLayers(*i);
                        } else {
                            if (i->name() == "tilesets" && i->type() == JSON_ARRAY) {
                                this->logger->debug() << "About to parse map tilesets.";
                                std::map<int, std::shared_ptr<MapTile>> tiles = this->ParseTilesets(*i);

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
                                                    parseJSON(*i);
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

                parseJSON(mapNode);
            }

            std::shared_ptr<MapLayer> Map::Parser::ParseLayer(const JSONNode &node) {
                std::shared_ptr<MapLayer> layer = std::make_shared<MapLayer>();

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

            std::vector<std::shared_ptr<MapLayer>> Map::Parser::ParseLayers(const JSONNode& node) {
                std::vector<std::shared_ptr<Objects::Maps::MapLayer>> layers;

                this->logger->debug() << "Parsing map layers.";

                for (auto it = node.begin(); it != node.end(); ++it) {
                    if (it->type() == JSON_NODE) {
                        layers.push_back(this->ParseLayer(*it));
                    }
                }

                this->logger->debug() << "Parsed map layers";

                return layers;
            }

            void Map::Parser::ParseLayerData(const JSONNode &node, std::shared_ptr<MapLayer> layer) {
                JSONNode::const_iterator i = node.begin();

                while (i != node.end()) {
                    layer->tiles.push_back((int)i->as_int());

                    i++;
                }
            }

            void Map::Parser::ParseLayerObjects(const JSONNode& node, std::shared_ptr<MapLayer> layer) {
                this->logger->debug() << "Parsing layer objects.";

                JSONNode::const_iterator i = node.begin();

                while (i != node.end()) {
                    // TODO: Break this out into its own method.
                    if (layer->name == "encounter_areas") {
                        this->logger->debug() << "Creating encounter area.";

                        // TODO: Figure out a better way to handle this. Obviously hard coding the map name is bogus.
                        std::string filename = "resources/encounter_areas/world01-" + i->at("properties").at("name").as_string() + ".json";

                        std::shared_ptr<MapEncounterArea> encounterArea = std::make_shared<MapEncounterArea>(filename);

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

                        std::shared_ptr<MapLoadPoint> loadPoint = std::make_shared<MapLoadPoint>();

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

                    i++;
                }
            }

            std::map<int, std::shared_ptr<Objects::Maps::MapTile>> Map::Parser::ParseTileset(const JSONNode& node) {
                JSONNode::const_iterator i = node.begin();
                int firstGID = 1;
                std::vector<std::shared_ptr<MapTile>> tiles;

                while (i != node.end()) {
                    if (i->name() == "tileproperties" && i->type() == JSON_NODE) {
                        JSONNode::const_iterator j = i->begin();

                        while (j != i->end()) {
                            if (j->type() == JSON_NODE) {
                                JSONNode::const_iterator k = j->begin();

                                std::shared_ptr<MapTile> tile = std::make_shared<MapTile>();

                                while (k != j->end()) {
                                    if (k->name() == "filename") {
                                        tile->filename = k->as_string();
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
                std::map <int, std::shared_ptr<MapTile>> result;

                for (auto it = tiles.begin(); it != tiles.end(); it++) {
                    result[GID] = *it;

                    GID++;
                }

                return result;
            }

            // For now this just sort of dumps all tilesets into one list of tiles.
            // TODO: Maybe make some sort of TileSet class which holds the sets of
            // tiles?
            std::map<int, std::shared_ptr<MapTile>> Map::Parser::ParseTilesets(const JSONNode& node) {
                std::map<int, std::shared_ptr<Objects::Maps::MapTile>> tiles;

                JSONNode::const_iterator i = node.begin();

                while (i != node.end()) {
                    if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
                        std::map<int, std::shared_ptr<MapTile>> t = this->ParseTileset(*i);

                        tiles.insert(t.begin(), t.end());
                    }

                    i++;
                }

                return tiles;
            }

            void Map::LuaInterface::Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext) {
                //const char LuaGameMap::className[] = "GameMap";
                //
                //Lunar<LuaGameMap>::RegType LuaGameMap::methods[] = {
                //    {"getWidth", &LuaGameMap::getWidth},
                //    {"getHeight", &LuaGameMap::getHeight},
                //    {"render", &LuaGameMap::render},
                //    {"getWalkability", &LuaGameMap::getWalkability},
                //    {"getObjects", &LuaGameMap::getObjects},
                //    {0, 0}
                //};


                //class LuaGameMap {
                //public:
                //    static const char className[];
                //    static Lunar<LuaGameMap>::RegType methods[];
                //
                //    Log::Logger* logger;
                //
                //    LuaGameMap(lua_State *L) {
                //        this->logger = new Log::Logger("lua");
                //
                //        const int argc = lua_gettop(L);
                //
                //        if (argc == 1) {
                //            this->realObject = (GameMap*)lua_touserdata(L, 1);
                //        } else {
                //            this->realObject = new GameMap();
                //        }
                //    }
                //
                //    ~LuaGameMap() {
                //        delete this->realObject;
                //    }
                //
                //    int getWidth(lua_State *L) {
                //        lua_pushnumber(L, this->realObject->width);
                //
                //        return 1;
                //    }
                //
                //    int getHeight(lua_State *L) {
                //        lua_pushnumber(L, this->realObject->height);
                //
                //        return 1;
                //    }
                //
                //    int render(lua_State *L) {
                //        const int xOffset = (int)luaL_checkinteger(L, 1);
                //        const int yOffset = (int)luaL_checkinteger(L, 2);
                //        const int xMovementOffset = (int)luaL_checkinteger(L, 3);
                //        const int yMovementOffset = (int)luaL_checkinteger(L, 4);
                //
                //        this->realObject->Render(xOffset, yOffset, xMovementOffset, yMovementOffset);
                //
                //        return 0;
                //    }
                //
                //    int getWalkability(lua_State *L) {
                //        int x = (int)luaL_checkinteger(L, 1);
                //        int y = (int)luaL_checkinteger(L, 2);
                //
                //        this->logger->debug() << "getWalkability called for (" << x << ", " << y << ")";
                //
                //        lua_pushboolean(L, this->realObject->GetWalkability(x, y));
                //
                //        return 1;
                //    }
                //
                //    int getObjects(lua_State *L) {
                //        const int x = luaL_checkint(L, 1);
                //
                //        const int y = (int)luaL_checkint(L, 2);
                //
                //        this->logger->debug() << "LuaGameMap::getObjects (" << x << ", " << y << ")";
                //
                //        std::vector<GameMapObject*> result = this->realObject->GetObjects(x, y);
                //
                //        lua_newtable(L);
                //
                //        int n = 1;
                //
                //        for (std::vector<GameMapObject*>::iterator object = result.begin(); object != result.end(); object++) {
                //            lua_pushlightuserdata(L, (void*)(*object));
                //
                //            lua_rawseti(L, -2, n);
                //
                //            n += 1;
                //        }
                //
                //        return 1;
                //    }
                //
                //private:
                //    GameMap* realObject;
                //};
            }
        }
    }
}
