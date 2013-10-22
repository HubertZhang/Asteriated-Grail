#ifndef TERMINATION_H
#define TERMINATION_H

class ActionIllegal{};


class GameTerminate
{
public:
    GameTerminate(int winner);
};

class LostAllMorale : public GameTerminate
{
public:
    LostAllMorale(int team);
};

class GrailFinished : public GameTerminate
{
public:
    GrailFinished(int team);
};

#endif // TERMINATION_H
