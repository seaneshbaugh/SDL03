#include "GameMapEncounterAreaParser.h"
#include "GameMapEncounterArea.h"

GameMapEncounterAreaParser::GameMapEncounterAreaParser() {
    this->logger = new Log::Logger("json");
}

GameMapEncounterAreaParser::~GameMapEncounterAreaParser() {
    if (this->logger != nullptr) {
        delete this->logger;
    }
}

bool GameMapEncounterAreaParser::Parse(std::string json, GameMapEncounterArea *encounterArea) {
    JSONNode assetListNode = libjson::parse(json);

//    std::function<void(const JSONNode&, std::string)> parseJSON = [&] (const JSONNode &node, std::string parentName) {
        JSONNode::const_iterator i = assetListNode.begin();

        while (i != assetListNode.end()) {
            if (i->name() == "mobs" && i->type() == JSON_ARRAY) {
                this->logger->debug() << "parsing mobs";
                JSONNode::const_iterator j = i->begin();

                while (j != i->end()) {
                    if (j->type() == JSON_NODE) {
                        JSONNode::const_iterator k = j->begin();

                        while (k != j->end()) {
                            if (k->name() == "enemies") {
                                this->logger->debug() << "parsing enemies for mob";
                                JSONNode::const_iterator l = k->begin();

                                std::string r = "";
                                std::vector<std::string> mob;

                                while (l != k->end()) {
                                    this->logger->debug() << std::setfill ('0') << std::setw(sizeof(unsigned char)*2)
                                    << std::hex << l->type();

                                    if (l->type() == JSON_STRING) {
                                        r += l->as_string() + ", ";
                                        mob.push_back(l->as_string());
                                    }

                                    l++;
                                }


                                this->logger->debug() << "parsed mob [" << r << "]";

                                encounterArea->mobs.push_back(mob);
                            }

                            k++;
                        }
                    }

                    j++;
                }
            }

//            std::string name;
//            if (parentName == "") {
//                name = i->name();
//            } else {
//                name = parentName + "." + i->name();
//            }
//
//            if (i->type() == JSON_ARRAY || i->type() == JSON_NODE) {
//                parseJSON(*i, name);
//            }
//
//            if (i->type() == JSON_STRING) {
//                std::cout << name << " => " << i->as_string() << std::endl;
//                (*encounterArea)->mobs = i->as_string();
//            }

            i++;
        }
//    };

//    parseJSON(assetListNode, "");

    return true;
}
