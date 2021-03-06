
#ifndef _POM_GAMEBOX_H_
#define _POM_GAMEBOX_H_

#include "cocos2d.h"
#include "Dango.h"

using cocos2d::Vector;

class GameBox
{
public:

    GameBox()
    {
        for (int i = 0; i < 8; ++i)
        {
            box_[i].reserve(15);
        }
    }
    
    Dango* get(unsigned int col, int line)
    {
        if (col >= 8)
        {
            CCLOG("[GameBox] [get] column out of range. col=%d", col);
            return nullptr;
        }
        return box_[col].at(line);
    }

    void add(unsigned int col, Dango* dango)
    {
        getColumn(col).pushBack(dango);
    }

    Vector<Dango*>& getColumn(unsigned int col)
    {
        if (col >= 8)
        {
            CCLOG("[GameBox] [getColumn] column out of range. col=%d", col);
        }
        return box_[col];
    }

    void remove(unsigned int col, int line)
    {
        if (col >= 8)
        {
            CCLOG("[GameBox] [remove] column out of range. col=%d", col);
            return;
        }
        box_[col].erase(box_[col].begin() + line, box_[col].begin() + line + 1);
    }

private:
    Vector<Dango*> box_[8];
};

#endif  // _POM_GAMEBOX_H_

