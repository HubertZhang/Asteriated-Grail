#include "cardlist.h"

CardList::CardList()
{
}
CardList::~CardList()
{

}
void CardList::initCardList()
{
    cardList[0].ckind = skill;    cardList[0].cname = ground;   cardList[0].ctype = attack;  cardList[0].cnature = ground;     cardList[0].cskillone = 11;   cardList[0].cskilltwo = 32;
    cardList[1].ckind = blood;    cardList[1].cname = ground;   cardList[1].ctype = attack;  cardList[1].cnature = ground;     cardList[1].cskillone = 22;   cardList[1].cskilltwo = 231;
    cardList[2].ckind = holy;     cardList[2].cname = sheild;   cardList[2].ctype = magic;   cardList[2].cnature = wind;       cardList[2].cskillone = -1;   cardList[2].cskilltwo = -1;
    cardList[3].ckind = magical;  cardList[3].cname = wind;     cardList[3].ctype = attack;  cardList[3].cnature = wind;       cardList[3].cskillone = 44;   cardList[3].cskilltwo = 222;
    cardList[4].ckind = blood;    cardList[4].cname = ground;   cardList[4].ctype = attack;  cardList[4].cnature = ground;     cardList[4].cskillone = 22;   cardList[4].cskilltwo = 231;
    cardList[5].ckind = skill;    cardList[5].cname = thunder;  cardList[5].ctype = attack;  cardList[5].cnature = thunder;    cardList[5].cskillone = 12;   cardList[5].cskilltwo = 32;
    cardList[6].ckind = magical;  cardList[6].cname = wind;     cardList[6].ctype = attack;  cardList[6].cnature = wind;       cardList[6].cskillone = 44;   cardList[6].cskilltwo = 222;
    cardList[7].ckind = skill;    cardList[7].cname = water;    cardList[7].ctype = attack;  cardList[7].cnature = water;      cardList[7].cskillone = 12;   cardList[7].cskilltwo = 31;
    cardList[8].ckind = intone;   cardList[8].cname = ground;   cardList[8].ctype = attack;  cardList[8].cnature = ground;     cardList[8].cskillone = 161;  cardList[8].cskilltwo = 111;
    cardList[9].ckind = intone;   cardList[9].cname = ground;   cardList[9].ctype = attack;  cardList[9].cnature = ground;     cardList[9].cskillone = 161;  cardList[9].cskilltwo = 111;
    cardList[10].ckind = skill;   cardList[10].cname = water;   cardList[10].ctype = attack; cardList[10].cnature = water;     cardList[10].cskillone = 12;  cardList[10].cskilltwo = 31;
    cardList[11].ckind = magical; cardList[11].cname = light;   cardList[11].ctype = magic;  cardList[11].cnature = light;     cardList[11].cskillone = -1;  cardList[11].cskilltwo = -1;
    cardList[12].ckind = magical; cardList[12].cname = sheild;  cardList[12].ctype = magic;  cardList[12].cnature = thunder;   cardList[12].cskillone = -1;  cardList[12].cskilltwo = -1;
    cardList[13].ckind = holy;    cardList[13].cname = sheild;  cardList[13].ctype = magic;  cardList[12].cnature = ground;    cardList[13].cskillone = -1;  cardList[13].cskilltwo = -1;
    cardList[14].ckind = magical; cardList[14].cname = sheild;  cardList[14].ctype = magic;  cardList[14].cnature = ground;    cardList[14].cskillone = -1;  cardList[14].cskilltwo = -1;
    cardList[15].ckind = holy;    cardList[15].cname = wind;    cardList[15].ctype = attack; cardList[15].cnature = wind;      cardList[15].cskillone = 62;  cardList[15].cskilltwo = 71;
    cardList[16].ckind = holy;    cardList[16].cname = thunder; cardList[16].ctype = attack; cardList[16].cnature = thunder;   cardList[16].cskillone = 61;  cardList[16].cskilltwo = 71;
    cardList[17].ckind = skill;   cardList[17].cname = thunder; cardList[17].ctype = attack; cardList[17].cnature = thunder;   cardList[17].cskillone = 12;  cardList[17].cskilltwo = 32;
    cardList[18].ckind = intone;  cardList[18].cname = water;   cardList[18].ctype = attack; cardList[18].cnature = water;     cardList[18].cskillone = 161; cardList[18].cskilltwo = 112;
    cardList[19].ckind = intone;  cardList[19].cname = water;   cardList[19].ctype = attack; cardList[19].cnature = water;     cardList[19].cskillone = 161; cardList[19].cskilltwo = 112;
    cardList[20].ckind = blood;   cardList[20].cname = light;   cardList[20].ctype = magic;  cardList[20].cnature = light;     cardList[20].cskillone = -1;  cardList[20].cskilltwo = -1;
    cardList[21].ckind = magical; cardList[21].cname = light;   cardList[21].ctype = magic;  cardList[21].cnature = light;     cardList[21].cskillone = -1;  cardList[21].cskilltwo = -1;
    cardList[22].ckind = holy;    cardList[22].cname = water;   cardList[22].ctype = attack; cardList[22].cnature = water;     cardList[22].cskillone = 62;  cardList[22].cskilltwo = 71;
    cardList[23].ckind = holy;    cardList[23].cname = water;   cardList[23].ctype = attack; cardList[23].cnature = water;     cardList[23].cskillone = 62;  cardList[23].cskilltwo = 71;
    cardList[24].ckind = intone;  cardList[24].cname = dark;    cardList[24].ctype = attack; cardList[24].cnature = dark;      cardList[24].cskillone = -1;  cardList[24].cskilltwo = -1;
    cardList[25].ckind = skill;   cardList[25].cname = ground;  cardList[25].ctype = attack; cardList[25].cnature = ground;    cardList[25].cskillone = 11;  cardList[25].cskilltwo = 32;
    cardList[26].ckind = intone;  cardList[26].cname = water;   cardList[26].ctype = attack; cardList[26].cnature = water;     cardList[26].cskillone = 161; cardList[26].cskilltwo = 112;
    cardList[27].ckind = intone;  cardList[27].cname = water;   cardList[27].ctype = attack; cardList[27].cnature = water;     cardList[27].cskillone = 161; cardList[27].cskilltwo = 112;
    cardList[28].ckind = holy;    cardList[28].cname = dark;    cardList[28].ctype = attack; cardList[28].cnature = dark;      cardList[28].cskillone = -1;  cardList[28].cskilltwo = -1;
    cardList[29].ckind = holy;    cardList[29].cname = dark;    cardList[29].ctype = attack; cardList[29].cnature = dark;      cardList[29].cskillone = -1;  cardList[29].cskilltwo = -1;
    cardList[30].ckind = holy;    cardList[30].cname = dark;    cardList[30].ctype = attack; cardList[30].cnature = dark;      cardList[30].cskillone = -1;  cardList[30].cskilltwo = -1;
    cardList[31].ckind = intone;  cardList[31].cname = dark;    cardList[31].ctype = attack; cardList[31].cnature = dark;      cardList[31].cskillone = -1;  cardList[31].cskilltwo = -1;
    cardList[32].ckind = holy;    cardList[32].cname = water;   cardList[32].ctype = attack; cardList[32].cnature = water;     cardList[32].cskillone = 62;  cardList[32].cskilltwo = 71;
    cardList[33].ckind = intone;  cardList[33].cname = massile; cardList[33].ctype = magic;  cardList[33].cnature = fire;      cardList[33].cskillone = -1;  cardList[33].cskilltwo = -1;
    cardList[34].ckind = magical; cardList[34].cname = massile; cardList[34].ctype = magic;  cardList[34].cnature = water;     cardList[34].cskillone = -1;  cardList[34].cskilltwo = -1;
    cardList[35].ckind = magical; cardList[35].cname = massile; cardList[35].ctype = magic;  cardList[35].cnature = water;     cardList[35].cskillone = -1;  cardList[35].cskilltwo = -1;
    cardList[36].ckind = blood;   cardList[36].cname = sheild;  cardList[36].ctype = magic;  cardList[36].cnature = thunder;   cardList[36].cskillone = -1;  cardList[36].cskilltwo = -1;
    cardList[37].ckind = magical; cardList[37].cname = massile; cardList[37].ctype = magic;  cardList[37].cnature = wind;      cardList[37].cskillone = -1;  cardList[37].cskilltwo = -1;
    cardList[38].ckind = holy;    cardList[38].cname = massile; cardList[38].ctype = magic;  cardList[38].cnature = thunder;   cardList[38].cskillone = -1;  cardList[38].cskilltwo = -1;
    cardList[39].ckind = skill;   cardList[39].cname = massile; cardList[39].ctype = magic;  cardList[39].cnature = thunder;   cardList[39].cskillone = -1;  cardList[39].cskilltwo = -1;
    cardList[40].ckind = blood;   cardList[40].cname = thunder; cardList[40].ctype = attack; cardList[40].cnature = thunder;   cardList[40].cskillone = 21;  cardList[40].cskilltwo = 231;
    cardList[41].ckind = blood;   cardList[41].cname = thunder; cardList[41].ctype = attack; cardList[41].cnature = thunder;   cardList[41].cskillone = 21;  cardList[41].cskilltwo = 231;
    cardList[42].ckind = skill;   cardList[42].cname = ground;  cardList[42].ctype = attack; cardList[42].cnature = ground;    cardList[42].cskillone = 11;  cardList[42].cskilltwo = 32;
    cardList[43].ckind = skill;   cardList[43].cname = ground;  cardList[43].ctype = attack; cardList[43].cnature = ground;    cardList[43].cskillone = 11;  cardList[43].cskilltwo = 32;
    cardList[44].ckind = magical; cardList[44].cname = wind;    cardList[44].ctype = attack; cardList[44].cnature = wind;      cardList[44].cskillone = 44;  cardList[44].cskilltwo = 222;
    cardList[45].ckind = magical; cardList[45].cname = wind;    cardList[45].ctype = attack; cardList[45].cnature = wind;      cardList[45].cskillone = 44;  cardList[45].cskilltwo = 222;
    cardList[46].ckind = blood;   cardList[46].cname = wind;    cardList[46].ctype = attack; cardList[46].cnature = wind;      cardList[46].cskillone = 22;  cardList[46].cskilltwo = 231;
    cardList[47].ckind = skill;   cardList[47].cname = thunder; cardList[47].ctype = attack; cardList[47].cnature = thunder;   cardList[47].cskillone = 11;  cardList[47].cskilltwo = 32;
    cardList[48].ckind = skill;   cardList[48].cname = thunder; cardList[48].ctype = attack; cardList[48].cnature = thunder;   cardList[48].cskillone = 11;  cardList[48].cskilltwo = 32;
    cardList[49].ckind = skill;   cardList[49].cname = poison;  cardList[49].ctype = magic;  cardList[49].cnature = water;     cardList[49].cskillone = -1;  cardList[49].cskilltwo = -1;
    cardList[50].ckind = blood;   cardList[50].cname = sheild;  cardList[50].ctype = magic;  cardList[50].cnature = fire;      cardList[50].cskillone = -1;  cardList[50].cskilltwo = -1;
    cardList[51].ckind = blood;   cardList[51].cname = sheild;  cardList[51].ctype = magic;  cardList[51].cnature = fire;      cardList[51].cskillone = -1;  cardList[51].cskilltwo = -1;
    cardList[52].ckind = blood;   cardList[52].cname = wind;    cardList[52].ctype = attack; cardList[52].cnature = wind;      cardList[52].cskillone = 22;  cardList[52].cskilltwo = 231;
    cardList[53].ckind = blood;   cardList[53].cname = wind;    cardList[53].ctype = attack; cardList[53].cnature = wind;      cardList[53].cskillone = 22;  cardList[53].cskilltwo = 231;
    cardList[54].ckind = intone;  cardList[54].cname = fire;    cardList[54].ctype = attack; cardList[54].cnature = fire;      cardList[54].cskillone = 161; cardList[54].cskilltwo = 113;
    cardList[55].ckind = intone;  cardList[55].cname = fire;    cardList[55].ctype = attack; cardList[55].cnature = fire;      cardList[55].cskillone = 161; cardList[55].cskilltwo = 113;
    cardList[56].ckind = blood;   cardList[56].cname = fire;    cardList[56].ctype = attack; cardList[56].cnature = fire;      cardList[56].cskillone = 22;  cardList[56].cskilltwo = 231;
    cardList[57].ckind = skill;   cardList[57].cname = wind;    cardList[57].ctype = attack; cardList[57].cnature = wind;      cardList[57].cskillone = 12;  cardList[57].cskilltwo = 32;
    cardList[58].ckind = intone;  cardList[58].cname = ground;  cardList[58].ctype = attack; cardList[58].cnature = ground;    cardList[58].cskillone = 162; cardList[58].cskilltwo = 111;
    cardList[59].ckind = intone;  cardList[59].cname = thunder; cardList[59].ctype = attack; cardList[59].cnature = thunder;   cardList[59].cskillone = 162; cardList[59].cskilltwo = 115;
    cardList[60].ckind = holy;    cardList[60].cname = light;   cardList[60].ctype = magic;  cardList[60].cnature = attack;    cardList[60].cskillone = 61;  cardList[60].cskilltwo = -1;
    cardList[61].ckind = magical; cardList[61].cname = thunder; cardList[61].ctype = attack; cardList[61].cnature = thunder;   cardList[61].cskillone = 45;  cardList[61].cskilltwo = 221;
    cardList[62].ckind = intone;  cardList[62].cname = water;   cardList[62].ctype = attack; cardList[62].cnature = water;     cardList[62].cskillone = 161; cardList[62].cskilltwo = 112;
    cardList[63].ckind = blood;cardList[63].cname = fire;cardList[63].ctype = attack;cardList[63].cnature = fire;cardList[63].cskillone = 22;cardList[63].cskilltwo = 231;
    cardList[64].ckind = skill;cardList[64].cname = poison;cardList[64].ctype = magic;cardList[64].cnature = thunder;cardList[64].cskillone = -1;cardList[64].cskilltwo = -1;
    cardList[65].ckind = skill;cardList[65].cname = fire;cardList[65].ctype = attack;cardList[65].cnature = fire;cardList[65].cskillone = 12;cardList[65].cskilltwo = 31;
    cardList[66].ckind = skill;cardList[66].cname = light;cardList[66].ctype = magic;cardList[66].cnature = light;cardList[66].cskillone = -1;cardList[66].cskilltwo = -1;
    cardList[67].ckind = intone;cardList[67].cname = sheild;cardList[67].ctype = magic;cardList[67].cnature = wind;cardList[67].cskillone = -1;cardList[67].cskilltwo = -1;
    cardList[68].ckind = holy;cardList[68].cname = dark;cardList[68].ctype = attack;cardList[68].cnature = dark;cardList[68].cskillone = -1;cardList[68].cskilltwo = -1;
    cardList[69].ckind = holy;cardList[69].cname = fire;cardList[69].ctype = attack;cardList[69].cnature = fire;cardList[69].cskillone = 61;cardList[69].cskilltwo = 71;
    cardList[70].ckind = intone;cardList[70].cname = sheild;cardList[70].ctype = magic;cardList[70].cnature = ground;cardList[70].cskillone = -1;cardList[70].cskilltwo = -1;
    cardList[71].ckind = blood;cardList[71].cname = sheild;cardList[71].ctype = magic;cardList[71].cnature = wind;cardList[71].cskillone = -1;cardList[71].cskilltwo = -1;
    cardList[72].ckind = holy;cardList[72].cname = thunder;cardList[72].ctype = attack;cardList[72].cnature = thunder;cardList[72].cskillone = 61;cardList[72].cskilltwo = 71;
    cardList[73].ckind = holy;cardList[73].cname = thunder;cardList[73].ctype = attack;cardList[73].cnature = thunder;cardList[73].cskillone = 61;cardList[73].cskilltwo = 71;
    cardList[74].ckind = holy;cardList[74].cname = poison;cardList[74].ctype = magic;cardList[74].cnature = wind;cardList[74].cskillone = -1;cardList[74].cskilltwo = -1;
    cardList[75].ckind = holy;cardList[75].cname = wind;cardList[75].ctype = attack;cardList[75].cnature = wind;cardList[75].cskillone = 62;cardList[75].cskilltwo = 71;
    cardList[76].ckind = holy;cardList[76].cname = wind;cardList[76].ctype = attack;cardList[76].cnature = wind;cardList[76].cskillone = 62;cardList[76].cskilltwo = 71;
    cardList[77].ckind = holy;cardList[77].cname = fire;cardList[77].ctype = attack;cardList[77].cnature = fire;cardList[77].cskillone = 61;cardList[77].cskilltwo = 71;
    cardList[78].ckind = skill;cardList[78].cname = light;cardList[78].ctype = magic;cardList[78].cnature = light;cardList[78].cskillone = -1;cardList[78].cskilltwo = -1;
    cardList[79].ckind = intone;cardList[79].cname = water;cardList[79].ctype = attack;cardList[79].cnature = water;cardList[79].cskillone = 161;cardList[79].cskilltwo = 112;
    cardList[80].ckind = blood;cardList[80].cname = water;cardList[80].ctype = attack;cardList[80].cnature = water;cardList[80].cskillone = 21;cardList[80].cskilltwo = 231;
    cardList[81].ckind = blood;cardList[81].cname = water;cardList[81].ctype = attack;cardList[81].cnature = water;cardList[81].cskillone = 21;cardList[81].cskilltwo = 231;
    cardList[82].ckind = blood;cardList[82].cname = water;cardList[82].ctype = attack;cardList[82].cnature = water;cardList[82].cskillone = 21;cardList[82].cskilltwo = 231;
    cardList[83].ckind = magical;cardList[83].cname = poison;cardList[83].ctype = magic;cardList[83].cnature = water;cardList[83].cskillone = -1;cardList[83].cskilltwo = -1;
    cardList[84].ckind = skill;cardList[84].cname = water;cardList[84].ctype = attack;cardList[84].cnature = water;cardList[84].cskillone = 12;cardList[84].cskilltwo = 31;
    cardList[85].ckind = skill;cardList[85].cname = water;cardList[85].ctype = attack;cardList[85].cnature = water;cardList[85].cskillone = 12;cardList[85].cskilltwo = 31;
    cardList[86].ckind = blood;cardList[86].cname = weak;cardList[86].ctype = magic;cardList[86].cnature = water;cardList[86].cskillone = -1;cardList[86].cskilltwo = -1;
    cardList[87].ckind = blood;cardList[87].cname = water;cardList[87].ctype = attack;cardList[87].cnature = water;cardList[87].cskillone = 21;cardList[87].cskilltwo = 231;
    cardList[88].ckind = holy;cardList[88].cname = thunder;cardList[88].ctype = attack;cardList[88].cnature = thunder;cardList[88].cskillone = 61;cardList[88].cskilltwo = 71;
    cardList[89].ckind = holy;cardList[89].cname = light;cardList[89].ctype = magic;cardList[89].cnature = light;cardList[89].cskillone = 62;cardList[89].cskilltwo = -1;
    cardList[90].ckind = skill;cardList[90].cname = weak;cardList[90].ctype = magic;cardList[90].cnature = ground;cardList[90].cskillone = -1;cardList[90].cskilltwo = -1;
    cardList[91].ckind = skill;cardList[91].cname = fire;cardList[91].ctype = attack;cardList[91].cnature = fire;cardList[91].cskillone = 12;cardList[91].cskilltwo = 31;
    cardList[92].ckind = holy;cardList[92].cname = weak;cardList[92].ctype = magic;cardList[92].cnature = fire;cardList[92].cskillone = -1;cardList[92].cskilltwo = -1;
    cardList[93].ckind = blood;cardList[93].cname = weak;cardList[93].ctype = magic;cardList[93].cnature = fire;cardList[93].cskillone = -1;cardList[93].cskilltwo = -1;
    cardList[94].ckind = magical;cardList[94].cname = weak;cardList[94].ctype = magic;cardList[94].cnature = wind;cardList[94].cskillone = -1;cardList[94].cskilltwo = -1;
    cardList[95].ckind = skill;cardList[95].cname = fire;cardList[95].ctype = attack;cardList[95].cnature = fire;cardList[95].cskillone = 12;cardList[95].cskilltwo = 31;
    cardList[96].ckind = skill;cardList[96].cname = fire;cardList[96].ctype = attack;cardList[96].cnature = fire;cardList[96].cskillone = 12;cardList[96].cskilltwo = 31;
    cardList[97].ckind = intone;cardList[97].cname = weak;cardList[97].ctype = magic;cardList[97].cnature = water;cardList[97].cskillone = -1;cardList[97].cskilltwo = -1;
    cardList[98].ckind = magical;cardList[98].cname = thunder;cardList[98].ctype = attack;cardList[98].cnature = thunder;cardList[98].cskillone = 45;cardList[98].cskilltwo = 221;
    cardList[99].ckind = magical;cardList[99].cname = thunder;cardList[99].ctype = attack;cardList[99].cnature = thunder;cardList[99].cskillone = 45;cardList[99].cskilltwo = 221;
    cardList[100].ckind = intone;cardList[100].cname = ground;cardList[100].ctype = attack;cardList[100].cnature = ground;cardList[100].cskillone = 162;cardList[100].cskilltwo = 111;
    cardList[101].ckind = skill;cardList[101].cname = ground;cardList[101].ctype = attack;cardList[101].cnature = ground;cardList[101].cskillone = 11;cardList[101].cskilltwo = 32;
    cardList[102].ckind = magical;cardList[102].cname = thunder;cardList[102].ctype = attack;cardList[102].cnature = thunder;cardList[102].cskillone = 45;cardList[102].cskilltwo = 221;
    cardList[103].ckind = skill;cardList[103].cname = light;cardList[103].ctype = magic;cardList[103].cnature = light;cardList[103].cskillone = -1;cardList[103].cskilltwo = -1;
    cardList[104].ckind = blood;cardList[104].cname = light;cardList[104].ctype = magic;cardList[104].cnature = light;cardList[104].cskillone = -1;cardList[104].cskilltwo = -1;
    cardList[105].ckind = blood;cardList[105].cname = light;cardList[105].ctype = magic;cardList[105].cnature = light;cardList[105].cskillone = -1;cardList[105].cskilltwo = -1;
    cardList[106].ckind = blood;cardList[106].cname = thunder;cardList[106].ctype = attack;cardList[106].cnature = thunder;cardList[106].cskillone = 21;cardList[106].cskilltwo = 231;
    cardList[107].ckind = blood;cardList[107].cname = fire;cardList[107].ctype = attack;cardList[107].cnature = fire;cardList[107].cskillone = 22;cardList[107].cskilltwo = 231;
    cardList[108].ckind = blood;cardList[108].cname = fire;cardList[108].ctype = attack;cardList[108].cnature = fire;cardList[108].cskillone = 22;cardList[108].cskilltwo = 231;
    cardList[109].ckind = magical;cardList[109].cname = ground;cardList[109].ctype = attack;cardList[109].cnature = ground;cardList[109].cskillone = 41;cardList[109].cskilltwo = 231;
    cardList[110].ckind = intone;cardList[110].cname = poison;cardList[110].ctype = magic;cardList[110].cnature = ground;cardList[110].cskillone = -1;cardList[110].cskilltwo = -1;
    cardList[111].ckind = holy;cardList[111].cname = poison;cardList[111].ctype = magic;cardList[111].cnature = water;cardList[111].cskillone = -1;cardList[111].cskilltwo = -1;
    cardList[112].ckind = skill;cardList[112].cname = wind;cardList[112].ctype = attack;cardList[112].cnature = wind;cardList[112].cskillone = 12;cardList[112].cskilltwo = 32;
    cardList[113].ckind = skill;cardList[113].cname = wind;cardList[113].ctype = attack;cardList[113].cnature = wind;cardList[113].cskillone = 12;cardList[113].cskilltwo = 32;
    cardList[114].ckind = intone;cardList[114].cname = fire;cardList[114].ctype = attack;cardList[114].cnature = fire;cardList[114].cskillone = 161;cardList[114].cskilltwo = 113;
    cardList[115].ckind = intone;cardList[115].cname = wind;cardList[115].ctype = attack;cardList[115].cnature = wind;cardList[115].cskillone = 162;cardList[115].cskilltwo = 114;
    cardList[116].ckind = intone;cardList[116].cname = wind;cardList[116].ctype = attack;cardList[116].cnature = wind;cardList[116].cskillone = 162;cardList[116].cskilltwo = 114;
    cardList[117].ckind = holy;cardList[117].cname = fire;cardList[117].ctype = attack;cardList[117].cnature = fire;cardList[117].cskillone = 61;cardList[117].cskilltwo = 71;
    cardList[118].ckind = magical;cardList[118].cname = fire;cardList[118].ctype = attack;cardList[118].cnature = fire;cardList[118].cskillone = 43;cardList[118].cskilltwo = 221;
    cardList[119].ckind = intone;cardList[119].cname = fire;cardList[119].ctype = attack;cardList[119].cnature = fire;cardList[119].cskillone = 161;cardList[119].cskilltwo = 113;
    cardList[120].ckind = magical;cardList[120].cname = water;cardList[120].ctype = attack;cardList[120].cnature = water;cardList[120].cskillone = 42;cardList[120].cskilltwo = 222;
    cardList[121].ckind = magical;cardList[121].cname = water;cardList[121].ctype = attack;cardList[121].cnature = water;cardList[121].cskillone = 42;cardList[121].cskilltwo = 222;
    cardList[122].ckind = magical;cardList[122].cname = water;cardList[122].ctype = attack;cardList[122].cnature = water;cardList[122].cskillone = 42;cardList[122].cskilltwo = 221;
    cardList[123].ckind = magical;cardList[123].cname = water;cardList[123].ctype = attack;cardList[123].cnature = water;cardList[123].cskillone = 42;cardList[123].cskilltwo = 221;
    cardList[124].ckind = blood;cardList[124].cname = thunder;cardList[124].ctype = attack;cardList[124].cnature = thunder;cardList[124].cskillone = 21;cardList[124].cskilltwo = 231;
    cardList[125].ckind = intone;cardList[125].cname = thunder;cardList[125].ctype = attack;cardList[125].cnature = thunder;cardList[125].cskillone = 162;cardList[125].cskilltwo = 115;
    cardList[126].ckind = intone;cardList[126].cname = thunder;cardList[126].ctype = attack;cardList[126].cnature = thunder;cardList[126].cskillone = 162;cardList[126].cskilltwo = 115;
    cardList[127].ckind = intone;cardList[127].cname = thunder;cardList[127].ctype = attack;cardList[127].cnature = thunder;cardList[127].cskillone = 162;cardList[127].cskilltwo = 115;
    cardList[128].ckind = magical;cardList[128].cname = fire;cardList[128].ctype = attack;cardList[128].cnature = fire;cardList[128].cskillone = 43;cardList[128].cskilltwo = 221;
    cardList[129].ckind = magical;cardList[129].cname = fire;cardList[129].ctype = attack;cardList[129].cnature = fire;cardList[129].cskillone = 43;cardList[129].cskilltwo = 221;
    cardList[130].ckind = magical;cardList[130].cname = fire;cardList[130].ctype = attack;cardList[130].cnature = fire;cardList[130].cskillone = 43;cardList[130].cskilltwo = 221;
    cardList[131].ckind = magical;cardList[131].cname = fire;cardList[131].ctype = attack;cardList[131].cnature = fire;cardList[131].cskillone = 43;cardList[131].cskilltwo = 221;
    cardList[132].ckind = magical;cardList[132].cname = ground;cardList[132].ctype = attack;cardList[132].cnature = ground;cardList[132].cskillone = 41;cardList[132].cskilltwo = 221;
    cardList[133].ckind = magical;cardList[133].cname = ground;cardList[133].ctype = attack;cardList[133].cnature = ground;cardList[133].cskillone = 41;cardList[133].cskilltwo = 222;
    cardList[134].ckind = magical;cardList[134].cname = ground;cardList[134].ctype = attack;cardList[134].cnature = ground;cardList[134].cskillone = 41;cardList[134].cskilltwo = 222;
    cardList[135].ckind = holy;cardList[135].cname = fire;cardList[135].ctype = attack;cardList[135].cnature = fire;cardList[135].cskillone = 61;cardList[135].cskilltwo = 71;
    cardList[136].ckind = skill;cardList[136].cname = wind;cardList[136].ctype = attack;cardList[136].cnature = wind;cardList[136].cskillone = 11;cardList[136].cskilltwo = 32;
    cardList[137].ckind = skill;cardList[137].cname = wind;cardList[137].ctype = attack;cardList[137].cnature = wind;cardList[137].cskillone = 11;cardList[137].cskilltwo = 32;
    cardList[138].ckind = blood;cardList[138].cname = ground;cardList[138].ctype = attack;cardList[138].cnature = ground;cardList[138].cskillone = 21;cardList[138].cskilltwo = 231;
    cardList[139].ckind = blood;cardList[139].cname = ground;cardList[139].ctype = attack;cardList[139].cnature = ground;cardList[139].cskillone = 21;cardList[139].cskilltwo = 231;
    cardList[140].ckind = holy;cardList[140].cname = ground;cardList[140].ctype = attack;cardList[140].cnature = ground;cardList[140].cskillone = 62;cardList[140].cskilltwo = -1;
    cardList[141].ckind = holy;cardList[141].cname = ground;cardList[141].ctype = attack;cardList[141].cnature = ground;cardList[141].cskillone = 62;cardList[141].cskilltwo = -1;
    cardList[142].ckind = intone;cardList[142].cname = wind;cardList[142].ctype = attack;cardList[142].cnature = wind;cardList[142].cskillone = 162;cardList[142].cskilltwo = 114;
    cardList[143].ckind = magical;cardList[143].cname = thunder;cardList[143].ctype = attack;cardList[143].cnature = thunder;cardList[143].cskillone = 45;cardList[143].cskilltwo = 221;
    cardList[144].ckind = holy;cardList[144].cname = ground;cardList[144].ctype = attack;cardList[144].cnature = ground;cardList[144].cskillone = 62;cardList[144].cskilltwo = 71;
    cardList[145].ckind = blood;cardList[145].cname = ground;cardList[145].ctype = attack;cardList[145].cnature = ground;cardList[145].cskillone = 21;cardList[145].cskilltwo = 231;
    cardList[146].ckind = intone;cardList[146].cname = wind;cardList[146].ctype = attack;cardList[146].cnature = wind;cardList[146].cskillone = 162;cardList[146].cskilltwo = 114;
    cardList[147].ckind = intone;cardList[147].cname = wind;cardList[147].ctype = attack;cardList[147].cnature = wind;cardList[147].cskillone = 162;cardList[147].cskilltwo = 114;
    cardList[148].ckind = blood;cardList[148].cname = wind;cardList[148].ctype = attack;cardList[148].cnature = wind;cardList[148].cskillone = 22;cardList[148].cskilltwo = 231;
    cardList[149].ckind = holy;cardList[149].cname = light;cardList[149].ctype = magic;cardList[149].cnature = light;cardList[149].cskillone = 61;cardList[149].cskilltwo = -1;

}
int CardList::getKind(int cardNum)
{
    return cardList[cardNum].ckind;
}
int CardList::getName(int cardNum)
{
    return cardList[cardNum].cname;
}
int CardList::getType(int cardNum)
{
    return cardList[cardNum].ctype;
}
int CardList::getSkillOne(int cardNum)
{
    return cardList[cardNum].skillone;
}
int CardList::getSkillTwo(int cardNum)
{
    return cardList[cardNum].skilltwo;
}
