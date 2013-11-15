#ifndef CARDPILE_H
#define CARDPILE_H
//The cardPile class is used to init the extractPile and the discardPile
class CardPile
{
private:
    int* extractPile;
    int* discardPile;
    int* endOfExtractPile;
    int* endOfDiscardPile;
    int* nextCard;
public:
    CardPile();
    ~CardPile();
    int getCard();  //发牌
    void putIntoPile(int card);   //放入弃牌堆
};

#endif // CARDPILE_H
