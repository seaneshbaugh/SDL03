#include "map_encounter_area.hpp"

namespace Game {
    namespace Objects {
        namespace Maps {
            MapEncounterArea::MapEncounterArea(const std::string& filename) {
                std::string contents;

                if (!Helpers::FileSystem::ReadFile(filename, contents)) {
                    throw;
                }

                MapEncounterArea::Parser parser;

                // See https://forum.libcinder.org/topic/solution-calling-shared-from-this-in-the-constructor
                auto wptr = std::shared_ptr<MapEncounterArea>(this, [](MapEncounterArea*){});

                parser.Parse(contents, std::static_pointer_cast<MapEncounterArea>(this->shared_from_this()));
            }

            MapEncounterArea::~MapEncounterArea() {
            }

            const std::string MapEncounterArea::Parser::logChannel = "json";

            MapEncounterArea::Parser::Parser() {
                this->logger = Services::Locator::LoggerService()->GetLogger(MapEncounterArea::Parser::logChannel);
            }

            MapEncounterArea::Parser::~Parser() {
            }

            void MapEncounterArea::Parser::Parse(const std::string& jsonString, std::shared_ptr<MapEncounterArea> encounterArea) {
                json encounterAreaNode = json::parse(jsonString);

                this->logger->debug() << "Parsing mobs.";

                for (auto i = encounterAreaNode["mobs"].begin(); i != encounterAreaNode["mobs"].end(); ++i) {
                    std::vector<std::string> mob;

                    for (auto j = (*i)["enemies"].begin(); j != (*i)["enemies"].end(); ++j) {
                        mob.push_back(j->get<std::string>());
                    }

                    encounterArea->mobs.push_back(mob);

                    this->logger->debug() << "Parsed mob [" << Helpers::String::Join(mob, ", ") << "].";
                }
            }

            void MapEncounterArea::LuaInterface::Bind(std::shared_ptr<sol::state> luaState) {
                sol::table objects = (*luaState.get())["objects"].get_or_create<sol::table>(sol::new_table());

                objects.new_usertype<MapEncounterArea>("MapEncounterArea",
                                                       sol::constructors<MapEncounterArea(const std::string&)>(),
                                                       "getX", &MapObject::x,
                                                       "getY", &MapObject::y,
                                                       "getType", &MapObject::GetType,
                                                       "getProperty", &MapObject::GetProperty,
                                                       "setProperty", &MapObject::SetProperty,
                                                       sol::base_classes, sol::bases<MapObject>()
                                                       );
            }
        }
    }
}
