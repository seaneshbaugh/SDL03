#include "GameMapEncounterArea.h"

GameMapEncounterArea::GameMapEncounterArea(std::string filename) {
    std::string contents;

    // Ew. This can fail silently. Gross.
    FileSystemHelpers::ReadFile(filename, contents);

    GameMapEncounterAreaParser parser;

    parser.Parse(contents, this);
}

GameMapEncounterArea::~GameMapEncounterArea() {
    
}
