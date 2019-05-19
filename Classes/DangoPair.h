
#ifndef _POM_DNAGOPAIR_H_
#define _POM_DNAGOPAIR_H_

#include "cocos2d.h"
#include "Dango.h"

using cocos2d::Ref;
using cocos2d::Sprite;
using cocos2d::Size;

class DangoPair: public Sprite
{
public:

    enum
    {
        TAG_UP,
        TAG_DOWN
    };

    DangoPair();
    virtual bool init();
   
    CREATE_FUNC(DangoPair);

    // 上下两个团子互换位置
    void switchPosition();
    Dango* getUpper();
    Dango* getLower();

};

#endif  //_POM_DNAGOPAIR_H_

