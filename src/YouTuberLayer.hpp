#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

class YouTuberLayer : public geode::Popup<> {

public:
    static constexpr float POPUP_WIDTH = 360.f;
    static constexpr float POPUP_HEIGHT = 275.f;

    static YouTuberLayer* create();

private:
    bool setup() override;
};

bool YouTuberLayer::setup() {

    auto screenSize = CCDirector::sharedDirector()->getWinSize();
    auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
    CCLabelBMFont* ytText = CCLabelBMFont::create("Youtubers:", "goldFont.fnt");

    ytText->setPosition({screenSize.width / 2, screenSize.height / 2 + 120});

    mainLayer->addChild(ytText);
    return true;
};

YouTuberLayer* YouTuberLayer::create() {
    auto ret = new YouTuberLayer;
    auto screenSize = CCDirector::sharedDirector()->getWinSize();
    if (ret->init(POPUP_WIDTH, POPUP_HEIGHT)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}