//
// SENAC BCC PI 2
// Projeto Betelgeuse
//
// engine.h
// centraliza as declarações do motor do jogo, ou seja
// tudo que não é conteúdo ou regra específica do jogo.

#ifndef betelgeuse_engine_engine_h
#define betelgeuse_engine_engine_h

// std includes
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

// platform specifics globals
#include <allegro5/allegro.h>
ALLEGRO_DISPLAY *window;
ALLEGRO_EVENT_QUEUE *queue;

// engine droplets includes
#include "logger.h"
#include "utils.h"

/// ----------------------------- STRUCTS and DEFINITIONS

// forward declarations
typedef struct Game Game;

// Funções de ciclo de vida de uma cena
typedef void (*gameloopControl)(Game *game, int frame);

// Estrutura de Cena
typedef struct Scene {
    gameloopControl onEnter;
    gameloopControl onFrame;
    gameloopControl onExit;
} Scene;

void changeScene(Game *game, Scene newScene);

// Estrutura que guarda todas as informações
// da tela, largura, algura e escala
typedef struct ScreenSetup {
    unsigned int height;
    unsigned int width;
    unsigned int scaleFactor;
    unsigned int fps;
} ScreenSetup;

// Estrutura que controla as regras do jogo
// e mantém o estado atual da partida

//Game levels
int LEVEL_1[5];
int LEVEL_2[5];
int LEVEL_3[5];

//Game hints
int HINTS[3];

#define CITIES_UNTIL_VICTORY        3
#define DESTINATIONS_COUNT          4
#define POINTS_OF_INTEREST_COUNT    3

#define WEEKDAY_MON 0
#define WEEKDAY_TUE 1
#define WEEKDAY_WED 2
#define WEEKDAY_THU 3
#define WEEKDAY_FRI 4

//Determines flight direction, 0 for forward travel, 1 for backward travel
typedef struct Travel {
    bool travelForward;
} Travel;

//Initializes the hint overlay
typedef struct Hint {
    bool showHint;
} Hint;

typedef struct Place {
    char* name;
    int pinX;
    int pinY;
    char* hint;
    char* hintImageName; //Not implemented
    char* citizenImageName;

    int minutesRequired;
} Place;

typedef struct Destination {
    char *name;
    char *ost;
    char *imageName;
    int minutesRequired;
    float latitude;
    float longitude;
} Destination;

typedef struct City {
    char *name;
    char *ost;
    char *flavorText[3];
    char *imageName;
    char *smallImageName;
    float latitude;
    float longitude;

    Place pointsOfInterest[POINTS_OF_INTEREST_COUNT];
    Destination destinations[DESTINATIONS_COUNT];
} City;

typedef struct GameplayContext {
    char *stolenItemText;
    int currentTime;

    int currentCity;
    int playerDestinationChoice;
    City cities[CITIES_UNTIL_VICTORY];
} GameplayContext;

typedef unsigned char actionState;

#define KEY_IS_UP       (actionState) 0x00
#define KEY_IS_DOWN     (actionState) 0x01
#define KEY_IS_PRESSED  (actionState) 0x10
#define KEY_IS_RELEASED (actionState) 0x11

typedef struct ActionKeys {
    actionState a;
    actionState b;
    actionState up;
    actionState right;
    actionState down;
    actionState left;
} ActionKeys;

// Menu que fica sobre as telas
typedef struct MenuOverlay {
    bool visible;
} MenuOverlay;

// Estrutura que agrupa toda as outras
// e representa o jogo atual

typedef struct Game {
    bool running;
    unsigned int frame;
    unsigned int sceneFrame;
    ActionKeys keyState;
    ScreenSetup screenSetup;
    Scene currentScene;
    GameplayContext gameplayContext;
    MenuOverlay menuOverlay;
    Travel travel;
    Hint hint;
} Game;


// Funções de apoio ao motor do jogo
Game createNewGame();
void loadGameData(Game *game);
void loadGameLevel(Game *game, int level[]);
void nextFrame(Game *game);
void endGame(Game *game);

// Asset drawing
typedef unsigned int IMAGE_ASSET;
IMAGE_ASSET loadImageAsset(char* name);
void drawImageAsset(IMAGE_ASSET tag, double x, double y);
void unloadImageAsset(IMAGE_ASSET tag);
void fillRGB(Game *game, unsigned char r, unsigned char g, unsigned char b);

// Text drawing
void setTextRGBColor(unsigned char r, unsigned char g, unsigned char b);
void drawText(const char *text,  double x, double y);
void drawCentralizedText(const char *text,  double x, double y);
char *readTXT(Game *game, char *filename, int line);
void drawTime(int totalMinutes);

// Play/pause audio
typedef unsigned int AUDIO_ASSET;
void setupAudioSfxPool();
void garbageCollectAudioSfxPool();
void stopSoundtrack(Game *game);
void playSfx(Game *game, char* name);
void playSoundtrack(Game *game, char* name);

// Geometric drawing
typedef struct Point {
    float x, y;
} Point;

typedef struct Rect {
    float x, y;
    float width, height;
} Rect;

typedef struct Color {
    unsigned char r, g, b;
} Color;

void drawRect(Rect frame, Color rgb);
void drawLine(Point a, Point b, Color rgb);

// Menu overlay
#define MENU_BACK_OPTION -1

typedef char* (*optionsDatasource)(Game *game, int option);
typedef void (*optionsCallback)(Game *game, int option);

void preloadMenuAssets(Game *game);
void unloadMenuAssets(Game *game);

void showMenu(Game *game, int optionsCount, optionsDatasource datasource, optionsCallback callback);

#endif
