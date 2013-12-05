#include "magister.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
extern CardList cardlist;
/* 1.普通技：法术 【魔爆冲击】：（弃1张法术牌【展示】）指定2名对手各弃一张法术牌【展示】，若其中有人不如此做，则对他造成2点法术伤害③。只要有一名对手不如此做，我方【战绩区】+1【宝石】。
   2.普通技：被动B 【魔弹掌握】：你主动使用【魔弹】时可以选择逆向传递。
   3.普通技：被动B 【魔弹融合】：你的地系或火系牌可以当【魔弹】使用。
   4.必杀技：法术 【毁灭风暴】：【宝石】×1 对任意2名对手各造成2点法术伤害③。
*/

Magister::Magister(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    server->textg->character[order]->setText("魔导");
}

void Magister::magicAction()
{

}

void Magister::beMagicMissileAttack()
{

}
