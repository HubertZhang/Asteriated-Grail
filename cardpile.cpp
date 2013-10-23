#include <algorithm>
#include "cardpile.h"
#define CARDNUMBER 150
//这个→ →是上英语课的时候写的所以bug很多算法很烂→ →
//只是架构不要在意。。。
CardPile::CardPile()
{    
    extractPile = new int[CARDNUMBER];
    discardPile = new int[CARDNUMBER];
    endOfExtractPile = Pile+CARDNUMBER;
    endOfDiscardPile = DiscardPile;
    nextCard = extractPile;
}
int CardPile::getCard()
{
    int returnCard = *nextCard;
    if(nextCard == endOfExtractPile)
    {
        nextCard = extractPile;
        endOfExtractPile = extractPile;
        shuffleCard(discardPile,endOfDiscardPile);
        for(int* i = endOfDiscardPile;1;i--)
        {
            putIntoPile(i,endOfExtractPile);
            endOfExtractPile ++;
            if(i == discardPile)
            {
                endOfDiscardPile = discardPile;
                endOfExtractPile --;
                break;
            }
        }
    }
    nextCard ++;
    return returnCard;
}
void CardPile::initExtractPile()
{
    int card = 0;
    for(int* i = extractPile;1;i++)
    {
        *i = card;
        card ++;
        if(i == endOfExtractPile)
        {
            break;
        }
    }
    shuffleCard(extractPile,endOfExtractPile);
}
void CardPile::putIntoPile(int* card,int* site)
{
    *site = *card;
}
void CardPile::shuffleCard(int *start, int *end)
{
    random_shuffle(start,end);
}
