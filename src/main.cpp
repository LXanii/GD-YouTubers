#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/utils/web.hpp>
#include <cctype>

using namespace geode::prelude;
bool downloaded = false;
std::set<std::string> YouTubers;

class $modify(YouTuberAlert, ProfilePage) {

CCSprite * badge;
CCMenuItemSpriteExtra* icon;

	void setupPageInfo(gd::string name, char const* chars) {
		ProfilePage::setupPageInfo(name,chars);

		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();

		if (!downloaded) {
			auto res = web::fetch("https://raw.githubusercontent.com/LXanii/GD-YouTubers/main/names.txt");
			if (!res) {
				log::info("Failed to fetch file from GitHub.");
			}
			else {
				auto data = res.value();
        
				std::istringstream iss(data);
				std::string temp_string;
				log::info("Downloaded YouTuber List");

				while (iss >> temp_string) {
					YouTubers.insert(temp_string);
				}
				downloaded = true;
			}
		}

		std::string player_name = m_usernameLabel->getString();

		for (const auto& names : YouTubers) {
			std::string lower_names(names.begin(), names.end());
			std::string lower_player_name(player_name.begin(), player_name.end());
			for (char &c : lower_names) {
				c = std::tolower(c);
			}
			for (char &c : player_name) {
				c = std::tolower(c);
			}
			if (lower_names == lower_player_name) {
				log::info("YouTuber Found! {}", names);
				CCNode* first_letter = reinterpret_cast<CCNode*>(m_usernameLabel->getChildren()->objectAtIndex(0));
				CCMenu* icon_menu = CCMenu::create();
				icon_menu->setPosition({0,0});

				m_fields->badge = CCSprite::create("youtuber.png"_spr);
				m_fields->icon = CCMenuItemSpriteExtra::create(m_fields->badge, this, menu_selector(YouTuberAlert::found_youtube));
				m_fields->icon->setScale(m_usernameLabel->getScale() + 0.2f);
				m_fields->icon->setPosition(first_letter->convertToWorldSpace(getPosition()));
				m_fields->icon->setPosition({m_fields->icon->getPositionX() - 13.f, m_fields->icon->getPositionY() + 10.f});

				CCLayer* layer = static_cast<CCLayer*>(getChildren()->objectAtIndex(0));

				icon_menu->addChild(m_fields->icon);
				layer->addChild(icon_menu);
			}
		}
	}

	void found_youtube(CCObject*) {
		FLAlertLayer::create("Youtuber Found!","This user is a <cr>prominent member</c> of the <cy>Geometry Dash</c> Community!","OK")->show();
		m_fields->icon->setScale(m_usernameLabel->getScale() + 0.2f);
	}

};