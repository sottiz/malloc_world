#include "malloc_world.h"

#ifndef RESOURCE_H
#define RESOURCE_H

#define _ESPACE_LIBRE           0
#define _EPEE_EN_BOIS_          1
#define _PIOCHE_EN_BOIS_        2
#define _SERPE_EN_BOIS_         3
#define _HACHE_EN_BOIS_         4
#define _SAPIN_                 5
#define _PIERRE_                6
#define _HERBE_                 7
#define _EPEE_EN_PIERRE_        8
#define _LANCE_EN_PIERRE_       9
#define _MARTEAU_EN_PIERRE_     10
#define _PLASTRON_EN_PIERRE_    11
#define _PIOCHE_EN_PIERRE_      12
#define _SERPE_EN_PIERRE_       13
#define _HACHE_EN_PIERRE_       14
#define _POTION_DE_VIE_1_       15
#define _HETRE_                 16
#define _FER_                   17
#define _LAVANDE_               18
#define _EPEE_EN_FER_           19
#define _LANCE_EN_FER_          20
#define _MARTEAU_EN_FER_        21
#define _PLASTRON_EN_FER_       22
#define _PIOCHE_EN_FER_         23
#define _SERPE_EN_FER_          24
#define _HACHE_EN_FER_          25
#define _POTION_DE_VIE_2_       26
#define _CHENE_                 27
#define _DIAMANT_               28
#define _CHANVRE_               29
#define _EPEE_EN_DIAMANT_       30
#define _LANCE_EN_DIAMANT_      31
#define _MARTEAU_EN_DIAMANT_    32
#define _PLASTRON_EN_DIAMANT_   33
#define _POTION_DE_VIE_3_       34

void printResource(int id){
    switch (id)
    {
    case _ESPACE_LIBRE :
        printf("___Espace vide___");
        break;
    case _EPEE_EN_BOIS_ :
        printf("Epée en bois");
        break;
    case _PIOCHE_EN_BOIS_ :
        printf("Pioche en bois");
        break;
    case _SERPE_EN_BOIS_ :
        printf("Serpe en bois");
        break;
    case _HACHE_EN_BOIS_ :
        printf("Hache en bois");
        break;
    case _SAPIN_ :
        printf("Sapin");
        break;
    case _PIERRE_ :
        printf("Pierre");
        break;
    case _HERBE_ :
        printf("Herbe");
        break;
    case _EPEE_EN_PIERRE_ :
        printf("Epée en pierre");
        break;
    case _LANCE_EN_PIERRE_ :
        printf("Lance en pierre");
        break;
    case _MARTEAU_EN_PIERRE_ :
        printf("Marteau en pierre");
        break;
    case _PLASTRON_EN_PIERRE_ :
        printf("Plastron en pierre");
        break;
    case _PIOCHE_EN_PIERRE_ :
        printf("Pioche en pierre");
        break;
    case _SERPE_EN_PIERRE_ :
        printf("Serpe en pierre");
        break;
    case _HACHE_EN_PIERRE_ :
        printf("Hache en pierre");
        break;
    case _POTION_DE_VIE_1_ :
        printf("Potion de vie 1");
        break;
    case _HETRE_ :
        printf("Hêtre");
        break;
    case _FER_ :
        printf("Fer");
        break;
    case _LAVANDE_ :
        printf("Lavande");
        break;
    case _EPEE_EN_FER_ :
        printf("Epée en fer");
        break;
    case _LANCE_EN_FER_ :
        printf("Lance en fer");
        break;
    case _MARTEAU_EN_FER_ :
        printf("Marteau en fer");
        break;
    case _PLASTRON_EN_FER_ :
        printf("Plastron en fer");
        break;
    case _PIOCHE_EN_FER_ :
        printf("Pioche en fer");
        break;
    case _SERPE_EN_FER_ :
        printf("Serpe en fer");
        break;
    case _HACHE_EN_FER_ :
        printf("Hache en fer");
        break;
    case _POTION_DE_VIE_2_ :
        printf("Potion de vie 2");
        break;
    case _CHENE_ :
        printf("Chêne");
        break;
    case _DIAMANT_ :
        printf("Diamant");
        break;
    case _CHANVRE_ :
        printf("Chanvre");
        break;
    case _EPEE_EN_DIAMANT_ :
        printf("Epée en diamant");
        break;
    case _LANCE_EN_DIAMANT_ :
        printf("Lance en diamant");
        break;
    case _MARTEAU_EN_DIAMANT_ :
        printf("Marteau en diamant");
        break;
    case _PLASTRON_EN_DIAMANT_ :
        printf("Plastron en diamant");
        break;
    case _POTION_DE_VIE_3_ :
        printf("Potion de vie 3");
        break;
    
    default:
        printf("___Unknown___");
        break;
    }
}

char findType(int value){
    switch (value) {
        case _ESPACE_LIBRE:
            return '0';
        case _EPEE_EN_BOIS_ :
            return 'w';
        case _PIOCHE_EN_BOIS_ :
            return 't';
        case _SERPE_EN_BOIS_ :
            return 't';
        case _HACHE_EN_BOIS_ :
            return 't';
        case _SAPIN_ :
            return 'r';
        case _PIERRE_ :
            return 'r';
        case _HERBE_ :
            return 'r';
        case _EPEE_EN_PIERRE_ :
            return 'w';
        case _LANCE_EN_PIERRE_ :
            return 'w';
        case _MARTEAU_EN_PIERRE_ :
            return 'w';
        case _PLASTRON_EN_PIERRE_ :
            return 's';
        case _PIOCHE_EN_PIERRE_ :
            return 't';
        case _SERPE_EN_PIERRE_ :
            return 't';
        case _HACHE_EN_PIERRE_ :
            return 't';
        case _POTION_DE_VIE_1_ :
            return 'h';
        case _HETRE_ :
            return 'r';
        case _FER_ :
            return 'r';
        case _LAVANDE_ :
            return 'r';
        case _EPEE_EN_FER_ :
            return 'w';
        case _LANCE_EN_FER_ :
            return 'w';
        case _MARTEAU_EN_FER_ :
            return 'w';
        case _PLASTRON_EN_FER_ :
            return 's';
        case _PIOCHE_EN_FER_ :
            return 't';
        case _SERPE_EN_FER_ :
            return 't';
        case _HACHE_EN_FER_ :
            return 't';
        case _POTION_DE_VIE_2_ :
            return 'h';
        case _CHENE_ :
            return 'r';
        case _DIAMANT_ :
            return 'r';
        case _CHANVRE_ :
            return 'r';
        case _EPEE_EN_DIAMANT_ :
            return 'w';
        case _LANCE_EN_DIAMANT_ :
            return 'w';
        case _MARTEAU_EN_DIAMANT_ :
            return 'w';
        case _PLASTRON_EN_DIAMANT_ :
            return 's';
        case _POTION_DE_VIE_3_ :
            return 'h';
        
        default:
            return '0';
    }
}

int findDurability(int value) {
    switch (value) {
        case _EPEE_EN_BOIS_ :
            return 10;
        case _PIOCHE_EN_BOIS_ :
            return 10;
        case _SERPE_EN_BOIS_ :
            return 10;
        case _HACHE_EN_BOIS_ :
            return 10;
        case _EPEE_EN_PIERRE_ :
            return 10;
        case _LANCE_EN_PIERRE_ :
            return 8;
        case _MARTEAU_EN_PIERRE_ :
            return 5;
        case _PIOCHE_EN_PIERRE_ :
            return 10;
        case _SERPE_EN_PIERRE_ :
            return 10;
        case _HACHE_EN_PIERRE_ :
            return 10;
        case _EPEE_EN_FER_ :
            return 10;
        case _LANCE_EN_FER_ :
            return 8;
        case _MARTEAU_EN_FER_ :
            return 5;
        case _PIOCHE_EN_FER_ :
            return 10;
        case _SERPE_EN_FER_ :
            return 10;
        case _HACHE_EN_FER_ :
            return 10;
        case _EPEE_EN_DIAMANT_ :
            return 10;
        case _LANCE_EN_DIAMANT_ :
            return 8;
        case _MARTEAU_EN_DIAMANT_ :
            return 5;
        
        default:
            return 0;
    }
}

int findDamage(int value) {
    switch (value) {
        case _EPEE_EN_BOIS_ :
            return 1;
        case _EPEE_EN_PIERRE_ :
            return 2;
        case _LANCE_EN_PIERRE_ :
            return 3;
        case _MARTEAU_EN_PIERRE_ :
            return 4;
        case _EPEE_EN_FER_ :
            return 5;
        case _LANCE_EN_FER_ :
            return 7;
        case _MARTEAU_EN_FER_ :
            return 10;
        case _EPEE_EN_DIAMANT_ :
            return 10;
        case _LANCE_EN_DIAMANT_ :
            return 15;
        case _MARTEAU_EN_DIAMANT_ :
            return 20;
        
        default:
            return 0;
    }
}

int findDmgResist(int value){
    switch (value) {
        case _PLASTRON_EN_PIERRE_ :
            return 10;
        case _PLASTRON_EN_FER_ :
            return 20;
        case _PLASTRON_EN_DIAMANT_ :
            return 40;
        
        default:
            return 0;
    }
}

int findHealPt(int value){
    switch (value) {
        case _POTION_DE_VIE_1_ :
            return 30;
        case _POTION_DE_VIE_2_ :
            return 80;
        case _POTION_DE_VIE_3_ :
            return 200;
        
        default:
            return 0;
    }
}

char findWeaponType(int value){
    switch (value) {
        case _EPEE_EN_BOIS_ :
            return 'e';
        case _EPEE_EN_PIERRE_ :
            return 'e';
        case _LANCE_EN_PIERRE_ :
            return 'l';
        case _MARTEAU_EN_PIERRE_ :
            return 'm';
        case _EPEE_EN_FER_ :
            return 'e';
        case _LANCE_EN_FER_ :
            return 'l';
        case _MARTEAU_EN_FER_ :
            return 'm';
        case _EPEE_EN_DIAMANT_ :
            return 'e';
        case _LANCE_EN_DIAMANT_ :
            return 'l';
        case _MARTEAU_EN_DIAMANT_ :
            return 'm';
        
        default:
            return '0';
    }
}

#endif