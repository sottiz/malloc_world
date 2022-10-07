#include "malloc_world.h"
#include "direction.h"
#include "item.h"

#ifndef PLAYER_H
#define PLAYER_H
#endif

typedef struct player{
    // PLAYER STRUCT IS USED WHEN MAKING MOVE OR ANY ACTION TO UPDATE EACH ATTRIBUTE
    int level, currentHp, hp; // currentHp & hp ARE USED TO HAVE A 'currentHp'/'hp' INDEX (EX : 78 / 100 HP)
    // HP ATTRIBUTE CORRESPONDS WITH THE MAX HP WHICH THE PLAYER CAN HAVE
    int posX, posY;
    int currentMap;
    int direction;
    item **inventory;
    item **weaponSet;
    item *plastron;
    int activeWeapon;
    int currentXp;
    int nextLevel;

}player;

player *newPlayer();
void craftItem(player *p, int myCraftValue);
int getNbOfPossibleCrafts(player *p);
item **getPossibleCrafts(player *p);
void printCraftsList(player *p);
void printInventory(player *p);
void printWeaponSet(player *p);
void freePlayer(player *p);
void updateXp(player *p);

// CREATE A NEW PLAYER
player *newPlayer()
{
    player *p = malloc(sizeof(player *) + 10 * sizeof(int));
    assert(p);
    p->level = 1;
    p->currentHp = 100;
    p->hp = 100; // MAX HP
    p->currentMap = 1;
    p->inventory = malloc(10*sizeof(item *) + 10*9*sizeof(int));
    p->inventory = initInventory();
    
    addItem(p->inventory, _PIOCHE_EN_BOIS_);
    addItem(p->inventory, _SERPE_EN_BOIS_);
    addItem(p->inventory, _HACHE_EN_BOIS_);
    
    p->direction = _SOUTH_;
    
    p->weaponSet = malloc(3*sizeof(item *) + 3*9*sizeof(int));
    p->weaponSet = initWeaponSet();
    p->weaponSet[0] = createItem(_EPEE_EN_BOIS_, 1);
    p->activeWeapon = 0;
    p->currentXp = 0;
    p->nextLevel = 10;
    

    p->plastron = createItem(_ESPACE_LIBRE, 0);
    
    return p;
}

void updateXp(player *p)
{
    if(p->currentXp >= p->nextLevel)
    {
        p->level += 1;
        p->currentXp = p->currentXp- p->nextLevel;
        p->nextLevel = p->nextLevel*2;
        switch (p->level)
        {
        case 2:
            p->hp += 10;
            break;
        case 3:
            p->hp += 20;
            break;
        case 4:
            p->hp += 30;
            break;
        case 5:
            p->hp += 40;
            break;
        case 6:
            p->hp += 50;
            break;
        case 7:
            p->hp += 50;
            break;
        case 8:
            p->hp += 50;
            break;
        case 9:
            p->hp += 75;
            break;
        case 10:
            p->hp += 75;
            break;
        default:
            break;
        }
        p->currentHp = p->hp;
    }
}

void freePlayer(player *p)
{
    assert(p);
    assert(p->weaponSet);
    assert(p->inventory);
    assert(p->plastron);
    freeWeaponSet(p->weaponSet);
    freeInventory(p->inventory);
    free(p->plastron);
    free(p);
}

void printWeaponSet(player *p)
{
    printf("                 ___________________________________________________\n");
    printf("                /                                                   \\\n");
    printf("                |                    WEAPON SET                     |\n");
    printf("                |                                                   |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |                                                   |\n");
    printf("                |----+ [EMPLACEMENT]--[ARME]                        |\n");
    printf("                |    | SPECS                                        |\n");
    printf("                |    +-------------------------------------------+  |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |\n");

    for(int i=0; i<3; i++){
            printf("                +----+ [%d]--[", i);
            printResource(p->weaponSet[i]->value);  
            printf("]\n");
            if(p->weaponSet[i]->value != _ESPACE_LIBRE){
                printf("                |    | DURABILITY | DAMAGE |\n");
                printf("                |    |     %d     ", p->weaponSet[i]->durability);
                if(p->weaponSet[i]->durability < 10)
                    printf(" ");
                printf("| ");
                printf("  %d   ", p->weaponSet[i]->damage);
                if(p->weaponSet[i]->damage < 10)
                    printf(" ");
                printf("| ");

            }
            else
                printf("                |    |");
            printf("\n                |    +-------------------------------------------+\n");
    }
    printf("                \\___________________________________________________/\n");
}

void printInventory(player *p)
{
    printf("                 ___________________________________________________\n");
    printf("                /                                                   \\\n");
    printf("                |                     INVENTORY                     |\n");
    printf("                |                                                   |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |                                                   |\n");
    printf("                |----+ [EMPLACEMENT]--[ITEM]                        |\n");
    printf("                |    | SPECS                                        |\n");
    printf("                |    +-------------------------------------------+  |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |\n");

    for(int i=0; i<10; i++){
        printf("                +----+ [%d]--[", i);
        printResource(p->inventory[i]->value);  
        printf("]\n");
        if(p->inventory[i]->type != '0')
            printf("                |    | QTY |");
        else 
            printf("                |    |");
        switch (p->inventory[i]->type)
        {
        case 'w':
            printf("    TYPE     | DURABILITY | DAMAGE\n");
            printf("                |    |  %d ", p->inventory[i]->quantity);
            if(p->inventory[i]->quantity < 10)
                printf(" ");
            printf("| ");
            printf("   Weapon   |     %d      |    %d", p->inventory[i]->durability, p->inventory[i]->damage);
            if(p->inventory[i]->durability < 10)
                printf(" ");
            break;

        case 't':
            printf("    TYPE     | DURABILITY\n");
            printf("                |    |  %d ", p->inventory[i]->quantity);
            if(p->inventory[i]->quantity < 10)
                printf(" ");
            printf("| ");
            printf("   Tool     |      %d", p->inventory[i]->durability);
            break;

        case 'r':
            printf("    TYPE     | MAX QUANTITY\n");
            printf("                |    |  %d ", p->inventory[i]->quantity);
            if(p->inventory[i]->quantity < 10)
                printf(" ");
            printf("| ");
             printf(" Resource   |       %d", p->inventory[i]->maxQuant);
            break;
        
        case 's':
            printf("    TYPE     | DAMAGE RESISTANCE\n");
            printf("                |    |  %d ", p->inventory[i]->quantity);
            if(p->inventory[i]->quantity < 10)
                printf(" ");
            printf("| ");
             printf("  Shield    |        %d%%", p->inventory[i]->dmgResist);
            break;

        case 'h':
            printf("    TYPE     | HP HEALED\n");
            printf("                |    |  %d ", p->inventory[i]->quantity);
            if(p->inventory[i]->quantity < 10)
                printf(" ");
            printf("| ");
             printf("   Heal     |     %dHP", p->inventory[i]->healPt);
            break;
        
        default:
            break;
        }
        //TYPE  | MAX QUANTITY | DURABILITY | DAMAGE | RESISTANCE | HP HEALED |\n");

        printf("\n");
        //if(i != 10){
            printf("                |    +-------------------------------------------+\n");
        //}
    }
    printf("                \\___________________________________________________/\n");

}

void craftItem(player *p, int myCraftValue)
{
    assert(p);
    assert(p->inventory);

    int nbOfPossibleCrafts = getNbOfPossibleCrafts(p);
    item **possibleCrafts = malloc(nbOfPossibleCrafts * sizeof(item *) + nbOfPossibleCrafts * sizeof(int));
    assert(possibleCrafts);

    int indexCraftList = craftAlreadyPresent(possibleCrafts, myCraftValue, nbOfPossibleCrafts);
    if(indexCraftList < 0 || indexCraftList >= 3)
        return;

    int **ingredients = getIngredientsNeeded(myCraftValue);
    int indexPlant = itemAlreadyPresent(p->inventory, ingredients[_PLANT_INDEX_][_RESOURCE_]);
    int indexRoc = itemAlreadyPresent(p->inventory, ingredients[_ROC_INDEX_][_RESOURCE_]);
    int indexWood = itemAlreadyPresent(p->inventory, ingredients[_WOOD_INDEX_][_RESOURCE_]);

    p->inventory[indexPlant]->quantity -= ingredients[_PLANT_INDEX_][_QUANTITY_];
    p->inventory[indexRoc]->quantity -= ingredients[_ROC_INDEX_][_QUANTITY_];
    p->inventory[indexWood]->quantity -= ingredients[_WOOD_INDEX_][_QUANTITY_];

    addItem(p->inventory, myCraftValue);

    freeCraftsList(possibleCrafts, nbOfPossibleCrafts);
    free(ingredients);    
}

void printCraftsList(player *p)
{
    assert(p);
    assert(p->inventory);
    int nbOfCrafts = getNbOfPossibleCrafts(p);

    item **myCraftsList = malloc(nbOfCrafts*sizeof(item *)+nbOfCrafts*9*sizeof(int));
    assert(myCraftsList);
    
    for(int i=0; i<nbOfCrafts; i++){
        myCraftsList[i] = malloc(sizeof(item *));
        myCraftsList[i] = createItem(0, 0);
        assert(myCraftsList[i]);
    }

    myCraftsList = getPossibleCrafts(p);
    int **ingredients = malloc(3 * sizeof(int[2]) + 2 * sizeof(int));

    assert(ingredients);
    for(int i=0; i<3; i++){
        ingredients[i] = malloc(2 * sizeof(int));
        assert(ingredients[i]);
        for(int j=0; j<2; j++){
            ingredients[i][j] = 0;
        }
    }     
     
    printf("                 ___________________________________________________\n");
    printf("                /                                                   \\\n");
    printf("                |                   MAKE CRAFTING                   |\n");
    printf("                |                                                   |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |                                                   |\n");
    printf("                |----+ [N°][CRAFT]                                  |\n");
    printf("                |    | Ingredients                                  |\n");
    printf("                |    +-------------------------------------------+  |\n");
    printf("                +---------------------------------------------------+\n");
    printf("                |\n");

    for(int i=0; i<nbOfCrafts; i++){
            assert(myCraftsList[i]);
            printf("                +----+ [N°%d]--[", i);
            printResource(myCraftsList[i]->value);  
            printf("]\n");
            ingredients = getIngredientsNeeded(myCraftsList[i]->value);
            printf("                |    | ");
            if(ingredients[_PLANT_INDEX_][_QUANTITY_] > 0)
            {
                printf("%d x ", ingredients[_PLANT_INDEX_][_QUANTITY_]);
                printResource(ingredients[_PLANT_INDEX_][_RESOURCE_]);
                printf("  ");
            }
            if(ingredients[_ROC_INDEX_][_QUANTITY_] > 0)
            {
                printf("%d x ", ingredients[_ROC_INDEX_][_QUANTITY_]);
                printResource(ingredients[_ROC_INDEX_][_RESOURCE_]);
                printf("  ");
            }
            if(ingredients[_WOOD_INDEX_][_QUANTITY_] > 0)
            {
                printf("%d x ", ingredients[_WOOD_INDEX_][_QUANTITY_]);
                printResource(ingredients[_WOOD_INDEX_][_RESOURCE_]);
                printf("  ");
            }
            printf("  ");
            
            printf("\n                |    +-------------------------------------------+\n");
    }
    printf("                \\___________________________________________________/\n");
}

item **getPossibleCrafts(player *p)
{
    assert(p);
    assert(p->inventory);

    int nbOfCraft = getNbOfPossibleCrafts(p);

    item **craftsList = malloc(25*sizeof(item *)+25*9*sizeof(int));
    assert(craftsList);
    
    for(int i=0; i<25; i++){
        craftsList[i] = malloc(sizeof(item *) + 9 * sizeof(int));
        craftsList[i] = createItem(0, 0);
        assert(craftsList[i]);
    }

    int plantQty, rocQty, woodQty;
    int i=0;

    plantQty = getResourceQuantity(p->inventory, _HERBE_);
    rocQty = getResourceQuantity(p->inventory, _PIERRE_);
    woodQty = getResourceQuantity(p->inventory, _SAPIN_);

    if(isThisRecipePossible(_EPEE_EN_BOIS_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _EPEE_EN_BOIS_);
        i += 1;
    }
    if(isThisRecipePossible(_EPEE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _EPEE_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_LANCE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _LANCE_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_MARTEAU_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _MARTEAU_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_PLASTRON_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _PLASTRON_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_PIOCHE_EN_BOIS_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _PIOCHE_EN_BOIS_);
        i += 1;
    }
    if(isThisRecipePossible(_PIOCHE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _PIOCHE_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_HACHE_EN_BOIS_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _HACHE_EN_BOIS_);
        i += 1;
    }
    if(isThisRecipePossible(_HACHE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _HACHE_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_SERPE_EN_BOIS_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _SERPE_EN_BOIS_);
        i += 1;
    }
    if(isThisRecipePossible(_SERPE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _SERPE_EN_PIERRE_);
        i += 1;
    }
    if(isThisRecipePossible(_POTION_DE_VIE_1_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
        addCraftToCraftsList(craftsList, _POTION_DE_VIE_1_);
        i += 1;
    }

    if(p->currentMap > 1)
    {
        plantQty = getResourceQuantity(p->inventory, _LAVANDE_);
        rocQty = getResourceQuantity(p->inventory, _FER_);
        woodQty = getResourceQuantity(p->inventory, _HETRE_);

            if(isThisRecipePossible(_EPEE_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _EPEE_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_LANCE_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _LANCE_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_MARTEAU_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _MARTEAU_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_PLASTRON_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _PLASTRON_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_PIOCHE_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _PIOCHE_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_HACHE_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _HACHE_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_SERPE_EN_FER_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _SERPE_EN_FER_);
                i += 1;
            }
            if(isThisRecipePossible(_POTION_DE_VIE_2_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                addCraftToCraftsList(craftsList, _POTION_DE_VIE_2_);
                i += 1;
            }

            if(p->currentMap > 2)
            {
                plantQty = getResourceQuantity(p->inventory, _CHANVRE_);
                rocQty = getResourceQuantity(p->inventory, _DIAMANT_);
                woodQty = getResourceQuantity(p->inventory, _CHENE_);

                if(isThisRecipePossible(_EPEE_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                    addCraftToCraftsList(craftsList, _EPEE_EN_DIAMANT_);
                    i += 1;
                }
                if(isThisRecipePossible(_LANCE_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                    addCraftToCraftsList(craftsList, _LANCE_EN_DIAMANT_);
                    i += 1;
                }
                if(isThisRecipePossible(_MARTEAU_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                    addCraftToCraftsList(craftsList, _MARTEAU_EN_DIAMANT_);
                    i += 1;
                }
                if(isThisRecipePossible(_PLASTRON_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                    addCraftToCraftsList(craftsList, _PLASTRON_EN_DIAMANT_);
                    i += 1;
                }
                if(isThisRecipePossible(_POTION_DE_VIE_3_, plantQty, rocQty, woodQty) == 1 && i < nbOfCraft){
                    addCraftToCraftsList(craftsList, _POTION_DE_VIE_3_);
                    i += 1;
                }
            }
    }
    
    return craftsList;
}

int getNbOfPossibleCrafts(player *p)
{
    assert(p);
    assert(p->inventory);
    int nbOfPossibleReceipts = 0;
    int plantQty, rocQty, woodQty;

    plantQty = getResourceQuantity(p->inventory, _HERBE_);
    rocQty = getResourceQuantity(p->inventory, _PIERRE_);
    woodQty = getResourceQuantity(p->inventory, _SAPIN_);

    if(isThisRecipePossible(_EPEE_EN_BOIS_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_EPEE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_LANCE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_MARTEAU_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_PLASTRON_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_PIOCHE_EN_BOIS_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_PIOCHE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_HACHE_EN_BOIS_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_HACHE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_SERPE_EN_BOIS_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_SERPE_EN_PIERRE_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;
    if(isThisRecipePossible(_POTION_DE_VIE_1_, plantQty, rocQty, woodQty) == 1)
        nbOfPossibleReceipts += 1;

    if(p->currentMap > 1)
    {
        plantQty = getResourceQuantity(p->inventory, _LAVANDE_);
        rocQty = getResourceQuantity(p->inventory, _FER_);
        woodQty = getResourceQuantity(p->inventory, _HETRE_);

            if(isThisRecipePossible(_EPEE_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_LANCE_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_MARTEAU_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_PLASTRON_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_PIOCHE_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_HACHE_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_SERPE_EN_FER_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;
            if(isThisRecipePossible(_POTION_DE_VIE_2_, plantQty, rocQty, woodQty) == 1)
                nbOfPossibleReceipts += 1;

            if(p->currentMap > 2)
            {
                plantQty = getResourceQuantity(p->inventory, _CHANVRE_);
                rocQty = getResourceQuantity(p->inventory, _DIAMANT_);
                woodQty = getResourceQuantity(p->inventory, _CHENE_);

                if(isThisRecipePossible(_DIAMANT_, plantQty, rocQty, woodQty) == 1)
                    nbOfPossibleReceipts += 1;
                if(isThisRecipePossible(_LANCE_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1)
                    nbOfPossibleReceipts += 1;
                if(isThisRecipePossible(_MARTEAU_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1)
                    nbOfPossibleReceipts += 1;
                if(isThisRecipePossible(_PLASTRON_EN_DIAMANT_, plantQty, rocQty, woodQty) == 1)
                    nbOfPossibleReceipts += 1;
                if(isThisRecipePossible(_POTION_DE_VIE_3_, plantQty, rocQty, woodQty) == 1)
                    nbOfPossibleReceipts += 1;
            }
    }
    
    return nbOfPossibleReceipts;
}