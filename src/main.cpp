#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>
#include <cctype>

using namespace geode::prelude;
bool downloaded = false;
std::set<std::string> YouTubers;

void download_list() {
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
}

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
		FLAlertLayer::create("Youtuber Found!","This user is a <cr>prominent member</c> of the <cy>Geometry Dash</c> Community!","OK")->show();
		if (is_mod) m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
		else m_fields->icon->setScale(m_usernameLabel->getScale() + 0.1f);
	}

};

class $modify(CommentCell) {

float badge_x_pos = 0.f;
CCSprite* badge;
CCSprite* badge_mod;

	void loadFromComment(GJComment* comment) {

		CommentCell::loadFromComment(comment);
		CCLayer* layer = static_cast<CCLayer*>(getChildren()->objectAtIndex(1));
		CCLabelBMFont* player_comment = static_cast<CCLabelBMFont*>(layer->getChildByID("comment-text-label"));
		std::string username = m_comment->m_userName;

		download_list();

		for (const auto& names : YouTubers) {
			std::string lower_names(names.begin(), names.end());
			std::string lower_player_name(username.begin(), username.end());
			for (char &c : lower_names) {
				c = std::tolower(c);
			}
			for (char &c : lower_player_name) {
				c = std::tolower(c);
			}

			lower_player_name.erase(remove_if(lower_player_name.begin(), lower_player_name.end(), isspace), lower_player_name.end());
			if (lower_names == lower_player_name) {

				if (player_comment) {

					CCNode* last_letter;
					m_fields->badge = CCSprite::create("youtuber.png"_spr);

					m_fields->badge->setScale(0.54);

					if (m_comment->m_modBadge < 1) {
						player_comment->setColor({ 255, 180, 185 });
					}
					else m_fields->badge_x_pos = 14.f;

					if (layer->getChildByID("username-label")) {

						CCLabelBMFont* username_btn = static_cast<CCLabelBMFont*>(layer->getChildByID("username-label"));
						
						m_fields->badge->setPosition(username_btn->convertToWorldSpace(username_btn->getContentSize()));
						m_fields->badge->setPosition({m_fields->badge->getPositionX() + 8.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 8.5f});
						this->addChild(m_fields->badge);

						if (layer->getChildByID("percentage-label")) {

							CCLabelBMFont* percent = static_cast<CCLabelBMFont*>(layer->getChildByID("percentage-label"));
							percent->setPositionX(percent->getPositionX() + 15);
						}

						if (static_cast<CCSprite*>(layer->getChildren()->objectAtIndex(2))) { // mod button
							m_fields->badge_x_pos = 107.f;
						}

					}
					else {

						CCMenu* main_menu = static_cast<CCMenu*>(layer->getChildByID("main-menu"));
						CCMenuItemSpriteExtra* username_btn = static_cast<CCMenuItemSpriteExtra*>(main_menu->getChildByID("username-button"));
						CCLabelBMFont* name_label = static_cast<CCLabelBMFont*>(username_btn->getChildren()->objectAtIndex(0));

						if (layer->getChildByID("percentage-label")) {

							CCLabelBMFont* percent = static_cast<CCLabelBMFont*>(layer->getChildByID("percentage-label"));
							percent->setPositionX(percent->getPositionX() + 15);
						}

						if (static_cast<CCSprite*>(layer->getChildren()->objectAtIndex(2))) {
							if (m_comment->m_modBadge > 0) {
								CCSprite* mod_btn = static_cast<CCSprite*>(layer->getChildren()->objectAtIndex(2));
								mod_btn->setVisible(false);

								m_fields->badge_mod = CCSprite::createWithSpriteFrameName(fmt::format("modBadge_0{}_001.png", m_comment->m_modBadge).c_str());
								m_fields->badge_mod->setScale(0.55);
								m_fields->badge_mod->setPosition(username_btn->getContentSize());
								m_fields->badge_mod->setPosition({m_fields->badge_mod->getPositionX() + 8, m_fields->badge_mod->getPositionY() - 7.5f});
								username_btn->addChild(m_fields->badge_mod);
							}

						} 

						m_fields->badge->setPosition(username_btn->getContentSize());
						m_fields->badge->setPosition({m_fields->badge->getPositionX() + 8.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 8.f});
						username_btn->addChild(m_fields->badge);

					}
				}
			}	
		}

		

	}
};

//chat color rgb: 255, 126, 135