
#ifndef _POM_DANGO_H_
#define _POM_DANGO_H_

#include "cocos2d.h"

using cocos2d::Ref;
using cocos2d::Sprite;

class Dango: public Sprite
{
public:

    Dango();

    enum DangoColor
    {
        GREEN,
        RED,
        BLUE,
        YELLOW,
        WHITE
    };

    virtual bool init(DangoColor color = GREEN); 

    static Dango* create(DangoColor color = GREEN);

};

#endif  // _POM_DANGO_H_

