#include "GameMap.h"
#include "MapParser.h"

SDL_Renderer* GameMap::renderer;

const char LuaGameMap::className[] = "GameMap";

Lunar<LuaGameMap>::RegType LuaGameMap::methods[] = {
    {"getWidth", &LuaGameMap::getWidth},
    {"getHeight", &LuaGameMap::getHeight},
    {"render", &LuaGameMap::render},
    {"getWalkability", &LuaGameMap::getWalkability},
    {"getObjects", &LuaGameMap::getObjects},
    {0, 0}
};

GameMap::GameMap() {
    this->name = "";

    this->filename = "";

    this->width = 0;

    this->height = 0;

    this->tilewidth = 0;

    this->tileheight = 0;

    this->logger = new Log::Logger("map");
}

GameMap::GameMap(std::string filename) : GameMap() {
    this->Load(filename);
}

GameMap::~GameMap() {

}

bool GameMap::ParseMapFile(std::string jsonString) {
    MapParser mapParser = MapParser();
    this->logger->debug() << "About to parse map file";
    return mapParser.Parse(jsonString, this);
}

// Maybe this should be put in some sort of texture loading class. Same with other resources.
bool GameMap::LoadTextures() {
    for (std::map<int, GameMapTile*>::iterator tile = this->tiles.begin(); tile != this->tiles.end(); tile++) {
        GameTexture* texture = new GameTexture();
        
        this->logger->debug() << "Loading map tile texture \"assets/images/map_tiles/" << tile->second->filename << "\".";
        if (!texture->Load("assets/images/map_tiles/" + tile->second->filename)) {
            // LOL MEMORY LEAKS?
            return false;
        }
        
        this->textures[tile->second->name] = texture;
        
        tile->second->texture = texture;
    }
    
    return true;
}

bool GameMap::Load(std::string filename) {
    this->logger->info() << "Loading map \"" << filename << "\".";

    std::string jsonString;

    if (!FileSystemHelpers::ReadFile(filename, jsonString)) {
        return false;
    }

    this->logger->info() << "Read " << jsonString.size() << " bytes";

    if (!this->ParseMapFile(jsonString)) {
        this->logger->error() << "Failed to parse map";

        return false;
    }
    
    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
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

void GameMap::Render(int xOffset, int yOffset, int xMovementOffset, int yMovementOffset) {
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

    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
        if ((*layer)->name != "walkability") {
            int x = 0;

            int y = 0;

            for (std::vector<int>::iterator tile = (*layer)->tiles.begin(); tile != (*layer)->tiles.end(); tile++) {
                if (x >= this->width) {
                    x = 0;

                    y++;
                }

                if (*tile != 0) {
                    SDL_Rect tilePosition = {((x - xOffset) * 32) + xMovementOffset, ((y - yOffset) * 32) + yMovementOffset, 32, 32};

                    SDL_RenderCopy(this->renderer, this->tiles[*tile]->texture->texture, nullptr, &tilePosition);
                }
                
                x++;
            }
        }
    }
}

bool GameMap::GetWalkability(int x, int y) {
    if (x < 0 || y < 0 || x > this->width - 1 || y > this->height - 1) {
        return false;
    }

    return this->tiles[this->walkabilityLayer->tiles[(y * this->width) + x]]->name == "walkable";
}

std::vector<GameMapObject*> GameMap::GetObjects(int x, int y) {
    std::vector<GameMapObject*> result;

    for (std::vector<GameMapLayer*>::iterator layer = this->layers.begin(); layer != this->layers.end(); layer++) {
        this->logger->debug() << "GameMap::GetObjects (" << x << ", " << y << ") getting objects for layer " << (*layer)->name;
        for (std::vector<GameMapObject*>::iterator object = (*layer)->objects.begin(); object != (*layer)->objects.end(); object++) {
            if (x >= (*object)->x && x < ((*object)->x + (*object)->width) && y >= (*object)->y && y < ((*object)->y + (*object)->height)) {
                result.push_back(*object);

                this->logger->debug() << "object with type " << (*object)->type << " (" << (*object)->x << ", " << (*object)->y << ") - (" << ((*object)->x + (*object)->width) << ", " << ((*object)->y + (*object)->height) << ") stepped on";
            } else {

                this->logger->debug() << "object with type " << (*object)->type << " (" << (*object)->x << ", " << (*object)->y << ") - (" << ((*object)->x + (*object)->width) << ", " << ((*object)->y + (*object)->height) << ") not stepped on";
            }
        }
    }

    return result;
}

void GameMap::SetNameFromFilename() {
    this->name = PathHelpers::BaseName(this->filename, true);
}
