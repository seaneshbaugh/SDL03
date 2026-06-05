#include "pathfinder.hpp"
#include "scene.hpp"

using Location = std::pair<int, int>;
using FrontierElement = std::pair<double, Location>;

namespace std {
    template <> struct hash<Location> {
        std::size_t operator()(const Location& id) const noexcept {
            return std::hash<int>()(id.first ^ (id.second << 16));
        }
    };
}

namespace Game {
    namespace Scenes {
        const std::string Pathfinder::logChannel = "scene";

        Pathfinder::Pathfinder(Scene* scene) : scene(scene) {
            this->logger = Services::Locator::LoggerService()->GetLogger(Pathfinder::logChannel);
        }

        Pathfinder::~Pathfinder() {
        }

        std::vector<Actor::Direction> Pathfinder::Pathfind(const Actor* actor, const int targetX, const int targetY) {
            std::priority_queue<FrontierElement, std::vector<FrontierElement>, std::greater<FrontierElement>> frontier;
            std::unordered_map<Location, Location> cameFrom;
            std::unordered_map<Location, double> costSoFar;
            Location start = std::make_pair(actor->GetCurrentTileX(), actor->GetCurrentTileY());

            frontier.emplace(0.0, start);
            cameFrom[start] = start;
            costSoFar[start] = 0.0;

            while (!frontier.empty()) {
                Location current = frontier.top().second;

                frontier.pop();

                if (current.first == targetX && current.second == targetY) {
                    break;
                }

                std::vector<Location> potentialNeighbors = {
                    {current.first, current.second - 1},
                    {current.first + 1, current.second},
                    {current.first, current.second + 1},
                    {current.first - 1, current.second}};

                std::vector<Location> neighbors;

                for (Location potentialNeighbor : potentialNeighbors) {
                    if (potentialNeighbor.first < 0 || potentialNeighbor.first >= this->scene->GetCurrentMap()->width || potentialNeighbor.second < 0 || potentialNeighbor.second >= this->scene->GetCurrentMap()->height) {
                        continue;
                    }

                    if (!this->scene->GetCurrentMap()->GetWalkability(potentialNeighbor.first, potentialNeighbor.second)) {
                        continue;
                    }

                    if (this->scene->IsTileBlocked(potentialNeighbor.first, potentialNeighbor.second, actor)) {
                        continue;
                    }

                    neighbors.push_back(potentialNeighbor);
                }

                for (const auto& next : neighbors) {
                    double newCost = costSoFar[current] + 1.0;

                    if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                        costSoFar[next] = newCost;
                        double priority = newCost + std::hypot(targetX - next.first, targetY - next.second);
                        frontier.emplace(priority, next);
                        cameFrom[next] = current;
                    }
                }
            }

            std::vector<Scenes::Actor::Direction> path;
            Location current = std::make_pair(targetX, targetY);

            if (cameFrom.find(current) == cameFrom.end()) {
                return {};
            }

            while (current != start) {
                Location previous = cameFrom[current];

                if (previous.first == current.first) {
                    if (previous.second < current.second) {
                        path.push_back(Scenes::Actor::Direction::Down);
                    } else {
                        path.push_back(Scenes::Actor::Direction::Up);
                    }
                } else if (previous.second == current.second) {
                    if (previous.first < current.first) {
                        path.push_back(Scenes::Actor::Direction::Right);
                    } else {
                        path.push_back(Scenes::Actor::Direction::Left);
                    }
                }

                current = previous;
            }

            std::reverse(path.begin(), path.end());

            return path;
        }
    }
}
