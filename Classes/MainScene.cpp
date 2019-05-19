
#include "MainScene.h"
#include "GameBackground.h"
#include "GameForeground.h"
#include "DangoLayer.h"

bool MainScene::init()
{
    CCLOG("main scene init! %d", 123);

    if (!Scene::init())
    {
        return false;
    }

    auto backgroundLayer = GameBackground::create();
    this->addChild(backgroundLayer, 1);

    auto dangoLayer = DangoLayer::create();
    this->addChild(dangoLayer, 2);

    auto foregroundLayer = GameForeground::create();
    this->addChild(foregroundLayer, 3);

    return true;
}

