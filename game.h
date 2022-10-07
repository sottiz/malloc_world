#ifndef GAME_H
#define GAME_H
#include "malloc_world.h"
#include "map.h"
#include "player.h"
#include "monster.h"


// GAME CLASS CONTAINS THE FULL GAME MANAGING THANKS TO OTHER CLASSES (MAP, PLAYER, ...)
// WE CAN OPERATE ON EVERY ASPECTS OF THE GAME RIGHT HERE
// THE MAIN FUNCTION IN 'main.c' CALLS PRINCIPLY THE GAME FUNCTIONS 

typedef struct game{
    // GAME STRUCT IS DEFINED BY A PLAYER AND A MAPSET (TAB OF SEVERAL MAPS) => WE HAVE ALL THE MAPS CREATED AT THE CREATION OF A GAME (4 MAPS)
    player *p;
    map **mapSet;
    map **lastMapSet;
    map ***previousDiffMapSet;
    int nbOfMap;
    monster **monsters;
    int idLastMonster;
}game;

game *newGame();
void closeGame(game *myGame);
void changeActiveWeapon(game *myGame, int newActiveWeapon);
void resetPlayerPos(game *myGame);
void printPlayer(game *myGame);
void printAll(game *myGame);
void moveUp(game *myGame);
void moveDown(game *myGame);
void moveLeft(game *myGame);
void moveRight(game *myGame);
int findTargetValueCase(game *myGame, int posX, int posY);
int findTargetXPos(game *myGame);
int findTargetYPos(game *myGame);
void farmResource(game *myGame, int caseValue, int posX, int posY);
void makeTp(game *myGame, int caseValue);
void dealWithPNJ(game *myGame);
void makeAction(game *myGame);
int caseValToItemVal(int caseValue);
map **getDiffMapSets(game *myGame);
void applyMapSetsDiff(game *myGame);
void updateDiffMapSetFrames(game *myGame);
void openBag(game *myGame);
void addWeapon(game *myGame, item **myInventory, int index);
void printFight(player *p, monster *m);
void attack(player *p, monster *m);
void consumePotion(player *p);
void changeWeapon(player *p);
int escape();
int fighting(game *myGame, int idMonster);
void monsterAttacks(player *p, monster *m);
void fight(game *myGame, int posX, int posY, int caseValue);





// FUNCTION FOR CREATING A NEW GAME
game *newGame()
{
    int nbCaseMap1 = ROWS_MAP_1 * COLS_MAP_1;
    int nbCaseMap2 = ROWS_MAP_2 * COLS_MAP_2;
    int nbCaseMap3 = ROWS_MAP_3 * COLS_MAP_3;
    int nbCaseMapset = nbCaseMap1 + nbCaseMap2 + nbCaseMap3;
    int nbCaseMapSetFrame = 8*nbCaseMapset;

    game *myGame = malloc(sizeof(myGame) + sizeof(player *) + 10*sizeof(int) + 10*sizeof(map **)+sizeof(map ***) + 2*sizeof(int) +10*nbCaseMapset*sizeof(int) + 23*sizeof(monster *) + 23*11*sizeof(int));
    myGame->monsters = malloc(23*sizeof(monster *)+23*11*sizeof(int));
    myGame->idLastMonster = 0;
    myGame->mapSet = malloc(3*sizeof(map **) + nbCaseMapset*sizeof(int));
    
    myGame->mapSet[0] = initMap(1);
    myGame->mapSet[1] = initMap(2);
    myGame->mapSet[2] = initMap(3);
    assert(myGame->mapSet);
    // BASICALLY WE MAKE :
    //      MAP 1 : 8 ROWS AND COLS, RANK 1
    //      MAP 2 : 12"                  "2
    //      MAP 3 : 15"                  "3


    
    
    myGame->lastMapSet = malloc(3*sizeof(map) + nbCaseMapset * sizeof(int));
    myGame->lastMapSet[0] = initMap(1);
    myGame->lastMapSet[1] = initMap(2);
    myGame->lastMapSet[2] = initMap(3);
    for(int i=0; i<3; i++)
    {
        initMapToZero(myGame->lastMapSet[i]);
    }
    // PUT THE PLAYER ON THE FIRST MAP ON A RAND POSITION
    putPlayer(myGame->mapSet[0]);
    myGame->p = malloc(sizeof(player *) + 10*sizeof(int));
    myGame->p = newPlayer();

    // UPDATE PLAYER STRUCT ATTRIBUTES
    myGame->p->posX = getXPlayerPos(myGame->mapSet[0]);
    myGame->p->posY = getYPlayerPos(myGame->mapSet[0]);
    myGame->p->currentMap = myGame->mapSet[0]->rank;
    //copyMapSet(myGame->lastMapSet, myGame->mapSet);

    myGame->previousDiffMapSet = malloc(8*(sizeof(myGame->mapSet) + nbCaseMapSetFrame*sizeof(int)));
    assert(myGame->previousDiffMapSet);
    initMapSetFrame(myGame->previousDiffMapSet, myGame->mapSet);
    myGame->nbOfMap = 3;
    
    return myGame;
}

void closeGame(game *myGame)
{
    assert(myGame);
    assert(myGame->mapSet);
    freeMapSet(myGame->mapSet, 4);
    freePlayer(myGame->p);
    free(myGame);
}

void changeActiveWeapon(game *myGame, int newActiveWeapon)
{  
    if(newActiveWeapon < 0 || newActiveWeapon > 2)
        return;
    myGame->p->activeWeapon = newActiveWeapon;
    return;
}

void resetPlayerPos(game *myGame)
{
    putPlayer(myGame->mapSet[myGame->p->currentMap-1]);
    myGame->mapSet[myGame->p->currentMap-1]->map[myGame->p->posX][myGame->p->posY] = 0;
    
    myGame->p->posX = getXPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
    myGame->p->posY = getYPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
}

void printMap(player *p, map *myMap)
{
    int line = myMap->rank == 1 ? COLS_MAP_1 : (myMap->rank == 2 ? COLS_MAP_2 : COLS_MAP_3);
    line = line*3+2;
    int titleLineSpaces = line-5;
    int titleSpacesBefore = titleLineSpaces / 2; 
    int titleSpacesAfter =  titleLineSpaces % 2 == 1 ? titleSpacesBefore+1 : titleSpacesBefore;


    printf("               ▛");
    for(int i=0; i<line; i++)
    {
        printf("▀");
    }
    printf("▜\n");

        printf("               ▌");
    for(int i=0; i<line; i++)
    {
        printf(" ");
    }
    printf("▐\n");

    printf("               ▌");
    for(int i=0; i<titleSpacesBefore; i++)
    {
        printf(" ");
    }
    printf("MAP %d", myMap->rank);
    for(int i=0; i<titleSpacesAfter; i++)
    {
        printf(" ");
    }
    printf("▐\n");

    printf("               ▌");
    for(int i=0; i<line; i++)
    {
        printf(" ");
    }
    printf("▐\n");


    printf("               ▙");
    for(int i=0; i<line; i++)
    {
        printf("▄");
    }
    printf("▟\n");

    printf("               ▌");
    for(int i=0; i<line; i++)
    {
        printf(" ");
    }
    printf("▐\n");

    int isPlayer = 0;

    for(int i=0; i<myMap->rows; i++){
                printf("               ▌  ");
                for(int j=0; j<myMap->cols; j++){
                    switch (findCaseType(myMap->map[i][j]))
                    {
                    case _IS_RESOURCE_:
                        if(myMap->map[i][j]==_PLANT_1_ || myMap->map[i][j]==_PLANT_2_ || myMap->map[i][j]==_PLANT_3_)
                            printf("❀  ");
                        else if(myMap->map[i][j]==_ROC_1_ || myMap->map[i][j]==_ROC_2_ || myMap->map[i][j]==_ROC_3_)
                            printf("ⓡ  ");
                        else
                            printf("ⓦ  ");
                        break;

                    case _IS_TP_CASE_:
                        if(p->currentMap == 1)
                            printf("⚁  ");
                        if(p->currentMap == 3)
                            printf("⚁  ");
                        if(p->currentMap == 2)
                        {
                            if(myMap->map[i][j]==_TP_CASE_2_TO_1)
                                printf("⚀  ");
                            else
                                printf("⚂  ");
                        }
                        break;
                    
                    case _IS_MONSTER_CASE_:
                        printf("ⓜ  ");
                        break;
                    
                    case _PLAYER_:
                        if(p->direction == _NORTH_)
                            printf("⮝  ");			
                        else if(p->direction == _SOUTH_)
                            printf("⮟  ");
                        else if(p->direction == _EAST_)
                            printf("⮞  ");
                        else
                            printf("⮜  ");
                        break;
                    
                    case _INFRANCHISSABLE_:
                        printf("■  ");
                        break;
                    case _FREE_CASE_:
                        printf("⬚  ");
                        break;
                    case _PNJ_CASE_:
                        printf("⚒  ");
                        break;
                    default:
                        printf("¤  ");
                        break;
                    }
                    if(myMap->map[i][j] == 1 && isPlayer == 0)
                    {
                        isPlayer = 1;
                    }
                }
                printf("▐");
                if(isPlayer == 1)
                {
                    printf(" ☜ ");
                    isPlayer = 0;
                }
                printf("\n");
    }    
    printf("               ▙");
    for(int i=0; i<line; i++)
    {
        printf("▄");
    }
    printf("▟\n");
}

void printPlayer(game *myGame)
{
    printf("                ▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("                ▌        PLAYER STATS       ▐\n");
    printf("                ▌                           ▐\n");
    printf("                ▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
    printf("    \t\t▌ HP           : %d/%d\n", myGame->p->currentHp, myGame->p->hp);
    printf("    \t\t▌ PLAYER LEVEL : %d\n", myGame->p->level);
    printf("    \t\t▌ EXPERIENCE   : %d/%d XP\n", myGame->p->currentXp, myGame->p->nextLevel);
    printf("                ▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
    printf("    \t\t▌ WEAPONS :\n");
    for(int i=0; i<3; i++)
    {
        if(myGame->p->activeWeapon == i)
        {
            printf("    \t\t▌     # %d ", i);
        }
        else
        {
            printf("    \t\t▌       %d ", i);
        }

        printResource(myGame->p->weaponSet[i]->value);
        printf("\n");
        if(myGame->p->weaponSet[i]->value != _ESPACE_LIBRE)
            printf("    \t\t▌         DUR : %d   DMG : %d\n", myGame->p->weaponSet[i]->durability, myGame->p->weaponSet[i]->damage);
        else
            printf("    \t\t▌\n");
    }
    printf("                ▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
    printf("    \t\t▌ POSITION     : [%d, %d]\n", myGame->p->posX, myGame->p->posY);
    printf("    \t\t▌ DIRECTION    : ");
    switch (myGame->p->direction)
    {
    case _NORTH_:
        printf("NORTH\n");
        break;
    case _SOUTH_:
        printf("SOUTH\n");
        break;
    case _EAST_:
        printf("EAST\n");
        break;
    case _WEST_:
        printf("WEST\n");
        break;
    
    default:
        break;
    }
    printf("    \t\t▌ CURRENT MAP  : %d\n", myGame->p->currentMap);
    printf("    \t\t▌ TARGET       : ");
    int targetXPos = findTargetXPos(myGame);
    int targetYPos = findTargetYPos(myGame);
    if(targetXPos >= 0 && targetXPos < myGame->mapSet[myGame->p->currentMap-1]->rows && targetYPos >= 0 && targetYPos < myGame->mapSet[myGame->p->currentMap-1]->cols){
        switch (findCaseType(findTargetValueCase(myGame, findTargetXPos(myGame), findTargetYPos(myGame))))
        {
        case _IS_RESOURCE_:
            printResource(caseValToItemVal(findTargetValueCase(myGame, findTargetXPos(myGame), findTargetYPos(myGame))));
            //printFarming(myGame->mapSet[myGame->p->currentMap - 1]->map[myGame->p->posX][myGame->p->posY]);
            break;
        case _IS_MONSTER_CASE_:
            printf("Monster (%d)", findTargetValueCase(myGame, findTargetXPos(myGame), findTargetYPos(myGame)));
            break;
        case _IS_TP_CASE_:
            printf("Make teleportation ?");
            break;
        case _PNJ_CASE_:
            printf("Talk to Pr. Malloc ?");
            break;
        case _INFRANCHISSABLE_:
            printf("Wall");
            break;
        default:
            break;
        }
    }
    printf("\n");
    printf("                ▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
}
// PRINTING FUNCTION FOR THE CURRENT GAME MAP AND THE PLAYER STATS
void printAll(game *myGame)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet[myGame->p->currentMap-1]);
    printf("\n\n");
    printMap(myGame->p, myGame->mapSet[myGame->p->currentMap-1]);
    printf("\n\n");
    printPlayer(myGame);
    printf("\n\n");
}

// MOVING FUNCTIONS
// PLAYER CAN MOVE ON A CASE ONLY IF THIS ONE IS FREE (i.e 'myGame->mapSet[currentMap]->[x, y] = 0')

    // myGame                           REPRESENTS THE ACTUAL GAME PLAYED
    // myGame->mapSet                   REPRESENTS THE SET OF MAPS WHICH CONSTITUTE THE GAME
    // myGame->p                        REPRESENTS THE PLAYER STRUCT OF THE GAME
    // myGame->p->currentMap            CORRESPONDS WITH THE CURRENT "RANK" OF THE MAP -> NOT THE POSITION OF THIS MAP IN THE MAPSET
    // myGame->mapSet[currentMap - 1]   CORRESPONDS WITH THE POSITION OF THE CURRENT MAP IN THE MAPSET

// MOVEUP FUNCTION -> FROM myGame->mapSet[currentMap]->map[x][y] TO myGame->mapSet[currentMap]->map[x - 1][y]
void moveUp(game *myGame)
{
    // CHECK IF PLAYER IS ORIENTED ON NORTH, IF NOT SET HIS DIRECTION ON
    if(myGame->p->direction != _NORTH_){
        myGame->p->direction = _NORTH_;
        return;
    }
    // CHECK IF WE ARE GOING OUT OF MAP
    if(myGame->p->posX-1 < 0)
        return;

    // GET NEXT COORD(x, y) POSITION 
    int posXNextCase = myGame->p->posX-1;
    int posYNextCase = myGame->p->posY;

    // GET VALUES OF NEXT CASE AND CURRENT POSITION OF THE PLAYER
    int nextCaseValue = myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase];
    int currentXPos = getXPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
    int currentYPos = getYPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);

    // CHECK IF NEXT CASE IS FREE AND IF WE STAY IN THE MAP ZONE -> ELSE, DON'T DO NOTHING
    if(posXNextCase >= 0 && posXNextCase < myGame->mapSet[myGame->p->currentMap-1]->rows && nextCaseValue == 0){
        myGame->mapSet[myGame->p->currentMap-1]->map[currentXPos][currentYPos] = 0;
        myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase] = 1;
        myGame->p->posX = posXNextCase;
        myGame->p->posY = posYNextCase;
    }
    return;
}

// MOVEDOWN FUNCTION -> FROM myGame->mapSet[currentMap]->map[x][y] TO myGame->mapSet[currentMap]->map[x + 1][y]
void moveDown(game *myGame)
{
    // CHECK IF PLAYER IS ORIENTED ON SOUTH, IF NOT SET HIS DIRECTION ON
    if(myGame->p->direction != _SOUTH_){
        myGame->p->direction = _SOUTH_;
        return;
    }
    // CHECK IF WE ARE GOING OUT OF MAP
    if(myGame->p->posX+1 >= myGame->mapSet[myGame->p->currentMap-1]->rows)
        return;

    // GET NEXT COORD(x, y) POSITION 
    int posXNextCase = myGame->p->posX+1;
    int posYNextCase = myGame->p->posY;

    // GET VALUES OF NEXT CASE AND CURRENT POSITION OF THE PLAYER
    int nextCaseValue = myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase];
    int currentXPos = getXPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
    int currentYPos = getYPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);

    // CHECK IF NEXT CASE IS FREE AND IF WE STAY IN THE MAP ZONE -> ELSE, DON'T DO NOTHING
    if(posXNextCase >= 0 && posXNextCase < myGame->mapSet[myGame->p->currentMap-1]->rows && nextCaseValue == 0){
        myGame->mapSet[myGame->p->currentMap-1]->map[currentXPos][currentYPos] = 0;
        myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase] = 1;
        myGame->p->posX = posXNextCase;
        myGame->p->posY = posYNextCase;
    }
    return;
}

// MOVELEFT FUNCTION -> FROM myGame->mapSet[currentMap]->map[x][y] TO myGame->mapSet[currentMap]->map[x][y - 1]
void moveLeft(game *myGame)
{
        // CHECK IF PLAYER IS ORIENTED ON EAST, IF NOT SET HIS DIRECTION ON
    if(myGame->p->direction != _WEST_){
        myGame->p->direction = _WEST_;
        return;
    }
    // CHECK IF WE ARE GOING OUT OF MAP
    if(myGame->p->posY-1 < 0)
        return;

    // GET NEXT COORD(x, y) POSITION 
    int posXNextCase = myGame->p->posX;
    int posYNextCase = myGame->p->posY-1;

    // GET VALUES OF NEXT CASE AND CURRENT POSITION OF THE PLAYER
    int nextCaseValue = myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase];
    int currentXPos = getXPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
    int currentYPos = getYPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);

    // CHECK IF NEXT CASE IS FREE AND IF WE STAY IN THE MAP ZONE -> ELSE, DON'T DO NOTHING
    if(posYNextCase >= 0 && posYNextCase < myGame->mapSet[myGame->p->currentMap-1]->cols && nextCaseValue == 0){
        myGame->mapSet[myGame->p->currentMap-1]->map[currentXPos][currentYPos] = 0;
        myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase] = 1;
        myGame->p->posX = posXNextCase;
        myGame->p->posY = posYNextCase;
    }
    return;
}

// MOVERIGHT FUNCTION -> FROM myGame->mapSet[currentMap]->map[x][y] TO myGame->mapSet[currentMap]->map[x][y + 1]
void moveRight(game *myGame)
{
    // CHECK IF PLAYER IS ORIENTED ON WEST, IF NOT SET HIS DIRECTION ON
    if(myGame->p->direction != _EAST_){
        myGame->p->direction = _EAST_;
        return;
    }
    // CHECK IF WE ARE GOING OUT OF MAP
    if(myGame->p->posY+1 > myGame->mapSet[myGame->p->currentMap-1]->cols)
        return;

    // GET NEXT COORD(x, y) POSITION 
    int posXNextCase = myGame->p->posX;
    int posYNextCase = myGame->p->posY+1;

    // GET VALUES OF NEXT CASE AND CURRENT POSITION OF THE PLAYER
    int nextCaseValue = myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase];
    int currentXPos = getXPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);
    int currentYPos = getYPlayerPos(myGame->mapSet[myGame->p->currentMap-1]);

    // CHECK IF NEXT CASE IS FREE AND IF WE STAY IN THE MAP ZONE -> ELSE, DON'T DO NOTHING
    if(posYNextCase >= 0 && posYNextCase < myGame->mapSet[myGame->p->currentMap-1]->cols && nextCaseValue == 0){
        myGame->mapSet[myGame->p->currentMap-1]->map[currentXPos][currentYPos] = 0;
        myGame->mapSet[myGame->p->currentMap-1]->map[posXNextCase][posYNextCase] = 1;
        myGame->p->posX = posXNextCase;
        myGame->p->posY = posYNextCase;
    }
    return;
}

int findTargetValueCase(game *myGame, int posX, int posY)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet);
    assert(myGame->mapSet[myGame->p->currentMap-1]);

    if(posX < 0 || posY < 0 || posX > myGame->mapSet[myGame->p->currentMap-1]->rows || posY > myGame->mapSet[myGame->p->currentMap-1]->cols)
        return _INFRANCHISSABLE_;

    return myGame->mapSet[myGame->p->currentMap-1]->map[posX][posY];
}

int findTargetXPos(game *myGame)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet);
    assert(myGame->mapSet[myGame->p->currentMap-1]);

    int currentMapIndex = myGame->p->currentMap - 1;
    int posXPlayer = getXPlayerPos(myGame->mapSet[currentMapIndex]);

    int posXtarget = -1;

    if(posXPlayer < 0 || posXPlayer > myGame->mapSet[currentMapIndex]->rows)
        return -1;

    switch (myGame->p->direction)
    {
    case _NORTH_:
        posXtarget = posXPlayer - 1;
        return (posXtarget < 0) ? -1 : posXtarget;

    case _SOUTH_:
        posXtarget = posXPlayer + 1;
        return (posXtarget > myGame->mapSet[currentMapIndex]->rows) ? -1 : posXtarget;

    case _EAST_:
        return posXPlayer;
    case _WEST_:
        return posXPlayer;
    
    default:
        return posXtarget;
    }
}

int findTargetYPos(game *myGame)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet);
    assert(myGame->mapSet[myGame->p->currentMap-1]);

    int currentMapIndex = myGame->p->currentMap - 1;
    int posYPlayer = getYPlayerPos(myGame->mapSet[currentMapIndex]);

    int posYtarget = -1;

    if(posYPlayer < 0 || posYPlayer > myGame->mapSet[currentMapIndex]->cols)
        return -1;

    switch (myGame->p->direction)
    {
    case _NORTH_:
        return posYPlayer;

    case _SOUTH_:
        return posYPlayer;

    case _EAST_:
        posYtarget = posYPlayer + 1;
        return (posYtarget > myGame->mapSet[currentMapIndex]->rows) ? -1 : posYtarget;
    case _WEST_:
        posYtarget = posYPlayer - 1;
        return (posYtarget > myGame->mapSet[currentMapIndex]->rows) ? -1 : posYtarget;
    
    default:
        return posYtarget;
    }
}

void farmResource(game *myGame, int caseValue, int posX, int posY)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet);
    assert(myGame->mapSet[myGame->p->currentMap-1]);
    assert(myGame->p->inventory);
    
    int itmValue = 0;
    switch (caseValue)
    {
    case _PLANT_1_:
        itmValue = _HERBE_;
        break;
    case _ROC_1_:
        itmValue = _PIERRE_;
        break;
    case _BOIS_1_:
        itmValue = _SAPIN_;
        break;
    case _PLANT_2_:
        itmValue = _LAVANDE_;
        break;
    case _ROC_2_:
        itmValue = _FER_;
        break;
    case _BOIS_2_:
        itmValue = _HETRE_;
        break;
    case _PLANT_3_:
        itmValue = _CHANVRE_;
        break;
    case _ROC_3_:
        itmValue = _DIAMANT_;
        break;
    case _BOIS_3_:
        itmValue = _CHENE_;
        break;
    default:
        return;
    }

    int index = itemAlreadyPresent(myGame->p->inventory, itmValue);
    int indexTool = indexToolNeeded(myGame->p->inventory, itmValue);

    if(indexTool < 0 || indexTool > 9){
        return;
    }

    int nextDurability = findNextDurability(myGame->p->inventory[indexTool], itmValue);
    if(nextDurability < 0)
        return;
    myGame->p->inventory[indexTool]->durability = nextDurability;

    if(index < 0)
        addItem(myGame->p->inventory, itmValue);
    else{
        if(myGame->p->inventory[index]->quantity < 20){
            addItem(myGame->p->inventory, itmValue);
        }
    }
    myGame->mapSet[myGame->p->currentMap - 1]->map[posX][posY] = _FREE_CASE_;

}

void makeTp(game *myGame, int caseValue)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->mapSet);

    switch(myGame->p->currentMap){
        case 1 :
            if(myGame->p->level < myGame->mapSet[1]->lvlRequired)
                return;
            myGame->mapSet[0]->map[myGame->p->posX][myGame->p->posY] = _FREE_CASE_;
            putPlayer(myGame->mapSet[1]);
            myGame->p->posX = getXPlayerPos(myGame->mapSet[1]);
            myGame->p->posY = getYPlayerPos(myGame->mapSet[1]);
            myGame->p->currentMap += 1;
            return;
        case 2 :
            switch(caseValue){
                case _TP_CASE_2_TO_1 :
                    myGame->mapSet[1]->map[myGame->p->posX][myGame->p->posY] = _FREE_CASE_;
                    putPlayer(myGame->mapSet[0]);
                    myGame->p->posX = getXPlayerPos(myGame->mapSet[0]);
                    myGame->p->posY = getYPlayerPos(myGame->mapSet[0]);
                    myGame->p->currentMap -= 1;
                    return;
                case _TP_CASE_2_TO_3 :
                    if(myGame->p->level < myGame->mapSet[2]->lvlRequired)
                        return;
                    myGame->mapSet[1]->map[myGame->p->posX][myGame->p->posY] = _FREE_CASE_;
                    putPlayer(myGame->mapSet[2]);
                    myGame->p->posX = getXPlayerPos(myGame->mapSet[2]);
                    myGame->p->posY = getYPlayerPos(myGame->mapSet[2]);
                    myGame->p->currentMap += 1;
                    return;
                default :
                    return;
                }
        case 3 :
            myGame->mapSet[2]->map[myGame->p->posX][myGame->p->posY] = _FREE_CASE_;
            putPlayer(myGame->mapSet[1]);
            myGame->p->currentMap -= 1;
            myGame->p->posX = getXPlayerPos(myGame->mapSet[1]);
            myGame->p->posY = getYPlayerPos(myGame->mapSet[1]);
            return;
        default :
            return;
    }
}

void dealWithPNJ(game *myGame)
{
    char input[50];
    int talking = 1;

    while(talking == 1)
    {
        printf("\n\n\nWhat can I do for you ?\n");
        printf("\t> Make crafting        - Press '1'\n");
        printf("\t> Repair my items      - Press '2'\n");
        printf("\t> Nothing, resume game - Press 'x'\n");

        scanf("%s", input);

        if(strcmp(input, "x") == 0)
        {
            talking = 0;
            break;
        }

        if(strcmp(input, "1") == 0)
        {
            printf("\n\n\nI let you consult the list of possible fabrications with your ingredients\n");
            printCraftsList(myGame->p);
            printf("\nOk, what do you want to craft ?\n");
            printf("\t> Make [N°]            - Press some id (example : 0 for N°0)\n");
            printf("\t> Nothing, go back     - Press 'x'\n");
            scanf("%s", input);
            if(strcmp(input, "x") == 0)
            {
                dealWithPNJ(myGame);
                break;
            }
            else
            {
                int id = atoi(input);
                if(getNbOfPossibleCrafts(myGame->p)-1 < id)
                {
                    printf("    +------------------------------------------------------+\n");
                    printf("    |  %d ", id);
                    if(id / 10 == 0)
                        printf(" ");
                    printf(": This id does not match any item you can craft  |\n");
                    printf("    +------------------------------------------------------+\n");
                    dealWithPNJ(myGame);
                    break;
                }
                else
                {
                    item **possibleCrafts = malloc(25*sizeof(item *) + 25*9*sizeof(int));
                    assert(possibleCrafts);
                    for(int i=0; i<25; i++){
                        possibleCrafts[i] = malloc(sizeof(item *) + 9 * sizeof(int));
                        possibleCrafts[i] = createItem(0, 0);
                        assert(possibleCrafts[i]);
                    }
                    possibleCrafts = getPossibleCrafts(myGame->p);
                    item *itemCrafted = createItem(possibleCrafts[id]->value, 1);
                    int **ingredients = getIngredientsNeeded(itemCrafted->value);
                    printf("\nAre you sure about your selection : ");
                    printResource(itemCrafted->value);
                    printf(" ?\n");
                    printf("\t> Cost is : ");
                    if(ingredients[_PLANT_INDEX_][_QUANTITY_] != 0)
                    {
                        printf("%d x ", ingredients[_PLANT_INDEX_][_QUANTITY_]);
                        printResource(ingredients[_PLANT_INDEX_][_RESOURCE_]);
                        printf("  ");
                    }
                    if(ingredients[_ROC_INDEX_][_QUANTITY_] != 0)
                    {
                        printf("%d x ", ingredients[_ROC_INDEX_][_QUANTITY_]);
                        printResource(ingredients[_ROC_INDEX_][_RESOURCE_]);
                        printf("  ");
                    }
                    if(ingredients[_WOOD_INDEX_][_QUANTITY_] != 0)
                    {
                        printf("%d x ", ingredients[_WOOD_INDEX_][_QUANTITY_]);
                        printResource(ingredients[_WOOD_INDEX_][_RESOURCE_]);
                        printf("  ");
                    }
                    printf("\n");
                    printf("\t\t> Yes  - Press 'y'\n");
                    printf("\t\t> No   - Press 'n'\n");
                    scanf("%s", input);

                    if(strcmp("y", input) == 0)
                    {
                        printf("\t\tYou obtain 1 x \n");
                        printResource(possibleCrafts[id]->value);
                        printf("\n");

                        if(!isFullInventory(myGame->p->inventory))
                        {
                            updateResourceForCrafting(myGame->p->inventory, ingredients);
                            addItem(myGame->p->inventory, possibleCrafts[id]->value);
                            printf("Inventory has been updated\n\n\n");
                            dealWithPNJ(myGame);
                            break;
                        }
                        else
                        {
                            printf("Inventory is full, come back later\n\n");
                            dealWithPNJ(myGame);
                            break;
                        }
                    }
                    if(strcmp("n", input) == 0)
                    {
                        dealWithPNJ(myGame);
                        break;
                    }
                }

            }
        }

        if(strcmp(input, "2") == 0)
        {
            repairAllDurability(myGame->p->inventory);
            repairWeaponSetDurability(myGame->p->weaponSet);
            printf("Ok, it's done !\n");
            dealWithPNJ(myGame);
            break;
        }
    }
}

void printFight(player *p, monster *m)
{
    printf("▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("▌    FIGHTING    ▐\n");
    printf("▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
    printf("   ▛▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("   ▌PLAYER\n");
    printf("   ▌%d/%dHP\n", p->currentHp, p->hp);
    printf("   ▙▄▄▄▄▄▄▄▄▄▄▟\n");
    printf("   ▛▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("   ▌MONSTER\n");
    printf("   ▌%d/%dHP\n", m->hpCurrent, m->hpMax);
    printf("   ▙▄▄▄▄▄▄▄▄▄▄▟\n\n");
}

void attack(player *p, monster *m)
{
    assert(p);
    if(p->weaponSet[p->activeWeapon]->durability < 1)
        return;
    m->hpCurrent = m->hpCurrent - p->weaponSet[p->activeWeapon]->damage;
    p->weaponSet[p->activeWeapon]->durability -= 1;
}

void consumePotion(player *p)
{
    int indexP1 = itemAlreadyPresent(p->inventory, _POTION_DE_VIE_1_);
    int indexP2 = itemAlreadyPresent(p->inventory, _POTION_DE_VIE_2_);
    int indexP3 = itemAlreadyPresent(p->inventory, _POTION_DE_VIE_3_);
    if(indexP1 == indexP2 && indexP2 == indexP3 && indexP1 == -1){
        printf("▌  You don't have potion\n\n");
        return;
    }
    printf("▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("▌    DRINK POTION     ▐\n");
    printf("▌    'c' > CANCEL     ▐\n");
    printf("▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n");
    if(indexP1 != -1)
    {
        printf("▌ [1]   ");
        printResource(_POTION_DE_VIE_1_);
        printf("\n");
    }
    if(indexP2 != -1)
    {
        printf("▌ [2]   ");
        printResource(_POTION_DE_VIE_2_);
        printf("\n");
    }
    if(indexP3 != -1)
    {
        printf("▌ [3]   ");
        printResource(_POTION_DE_VIE_3_);
        printf("\n");
    }
    printf("▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n\n");
    printf("▌    Select with an id ('1', '2', '3')\n");
    char input[50];
    scanf("%s", input);
    if(strcmp("1", input) != 0 && strcmp("2", input) != 0 && strcmp("3", input) != 0)
    {
        consumePotion(p);
        return;
    }
    if(strcmp("1", input) == 0 && indexP1 != -1)
    {
        if(p->currentHp == p->hp)
            return;
        p->currentHp += p->inventory[indexP1]->healPt;
        if(p->currentHp >= p->hp)
            p->currentHp = p->hp;
        delItem(p->inventory, indexP1);
        return;
    }
    if(strcmp("2", input) == 0 && indexP2 != -1)
    {
        p->currentHp += p->inventory[indexP2]->healPt;
        delItem(p->inventory, indexP2);
        return;
    }
    if(strcmp("3", input) == 0 && indexP3 != -1)
    {
        p->currentHp += p->inventory[indexP3]->healPt;
        delItem(p->inventory, indexP3);
        return;
    }
}

void changeWeapon(player *p)
{
    char input[50];
    printWeaponSet(p);
    printf("\n▌    Select a weapon with id ('0', '1', '2')\n");
    scanf("%s", input);
    if(strcmp("0", input)==0)
    {
        p->activeWeapon = 0;
        return;
    }
    if(strcmp("1", input)==0)
    {
        p->activeWeapon = 1;
        return;
    }
    if(strcmp("2", input)==0)
    {
        p->activeWeapon = 2;
        return;
    }
    changeWeapon(p);
}

int escape()
{
    int r = rand()%100;
    if(r <= 30)
    {
        return 1;
    }
    printf("  Escape failed\n");
    return 0;
}

int fighting(game *myGame, int idMonster)
{
    assert(myGame->monsters[idMonster]);
    printFight(myGame->p, myGame->monsters[idMonster]);
    printf("▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜\n");
    printf("▌  'a' > ATTACK        ▐\n");
    printf("▌  'c' > CHANGE WEAPON ▐   Active weapon : "); 
    printResource(myGame->p->weaponSet[myGame->p->activeWeapon]->value);
    printf(" (DUR:%d  DMG:%d)\n", myGame->p->weaponSet[myGame->p->activeWeapon]->durability, myGame->p->weaponSet[myGame->p->activeWeapon]->damage);
    printf("▌  'd' > DRINK POTION  ▐\n");
    printf("▌  'e' > ESCAPE        ▐\n");
    printf("▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟\n\n");
    char input[50];
    int esc = 0;
    scanf("%s", input);
    if(strcmp(input, "a")==0)
    {
        attack(myGame->p, myGame->monsters[idMonster]);
        return 0;
    }
    if(strcmp(input, "c")==0)
    {
        changeWeapon(myGame->p);
        return 0;
    }
    if(strcmp(input, "d")==0)
    {
        consumePotion(myGame->p);
        return 0;
    }
    if(strcmp(input, "e")==0)
    {
        esc = escape(myGame, myGame->monsters[idMonster]);
        return esc;
    }
    fighting(myGame, idMonster);
    return 0;
}

void monsterAttacks(player *p, monster *m)
{
    if(m->hpCurrent <= 0)
        return;
    int damage = m->damage - (p->plastron->dmgResist * m->damage/100);
    p->currentHp = p->currentHp - damage;
    printf(" Monster attacks, you lost %dhp\n\n", damage);
    return;
}

void fight(game *myGame, int posX, int posY, int caseValue)
{
    myGame->monsters[myGame->idLastMonster] = newMonster(caseValue);
    myGame->monsters[myGame->idLastMonster]->currentMap = myGame->p->currentMap;
    myGame->monsters[myGame->idLastMonster]->direction = myGame->p->direction%2 == 0 ? myGame->p->direction+1 : myGame->p->direction-1;
    myGame->monsters[myGame->idLastMonster]->posX = posX;
    myGame->monsters[myGame->idLastMonster]->posY = posY;
    myGame->monsters[myGame->idLastMonster]->id = myGame->idLastMonster;

    int escape = 0;

    while(myGame->p->currentHp > 0 && myGame->monsters[myGame->idLastMonster]->hpCurrent > 0)
    {
        escape = fighting(myGame, myGame->monsters[myGame->idLastMonster]->id);
        monsterAttacks(myGame->p, myGame->monsters[myGame->idLastMonster]);
        if(escape == 1)
          return;  
    }
    if(myGame->p->currentHp > 0)
    {
        myGame->p->currentXp = myGame->p->currentXp+caseValue;
        updateXp(myGame->p);
        myGame->mapSet[myGame->p->currentMap-1]->map[posX][posY] = 0;
    }
}

void makeAction(game *myGame)
{
    assert(myGame);
    int posX = findTargetXPos(myGame);
    int posY = findTargetYPos(myGame);

    if(posX == -1 || posY == -1)
        return;
    
    int caseValue = findTargetValueCase(myGame, posX, posY);
    if(caseValue == _INFRANCHISSABLE_)
        return;
    switch (findCaseType(caseValue))
    {
    case _IS_RESOURCE_:
        copyMapSet(myGame->lastMapSet, myGame->mapSet);
        farmResource(myGame, caseValue, posX, posY);
        return;
    
    case _IS_TP_CASE_ :
        copyMapSet(myGame->lastMapSet, myGame->mapSet);
        makeTp(myGame, caseValue);
        return;
    
    case _IS_MONSTER_CASE_ :
        copyMapSet(myGame->lastMapSet, myGame->mapSet);
        fight(myGame, posX, posY, caseValue);
        return;

    case _PNJ_CASE_ :
        dealWithPNJ(myGame);
        return;

    default:
        return;
    }
}

int caseValToItemVal(int caseValue)
{
    switch (caseValue)
    {
    case _PLANT_1_:
        return _HERBE_;
    case _PLANT_2_:
        return _LAVANDE_;
    case _PLANT_3_:
        return _CHANVRE_;
    case _ROC_1_:
        return _PIERRE_;
    case _ROC_2_:
        return _FER_;
    case _ROC_3_:
        return _DIAMANT_;
    case _BOIS_1_:
        return _SAPIN_;
    case _BOIS_2_:
        return _HETRE_;
    case _BOIS_3_:
        return _CHENE_;
    case _TP_CASE_1_TO_2:
        return _TP_CASE_1_TO_2;
    case _TP_CASE_2_TO_3 :
        return _TP_CASE_2_TO_3;
    case _FREE_CASE_:
        return _FREE_CASE_;
    case _INFRANCHISSABLE_:
        return _INFRANCHISSABLE_;
    default:
        if(findCaseType(caseValue) == _IS_MONSTER_CASE_)
            return _IS_MONSTER_CASE_;
        return _INFRANCHISSABLE_;
    }
}

map **getDiffMapSets(game *myGame)
{
    int nbMapSetCase = ((myGame->mapSet[0]->rows) * (myGame->mapSet[0]->cols))+ ((myGame->mapSet[1]->rows) * (myGame->mapSet[1]->cols))+ ((myGame->mapSet[2]->rows) * (myGame->mapSet[2]->cols));

    map **myDiffMapSet = malloc(3*sizeof(map *) + nbMapSetCase*sizeof(int));
    initMapSet(myDiffMapSet);

    for(int i=0; i<3; i++)
    {
            myDiffMapSet[i] = initMap(i+1);
            initMapToZero(myDiffMapSet[i]);
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<myDiffMapSet[i]->rows; j++)
        {
            for(int k=0; k<myDiffMapSet[i]->cols; k++)
            {
                if(myGame->lastMapSet[i]->map[j][k] != myGame->mapSet[i]->map[j][k] && myGame->mapSet[i]->map[j][k] != 1)
                {
                    myDiffMapSet[i]->map[j][k] = myGame->lastMapSet[i]->map[j][k];
                }
            }
        }
    }

    return myDiffMapSet;
}

void applyMapSetsDiff(game *myGame)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j< myGame->mapSet[i]->rows; j++)
        {
            for(int k=0; k<myGame->mapSet[i]->cols; k++)
            {
                if(myGame->previousDiffMapSet[7][i]->map[j][k] != 0)
                {
                    if(myGame->mapSet[i]->map[j][k] != _PLAYER_)
                        myGame->mapSet[i]->map[j][k] = myGame->previousDiffMapSet[7][i]->map[j][k];
                    else
                        myGame->previousDiffMapSet[6][i]->map[j][k] = myGame->previousDiffMapSet[7][i]->map[j][k];
                }
            }
        }
    }
}

void updateDiffMapSetFrames(game *myGame)
{
    int nbMapSetCase = ((myGame->mapSet[0]->rows) * (myGame->mapSet[0]->cols))+ ((myGame->mapSet[1]->rows) * (myGame->mapSet[1]->cols))+ ((myGame->mapSet[2]->rows) * (myGame->mapSet[2]->cols));
    
    
    applyMapSetsDiff(myGame);
    

    map **myDiffMapSet = malloc(3*sizeof(map *) + nbMapSetCase*sizeof(int));
    initMapSet(myDiffMapSet);
    myDiffMapSet[0] = initMap(1);
    myDiffMapSet[1] = initMap(2);
    myDiffMapSet[2] = initMap(3);

    for(int i=0; i<3; i++)
    {
        initMapToZero(myDiffMapSet[i]);
    }
            
    myDiffMapSet = getDiffMapSets(myGame);

    
    for(int i=7; i>0; i--){
        copyMapSet(myGame->previousDiffMapSet[i], myGame->previousDiffMapSet[i-1]);
    }
    copyMapSet(myGame->previousDiffMapSet[0], myDiffMapSet);
    
}

void addWeapon(game *myGame, item **myInventory, int index)
{
    assert(myInventory);
    if(findType(myInventory[index]->value) != 'w')
        return;
    for(int i=0; i<3; i++){
        if(myGame->p->weaponSet[i]->value == _ESPACE_LIBRE){
            free(myGame->p->weaponSet[i]);
            myGame->p->weaponSet[i] = malloc(sizeof(item *) + 9 * sizeof(int));
            assert(myGame->p->weaponSet[i]);
            myGame->p->weaponSet[i] = createItem(myInventory[index]->value, 1);
            myGame->p->weaponSet[i]->durability = myGame->p->inventory[index]->durability;
            assert(myGame->p->weaponSet[i]);
            return;
        }
    }
}

void moveWeaponToInventory(game *myGame, item **myWeaponSet, int index)
{
    assert(myGame);
    assert(myWeaponSet);
    assert(myGame->p->inventory);

    for(int i=0; i<10; i++)
    {
        if(myGame->p->inventory[i]->value == _ESPACE_LIBRE)
        {
            free(myGame->p->weaponSet[i]);
            myGame->p->weaponSet[i] = malloc(sizeof(item *) + 9 * sizeof(int));
            assert(myGame->p->weaponSet[i]);
            myGame->p->inventory[i] = createItem(myWeaponSet[index]->value, 1);
            myGame->p->inventory[i]->durability = myGame->p->weaponSet[index]->durability;
            assert(myGame->p->inventory[i]);
            myWeaponSet[index] = createItem(_ESPACE_LIBRE, 0);
            assert(myWeaponSet[index]);
            return;
        }
    }    
}

void openBag(game *myGame)
{
    assert(myGame);
    assert(myGame->p);
    assert(myGame->p->inventory);
    char input[50];
    printf("\n\n\n");
    printf("\t             ___________       \n");
    printf("\t  ___________/==========/|_____________\n");
    printf("\t /           ||         ||            /|\n");
    printf("\t/____________________________________/ |\n");
    printf("\t|-----------------------------------|  |\n");
    printf("\t|    > Manage inventory  - Press '0'|  |\n");
    printf("\t+-----------------------------------|  |\n");
    printf("\t|-----------------------------------|  |\n");
    printf("\t|    > Manage weapon set - Press '1'|  |\n");
    printf("\t+-----------------------------------|  |\n");
    printf("\t|-----------------------------------|  |\n");
    printf("\t|    > Manage armor      - Press '2'|  |\n");
    printf("\t+-----------------------------------|  |\n");
    printf("\t+-----------------------------------|  |\n");
    printf("\t|    > Close bag         - Press 'x'| /\n");
    printf("\t|___________________________________|/\n");


        scanf("%s", input);

        if(strcmp(input, "0") == 0)
        {
            printInventory(myGame->p);
            printf("\t> Select an item - Press an id [0-9]\n");
            scanf("%s", input);
            if(input[0] < '0' || input[0] > '9')
            {
                printf("Invalid value\n");
                openBag(myGame);
                return;
            }
            int id = atoi(input);
            if(id > 9)
            {
                printf("Invalid value\n");
                openBag(myGame);
                return;
            }

            printf("\t> [%d] ", id);
            printResource(myGame->p->inventory[id]->value);
            printf(" :\n");
            switch (findType(myGame->p->inventory[id]->value))
            {
            case 'r':
                printf("\t> Delete '");
                printResource(myGame->p->inventory[id]->value);
                printf("'? - Press 'y' or 'n'\n");
                scanf("%s", input);
                if(strcmp("y", input)==0)
                {
                    int nbResource = getResourceQuantity(myGame->p->inventory, myGame->p->inventory[id]->value);
                    printf("\t> How many (you have %d) ? - Press a value or 'a' for all:", nbResource);
                    scanf("%s", input);
                    int nbToDelete = atoi(input);
                    if(strcmp("a", input)==0){
                        nbToDelete = getResourceQuantity(myGame->p->inventory, myGame->p->inventory[id]->value);
                    }
                    if(nbToDelete > nbResource || nbToDelete < 1)
                    {
                        printf("\tCan't get rid of %d x ", nbToDelete);
                        printResource(myGame->p->inventory[id]->value);
                        printf("\n");
                        openBag(myGame);
                        return;
                    }
                    else
                    {
                        printf("\t> Are you sure to get rid of %d ", nbToDelete);
                        printResource(myGame->p->inventory[id]->value);
                        printf("? - Press 'y' or 'n'\n");
                        scanf("%s", input);
                        if(strcmp("y", input)==0)
                        {
                            printf("\t %d x ", nbToDelete);
                            printResource(myGame->p->inventory[id]->value);
                            printf(" removed\n");
                            for(int i=0; i<nbToDelete; i++)
                            {
                                delItem(myGame->p->inventory, id);
                            }
                            openBag(myGame);
                            return;
                        }
                        if(strcmp("n", input)==0)
                        {
                            openBag(myGame);
                            return;
                        }
                    }
                }
                openBag(myGame);
                return;
            case 'w':
                printf("\t> Add to weapon set - Press '0'\n");
                printf("\t> Delete            - Press '1'\n");
                printf("\t> Exit              - Press 'x'\n");
                scanf("%s", input);
                if(strcmp("0", input)==0)
                {
                    if(isFullWeaponSet(myGame->p->weaponSet)==0)
                    {
                        printResource(myGame->p->inventory[id]->value);
                        printf(" added to weapon set\n");
                        addWeapon(myGame, myGame->p->inventory, id);
                        delItem(myGame->p->inventory, id);
                        openBag(myGame);
                        return;
                    }
                    else
                    {
                        printWeaponSet(myGame->p);
                        printf("Weapon set is full\n");
                        openBag(myGame);
                        return;
                    }
                }
                if(strcmp("1", input)==0)
                {
                    printf("\t> Are you sure to get rid of '");
                    printResource(myGame->p->inventory[id]->value);
                    printf("'? - Press 'y' or 'n'\n");
                    scanf("%s", input);
                    if(strcmp("y", input)==0)
                    {
                        printResource(myGame->p->inventory[id]->value);
                        printf(" has been deleted\n");
                        delItem(myGame->p->inventory, id);
                        openBag(myGame);
                        return;
                    }
                    if(strcmp("n", input)==0)
                    {
                        openBag(myGame);
                        return;
                    }
                    openBag(myGame);
                    return;
                }
                openBag(myGame);
                return;
            case 's':
                printf("\t> Equip  - Press '0'\n");
                printf("\t> Delete - Press '1'\n");
                printf("\t> Exit   - Press 'x'\n");
                scanf("%s", input);
                if(strcmp("0", input)==0)
                {
                    if(myGame->p->plastron->value == 0)
                    {
                        printf("\t'");
                        printResource(myGame->p->inventory[id]->value);
                        printf("' is equiped\n");
                        myGame->p->plastron = createItem(myGame->p->inventory[id]->value, 1);
                        delItem(myGame->p->inventory, id);
                        openBag(myGame);
                        return;
                    }
                    else{
                        printf("\tYou already wear an armor (");
                        printResource(myGame->p->plastron->value);
                        printf(")\n");
                        openBag(myGame);
                        return;
                    }
                }
                if(strcmp("1", input)==0)
                {
                    printf("\t> Are you sure to get rid of '");
                    printResource(myGame->p->inventory[id]->value);
                    printf("'? - Press 'y' or 'n'\n");
                    scanf("%s", input);
                    if(strcmp("y", input)==0)
                    {
                        printf("\t'");
                        printResource(myGame->p->inventory[id]->value);
                        printf("' has been deleted\n");
                    }
                    openBag(myGame);
                    return;
                }
                openBag(myGame);
                return;
            case 'h':
                printf("\t> Drink (%dhp healed) - Press '0'\n", myGame->p->inventory[id]->healPt);
                printf("\t> Delete              - Press '1'\n");
                printf("\t> Exit                - Press 'x'\n");
                scanf("%s", input);
                if(strcmp("0", input)==0)
                {
                    if(myGame->p->hp == myGame->p->currentHp)
                    {
                        printf("You already have all your hp\n");
                        openBag(myGame);
                        return;
                    }
                    else
                    {
                        printf("\t'");
                        printResource(myGame->p->inventory[id]->value);
                        printf("' %dhp\n", myGame->p->inventory[id]->healPt);
                        myGame->p->currentHp += myGame->p->inventory[id]->healPt;
                        delItem(myGame->p->inventory, id);
                        if(myGame->p->currentHp > myGame->p->hp)
                        {
                            myGame->p->currentHp = myGame->p->hp;
                            openBag(myGame);
                            return;
                        }
                    }
                }
                if(strcmp("1", input)==0)
                {
                    printf("\tAre you sure you want to get rid of '");
                    printResource(myGame->p->inventory[id]->value);
                    printf("'? - Press 'y' or 'n'\n");
                    scanf("%s", input);
                    if(strcmp("y", input)==0)
                    {
                        printf("\t'");
                        printResource(myGame->p->inventory[id]->value);
                        printf("' has been deleted\n");
                        delItem(myGame->p->inventory, id);
                        openBag(myGame);
                        return;
                    }
                }
                openBag(myGame);
                return;
            case 't':
                printf("\t> Delete - Press '0'\n");
                printf("\t> Exit   - Press 'x'\n");
                scanf("%s", input);
                if(strcmp("0", input)==0)
                {
                    printf("Are you sure to get rid of this '");
                    printResource(myGame->p->inventory[id]->value);
                    printf("'? - Press 'y' or 'n'\n");
                    scanf("%s", input);
                    if(strcmp("y", input)==0)
                    {
                        printf("'");
                        printResource(myGame->p->inventory[id]->value);
                        printf("' has been deleted\n");
                        delItem(myGame->p->inventory, id);
                        openBag(myGame);
                        return;
                    }
                    openBag(myGame);
                    return;
                }
                openBag(myGame);
                return;
            
            default:
                openBag(myGame);
            }
        }
        if(strcmp(input, "1") == 0)
        {
            printWeaponSet(myGame->p);
            printf("\t> Select a weapon - Press an id [0-2]\n");
            scanf("%s", input);
            if(input[0] < '0' || input[0] > '2')
            {
                printf("Invalid value\n");
                openBag(myGame);
                return;
            }
            int id = atoi(input);
            if(id > 2)
            {
                printf("Invalid value\n");
                openBag(myGame);
                return;
            }
            printf("\t> [%d] ", id);
            printResource(myGame->p->weaponSet[id]->value);
            printf(" :\n");
            printf("\t> Put in inventory - Press '0'");
            printf("\t> Delete           - Press '1'\n");
            printf("\t> Exit             - Press 'x'\n");
            scanf("%s", input);
            if(strcmp("0", input) == 0)
            {
                if(isFullInventory(myGame->p->inventory) == 1)
                {
                    printf("\t Inventory is already full\n");
                    openBag(myGame);
                    return;
                }
                else
                {
                    printf("\t'");
                    printResource(myGame->p->weaponSet[id]->value);
                    printf("' has been replaced in your inventory\n");
                    moveWeaponToInventory(myGame, myGame->p->weaponSet, id);
                    openBag(myGame);
                    return;
                }
            }
            openBag(myGame);
            return;
        }
        if(strcmp(input, "2") == 0)
        {
            if(myGame->p->plastron->value == _ESPACE_LIBRE)
            {
                printf("You don't have armor equiped\n");
                openBag(myGame);
                return;
            }
            printf("\t You wear a '");
            printResource(myGame->p->plastron->value);
            printf("' which protects you from %d%% damage\n\n", myGame->p->plastron->dmgResist);
            printf("\t> Put in inventory - Press '0'\n");
            printf("\t> Delete           - Press '1'\n");
            printf("\t> Exit             - Press 'x'\n");
            scanf("%s", input);
            if(strcmp("0", input) == 0)
            {
                if(isFullInventory(myGame->p->inventory) == 1)
                {
                    printf("\t> Inventory is already full\n");
                    openBag(myGame);
                    return;
                }
                else
                {
                    printf("\t'");
                    printResource(myGame->p->plastron->value);
                    printf("' has been replaced in inventory\n");
                    addItem(myGame->p->inventory, myGame->p->plastron->value);
                    free(myGame->p->plastron);
                    myGame->p->plastron = createItem(_ESPACE_LIBRE, 0);
                    assert(myGame->p->plastron);
                    openBag(myGame);
                    return;
                }
            }
            if(strcmp("1", input) == 0)
            {
                printf("\tDelete '");
                printResource(myGame->p->plastron->value);
                printf("'? - Press 'y' or 'n'\n");
                scanf("%s", input);
                if(strcmp("y", input) == 0)
                {
                    printf("'");
                    printResource(myGame->p->plastron->value);
                    printf("' has been deleted\n");
                    free(myGame->p->plastron);
                    myGame->p->plastron = createItem(_ESPACE_LIBRE, 0);
                    assert(myGame->p->plastron);
                    openBag(myGame);
                    return; 
                }
                openBag(myGame);
                return;
            }
            openBag(myGame);
            return;
        }
}

#endif