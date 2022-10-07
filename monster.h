#ifndef MONSTER_H
#define MONSTER_H
#include "malloc_world.h"
#include "case_values.h"
#include "resource.h"

typedef struct monster
{
    int id;
    int value;
    int level;
    int hpCurrent, hpMax;
    int damage;
    int xp;
    int posX, posY;
    int currentMap;
    int direction;
    int respawnCounter;
}monster;

monster *newMonster(int value)
{
    monster *myMonster = malloc(sizeof(myMonster) + 12*sizeof(int));
    assert(myMonster);
    myMonster->level = value < _MIN_MONSTER_2_ ? 1 : (value < _MIN_MONSTER_3_ ? 2 : 3);
    myMonster->hpMax = (value / 10) * 10;
    myMonster->hpCurrent = myMonster->hpMax;
    myMonster->damage = myMonster->level < 2 ? (2 + (value%5)) : (myMonster->level < 3 ? (5 + (value%10)) : (10 + (value%20)));
    myMonster->xp = value;
    myMonster->respawnCounter = 15;

    return myMonster;
}

#endif