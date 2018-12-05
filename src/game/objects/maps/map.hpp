#ifndef SDL03_Game_Objects_Maps_Map
#define SDL03_Game_Objects_Maps_Map

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "LoggerCpp.h"
#include "libjson.h"

#include "../../../helpers/file_system.hpp"
#include "../../../helpers/path.hpp"

#include "../base.hpp"
#include "map_object.hpp"
#include "map_layer.hpp"
#include "map_tile.hpp"
#include "map_encounter_area.hpp"
#include "map_load_point.hpp"

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
                void Render(const int xOffset, const int yOffset, const int xMovementOffset, const int yMovementOffset);
                bool GetWalkability(const int x, const int y);
                std::vector <std::shared_ptr<MapObject>> GetObjects(const int x, const int y);

            private:
                static const std::string logChannel;

                // TODO: Reconsider this function.
                void SetNameFromFilename();

                class Parser {
                public:
                    Parser();
                    ~Parser();
                    void Parse(const std::string& jsonString, std::shared_ptr<Map> map);

                private:
                    static const std::string logChannel;

                    std::shared_ptr<Log::Logger> logger;
                    std::shared_ptr<MapLayer> ParseLayer(const JSONNode& node);
                    std::vector<std::shared_ptr<MapLayer>> ParseLayers(const JSONNode& node);
                    void ParseLayerData(const JSONNode& node, std::shared_ptr<MapLayer> layer);
                    void ParseLayerObjects(const JSONNode& node, std::shared_ptr<MapLayer> layer);
                    std::map<int, std::shared_ptr<MapTile>> ParseTileset(const JSONNode& node);
                    std::map<int, std::shared_ptr<MapTile>> ParseTilesets(const JSONNode& node);
                };

            public:
                class LuaInterface {
                public:
                    static void Bind(std::shared_ptr<LuaIntf::LuaContext> luaContext);
                };
            };
        }
    }
}

#endif
