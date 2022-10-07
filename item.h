#ifndef ITEM_H
#define ITEM_H
#include "malloc_world.h"
#include "resource.h"

#define _PLANT_INDEX_ 0
#define _ROC_INDEX_ 1
#define _WOOD_INDEX_ 2
#define _RESOURCE_ 0
#define _QUANTITY_ 1

typedef struct item{
    int value;
    int quantity;
    char type;
    int durability;
    int damage;
    int maxQuant;
    int dmgResist;
    int healPt;
    int maxDurability;
}item;

item *createItem(int value, int qty);
void freeItem(item *itm);
item **initInventory();
void freeInventory(item **myInventory);
int itemAlreadyPresent(item **myInventory, int value);
void addItem(item **myInventory, int value);
void delItem(item** myInventory, int index);
void freeWeaponSet(item **myWeaponSet);
item **initWeaponSet();
void delWeapon(item **myWeaponSet, int index);
int findNextDurability(item *tool, int resourceValue);
int indexToolNeeded(item **myInventory, int resourceValue);
void repairWeaponSetDurability(item **myWeaponSet);
void repairAllDurability(item **myInventory);
int getResourceQuantity(item **myInventory, int resourceValue);
int** getIngredientsNeeded(int myCraftValue);
int isThisRecipePossible(int myCraftValue, int myPlantQty, int myRocQty, int myWoodQty);
item **createCraftList(int length);
void addCraftToCraftsList(item **myCraftsList, int myCraftValue);
int craftAlreadyPresent(item **myCraftList, int myCraftValue, int length);
void freeCraftsList(item **myCraftsList, int length);
int isFullWeaponSet(item **myWeaponSet);
int isFullInventory(item **myInventory);
void updateResourceForCrafting(item **myInventory, int **myIngredients);

item *createItem(int value, int qty)
{
    item *itm = malloc(sizeof(itm)+9*sizeof(int));
    assert(itm);
    itm->value = value;
    itm->quantity = qty;
    itm->type = findType(value);
    itm->durability     = itm->type == 'w' || itm->type == 't' ? findDurability(value) : 0;
    itm->maxDurability  = itm->durability;
    itm->damage         = itm->type == 'w' ? findDamage(value) : 0;
    itm->maxQuant       = itm->type == 'r' ? 20 : 0;
    itm->dmgResist      = itm->type == 's' ? findDmgResist(value) : 0;
    itm->healPt         = itm->type == 'h' ? findHealPt(value) : 0;

    return itm;
}

void freeItem(item *itm)
{
    assert(itm);
    free(itm);
}

item **initInventory()
{
    item **myInventory = malloc(10*sizeof(item *)+10*9*sizeof(int));
    assert(myInventory);
    
    for(int i=0; i<10; i++){
        myInventory[i] = malloc(sizeof(item *));
        myInventory[i] = createItem(0, 0);
        assert(myInventory[i]);
    }

    return myInventory;
}

void freeInventory(item **myInventory)
{
    assert(myInventory);
    for(int i=0; i<10; i++){
        assert(myInventory[i]);
        free(myInventory[i]);
    }
    free(myInventory);
}

int itemAlreadyPresent(item **myInventory, int value)
{
    assert(myInventory);
    for(int i=0; i<10; i++){
        assert(myInventory[i]);
        if(myInventory[i]->value == value){
            return i;
        }
    }
    return -1;
}

void addItem(item **myInventory, int value)
{
    assert(myInventory);
    int index = itemAlreadyPresent(myInventory, value);
    if(findType(value) != 'r' || index == -1){
        for(int i=0; i<10; i++){
            assert(myInventory[i]);
            if(myInventory[i]->type == '0'){
                free(myInventory[i]);
                myInventory[i] = malloc(sizeof(item *));
                assert(myInventory[i]);
                myInventory[i] = createItem(value, 1);
                assert(myInventory[i]);
                return;
            }
        }
    } else {
        if(index > -1 && index < 10 && findType(value) == 'r'){
            assert(myInventory[index]);
            if(findType(value))
                myInventory[index]->quantity += 1;
        }
        return;
    }
}

void delItem(item** myInventory, int index)
{
    assert(myInventory);
    if(myInventory[index]->quantity != 1 && myInventory[index]->quantity > 0){
        assert(myInventory[index]);
        myInventory[index]->quantity -= 1;
        return;
    } else {
        free(myInventory[index]);
        myInventory[index] = createItem(_ESPACE_LIBRE, 0);
        assert(myInventory[index]);
        return;
    }
}

void freeWeaponSet(item **myWeaponSet)
{
    assert(myWeaponSet);
    for(int i=0; i<3; i++){
        assert(myWeaponSet[i]);
        free(myWeaponSet[i]);
    }
    free(myWeaponSet);
}

item **initWeaponSet()
{
    item **myWeaponSet = malloc(3*sizeof(item *) + 3*9*sizeof(int));
    assert(myWeaponSet);
    for(int i=0; i<3; i++){
        myWeaponSet[i] = malloc(sizeof(item *));
        myWeaponSet[i] = createItem(0, 0);
        assert(myWeaponSet[i]);
    }
    return myWeaponSet;
}

void delWeapon(item **myWeaponSet, int index)
{

    assert(myWeaponSet);
    free(myWeaponSet[index]);
    myWeaponSet[index] = createItem(_ESPACE_LIBRE, 0);
    assert(myWeaponSet[index]);
    return;
}

int findNextDurability(item *tool, int resourceValue)
{
    if(resourceValue == _PIERRE_ || resourceValue == _SAPIN_ || resourceValue == _HERBE_){
        return (tool->durability - ( (tool->maxDurability) / 10));
    }

    if(resourceValue == _FER_ || resourceValue == _HETRE_ || resourceValue == _LAVANDE_){
        return (tool->durability - ( (tool->maxDurability) /10) * 2);
    }

    if(resourceValue == _DIAMANT_ || resourceValue == _CHENE_ || resourceValue == _CHANVRE_){
        return (tool->durability - ( (tool->maxDurability) / 10) * 4);
    }
    return -1;
}

int indexToolNeeded(item **myInventory, int resourceValue)
{
    assert(myInventory);
    int index = -1;
    switch (resourceValue)
    {
    case _HERBE_:
        index = itemAlreadyPresent(myInventory, _SERPE_EN_BOIS_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _SERPE_EN_PIERRE_) : index;
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _SERPE_EN_FER_) : index;
        return index;

    case _PIERRE_:
        index = itemAlreadyPresent(myInventory, _PIOCHE_EN_BOIS_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _PIOCHE_EN_PIERRE_) : index;
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _PIOCHE_EN_FER_) : index;
        return index;

    case _SAPIN_:
        index = itemAlreadyPresent(myInventory, _HACHE_EN_BOIS_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _HACHE_EN_PIERRE_) : index;
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _HACHE_EN_FER_) : index;
        return index;

    case _LAVANDE_:
        index = itemAlreadyPresent(myInventory, _SERPE_EN_PIERRE_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _SERPE_EN_FER_) : index;
        return index;

    case _FER_:
        index = itemAlreadyPresent(myInventory, _PIOCHE_EN_PIERRE_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _PIOCHE_EN_FER_) : index;
        return index;

    case _HETRE_:
        index = itemAlreadyPresent(myInventory, _HACHE_EN_PIERRE_);
        index = (index < 0) || (findNextDurability(myInventory[index], resourceValue) < 1) ? itemAlreadyPresent(myInventory, _HACHE_EN_FER_) : index;
        return index;

    case _CHANVRE_:
        index = itemAlreadyPresent(myInventory, _SERPE_EN_FER_);
        return index;

    case _DIAMANT_:
        index = itemAlreadyPresent(myInventory, _PIOCHE_EN_FER_);
        return index;

    case _CHENE_:
        index = itemAlreadyPresent(myInventory, _HACHE_EN_FER_);
        return index;
        
    
    default:
        return index;
    }
    return index;
}

void repairWeaponSetDurability(item **myWeaponSet)
{
    assert(myWeaponSet);
    for(int i=0; i<3; i++)
    {
        assert(myWeaponSet[i]);
        myWeaponSet[i]->durability = myWeaponSet[i]->maxDurability;
    }
}

void repairAllDurability(item **myInventory)
{
    assert(myInventory);
    for(int i=0; i<10; i++){
        assert(myInventory[i]);
        if(myInventory[i]->maxDurability > 0)
            myInventory[i]->durability = myInventory[i]->maxDurability;
    }
    return;
}

int getResourceQuantity(item **myInventory, int resourceValue)
{
    assert(myInventory);

    int index = itemAlreadyPresent(myInventory, resourceValue);
    if(index < 0 || index >10)
        return 0;

    return myInventory[index]->quantity;
}

int** getIngredientsNeeded(int myCraftValue)
{
    int **recipe = malloc(3 * sizeof(int[2]));

    assert(recipe);
    for(int i=0; i<3; i++){
        recipe[i] = malloc(2 * sizeof(int));
        assert(recipe[i]);
        for(int j=0; j<2; j++){
            recipe[i][j] = 0;
        }
    }           
    switch (myCraftValue)
    {
    case _EPEE_EN_BOIS_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _EPEE_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _EPEE_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 4;
        return recipe;
    case _EPEE_EN_DIAMANT_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _CHENE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _DIAMANT_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 5;
        return recipe;
    
    case _LANCE_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 4;
        return recipe;
    case _LANCE_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _LANCE_EN_DIAMANT_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _CHENE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        recipe[_ROC_INDEX_][_RESOURCE_] = _DIAMANT_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 6;
        return recipe;

    case _MARTEAU_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 6;
        return recipe;
    case _MARTEAU_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 7;
        return recipe;
    case _MARTEAU_EN_DIAMANT_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _CHENE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _DIAMANT_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 8;
        return recipe;

    case _PLASTRON_EN_PIERRE_:
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 10;
        return recipe;
    case _PLASTRON_EN_FER_:
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 12;
        return recipe;
    case _PLASTRON_EN_DIAMANT_:
        recipe[_ROC_INDEX_][_RESOURCE_] = _DIAMANT_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 16;
        return recipe;

    case _PIOCHE_EN_BOIS_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _PIOCHE_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _PIOCHE_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 4;
        return recipe;

    case _HACHE_EN_BOIS_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _HACHE_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _HACHE_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 4;
        return recipe;

    case _SERPE_EN_BOIS_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _SERPE_EN_PIERRE_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _SAPIN_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _PIERRE_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 3;
        return recipe;
    case _SERPE_EN_FER_:
        recipe[_WOOD_INDEX_][_RESOURCE_] = _HETRE_;
        recipe[_WOOD_INDEX_][_QUANTITY_] = 2;
        recipe[_ROC_INDEX_][_RESOURCE_] = _FER_;
        recipe[_ROC_INDEX_][_QUANTITY_] = 4;
        return recipe;

    case _POTION_DE_VIE_1_:
        recipe[_PLANT_INDEX_][_RESOURCE_] = _HERBE_;
        recipe[_PLANT_INDEX_][_QUANTITY_] = 2;
        return recipe;
    case _POTION_DE_VIE_2_:
        recipe[_PLANT_INDEX_][_RESOURCE_] = _LAVANDE_;
        recipe[_PLANT_INDEX_][_QUANTITY_] = 2;
        return recipe;
    case _POTION_DE_VIE_3_:
        recipe[_PLANT_INDEX_][_RESOURCE_] = _CHANVRE_;
        recipe[_PLANT_INDEX_][_QUANTITY_] = 2;
        return recipe;
    
    default:
        return recipe;
    }
}

int isThisRecipePossible(int myCraftValue, int myPlantQty, int myRocQty, int myWoodQty)
{
    int res = 1;

    int **ingredientsNeeded = malloc(3 * sizeof(int[2]));
    assert(ingredientsNeeded);

    for(int i=0; i<3; i++){
        ingredientsNeeded[i] = malloc(2 * sizeof(int));
        assert(ingredientsNeeded[i]);
        for(int j=0; j<2; j++){
            ingredientsNeeded[i][j] = 0;
        }
    }
    ingredientsNeeded = getIngredientsNeeded(myCraftValue);
    
    if(ingredientsNeeded[_PLANT_INDEX_][_QUANTITY_] > myPlantQty)
        res = 0;
    if(ingredientsNeeded[_ROC_INDEX_][_QUANTITY_] > myRocQty)
        res = 0;
    if(ingredientsNeeded[_WOOD_INDEX_][_QUANTITY_] > myWoodQty)
        res = 0;

    free(ingredientsNeeded); 
    
    return res;
}

item **createCraftList(int length)
{
    item **craftsList = malloc(length * sizeof(item *) + length * 9 * sizeof(int));
    assert(craftsList);

    for(int i=0; i<length; i++){
        craftsList[i] = malloc(sizeof(item *));
        craftsList[i] = createItem(0, 0);
        assert(craftsList[i]);
    }

    return craftsList;
}

void addCraftToCraftsList(item **myCraftsList, int myCraftValue)
{
    assert(myCraftsList);
    for(int i=0; i<25; i++)
    {
        assert(myCraftsList[i]);
        if(myCraftsList[i]->value == 0)
        {
            myCraftsList[i] = createItem(myCraftValue, 1);
            assert(myCraftsList[i]);
            return;
        }
    }
}

int craftAlreadyPresent(item **myCraftList, int myCraftValue, int length)
{
    assert(myCraftList);
    for(int i=0; i<length; i++){
        assert(myCraftList[i]);
        if(myCraftList[i]->value == myCraftValue){
            return i;
        }
    }
    return -1;
}

void freeCraftsList(item **myCraftsList, int length)
{
    assert(myCraftsList);
    for(int i=0; i<length; i++)
    {
        assert(myCraftsList[i]);
        freeItem(myCraftsList[i]);
    }
    free(myCraftsList);
}

int isFullWeaponSet(item **myWeaponSet)
{
    assert(myWeaponSet);
    for(int i=0; i<3; i++)
    {
        if(myWeaponSet[i]->value == _ESPACE_LIBRE)
            return 0;
    }
    return 1;
}

int isFullInventory(item **myInventory)
{
    assert(myInventory);
    for(int i=0; i<10; i++)
    {
        assert(myInventory[i]);
        if(myInventory[i]->value == 0)
            return 0;
    }
    return 1;
}

void updateResourceForCrafting(item **myInventory, int **myIngredients)
{
    assert(myInventory);
    assert(myIngredients);

    int idxPlant = itemAlreadyPresent(myInventory, myIngredients[_PLANT_INDEX_][_RESOURCE_]);
    int idxRoc = itemAlreadyPresent(myInventory, myIngredients[_ROC_INDEX_][_RESOURCE_]);
    int idxBois = itemAlreadyPresent(myInventory, myIngredients[_WOOD_INDEX_][_RESOURCE_]);

    if(idxPlant < 0 || idxPlant > 9 || idxRoc < 0 || idxRoc > 9 || idxBois < 0 || idxBois > 9)
        return;

    assert(myInventory[idxPlant]);
    assert(myInventory[idxRoc]);
    assert(myInventory[idxBois]);

    for(int p=0; p<myIngredients[_PLANT_INDEX_][_QUANTITY_]; p++)
        delItem(myInventory, idxPlant);
    
    for(int p=0; p<myIngredients[_ROC_INDEX_][_QUANTITY_]; p++)
        delItem(myInventory, idxRoc);
    
    for(int p=0; p<myIngredients[_WOOD_INDEX_][_QUANTITY_]; p++)
        delItem(myInventory, idxBois);
}

#endif