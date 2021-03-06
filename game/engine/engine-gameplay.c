//
// SENAC BCC PI 2
// Projeto Betelgeuse
//
// engine-gameplay.c
// preenche a struct de jogo com as cidades, crimes, dicas
// locais de interesse, dia da semana e horário

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "engine.h"

void loadGameData(Game *game){

    //RANDOMIZE LEVELS
    //BASE: [CRIME = ATUAL,CERTO,ERRADO,ERRADO,ERRADO] [DICA1,DICA2,DICA3] -> referente ao destino certo
    //Números são referentes aos índices da array original
    //LEVEL 1:      [0,1*,2,3,4]     *destino certo
    //LEVEL 2:      [1,5*,6,7,8]     *destino certo
    //LEVEL 3:      [5,9*,10,11,12]  *fim de jogo

    int size = 16; // 16 cidades
    int array[size];
    int i;

    //Preenche o vetor com 1-16 (Número de cidades no jogo)
    for (i = 0; i < size; i++) {
        array[i] = i + 1;
    }

    //Embaralha o vetor de cidades
    srand(time(NULL));
    for(i = size-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&array[i], &array[j]);
    }

    //Preenche o level seguindo a descrição no início da função
    LEVEL_1[0] = array[0]; // Cidade atual é a cidade onde o crime aconteceu
    LEVEL_2[0] = array[1]; // Destino certo é a nova cidade atual
    LEVEL_3[0] = array[5]; // Destino certo é a nova cidade atual
    for(i = 1; i < 5; i++){
        LEVEL_1[i] = array[i];
        LEVEL_2[i] = array[i+4];
        LEVEL_3[i] = array[i+8];
    }

	game->gameplayContext.currentTime = 0; //Sets current time in minutes to 0
	game->gameplayContext.currentCity = 0;

	loadGameLevel(game, LEVEL_1); //Loads the first level
}

void loadGameLevel(Game *game, int level[]){

    //HINTS: [0,1,2] [Place 0, Place 1, Place 2] recarrega o vetor cada level para melhorar a randomização de partidas
    int sizeHints = 6; // 6 dicas por cidade
    int arrayHints[sizeHints];
    int i;

    //Preenche o vetor com 1-6 (Número de dicas por cidade)
    for (i = 0; i < sizeHints; i++) {
        arrayHints[i] = i + 1;
    }

    //Embaralha o vetor de dicas
    srand(time(NULL));
    for(i = sizeHints-1; i > 0; i--){
        int j = rand() % (i+1);
        swap(&arrayHints[i], &arrayHints[j]);
    }

    for(i = 0; i < 3; i++){ // 3 dicas por cidade para 3 pontos de interesse
        HINTS[i] = arrayHints[i];
    }

    //CRIMES -> Arquivo: crime.txt; Definir parâmetro LINE da função 'readTXT'
    //INFO CIDADES + CIDADE + IMAGEM -> Arquivo: cidade.txt; Definir parâmetro LINE da função 'readTXT'
    //DICAS -> Arquivo: dica.txt; Definir parâmetro LINE para input da função 'readTXT'

	Logger.info("loading game data");
    game->gameplayContext.stolenItemText = readTXT(game, "data/crime.txt", 3*level[0] - 2);

	// City:
	City correctCity;
    correctCity.name = readTXT(game, "data/cidade.txt", 7*level[0] - 7);
    correctCity.ost = readTXT(game, "data/cidade.txt", 7*level[0] - 5);
    correctCity.flavorText[0] = readTXT(game, "data/cidade.txt", 7*level[0] - 4);
    correctCity.flavorText[1] = readTXT(game, "data/cidade.txt", 7*level[0] - 3);
    correctCity.flavorText[2] = readTXT(game, "data/cidade.txt", 7*level[0] - 2);
    correctCity.imageName = readTXT(game, "data/dica.txt", 18*level[0] - 17);
    correctCity.smallImageName = readTXT(game, "data/cidade.txt", 7*level[0] - 6);
    correctCity.latitude  = atof(readTXT(game, "data/coordenadas.txt", 4*level[0]-3));
    correctCity.longitude = atof(readTXT(game, "data/coordenadas.txt", 4*level[0]-2));
	game->gameplayContext.cities[game->gameplayContext.currentCity] = correctCity;

	// Pontos de interesse

	Place p0;
	p0.name = readTXT(game, "data/dica.txt", 18*level[0] - 16); // 16 linhas do dicas txt
	p0.pinX = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 15));
	p0.pinY = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 14));
	p0.hint = readTXT(game, "data/dica.txt", 18*level[1] - HINTS[0] - 1);
	p0.hintImageName 	= "hint-citizen-c.png"; //Not implemented
	p0.citizenImageName = "hint-citizen-c.png";
	p0.minutesRequired 	= 120;
	game->gameplayContext.cities[game->gameplayContext.currentCity].pointsOfInterest[0] = p0;

	Place p1;
	p1.name = readTXT(game, "data/dica.txt", 18*level[0] - 13);
	p1.pinX = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 12));
	p1.pinY = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 11));
	p1.hint = readTXT(game, "data/dica.txt", 18*level[1] - HINTS[1] - 1);
	p1.hintImageName 	= "hint-citizen-c.png"; //Not implemented
	p1.citizenImageName = "hint-citizen-c.png";
	p1.minutesRequired 	= 100;
	game->gameplayContext.cities[game->gameplayContext.currentCity].pointsOfInterest[1] = p1;

	Place p2;
	p2.name = readTXT(game, "data/dica.txt", 18*level[0] - 10);
	p2.pinX = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 9));
	p2.pinY = atoi(readTXT(game, "data/dica.txt", 18*level[0] - 8));
	p2.hint = readTXT(game, "data/dica.txt", 18*level[1] - HINTS[2] - 1);
	p2.hintImageName 	= "hint-citizen-c.png"; //Not implemented
	p2.citizenImageName = "hint-citizen-c.png";
	p2.minutesRequired 	= 140;
	game->gameplayContext.cities[game->gameplayContext.currentCity].pointsOfInterest[2] = p2;

	// Destinos

    //d0 is the right choice
	Destination d0;
	d0.name = readTXT(game, "data/cidade.txt", 7*level[1] - 7);
	d0.ost = readTXT(game, "data/cidade.txt", 7*level[1] - 5);
    d0.imageName = readTXT(game, "data/dica.txt", 18*level[1] - 17);
    d0.latitude  = atof(readTXT(game, "data/coordenadas.txt", 4*level[1]-3));
    d0.longitude = atof(readTXT(game, "data/coordenadas.txt", 4*level[1]-2));
    d0.minutesRequired = travelTime(correctCity.latitude, correctCity.longitude, d0.latitude, d0.longitude);
    game->gameplayContext.cities[game->gameplayContext.currentCity].destinations[0] = d0;

    Destination d1;
	d1.name = readTXT(game, "data/cidade.txt", 7*level[2] - 7);
	d1.ost = readTXT(game, "data/cidade.txt", 7*level[2] - 5);
    d1.imageName = readTXT(game, "data/dica.txt", 18*level[2] - 17);
    d1.latitude  = atof(readTXT(game, "data/coordenadas.txt", 4*level[2]-3));
    d1.longitude = atof(readTXT(game, "data/coordenadas.txt", 4*level[2]-2));
    d1.minutesRequired = travelTime(correctCity.latitude, correctCity.longitude, d1.latitude, d1.longitude);
    game->gameplayContext.cities[game->gameplayContext.currentCity].destinations[1] = d1;

    Destination d2;
	d2.name = readTXT(game, "data/cidade.txt", 7*level[3] - 7);
	d2.ost = readTXT(game, "data/cidade.txt", 7*level[3] - 5);
    d2.imageName = readTXT(game, "data/dica.txt", 18*level[3] - 17);
    d2.latitude  = atof(readTXT(game, "data/coordenadas.txt", 4*level[3]-3));
    d2.longitude = atof(readTXT(game, "data/coordenadas.txt", 4*level[3]-2));
    d2.minutesRequired = travelTime(correctCity.latitude, correctCity.longitude, d2.latitude, d2.longitude);
    game->gameplayContext.cities[game->gameplayContext.currentCity].destinations[2] = d2;

    Destination d3;
	d3.name = readTXT(game, "data/cidade.txt", 7*level[4] - 7);
	d3.ost = readTXT(game, "data/cidade.txt", 7*level[4] - 5);
    d3.imageName = readTXT(game, "data/dica.txt", 18*level[4] - 17);
    d3.latitude  = atof(readTXT(game, "data/coordenadas.txt", 4*level[4]-3));
    d3.longitude = atof(readTXT(game, "data/coordenadas.txt", 4*level[4]-2));
    d3.minutesRequired = travelTime(correctCity.latitude, correctCity.longitude, d3.latitude, d3.longitude);
    game->gameplayContext.cities[game->gameplayContext.currentCity].destinations[3] = d3;
}
