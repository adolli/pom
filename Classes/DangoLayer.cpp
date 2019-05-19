#include "DangoLayer.h"

USING_NS_CC;

// speed contant
const float DangoLayer::SPEED_FAST = 2.4f;
const float DangoLayer::SPEED_NORMAL = 1.0f;
const float DangoLayer::SPEED_SLOW = 0.5f;

const float DangoLayer::TIME_PER_UNIT_HEIGHT = 0.2f;
const float DangoLayer::TIME_PER_UNIT_WIDTH = 0.06f;

// on "init" you need to initialize your instance
bool DangoLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    dangos_ = Sprite::create();
    dangos_->setPosition(Point(202 - 80, 202 - 4 * 36));
    this->addChild(dangos_);

    auto leftButtonLabel = LabelTTF::create("Left", "Arial", 24);
    auto leftButton = MenuItemLabel::create(leftButtonLabel, CC_CALLBACK_1(DangoLayer::leftButtonCallback, this));
    leftButton->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                  origin.y + 48));
    auto rightButtonLabel = LabelTTF::create("Right", "Arial", 24);
    auto rightButton = MenuItemLabel::create(rightButtonLabel, CC_CALLBACK_1(DangoLayer::rightButtonCallback, this));
    rightButton->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                   origin.y + 48 + 48));
    auto upButtonLabel = LabelTTF::create("Up", "Arial", 24);
    auto upButton = MenuItemLabel::create(upButtonLabel, CC_CALLBACK_1(DangoLayer::upButtonCallback, this));
    upButton->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                  origin.y + 48 * 3));
    auto downButtonLabel = LabelTTF::create("Down", "Arial", 24);
    auto downButton = MenuItemLabel::create(downButtonLabel, CC_CALLBACK_1(DangoLayer::downButtonCallback, this));
    downButton->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                   origin.y + 48 * 4));
    auto addDangoLabel = LabelTTF::create("add", "Arial", 24);
    auto addDango = MenuItemLabel::create(addDangoLabel, CC_CALLBACK_1(DangoLayer::addOneDango, this));
    addDango->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                   origin.y + 48 * 5));
    auto removeDangoLabel = LabelTTF::create("remove", "Arial", 24);
    auto removeDango = MenuItemLabel::create(removeDangoLabel, CC_CALLBACK_1(DangoLayer::removeOneDango, this));
    removeDango->setPosition(Point(origin.x + visibleSize.width - 200 ,
                                   origin.y + 48 * 6));

    // create menu, it's an autorelease object
    auto menu = Menu::create(leftButton, rightButton, upButton, downButton, addDango, removeDango, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    auto addDangoAction = CallFuncN::create(CC_CALLBACK_1(DangoLayer::addOneDango, this));
    auto delay5s = DelayTime::create(4.0f);
    this->runAction(Repeat::create(Sequence::create(addDangoAction, delay5s, NULL), 20));


    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(DangoLayer::onKeyReleased, this);
    listener->onKeyPressed = CC_CALLBACK_2(DangoLayer::onKeyPress, this); 
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void DangoLayer::leftButtonCallback(Ref* sender)
{
    auto pos = dangos_->getPosition(); 
    pos.x--;
    dangos_->setPosition(pos);
}

void DangoLayer::rightButtonCallback(Ref* sender)
{
    auto pos = dangos_->getPosition();
    pos.x++;
    dangos_->setPosition(pos);
}

void DangoLayer::upButtonCallback(Ref* sender)
{
    auto pos = dangos_->getPosition();
    pos.y++;
    dangos_->setPosition(pos);
}

void DangoLayer::downButtonCallback(Ref* sender)
{
    auto pos = dangos_->getPosition();
    pos.y--;
    dangos_->setPosition(pos);
}

/**
 * @brief   run the action of moving the falling down to the top edge of box 
 * @param   speed the falling speed, second per dango height
 */
FiniteTimeAction* DangoLayer::getMoveDownAction()
{
    CCASSERT(fallingDangos_ != nullptr, "no falling dangos when calling getMoveDownAction();");

    // total height 15 dangos, 0.2s per dango, 0.2s per 32px
    int h = gameBox_.getColumn(focusedColumn_).size();
    Point pos = fallingDangos_->getPosition();
    float unitHeight = fallingDangos_->getContentSize().height / 2;  // two dangos in tuple
    int heightRemaining = h * unitHeight - pos.y;    // dango height = 36px 
    float moveTime = abs(heightRemaining) * (TIME_PER_UNIT_HEIGHT / unitHeight);
    auto moveDown = MoveBy::create(moveTime, Vec2(0, heightRemaining));
    return moveDown; 
}

void DangoLayer::addOneDango(Ref*)
{
    CCLOG("[DangoLayer] [addOneDango]  enter");
    if (fallingDangos_)
    {
        // there is still a dango falli ng
        CCLOG("[DangoLayer] [addOneDango] there still a dango falling, return.");
        return;
    }

    focusedColumn_ = 3;
    fallingDangos_ = DangoPair::create();

    int h = gameBox_.getColumn(focusedColumn_).size();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    CCLOG("[DangoLayer] [addOneDango] colsize=%d", h);
   
    float unitWidth = fallingDangos_->getContentSize().width; 
    fallingDangos_->setPosition(Point(focusedColumn_ * unitWidth, visibleSize.height));
    auto moveDown = getMoveDownAction();
    auto moveAndReg = Sequence::create(moveDown, CallFuncN::create(CC_CALLBACK_1(DangoLayer::registerDango, this)), NULL);
    fallingSpeedRegulator_ = Speed::create(moveAndReg, SPEED_NORMAL); 
    fallingDangos_->runAction(fallingSpeedRegulator_);
    dangos_->addChild(fallingDangos_);
}

void DangoLayer::removeOneDango(Ref* sender)
{ 
    CCLOG("[DangoLayer] [removeOneDango] enter");
    unsigned int col = rand() % 8;
    if (gameBox_.getColumn(col).size() == 0)
    {
        return;
    }
    unsigned int h = rand() % (gameBox_.getColumn(col).size());
    CCLOG("[DangoLayer] [removeOneDango] col=%d h=%d", col, h);
    Dango* rmdango = gameBox_.get(col, h);
    if (rmdango)
    {
        rmdango->setOpacity(0);
        dangos_->removeChild(rmdango, true);
        gameBox_.remove(col, h);
        for (int i = h; i < gameBox_.getColumn(col).size(); ++i)
        {
            auto dango = gameBox_.get(col, i);
            auto moveBy = MoveBy::create(0.5f, Vec2(0, -dango->getContentSize().height));
            dango->runAction(moveBy);
        }
    }
}

void DangoLayer::changeFocusedColumn(EventKeyboard::KeyCode keyCode)
{
    if (!fallingDangos_)
    {
        return;
    }

    // switch direction
    int dir = 0;
    int dx = fallingDangos_->getContentSize().width;
    int unitHeight = fallingDangos_->getContentSize().height;
    CCLOG("[DangoLayer] [changeFocusedColumn] [I] falling dango width is %d px", dx);
    if ((int)keyCode == 23)
    {
        // left
        dir = -1; 
    }
    else if ((int)keyCode == 24)
    {
        // right 
        dir = 1;
    }
    else
    {
        return;
    }

    // key left/right
    // 1. 中断当前action
    // 2. 移动一列位置
    // 3. 计算还需要下降多少，通过速度算出时间
    // 4. runAction并设置好落到地时的callback
    if ((focusedColumn_ > 0 && dir == -1) || (focusedColumn_ < 7 && dir == 1))
    {
        int newCol = focusedColumn_ + dir;
        int h = gameBox_.getColumn(newCol).size();
        Point pos = fallingDangos_->getPosition();
        int heightRemaining = h * unitHeight - pos.y;    // dango height = 36px 
        if (heightRemaining < 0)
        {
            focusedColumn_ = newCol;
            getActionManager()->removeAllActionsFromTarget(fallingDangos_);
            auto moveHorizon = MoveTo::create(TIME_PER_UNIT_WIDTH, Point(newCol * dx, pos.y));
            auto moveDown = getMoveDownAction(); 
            auto moveAndReg = Sequence::create(moveHorizon,  moveDown, CallFuncN::create(CC_CALLBACK_1(DangoLayer::registerDango, this)), NULL);
            fallingSpeedRegulator_ = Speed::create(moveAndReg, SPEED_NORMAL); 
            fallingDangos_->runAction(fallingSpeedRegulator_);
        }
    } 
}

void DangoLayer::changeFallingSpeed(float speed)
{
    if (!fallingDangos_)
    {
        return;
    }

    CCLOG("[DangoLayer] [changeFallingSpeed] [I] the falling dango has aligned.");
    if (fallingSpeedRegulator_ && !fallingSpeedRegulator_->isDone())
    {
        fallingSpeedRegulator_->setSpeed(speed);
    }
}

void DangoLayer::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("[DangoLayer] [onKeyPress] Key with keycode %d pressed", keyCode);
    changeFocusedColumn(keyCode);
    if ((int)keyCode == 26)
    {
        changeFallingSpeed(SPEED_FAST);
    }
    else if ((int)keyCode == 56)
    {
        if (fallingDangos_)
        {
            fallingDangos_->switchPosition();
        }
    }
}

void DangoLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    // key (left) [code=23]
    // key (right) [code=24]
    // key (up) [code=25]
    // key (down) [code=26]
    // key (space) [code=56]
    CCLOG("[DangoLayer] [onKeyReleased] Key with keycode %d released", keyCode);
    if ((int)keyCode == 26)
    {
        changeFallingSpeed(SPEED_NORMAL);
    }
}

void DangoLayer::registerDango(Ref*)
{
    CCLOG("[DangoLayer] [registerDango] dango added! col=%d", focusedColumn_);
    
    Dango* dangoU = fallingDangos_->getUpper();
    Dango* dangoD = fallingDangos_->getLower();
    gameBox_.add(focusedColumn_, dangoD);
    gameBox_.add(focusedColumn_, dangoU);
    
    // release the fallingdango
    fallingDangos_ = nullptr;
}



