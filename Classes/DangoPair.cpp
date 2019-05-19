
#include "DangoPair.h"
#include "RandomGenerator.h"

USING_NS_CC;

DangoPair::DangoPair()
{
}

bool DangoPair::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // generate random color
    RandomGenerator rg;
    rg.setDistribution({ 0.2375, 0.2375, 0.2375, 0.2375, 0.05 });
    int color[2] = { rg.generate(), rg.generate() };
    auto dangoU = Dango::create(Dango::DangoColor(color[0]));
    auto dangoD = Dango::create(Dango::DangoColor(color[1]));
    CCLOG("[DangoPair] [init] [I] random dango color: colorU=%d, colorD=%d", color[0], color[1]);
    dangoU->setPosition(Point(0, dangoU->getContentSize().height));
    dangoD->setPosition(Point(0, 0));
    dangoU->setTag(TAG_UP);
    dangoD->setTag(TAG_DOWN);

    this->addChild(dangoU);
    this->addChild(dangoD);
    _contentSize.width = dangoU->getContentSize().width;
    _contentSize.height = dangoU->getContentSize().height * 2;

    return true;
}

Dango* DangoPair::getUpper()
{
    return static_cast<Dango*>(getChildByTag(TAG_UP));
}

Dango* DangoPair::getLower()
{
    return static_cast<Dango*>(getChildByTag(TAG_DOWN));
}

void DangoPair::switchPosition()
{
    auto dangoU = getChildByTag(TAG_UP);
    auto dangoD = getChildByTag(TAG_DOWN);
   
    // stop current action
    getActionManager()->removeAllActionsFromTarget(dangoU);
    getActionManager()->removeAllActionsFromTarget(dangoD);
     
    // switch tag
    dangoU->setTag(TAG_DOWN);
    dangoD->setTag(TAG_UP);

    // switch position
    auto moveU = MoveTo::create(0.2f, Point(0, dangoU->getContentSize().height));
    auto moveD = MoveTo::create(0.2f, Point(0, 0));
    dangoU->runAction(moveD);
    dangoD->runAction(moveU);
}



