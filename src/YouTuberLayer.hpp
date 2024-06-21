#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>

using namespace geode::prelude;
extern std::set<std::string> YouTubers;

class YouTuberLayer : public geode::Popup<> {

public:
    static constexpr float POPUP_WIDTH = 310.f;
    static constexpr float POPUP_HEIGHT = 275.f;

    static YouTuberLayer* create();

private:
    bool setup() override;
};

bool YouTuberLayer::setup() {

    auto screenSize = CCDirector::sharedDirector()->getWinSize();
    auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
    CCLabelBMFont* ytText = CCLabelBMFont::create(fmt::format("Youtubers ({})", YouTubers.size()).c_str(), "goldFont.fnt");
    CCLabelBMFont* infoText = CCLabelBMFont::create("List downloaded from the GitHub Repo*", "chatFont.fnt");
    infoText->setOpacity(125);
    infoText->setScale(0.65);
    CCScale9Sprite* namesBG = CCScale9Sprite::create("square02_001-uhd.png");
    namesBG->setContentSize(CCSize(227, 212));

    ScrollLayer* ytNames = ScrollLayer::create({304, 212}, true, true);

    ytNames->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setGap(5.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(212)
            ->setAutoScale(false)
    );

    ytNames->setPosition({46, 27});
    for (const auto& GHNames : YouTubers) {
        CCLabelBMFont* name = CCLabelBMFont::create(GHNames.c_str(), "bigFont.fnt");
        name->setScale(0.75);
        name->setID(GHNames.c_str());
        ytNames->m_contentLayer->addChild(name);
        ytNames->m_contentLayer->updateLayout();
    }

    ytNames->m_contentLayer->updateLayout();

    ytText->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 + 97}); // weird placements cuz of initAnchored thanks
    infoText->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 - 143});
    namesBG->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 - 27});
    namesBG->setOpacity(80);

    mainLayer->addChild(ytText);
    mainLayer->addChild(infoText);
    mainLayer->addChild(ytNames, 1);
    mainLayer->addChild(namesBG);
    return true;
};

YouTuberLayer* YouTuberLayer::create() {
    auto ret = new YouTuberLayer;
    auto screenSize = CCDirector::sharedDirector()->getWinSize();
    if (ret->initAnchored(POPUP_WIDTH, POPUP_HEIGHT)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}