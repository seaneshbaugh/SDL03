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
                JSONNode encounterAreaNode = libjson::parse(jsonString);

                for (auto i = encounterAreaNode.begin(); i != encounterAreaNode.end(); ++i) {
                    if (i->name() == "mobs" && i->type() == JSON_ARRAY) {
                        this->logger->debug() << "parsing mobs";

                        for (auto j = i->begin(); j != i->end(); ++j) {
                            if (j->type() == JSON_NODE) {
                                for (auto k = j->begin(); k != j->end(); ++k) {
                                    if (k->name() == "enemies") {
                                        this->logger->debug() << "parsing enemies for mob";

                                        std::string r = "";
                                        std::vector<std::string> mob;

                                        for (auto l = k->begin(); l != k->end(); ++l) {
                                            if (l->type() == JSON_STRING) {
                                                r += l->as_string() + ", ";
                                                mob.push_back(l->as_string());
                                            }
                                        }

                                        this->logger->debug() << "parsed mob [" << r << "]";

                                        encounterArea->mobs.push_back(mob);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
