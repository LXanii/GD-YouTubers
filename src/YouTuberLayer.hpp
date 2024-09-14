#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/GJSearchObject.hpp>
#include <cocos2d.h>

using namespace geode::prelude;
extern std::set<std::string> YouTubers;
extern std::set<std::string> Streamers;

class YouTuberLayer : public geode::Popup<> {

public:
    static constexpr float POPUP_WIDTH = 310.f;
    static constexpr float POPUP_HEIGHT = 275.f;

    static YouTuberLayer* create();
    
    void searchPlayer(CCObject*);
    void openDiscord(CCObject*);
    void openRepo(CCObject*);
    void request(CCObject*);

private:
    bool setup() override;
};

bool YouTuberLayer::setup() {

    auto screenSize = CCDirector::sharedDirector()->getWinSize();
    auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
    CCLabelBMFont* ytText = CCLabelBMFont::create("YouTubers", "goldFont.fnt");
    CCLabelBMFont* infoText = CCLabelBMFont::create(fmt::format("List downloaded from GitHub* ({})", YouTubers.size()).c_str(), "chatFont.fnt");
    infoText->setOpacity(125);
    infoText->setScale(0.55);
    CCScale9Sprite* namesBG = CCScale9Sprite::create("square02_001-uhd.png");
    namesBG->setContentSize(CCSize(227, 212));

    auto reqbtnBG = CCScale9Sprite::create("GJ_button_04-uhd.png");
    CCLabelBMFont* reqText = CCLabelBMFont::create("Req", "bigFont.fnt");
    reqText->setScale(0.5);
    reqText->setPosition({21, 14});
    reqbtnBG->setContentSize({41.5, 25});
    reqbtnBG->addChild(reqText);
    //reqText->setPosition({reqbtnBG->getPositionX(), reqbtnBG->getPositionY()});
    auto reqbtn = CCMenuItemSpriteExtra::create(reqbtnBG, this, menu_selector(YouTuberLayer::request));
    CCMenu* bottomMenu = CCMenu::create();

    bottomMenu->setScale(0.8);
    bottomMenu->setID("bottom-menu");

    bottomMenu->setLayout(
        RowLayout::create()
        ->setGap(5.f)
        ->setAutoScale(true)
        ->setAxisAlignment(AxisAlignment::Start)
    );

    bottomMenu->addChild(reqbtn);
    bottomMenu->updateLayout();

    CCMenu* rightSide = CCMenu::create();
    rightSide->setID("right-side-menu");

    rightSide->setLayout(
        ColumnLayout::create()
        ->setGap(5.f)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::End)
    );


    CCSprite* discordSprite = CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png");
    CCSprite* githubSprite = CCSprite::create("githubBtn.png"_spr);
    CCMenuItemSpriteExtra* discordBtn = CCMenuItemSpriteExtra::create(discordSprite, this, menu_selector(YouTuberLayer::openDiscord));
    CCMenuItemSpriteExtra* githubBtn = CCMenuItemSpriteExtra::create(githubSprite, this, menu_selector(YouTuberLayer::openRepo));

    rightSide->addChild(discordBtn);
    rightSide->addChild(githubBtn);
    rightSide->setPosition({rightSide->getPositionX() + 2, rightSide->getPositionY() - 51});
    rightSide->updateLayout();

    ScrollLayer* ytNames = ScrollLayer::create({304, 212}, true, true);

    ytNames->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setGap(5.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(212)
            ->setAutoScale(true)
    );

    ytNames->setPosition({46, 27});
    for (const auto& GHNames : YouTubers) {
        CCMenu* menu = CCMenu::create();
        menu->setPositionY(menu->getPositionY() + 9);
        menu->setContentSize({0,20});
        CCLabelBMFont* name = CCLabelBMFont::create(GHNames.c_str(), "bigFont.fnt");
        name->setScale(0.65);
        CCMenuItemSpriteExtra* nameholder = CCMenuItemSpriteExtra::create(name, this, menu_selector(YouTuberLayer::searchPlayer));
        menu->setID(GHNames.c_str());
        nameholder->setID(GHNames.c_str());
        menu->addChild(nameholder);
        ytNames->m_contentLayer->addChild(menu);
        ytNames->m_contentLayer->updateLayout();
    }

    ytNames->m_contentLayer->updateLayout();
    ytNames->m_contentLayer->setPositionX(ytNames->m_contentLayer->getPositionX() + 106);

    ytText->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 + 97}); // weird placements cuz of initAnchored thanks
    infoText->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 - 143});
    namesBG->setPosition({screenSize.width / 2 - 130, screenSize.height / 2 - 27});
    bottomMenu->setPosition({bottomMenu->getPositionX() - 50,infoText->getPositionY()});
    namesBG->setOpacity(80);

    mainLayer->addChild(ytText);
    mainLayer->addChild(infoText);
    mainLayer->addChild(ytNames, 1);
    mainLayer->addChild(namesBG);
    mainLayer->addChild(rightSide);
    mainLayer->addChild(bottomMenu);
    return true;
};

void YouTuberLayer::searchPlayer(CCObject* sender) {
    std::string player = static_cast<CCNode*>(sender)->getID();
    GJSearchObject* lookupTerm = GJSearchObject::create(SearchType::Users, player);
    LevelBrowserLayer* search = LevelBrowserLayer::create(lookupTerm);

    addChild(search);
};

void YouTuberLayer::openDiscord(CCObject*) {
    geode::createQuickPopup("Join Discord", "Would you like to join the <cb>Discord Server</c>?", "NO", "OK", [] (auto fl, bool btn2) {
                if (btn2) geode::utils::web::openLinkInBrowser("https://discord.gg/W3BkznGTV8");});
};

void YouTuberLayer::openRepo(CCObject*) {
    geode::utils::web::openLinkInBrowser("https://github.com/LXanii/GD-YouTubers/blob/main/names.txt");
};

void YouTuberLayer::request(CCObject*) {
    geode::createQuickPopup("Request YouTubers", "Join the <cb>Discord Server</c> to recommend YouTubers!\n(#Suggestions Channel)", "NO", "OK", [] (auto fl, bool btn2) {
                if (btn2) geode::utils::web::openLinkInBrowser("https://discord.gg/W3BkznGTV8");});
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