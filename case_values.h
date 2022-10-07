#include "malloc_world.h"

#ifndef CASE_VALUES_H
#define CASE_VALUES_H

#define _TP_CASE_2_TO_3 -3
#define _TP_CASE_3_TO_2 -3
#define _TP_CASE_1_TO_2 -2
#define _TP_CASE_2_TO_1  -2
#define _INFRANCHISSABLE_  -1
#define _FREE_CASE_ 0
#define _PLAYER_ 1
#define _PNJ_CASE_ 2
#define _PLANT_1_ 3
#define _ROC_1_ 4
#define _BOIS_1_ 5
#define _PLANT_2_ 6
#define _ROC_2_ 7
#define _BOIS_2_ 8
#define _PLANT_3_ 9
#define _ROC_3_ 10
#define _BOIS_3_ 11
#define _MIN_MONSTER_1_ 12
#define _MAX_MONSTER_1_ 44
#define _MIN_MONSTER_2_ 45
#define _MAX_MONSTER_2_ 69
#define _MIN_MONSTER_3_ 70
#define _MAX_MONSTER_3_ 98
#define _BOSS_ 99

#define _IS_RESOURCE_ 100
#define _IS_TP_CASE_ 101
#define _IS_MONSTER_CASE_ 102

int findCaseType(int value) {
    if(value < _TP_CASE_2_TO_3 && value > _BOSS_)
        return value;

    switch (value)
    {
    case _PLAYER_:
        return _PLAYER_;
    case _PNJ_CASE_:
        return _PNJ_CASE_;
    case _TP_CASE_2_TO_3:
        return _IS_TP_CASE_;
    case _TP_CASE_1_TO_2:
        return _IS_TP_CASE_;
    case _INFRANCHISSABLE_:
        return _INFRANCHISSABLE_;
    case _FREE_CASE_:
        return _FREE_CASE_;
    case _PLANT_1_:
        return _IS_RESOURCE_;
    case _ROC_1_:
        return _IS_RESOURCE_;
    case _BOIS_1_:
        return _IS_RESOURCE_;
    case _PLANT_2_:
        return _IS_RESOURCE_;
    case _ROC_2_:
        return _IS_RESOURCE_;
    case _BOIS_2_:
        return _IS_RESOURCE_;
    case _PLANT_3_:
        return _IS_RESOURCE_;
    case _ROC_3_:
        return _IS_RESOURCE_;
    case _BOIS_3_:
        return _IS_RESOURCE_;
    case _BOSS_:
        return _BOSS_;
    default:
        if(value >= _MIN_MONSTER_1_ && value <= _MAX_MONSTER_3_)
            return _IS_MONSTER_CASE_;
        break;
    }
    return _INFRANCHISSABLE_;
}

#endif