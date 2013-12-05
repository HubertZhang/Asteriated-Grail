#include "elementalist.h"
#include "cardList.h"
#include"server.h"
#include"card.h"
#include"textgui.h"
extern CardList cardlist;
/*
 *1. 普通技：法术 【元素点燃】：（移除3点【元素】）对目标角色造成2点法术伤害③，额外+1【法术行动】；不能和【元素吸收】同时发动。
  2. 独有技：法术 【陨石】：对目标角色造成1点法术伤害③，额外+1【法术行动】；（若你额外弃1张地系牌【展示】①）本次伤害额外+1。
  3. 独有技：法术 【风刃】：对目标角色造成1点法术伤害③，额外+1【攻击行动】；（若你额外弃1张风系牌【展示】①）本次伤害额外+1。
  4. 独有技：法术 【火球】：对目标角色造成2点法术伤害③；（若你额外弃1张火系牌【展示】①）本次伤害额外+1。
  5. 独有技：法术 【雷击】：对目标角色造成1点法术伤害③，我方【战绩区】+1【宝石】；（若你额外弃1张雷系牌【展示】①）本次伤害额外+1。
  6. 独有技：法术 【冰冻】：对目标角色造成1点法术伤害③，并指定1名角色+1【治疗】；（若你额外弃1张水系牌【展示】①）本次伤害额外+1。
  7. 必杀技：法术 【月光】：【宝石】×1 对目标角色造成（X+1）点法术伤害③，X为你剩余的【能量】数。

 */
Elementalist::Elementalist(Server* server,int order,int teamNumber,int character):
    Player(server,order,teamNumber,character)
{
    //target = -1;
    server->textg->character[order]->setText("封印");
}
