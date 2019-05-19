#ifndef __GAMEFOREGROUND_LAYER_H__
#define __GAMEFOREGROUND_LAYER_H__

#include "cocos2d.h"

using cocos2d::Ref;
using cocos2d::Layer;

class GameForeground : public Layer
{
public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Ref* pSender);

    
    // implement the "static create()" method manually
    CREATE_FUNC(GameForeground);
};

#endif // __GAMEFOREGROUND_LAYER_H__
