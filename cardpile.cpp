#include <algorithm>
#include "cardpile.h"
using namespace std;
#define CARDNUMBER 150
//这个→ →是上英语课的时候写的所以bug很多算法很烂→ →
//只是架构不要在意。。。
CardPile::CardPile()
{    
    extractPile = new int[CARDNUMBER];
    discardPile = new int[CARDNUMBER];
    endOfExtractPile = extractPile+CARDNUMBER;
    endOfDiscardPile = discardPile;

    for (int i=0; i<CARDNUMBER; i++)
    {
        *(extractPile + i) = i;
    }

    random_shuffle(extractPile,endOfExtractPile);

    nextCard = extractPile;
}

CardPile::~CardPile()
{
    delete []extractPile;
    delete []discardPile;
}

int CardPile::getCard()
{
    int returnCard = *nextCard;
    if(nextCard == endOfExtractPile)
    {
        int* temp = discardPile;
        discardPile = extractPile;
        extractPile = temp;

        nextCard = extractPile;
        endOfExtractPile = endOfDiscardPile;
        endOfDiscardPile = discardPile;

        random_shuffle(extractPile,endOfExtractPile);
    }
    else
    {
        nextCard ++;
    }
    return returnCard;
}

void CardPile::putIntoPile(int card)
{
    *endOfDiscardPile = card;
    endOfDiscardPile++;
}

