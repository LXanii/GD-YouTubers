#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <cctype>

#include "CommentCell.h"

using namespace geode::prelude;

class $modify(YouTuberAlert, ProfilePage) {

CCSprite * badge;
CCMenuItemSpriteExtra* icon;
bool is_mod;

	void setupPageInfo(gd::string name, char const* chars) {
		ProfilePage::setupPageInfo(name,chars);

		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();

		download_list();

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
			lower_player_name.erase(remove_if(lower_player_name.begin(), lower_player_name.end(), isspace), lower_player_name.end());
			if (lower_names == lower_player_name) {
				log::info("YouTuber Found! {}", names);
				CCNode* first_letter = reinterpret_cast<CCNode*>(m_usernameLabel->getChildren()->objectAtIndex(0));

				CCMenu* icon_menu = CCMenu::create();
				icon_menu->setPosition({0,0});

				m_fields->badge = CCSprite::create("youtuber.png"_spr);
				
				m_fields->icon = CCMenuItemSpriteExtra::create(m_fields->badge, this, menu_selector(YouTuberAlert::found_youtube));
				m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
				m_fields->icon->setPosition(first_letter->convertToWorldSpace(getPosition()));

				CCLayer* layer = static_cast<CCLayer*>(getChildren()->objectAtIndex(0));
				CCNode* mod_badge = layer->getChildByID("mod-badge");

				if (mod_badge) {
					is_mod = true;
					m_fields->icon->setPosition({m_fields->icon->getPositionX() - 35.f, m_fields->icon->getPositionY() + 11.9f});
					m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
				}
				else  {
					is_mod = false;
					m_fields->icon->setPosition({m_fields->icon->getPositionX() - 12.f, m_fields->icon->getPositionY() + 9.9f});
					m_fields->icon->setScale(m_usernameLabel->getScale() + 0.1f);
				}

				icon_menu->addChild(m_fields->icon);
				layer->addChild(icon_menu);
			}
		}
	}

	void found_youtube(CCObject*) {
		FLAlertLayer::create("YouTuber Found!","This user is a <cr>prominent member</c> of the <cy>Geometry Dash</c> Community!","OK")->show();
		if (is_mod) m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
		else m_fields->icon->setScale(m_usernameLabel->getScale() + 0.1f);
	}

};
