#ifndef MAP_H
#define MAP_H
#include "malloc_world.h"
#include "case_values.h"

#define ROWS_MAP_1 12
#define COLS_MAP_1 12
#define ROWS_MAP_2 15
#define COLS_MAP_2 15
#define ROWS_MAP_3 19
#define COLS_MAP_3 19

#define _IS_WOOD_ 103
#define _IS_ROC_ 104
#define _IS_PLANT_ 105

typedef struct map
{
    int **map;
    int rank;
    int rows, cols;
    int nbTrees, nbMonsters, nbPlants, nbRocs;
    int treeVal, plantVal, rocVal;
    int lvlRequired;
}map;

int getXPlayerPos(map *myMap);
int getYPlayerPos(map *myMap);
void putTrees(map *myMap);
void putWalls(map *myMap);
void putMonsters(map *myMap);
void putPlants(map *myMap);
int getNbTpCases(map *myMap);
void putTpCases(map *myMap);
void putRocs(map *myMap);
void putPnj(map *myMap);
void putPlayer(map *myMap);
void putAll(map *myMap);
map *initMap(int rank);
void initMapToZero(map *myMap);
void initMapSetFrame(map ***myPreviousMapSetFrames, map **myMapSet);
void copyMapSet(map **dest, map **src);
void setMapSetDifferences(map **newMapSet, map **diffMapSet);

void freeMap(map *myMap)
{
    assert(myMap);
    free(myMap->map);
    free(myMap);
}

void freeMapSet(map **myMapSet, int nbOfMaps)
{
    assert(myMapSet);
    for(int i=0; i<nbOfMaps; i++)
    {
        assert(myMapSet[i]);
        freeMap(myMapSet[i]);
    }
    free(myMapSet);
}

// GETTERS FOR PLAYER POSITION
int getXPlayerPos(map *myMap)
{
    for(int i = 0; i < myMap->rows; i++)
    {
        for(int j = 0; j < myMap->cols; j++)
        {
            if(myMap->map[i][j] == 1) 
            {
                return i;
            }
        }
    }
    return 0;
}
int getYPlayerPos(map *myMap)
{
    for(int i = 0; i < myMap->rows; i++)
    {
        for(int j = 0; j < myMap->cols; j++)
        {
            if(myMap->map[i][j] == 1) 
            {
                return j;
            }
        }
    }
    return 0;
}

void putTrees(map *myMap)
{
    int nbTrees, x, y, i, val;
    // NUMBER OF TREES DEPENDS ON THE RANK OF THE MAP
    nbTrees = myMap->nbTrees;
    val = myMap->rank == 1 ? _BOIS_1_ : (myMap->rank == 2 ? _BOIS_2_ : _BOIS_3_);
    i = 0;

    while(i<nbTrees)
    {
        // RANDOM POSITION ON MAP
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        if(myMap->map[x][y]==0)
        {
            // PUT TREE VALUE ON THE [X, Y] RANDOM POSITION GENERATED
            // ONLY IF THE MAP[X, Y] CASE IS FREE
            myMap->map[x][y] = val;
            i++;
        }
    }
}

void putWalls(map *myMap)
{
    int nbWalls, x, y, i;
    // NUMBER OF WALLS DEPENDS ON THE RANK OF THE MAP
    nbWalls = (myMap->rows+myMap->cols)/2;
    i = 0;

    while(i<nbWalls)
    {
        // RANDOM POSITION ON MAP
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        if(myMap->map[x][y]== _FREE_CASE_)
        {
            // PUT WALL VALUE ON THE [X, Y] RANDOM POSITION GENERATED
            // ONLY IF THE MAP[X, Y] CASE IS FREE
            myMap->map[x][y] = _INFRANCHISSABLE_;
            i++;
        }
    }
}

void putMonsters(map *myMap)
{
    int nbMonsters, x, y, i, min_val, max_val, randMonster;
    // NUMBER OF MONSTERS DEPENDS ON THE RANK OF THE MAP
    nbMonsters = myMap->nbMonsters;
    i = 0;
    // MONSTER LEVELS DEPENDS ON THE CURRENT RANK OF THE MAP [MAP 1 : 12-44] [MAP 2 : 45-69] [MAP 3 : 70-98]
    // BOSS MONSTER IS AN EXCEPTION WITH VALUE 99
    min_val = myMap->rank == 1 ? 12 : (myMap->rank == 2 ? 45 : 70);
    max_val = myMap->rank == 1 ? 44 : (myMap->rank == 2 ? 69 : 98);

    while(i<nbMonsters)
    {
        // RANDOM POSITION ON MAP
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);

        // GENERATE A RANDOM MONSTER VALUE DEPENDING ON PREVIOUS CALCULATED VALUES
        randMonster = (rand()%(max_val - min_val +1)) + min_val;

        if(myMap->map[x][y] == 0)
        {
            // PUT MONSTER VALUE ON THE [X, Y] RANDOM POSITION GENERATED
            // ONLY IF THE MAP[X, Y] CASE IS FREE
            myMap->map[x][y] = randMonster;
            i++;
        }
    }
}

void putPlants(map *myMap)
{
    int nbPlants, x, y, i, val;
    // NUMBER OF PLANTS DEPENDS ON THE RANK OF THE MAP
    nbPlants = myMap->nbPlants;
    val = myMap->rank == 1 ? _PLANT_1_ : (myMap->rank == 2 ? _PLANT_2_ : _PLANT_3_);
    i = 0;

    while(i<nbPlants){
        // RANDOM POSITION ON MAP
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        

        if(myMap->map[x][y] == 0){
            // PUT PLANT VALUE ON THE [X, Y] RANDOM POSITION GENERATED
            // ONLY IF THE MAP[X, Y] CASE IS FREE
            myMap->map[x][y] = val;
            i++;
        }
    }
}

int getNbTpCases(map *myMap)
{
    int nbTpCases = 0;
    if(myMap->rank == 1){
        for(int i=0; i<myMap->rows; i++){
            for(int j=0; j<myMap->cols; j++){
                if(myMap->map[i][j] == -2)
                    nbTpCases += 1;
            }
        }
    }

    if(myMap->rank == 2){
        for(int i=0; i<myMap->rows; i++){
            for(int j=0; j<myMap->cols; j++){
                if(myMap->map[i][j] == -2 || myMap->map[i][j] == -3)
                    nbTpCases += 1;
            }
        }
    }

    if(myMap->rank == 3){
        for(int i=0; i<myMap->rows; i++){
            for(int j=0; j<myMap->cols; j++){
                if(myMap->map[i][j] == -3)
                    nbTpCases += 1;
            }
        }
    }

    return nbTpCases;
}

void putTpCases(map *myMap)
{
    int x, y;

    if(myMap->rank == 1){
        while(getNbTpCases(myMap) != 1){
            x = rand()%(myMap->rows);
            y = rand()%(myMap->cols);
            if(myMap->map[x][y] == _FREE_CASE_)
                myMap->map[x][y] = _TP_CASE_1_TO_2;
        }
    }

    if(myMap->rank == 3){
        while(getNbTpCases(myMap) != 1){
            x = rand()%(myMap->rows);
            y = rand()%(myMap->cols);
            if(myMap->map[x][y] == _FREE_CASE_)
                myMap->map[x][y] = _TP_CASE_3_TO_2;
        }
    }

    if(myMap->rank == 2){
        int tp_2_to_1 = 0;
        int tp_2_to_3 = 0;
        while(getNbTpCases(myMap) != 2){
            x = rand()%(myMap->rows);
            y = rand()%(myMap->cols);
            if(myMap->map[x][y] == _FREE_CASE_ && tp_2_to_1 == 0){
                myMap->map[x][y] = _TP_CASE_2_TO_1;
                tp_2_to_1 = 1;
            }
            x = rand()%(myMap->rows);
            y = rand()%(myMap->cols);
            if(myMap->map[x][y] == _FREE_CASE_ && tp_2_to_3 == 0){
                myMap->map[x][y] = _TP_CASE_2_TO_3;
                tp_2_to_3 = 1;
            }
        }
    }
}

void putRocs(map *myMap)
{
    int nbRocs, x, y, i, val;
    // NUMBER OF ROCS DEPENDS ON THE RANK OF THE MAP
    nbRocs = myMap->nbPlants;
    val = myMap->rank == 1 ? _ROC_1_ : (myMap->rank == 2 ? _ROC_2_ : _ROC_3_);
    i = 0;

    while(i<nbRocs){
        // RANDOM POSITION ON MAP
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        

        if(myMap->map[x][y] == 0){
            // PUT ROC VALUE ON THE [X, Y] RANDOM POSITION GENERATED
            // ONLY IF THE MAP[X, Y] CASE IS FREE
            myMap->map[x][y] = val;
            i++;
        }
    }
}

void putPnj(map *myMap)
{
    int i, x, y;
    i = 0;

    while(i<1){
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        // PUT PNJ VALUE ON THE [X, Y] RANDOM POSITION GENERATED
        // ONLY IF THE MAP[X, Y] CASE IS FREE
        if(myMap->map[x][y] == _FREE_CASE_){
            myMap->map[x][y] = 2;
            i++;
        }
    }
}

void putPlayer(map *myMap)
{
    int i, x, y;
    i = 0;

    while(i<1){
        x = rand()%(myMap->rows);
        y = rand()%(myMap->cols);
        // PUT PLAYER VALUE ON THE [X, Y] RANDOM POSITION GENERATED
        // ONLY IF THE MAP[X, Y] CASE IS FREE
        if(myMap->map[x][y] == _FREE_CASE_){
            myMap->map[x][y] = 1;
            i++;
        }
    }
}

void putAll(map *myMap)
{
    putWalls(myMap);
    putTrees(myMap);
    putPlants(myMap);
    putRocs(myMap);
    putMonsters(myMap);
    putTpCases(myMap);
    putPnj(myMap);
}

map *initMap(int rank)
{
    // CREATING MAP TAB OF TABS ALLOCATING INT TAB FIRSTLY ...
    int nbCase = rank == 1 ? ROWS_MAP_1*COLS_MAP_1 : (rank == 2 ? ROWS_MAP_2*COLS_MAP_2 : ROWS_MAP_3*COLS_MAP_3);
    
    map *myMap = malloc(sizeof(myMap) + nbCase + 11*sizeof(int));
    assert(myMap);
    myMap->rows = rank == 1 ? ROWS_MAP_1 : (rank == 2 ? ROWS_MAP_2 : ROWS_MAP_3);
    myMap->cols = rank == 1 ? COLS_MAP_1 : (rank == 2 ? COLS_MAP_2 : COLS_MAP_3);
    myMap->map = malloc(sizeof(int **) + + myMap->rows*sizeof(int *) + nbCase*sizeof(int));
    assert(myMap->map);
    myMap->rank = rank;
    // NUMBER OF EACH RESOURCE (TREES, PLANTS, MONSTERS, ROCS) DEPENDS ON THE RANK OF THE MAP
    myMap->nbTrees = (myMap->rows+myMap->cols)/2;
    myMap->nbMonsters = rank == 1 ? 5 : (rank == 2 ? 8 : 10);
    myMap->nbRocs = (myMap->rows+myMap->cols)/2;
    myMap->nbPlants = (myMap->rows+myMap->cols)/2;
    // TREE VALUE ON MAP DEPENDS ON THE RANK OF THE MAP [MAP 1 : 5] [MAP 2 : 8] [MAP 3 : 11]
    myMap->treeVal = myMap->rank == 1 ? 5 : (myMap->rank == 2 ? 8 : 11);
    // PLANT VALUE ON MAP DEPENDS ON THE RANK OF THE MAP [MAP 1 : 3] [MAP 2 : 6] [MAP 3 : 9]
    myMap->plantVal = myMap->rank == 1 ? 3 : (myMap->rank == 6 ? 8 : 9);
    // ROC VALUE ON MAP DEPENDS ON THE RANK OF THE MAP [MAP 1 : 4] [MAP 2 : 7] [MAP 3 : 10]
    myMap->rocVal = myMap->rank == 1 ? 4 : (myMap->rank == 2 ? 7 : 10);
    myMap->lvlRequired = myMap->rank == 1 ? 0 : (myMap->rank == 2 ? 3 : 7);
    /*
    printf("rank: %d\t", myMap->rank);
    printf("rows: %d\t", myMap->rows);
    printf("cols: %d\t", myMap->cols);
    printf("nbcases: %d\n", nbCase);
    */

    for(int i=0; i<myMap->rows; i++){
        // ALLOCATING TABS OF MAP FOR EACH ROW
        myMap->map[i] = malloc(myMap->cols *sizeof(int *) + myMap->cols*sizeof(int));
        assert(myMap->map[i]);
        // INITIALIZING EACH CASE TO '0' VALUE
        for(int j=0; j<myMap->cols; j++){
            myMap->map[i][j] = 0;
        }
    }
    // PUT EVERY RESOURCES ON THE BLANK MAP
    putAll(myMap);
    return myMap;
}

void initMapToZero(map *myMap)
{
    assert(myMap);

    for(int i=0; i<myMap->rows; i++)
    {
        for(int j=0; j<myMap->cols; j++)
        {
            myMap->map[i][j] = 0;
        }
    }
}

void initMapSet(map **myMapSet)
{
    assert(myMapSet);

    int NB_CASES_MAP_1, NB_CASES_MAP_2, NB_CASES_MAP_3;
    NB_CASES_MAP_1 = ROWS_MAP_1 * COLS_MAP_1;
    NB_CASES_MAP_2 = ROWS_MAP_2 * COLS_MAP_2;
    NB_CASES_MAP_3 = ROWS_MAP_3 * COLS_MAP_3;
    
    int NB_CASES_MAPSET = NB_CASES_MAP_1 + NB_CASES_MAP_2 + NB_CASES_MAP_3;

    myMapSet = malloc(sizeof(map **) + NB_CASES_MAPSET*sizeof(int) + 3*11*sizeof(int));
    myMapSet[0] = malloc(sizeof(map *) + NB_CASES_MAP_1*sizeof(int) + 11*sizeof(int));
    assert(myMapSet[0]);
    myMapSet[1] = malloc(sizeof(map *) + NB_CASES_MAP_2*sizeof(int) + 11*sizeof(int));
    assert(myMapSet[1]);
    myMapSet[2] = malloc(sizeof(map *) + NB_CASES_MAP_3*sizeof(int) + 11*sizeof(int));
    assert(myMapSet[2]);
    
    myMapSet[0] = initMap(1);
    myMapSet[1] = initMap(2);
    myMapSet[2] = initMap(3);
}

void initMapSetFrame(map ***myPreviousMapSetFrames, map **myMapSet)
{
    assert(myPreviousMapSetFrames);
    assert(myMapSet);
    int nbMapSetCase = ((myMapSet[0]->rows) * (myMapSet[0]->cols))+ ((myMapSet[1]->rows) * (myMapSet[1]->cols))+ ((myMapSet[2]->rows) * (myMapSet[2]->cols));
    for(int i=0; i<8; i++)
    {
        myPreviousMapSetFrames[i] = malloc(3*sizeof(myMapSet) + nbMapSetCase*sizeof(int));
        initMapSet(myPreviousMapSetFrames[i]);
        for(int j=0; j<3; j++)
        {
            myPreviousMapSetFrames[i][j] = initMap(j+1);
            initMapToZero(myPreviousMapSetFrames[i][j]);
        }
    }
}

void copyMapSet(map **dest, map **src)
{
    assert(dest);
    assert(src);
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<src[i]->rows; j++)
        {
            for(int k=0; k<src[i]->cols; k++)
            {
                if(src[i]->map[j][k] != _PLAYER_)
                    dest[i]->map[j][k] = src[i]->map[j][k];
                else
                    dest[i]->map[j][k] = 0;
            }
        }
    }
}

void setMapSetDifferences(map **newMapSet, map **diffMapSet)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<newMapSet[i]->rows; j++)
        {
            for(int k=0; k<newMapSet[i]->rows; k++)
            {
                if(diffMapSet[i]->map[j][k] != 0 && newMapSet[i]->map[j][k] == 0 && diffMapSet[i]->map[j][k] != 1)
                {
                    newMapSet[i]->map[j][k] = diffMapSet[i]->map[j][k];
                }
            }
        }
    }
}

#endif