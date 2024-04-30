#include "Monster.h"
#include "Bars_Health.h"
#include "Menu.h"
#include "Time.h"

SDL_Window *appWindow;
SDL_Renderer *appRenderer;
SDL_Event appEvent;
Mix_Music *bgMusic;
TTF_Font *titleFont;
TTF_Font *menuFont;
using namespace std;

vector<Creature *> SpawnCreatures(int numberOfCreatures, int creatureType, double initialPosition, double finalPosition) {
    vector<Creature *> creatures;
    Creature *creaturePool = new Creature[20];
    string imagePath;
    if (creatureType == 1) imagePath = "Creature/Type1/creature_run_left.png";
    else if (creatureType == 3) imagePath = "Creature/Type3/creature_run_left.png";
    for (int i = 0; i < numberOfCreatures; i++) {
        Creature *creature = (creaturePool + i);
        creature->setType(creatureType);
        if (creature != NULL) {
            creature->loadImage(imagePath, appRenderer);
            creature->initializeClips();
            creature->setXPosition((initialPosition + i * ((finalPosition - initialPosition) / numberOfCreatures)) * TILE_SIZE);
            creature->setMidPoint(creature->getXPosition());
            creature->setYPosition(2 * TILE_SIZE);
            creature->setMovementRange(creature->getXPosition());
            creature->initializeHealth(creatureType);
            creatures.push_back(creature);
        }
    }
    return creatures;
}

void ManageCreatures(vector<Creature *> &creatures, GameMap &map, Player &player) {
    for (int i = 0; i < creatures.size(); i++) {
        Creature *creature = creatures[i];
        if (creature != nullptr && creature->isDefeated()) {
            creatures[i] = nullptr;
            continue;
        }
        if (creature != nullptr && creature->detectPlayer(map)) {
            creature->updateMapPosition(map.startX, map.startY);
            creature->move(map);
            if (player.getXPosition() < creature->getStart() || player.getXPosition() > creature->getEnd()) {
                creature->patrolArea();
            } else {
                creature->engage(player);
                creature->attack(player);
                creature->receiveDamage(player);
            }
            creature->render(appRenderer);
        } else if (creature != nullptr && !creature->detectPlayer(map)) {
            if (player.getXPosition() + SCREEN_WIDTH >= creature->getXPosition() &&
                player.getXPosition() - SCREEN_WIDTH <= creature->getXPosition()) {
                creature->updateMapPosition(map.startX, map.startY);
                creature->move(map);
                creature->patrolArea();
                creature->render(appRenderer);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    initializeSDL(appWindow, appRenderer);
    bool gameEnded = false;
    bool gameActive = false;
    Chrono gameChrono;
    bgMusic = Mix_LoadMUS("background_music.mp3");
    Mix_PlayMusic(bgMusic, -1);
    SDL_Texture *startBackground = loadTexture("Interface/StartScreen.jpg", appRenderer);

    UserInterface mainMenu;
    mainMenu.setMenuType(0);
    mainMenu.setMenuColor();

    UserInterface endMenu;
    endMenu.setMenuType(1);
    endMenu.setMenuColor();

    titleFont = TTF_OpenFont("fonts/game_font.ttf", 40);
    menuFont = TTF_OpenFont("fonts/game_font.ttf", 80);

    while (!mainMenu.isMenuActive()) {
        while (SDL_PollEvent(&appEvent) != 0) {
            if (appEvent.type == SDL_QUIT) {
                mainMenu.setExitFlag(true);
            }
            mainMenu.handleMenuInput(appEvent, appRenderer);
        }
        SDL_SetRenderDrawColor(appRenderer, 255, 255, 255, 255);
        SDL_RenderClear(appRenderer);
        SDL_RenderCopy(appRenderer, startBackground, NULL, NULL);
        mainMenu.displayMenu(menuFont, appRenderer);
        SDL_RenderPresent(appRenderer);
    }

    if (mainMenu.isMenuActive()) {
        gameActive = true;
        endMenu.setMenuActive(true);
        startBackground = loadTexture("images/game_background.jpg", appRenderer);
    }

    while (gameActive) {
        if (endMenu.isMenuActive()) {
            GameMap currentMap;
            currentMap.loadMapData();
            currentMap.loadTiles(appRenderer);

            Player gamePlayer;
            gamePlayer.setYPosition(0 * TILE_SIZE);
            gamePlayer.loadImage("Player/player_stand_right.png", appRenderer);
            gamePlayer.initializeFrame();
            vector<Creature *> creatures = SpawnCreatures(10, 1, 120, 117);
            HealthSystem healthSystem;
            healthSystem.setHealthType(0);
            healthSystem.loadImage("Health/HealthBar.png", appRenderer);
            healthSystem.initializeFrame();
            string finalScore;
            Text elapsedTimeText;
            Text scoreText;
            elapsedTimeText.setColor(Text::WHITE_TEXT);
            scoreText.setColor(Text::WHITE_TEXT);
            SDL_RenderCopy(appRenderer, startBackground, NULL, NULL);
            gameChrono.start();
            while (!gameEnded) {
                gameChrono.startFrame();
                while (SDL_PollEvent(&appEvent) != 0) {
                    if (appEvent.type == SDL_QUIT) {
                        gameEnded = true;
                        endMenu.setMenuActive(false);
                        gameActive = false;
                    }
                    if (gamePlayer.isDead() || gamePlayer.hasWon()) {
                        endMenu.checkWinStatus(gamePlayer.hasWon());
                        gameEnded = true;
                    }
                    gamePlayer.processInput(appEvent, appRenderer);
                }
                SDL_SetRenderDrawColor(appRenderer, 255, 255, 255, 255);
                SDL_RenderClear(appRenderer);
                SDL_RenderCopy(appRenderer, startBackground, NULL, NULL);
                GameMap activeMap = currentMap.getMap();
                gamePlayer.updateMapPosition(activeMap.startX, activeMap.startY);
                gamePlayer.move(activeMap);
                gamePlayer.detectDamage();
                gamePlayer.updateFrame();
                gamePlayer.collectPoints(activeMap);
                gamePlayer.unlockPath(activeMap);
                gamePlayer.render(appRenderer);
                gamePlayer.resetDamageFlags();
                healthSystem.render(gamePlayer, appRenderer);
                currentMap.setMap(activeMap);
                currentMap.drawMap(appRenderer);
                ManageCreatures(creatures, activeMap, gamePlayer);
                string timeDisplay = "Time: ";
                Uint32 elapsedTime = gameChrono.getElapsedTime() / 1000;
                timeDisplay += to_string(elapsedTime);
                elapsedTimeText.setText(timeDisplay);
                elapsedTimeText.loadFromRenderedText(titleFont, appRenderer);
                elapsedTimeText.render(appRenderer, SCREEN_WIDTH - 2.5 * TILE_SIZE, 15);

                string scoreDisplay = "Score: ";
                Uint32 points = gamePlayer.getPoints();
                scoreDisplay += to_string(points);
                scoreText.setText(scoreDisplay);
                scoreText.loadFromRenderedText(titleFont, appRenderer);
                scoreText.render(appRenderer, SCREEN_WIDTH / 2 - TILE_SIZE, 15);

                finalScore = "YOUR SCORE: " + to_string(elapsedTime) + "s";

                SDL_RenderPresent(appRenderer);

                int frameTime = gameChrono.getFrameTime();
                if (frameTime < FRAME_DELAY) {
                    SDL_Delay(FRAME_DELAY - frameTime);
                }
            }

            SDL_RenderClear(appRenderer);

            if (endMenu.isMenuActive()) {
                if (endMenu.hasWon()) {
                    startBackground = loadTexture("Interface/WinScreen.jpg", appRenderer);
                    endMenu.displayFinalScore(finalScore);
                } else {
                    startBackground = loadTexture("Interface/LoseScreen.jpg", appRenderer);
                }
                endMenu.setMenuColor();
            }

            while (!endMenu.isMenuClosed() && endMenu.isMenuActive()) {
                SDL_SetRenderDrawColor(appRenderer, 255, 255, 255, 255);
                SDL_RenderClear(appRenderer);
                SDL_RenderCopy(appRenderer, startBackground, NULL, NULL);
                endMenu.displayMenu(menuFont, appRenderer);
                SDL_RenderPresent(appRenderer);
                while (SDL_PollEvent(&appEvent) != 0) {
                    if (appEvent.type == SDL_QUIT) {
                        endMenu.setMenuActive(false);
                        endMenu.setMenuClosed(true);
                        gameActive = false;
                    }
                    endMenu.handleMenuInput(appEvent, appRenderer);
                }
            }
            if (endMenu.isMenuActive()) {
                startBackground = loadTexture("images/game_background.jpg", appRenderer);
                gameEnded = false;
                endMenu.setMenuClosed(false);
            } else {
                gameActive = false;
            }
        }
    }

    return 0;
}
