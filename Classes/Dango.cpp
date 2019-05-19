
#include "Dango.h"

USING_NS_CC;

Dango::Dango()
{
}

bool Dango::init(Dango::DangoColor color)
{
    return Sprite::initWithFile("dango-texture.png", Rect((int) color * 64, 0, 40, 36));
}

Dango* Dango::create(Dango::DangoColor color)
{
    Dango* dango = new Dango();
    if (dango && dango->init(color))
    {
        dango->autorelease();
        return dango;
    }
    CC_SAFE_DELETE(dango);
    return nullptr;
}

