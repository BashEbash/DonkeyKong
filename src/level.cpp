#include <vector>
#include "level.hpp"
#include <vector>
#include <tuple>

GameLevel1::GameLevel1(int window_height, std::vector<int> section_dim, int slope, SDL_Renderer* renderer)
    : window_height(window_height),
    section_dim(section_dim),

    row_tops({ window_height - 2 * section_dim[1],
               window_height - 6 * section_dim[1],
               window_height - 10 * section_dim[1],
               window_height - 14 * section_dim[1],
               window_height - 18 * section_dim[1],
               window_height - 22 * section_dim[1] }),
    
    slope(section_dim[1] / slope),
    renderer(renderer)
{
    setupLevel();
}
const std::vector<std::tuple<int, int, int, int>>& GameLevel1::getTargets() const
{
    return targets;
}

std::vector<Ladder> GameLevel1::getLadders() {
    std::vector<Ladder> extractedLadders;
    extractedLadders.reserve(ladders.size()); // Reserve space for efficiency if needed

    for (const auto& ladderPtr : ladders) {
        extractedLadders.push_back(*ladderPtr); // Dereference the shared pointer to get the object
    }

    return extractedLadders;
}

std::vector<Hammer> GameLevel1::getHammers() {
    std::vector<Hammer> extractedHammers;
    extractedHammers.reserve(hammers.size()); // Reserve space for efficiency if needed

    for (const auto& hammerPtr : hammers) {
        extractedHammers.push_back(*hammerPtr); // Dereference the shared pointer to get the object
    }

    return extractedHammers;
}

std::vector<Bridge> GameLevel1::getBridges() {
    std::vector<Bridge> extractedBridges;
    extractedBridges.reserve(bridges.size()); // Reserve space for efficiency if needed

    for (const auto& bridgePtr : bridges) {
        extractedBridges.push_back(*bridgePtr); // Dereference the shared pointer to get the object
    }

    return extractedBridges;
}

void GameLevel1::setupLevel() {
    // Open file
    FILE* inputLevel;
    fopen_s(&inputLevel, "levels/level1.txt", "r");
    if (inputLevel == NULL) {
        fprintf(stderr, "Unable to open file!\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), inputLevel)) {
        char objectType[20];
        sscanf(line, "%s", objectType);

        if (strcmp(objectType, "row_tops") == 0) {
            int row_top;
            char* token = strtok(line, " ");
            while (token != NULL) {
                if (sscanf_s(token, "%d", &row_top)) {
                    row_tops.push_back(row_top);
                }
                token = strtok(NULL, " ");
            }
        }
        else if (strcmp(objectType, "bridge") == 0) {
            int x, y, length;
            sscanf_s(line, "%*s %d %d %d", &x, &y, &length);
            bridges.push_back(std::make_shared<Bridge>(x, y, length, section_dim));
        }
        else if (strcmp(objectType, "ladder") == 0) {
            int x, y, length;
            sscanf_s(line, "%*s %d %d %d", &x, &y, &length);
            ladders.push_back(std::make_shared<Ladder>(x, y, length, section_dim));
        }
        else if (strcmp(objectType, "hammer") == 0) {
            int x, y;
            sscanf_s(line, "%*s %d %d", &x, &y);
            hammers.push_back(std::make_shared<Hammer>(x, y, section_dim, renderer));
        }
        else if (strcmp(objectType, "target") == 0) {
            int x, y, width, height;
            sscanf_s(line, "%*s %d %d %d %d", &x, &y, &width, &height);
            targets.push_back(std::make_tuple(x, y, width, height));
        }
    }
    fclose(inputLevel);
}