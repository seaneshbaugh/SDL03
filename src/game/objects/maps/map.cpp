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

                this->logger->debug() << "Parsing map file.";

                // See https://forum.libcinder.org/topic/solution-calling-shared-from-this-in-the-constructor
                auto wptr = std::shared_ptr<Map>(this, [](Map*){});

                mapParser.Parse(jsonString, std::static_pointer_cast<Map>(this->shared_from_this()));

                this->logger->debug() << "Parsed map file.";

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

                this->filename = filename;

                this->name = Helpers::Path::BaseName(this->filename, true);

                std::string jsonString;

                if (!Helpers::FileSystem::ReadFile(filename, jsonString)) {
                    this->logger->error() << "Failed to load map.";

                    return false;
                }

                if (!this->ParseMapFile(jsonString)) {
                    this->logger->error() << "Failed to parse map.";

                    return false;
                }

                for (auto layer = this->layers.begin(); layer != this->layers.end(); ++layer) {
                    if ((*layer)->name == "walkability") {
                        this->walkabilityLayer = *layer;

                        break;
                    }
                }

                if (!this->LoadTextures()) {
                    this->logger->error() << "Failed to load textures";

                    return false;
                }

                this->logger->info() << "Map loaded.";

                return true;
            }

            int Map::GetWidth() {
                return this->width;
            }

            int Map::GetHeight() {
                return this->height;
            }

            bool Map::GetWalkability(int x, int y) {
                if (x < 0 || y < 0 || x > this->width - 1 || y > this->height - 1) {
                    return false;
                }

                return this->tiles[this->walkabilityLayer->tiles[(y * this->width) + x]]->name == "walkable";
            }

            std::vector<std::shared_ptr<MapObject>> Map::GetObjects(int x, int y) {
                std::vector<std::shared_ptr<MapObject>> result;

                for (auto layer = this->layers.begin(); layer != this->layers.end(); ++layer) {
                    this->logger->debug() << "Getting objects for layer \"" << (*layer)->name << "\" at (" << x << ", " << y << ").";

                    for (auto object = (*layer)->objects.begin(); object != (*layer)->objects.end(); ++object) {
                        if (x >= (*object)->x && x < ((*object)->x + (*object)->width) && y >= (*object)->y && y < ((*object)->y + (*object)->height)) {
                            this->logger->debug() << "Object with type " << (*object)->GetType() << " (" << (*object)->x << ", " << (*object)->y << ") - (" << ((*object)->x + (*object)->width) << ", " << ((*object)->y + (*object)->height) << ") stepped on";

                            result.push_back(*object);
                        }
                    }
                }

                return result;
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

                for (auto layer = this->layers.begin(); layer != this->layers.end(); ++layer) {
                    // TODO: Add visible flag to layers and use that rather than comparing strings
                    // every single frame.
                    if ((*layer)->name == "walkability") {
                        continue;
                    }

                    int x = 0;
                    int y = 0;

                    for (auto tile = (*layer)->tiles.begin(); tile != (*layer)->tiles.end(); ++tile) {
                        if (x >= this->width) {
                            x = 0;

                            y++;
                        }

                        if (*tile != 0) {
                            SDL_Rect tilePosition = {((x - xOffset) * 32) + xMovementOffset, ((y - yOffset) * 32) + yMovementOffset, 32, 32};

                            Services::Locator::VideoService()->Render(this->tiles[*tile]->texture, nullptr, &tilePosition);
                        }

                        x++;
                    }
                }
            }

            const std::string Map::Parser::logChannel = "json";

            Map::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(Map::Parser::logChannel);
            }

            Map::Parser::~Parser() {
            }

            // TODO: Strongly consider making ALL parse methods take a json node as an argument
            // instead of a string. Turning a string into a json node, while technically "parsing"
            // isn't the "parsing" I have in mind here. This would allow for a greater amount of
            // consistency between parsing functions and make moving them into their own nested
            // classes easier. The Map::Parser shouldn't need to know anything about what it means
            // to parse a MapLayer. A hypothetical MapLayer::Parser should be able to handle that
            // and have a cosnsitent interface with a hypothetical MapObject::Parser class.
            void Map::Parser::Parse(const std::string& jsonString, std::shared_ptr<Map> map) {
                this->logger->debug() << "Parsing map file.";

                this->mapName = map->name;

                json mapNode = json::parse(jsonString);

                map->tilewidth = mapNode["tilewidth"].get<int>();
                map->tileheight = mapNode["tileheight"].get<int>();
                map->width = mapNode["width"].get<int>();
                map->height = mapNode["height"].get<int>();

                this->tilewidth = map->tilewidth;
                this->tileheight = map->tileheight;

                map->layers = this->ParseLayers(mapNode["layers"]);

                map->tiles = this->ParseTilesets(mapNode["tilesets"]);

                this->logger->debug() << "Parsed map file.";
            }

            // TODO: Move this to a nested class for MapLayer.
            std::shared_ptr<MapLayer> Map::Parser::ParseLayer(const json& node) {
                std::shared_ptr<MapLayer> layer = std::make_shared<MapLayer>();

                this->logger->debug() << "Parsing map layer.";

                layer->name = node["name"].get<std::string>();

                if (node.find("data") != node.end()) {
                    this->ParseLayerData(node["data"], layer);
                }

                if (node.find("objects") != node.end()) {
                    this->ParseLayerObjects(node["objects"], layer);
                }

                this->logger->debug() << "Parsed map layer \"" << layer->name << "\".";

                return layer;
            }

            std::vector<std::shared_ptr<MapLayer>> Map::Parser::ParseLayers(const json& node) {
                std::vector<std::shared_ptr<Objects::Maps::MapLayer>> layers;

                this->logger->debug() << "Parsing map layers.";

                for (auto it = node.begin(); it != node.end(); ++it) {
                    if (it->is_object()) {
                        layers.push_back(this->ParseLayer(*it));
                    }
                }

                this->logger->debug() << "Parsed map layers";

                return layers;
            }

            void Map::Parser::ParseLayerData(const json &node, std::shared_ptr<MapLayer> layer) {
                for (auto it = node.begin(); it != node.end(); ++it) {
                    layer->tiles.push_back(it->get<int>());
                }
            }

            std::vector<std::shared_ptr<MapEncounterArea>> Map::Parser::ParseEncounterAreas(const json& node) {
                std::vector<std::shared_ptr<MapEncounterArea>> encounterAreas;

                for (auto i = node.begin(); i != node.end(); ++i) {
                    this->logger->info() << "Creating encounter area.";

                    std::map<std::string, std::string> properties;

                    for  (auto j = (*i)["properties"].begin(); j != (*i)["properties"].end(); ++j) {
                        const std::string name = (*j)["name"].get<std::string>();
                        const std::string value = (*j)["value"].get<std::string>();

                        properties[name] = value;
                    }

                    std::string filename = "resources/encounter_areas/" + this->mapName + "-" + properties["name"] + ".json";

                    std::shared_ptr<MapEncounterArea> encounterArea = std::make_shared<MapEncounterArea>(filename);

                    encounterArea->SetType((*i)["type"].get<std::string>());
                    encounterArea->x = (*i)["x"].get<int>() / this->tilewidth;
                    encounterArea->y = (*i)["y"].get<int>() / this->tileheight;
                    encounterArea->width = (*i)["width"].get<int>() / this->tilewidth;
                    encounterArea->height = (*i)["height"].get<int>() / this->tileheight;

                    encounterArea->SetProperties(properties);

                    encounterAreas.push_back(encounterArea);
                }

                return encounterAreas;
            }

            std::vector<std::shared_ptr<MapLoadPoint>> Map::Parser::ParseLoadPoints(const json& node) {
                std::vector<std::shared_ptr<MapLoadPoint>> loadPoints;

                for (auto i = node.begin(); i != node.end(); ++i) {
                    this->logger->debug() << "Creating load point.";

                    std::shared_ptr<MapLoadPoint> loadPoint = std::make_shared<MapLoadPoint>();

                    std::map<std::string, std::string> properties;

                    for  (auto j = (*i)["properties"].begin(); j != (*i)["properties"].end(); ++j) {
                        const std::string name = (*j)["name"].get<std::string>();
                        const std::string value = (*j)["value"].get<std::string>();

                        properties[name] = value;
                    }

                    loadPoint->SetType((*i)["type"].get<std::string>());
                    loadPoint->x = (*i)["x"].get<int>() / this->tilewidth;
                    loadPoint->y = (*i)["y"].get<int>() / this->tileheight;
                    loadPoint->width = (*i)["width"].get<int>() / this->tilewidth;
                    loadPoint->height = (*i)["height"].get<int>() / this->tileheight;

                    loadPoint->SetProperties(properties);

                    loadPoints.push_back(loadPoint);
                }

                return loadPoints;
            }

            // TODO: Figure out a better way to handle this. Maybe some sort of map between layer name
            // (which should really layer type as some sort of enum) and a parsing function. This
            // works for now.
            void Map::Parser::ParseLayerObjects(const json& node, std::shared_ptr<MapLayer> layer) {
                if (layer->name == "encounter_areas") {
                    this->logger->debug() << "Parsing encounter area layer objects.";

                    std::vector<std::shared_ptr<MapEncounterArea>> encounterAreas = this->ParseEncounterAreas(node);

                    layer->objects.insert(layer->objects.end(), encounterAreas.begin(), encounterAreas.end());
                } else if (layer->name == "load_points") {
                    this->logger->debug() << "Parsing load point layer objects.";

                    std::vector<std::shared_ptr<MapLoadPoint>> loadPoints = this->ParseLoadPoints(node);

                    layer->objects.insert(layer->objects.end(), loadPoints.begin(), loadPoints.end());
                } else {
                    // TODO: Maybe just skip unknown object layer types? I can see a possible need for
                    // layers of objects that are only used for the map editor or for some other sort
                    // of informational purpose (comments? notes?) that aren't used by the game. But
                    // that thought makes me think that maybe I need some sort of asset pipeline to
                    // take the raw assets and strip them down and condition them for final use. But
                    // in that case why use JSON and not some sort of custom binary format that closely
                    // mirrors how things laid out at runtime? I dunno.
                    throw std::runtime_error("Unknown map object layer type.");
                }
            }

            std::map<int, std::shared_ptr<Objects::Maps::MapTile>> Map::Parser::ParseTileset(const json& node) {
                std::vector<std::shared_ptr<MapTile>> tiles;

                for (auto i = node["tiles"].begin(); i != node["tiles"].end(); ++i) {
                    std::shared_ptr<MapTile> tile = std::make_shared<MapTile>();

                    std::map<std::string, std::string> properties;

                    for  (auto j = (*i)["properties"].begin(); j != (*i)["properties"].end(); ++j) {
                        const std::string name = (*j)["name"].get<std::string>();
                        const std::string value = (*j)["value"].get<std::string>();

                        properties[name] = value;
                    }

                    tile->filename = properties["filename"];
                    tile->name = properties["name"];
                    tile->width = (*i)["imagewidth"];
                    tile->height = (*i)["imageheight"];

                    tiles.push_back(tile);
                }

                int gid = node["firstgid"].get<int>();
                std::map <int, std::shared_ptr<MapTile>> result;

                for (auto it = tiles.begin(); it != tiles.end(); ++it) {
                    result[gid] = *it;

                    gid++;
                }

                return result;
            }

            // For now this just sort of dumps all tilesets into one list of tiles.
            // TODO: Maybe make some sort of TileSet class which holds the sets of
            // tiles?
            std::map<int, std::shared_ptr<MapTile>> Map::Parser::ParseTilesets(const json& node) {
                std::map<int, std::shared_ptr<Objects::Maps::MapTile>> tiles;

                for (auto it = node.begin(); it != node.end(); ++it) {
                    std::map<int, std::shared_ptr<MapTile>> t = this->ParseTileset(*it);

                    tiles.insert(t.begin(), t.end());
                }

                return tiles;
            }

            void Map::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                objects.new_usertype<Map>("Map",
                                          sol::constructors<Map(), Map(const std::string&)>(),
                                          "getWidth", &Map::GetWidth,
                                          "getHeight", &Map::GetHeight,
                                          "getWalkability", &Map::GetWalkability,
                                          "getObjects", &Map::GetObjects,
                                          "render", &Map::Render
                                          );
            }
        }
    }
}
