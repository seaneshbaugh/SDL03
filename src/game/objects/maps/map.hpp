#ifndef SDL03_Game_Objects_Maps_Map
#define SDL03_Game_Objects_Maps_Map

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../../../lib/LoggerCpp/include/LoggerCpp/LoggerCpp.h"
#include "../../../../lib/nlohmann/json.hpp"

#include "../../../helpers/file_system.hpp"
#include "../../../helpers/path.hpp"

#include "../base.hpp"
#include "map_object.hpp"
#include "map_layer.hpp"
#include "map_tile.hpp"
#include "map_encounter_area.hpp"
#include "map_load_point.hpp"

using json = nlohmann::json;

namespace Game {
    namespace Parsers {
        class MapParser;
    }

    namespace Objects {
        namespace Maps {
            class Map : public Objects::Base {
            public:
                std::string name;
                std::string filename;
                int width;
                int height;
                int tilewidth;
                int tileheight;
                std::vector <std::shared_ptr<MapLayer>> layers;
                std::map <int, std::shared_ptr<MapTile>> tiles;
                std::map<std::string, std::shared_ptr<Resources::Texture>> textures;
                std::shared_ptr<MapLayer> walkabilityLayer;

                Map();
                Map(const std::string& filename);
                ~Map();
                bool ParseMapFile(const std::string& jsonString);
                bool LoadTextures();
                bool Load(const std::string& filename);
                int GetWidth();
                int GetHeight();
                bool GetWalkability(const int x, const int y);
                std::vector <std::shared_ptr<MapObject>> GetObjects(const int x, const int y);
                void Render(const int xOffset, const int yOffset, const int xMovementOffset, const int yMovementOffset);

            private:
                static const std::string logChannel;

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, std::shared_ptr<Map> map);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                    std::string mapName;
                    int tilewidth;
                    int tileheight;

                    std::shared_ptr<MapLayer> ParseLayer(const json& node);
                    std::vector<std::shared_ptr<MapLayer>> ParseLayers(const json& node);
                    void ParseLayerData(const json& node, std::shared_ptr<MapLayer> layer);
                    void ParseLayerObjects(const json& node, std::shared_ptr<MapLayer> layer);
                    std::vector<std::shared_ptr<MapEncounterArea>> ParseEncounterAreas(const json& node);
                    std::vector<std::shared_ptr<MapLoadPoint>> ParseLoadPoints(const json& node);
                    std::map<int, std::shared_ptr<MapTile>> ParseTileset(const json& node);
                    std::map<int, std::shared_ptr<MapTile>> ParseTilesets(const json& node);
                };

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<sol::state> luaState);
                };
            };
        }
    }
}

#endif
