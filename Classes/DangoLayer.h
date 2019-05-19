#ifndef __DANGOLAYER_LAYER_H__
#define __DANGOLAYER_LAYER_H__

#include "cocos2d.h"
#include "GameBox.h"
#include "DangoPair.h"

using cocos2d::Ref;
using cocos2d::Layer;
using cocos2d::Sprite;
using cocos2d::Point;
using cocos2d::EventKeyboard;
using cocos2d::Event;
using cocos2d::FiniteTimeAction;
using cocos2d::Speed;

class DangoLayer : public Layer
{
public:
    
    static const float SPEED_FAST;
    static const float SPEED_NORMAL;
    static const float SPEED_SLOW;

    static const float TIME_PER_UNIT_WIDTH;
    static const float TIME_PER_UNIT_HEIGHT;

    DangoLayer(): dangos_(nullptr), fallingDangos_(nullptr), focusedColumn_(3), fallingSpeedRegulator_(nullptr)
    {
    }

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
        
    // implement the "static create()" method manually
    CREATE_FUNC(DangoLayer);

    
    void leftButtonCallback(Ref* pSender);
    void rightButtonCallback(Ref* pSender);
    void upButtonCallback(Ref* pSender);
    void downButtonCallback(Ref* pSender);
    void addOneDango(Ref* sender);
    void removeOneDango(Ref* sender);
    void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void registerDango(Ref*);


protected:
    
    GameBox gameBox_;
    Sprite* dangos_;
    DangoPair* fallingDangos_;
    unsigned int focusedColumn_;
    Speed* fallingSpeedRegulator_;

private:
    void changeFocusedColumn(EventKeyboard::KeyCode keyCode);
    void changeFallingSpeed(float speed);
    FiniteTimeAction* getMoveDownAction();

};

#endif // __DANGOLAYER_LAYER_H__

