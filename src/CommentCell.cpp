#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include <Geode/utils/web.hpp>
#include <cctype>

#include "CommentCell.h"

using namespace geode::prelude;

bool ytdownload = false;
bool twitchdownload = false;

std::set<std::string> YouTubers;
std::set<std::string> Streamers;
EventListener<web::WebTask> m_listener;
EventListener<web::WebTask> m_listener1;

void downloadYT() {
	
	if (!ytdownload) {

		m_listener.bind([] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
				auto data = res->string().unwrapOr("eee"); // wowzers found whats printing
				log::debug("Attempting to download YouTuber List..");
				std::istringstream iss(data);
				std::string temp_string;

				while (iss >> temp_string) {
					YouTubers.insert(temp_string);
				}
				ytdownload = true;
				log::debug("Download Complete [{} Found].", YouTubers.size());

            } else if (e->isCancelled()) {
                log::info("Failed to fetch file from GitHub.");
            }
        });

        auto req = web::WebRequest();
        m_listener.setFilter(req.get("https://raw.githubusercontent.com/LXanii/GD-YouTubers/main/names.txt"));
		}
	}

void downloadTwitch() {
	
	if (!twitchdownload) {

		m_listener1.bind([] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
				auto data = res->string().unwrapOr("eee"); // wowzers found whats printing
				log::debug("Attempting to download Streamer List..");
				std::istringstream iss(data);
				std::string temp_string;

				while (iss >> temp_string) {
					Streamers.insert(temp_string);
				}
				twitchdownload = true;
				log::debug("Download Complete [{} Found].", Streamers.size());

            } else if (e->isCancelled()) {
                log::info("Failed to fetch file from GitHub.");
            }
        });

        auto req = web::WebRequest();
        m_listener1.setFilter(req.get("https://raw.githubusercontent.com/LXanii/GD-YouTubers/main/streamers.txt"));
		}
	}

class $modify(CommentCell) {

	struct Fields {
		float badge_x_pos = 0.f;
		CCSprite* badge;
		CCSprite* badge_mod;
	};

	void loadFromComment(GJComment* comment) {

		CommentCell::loadFromComment(comment);
		CCLayer* layer = static_cast<CCLayer*>(getChildren()->objectAtIndex(1));
		CCLabelBMFont* player_comment_small = static_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("comment-text-label"));
		CCLabelBMFont* player_comment_big_area = static_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("comment-text-area"));

		CCDirector* director = CCDirector::get();
		CCScene* scene = director->getRunningScene();
		
		std::string username = m_comment->m_userName;

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
				
				if (player_comment_small || player_comment_big_area) {

					CCNode* last_letter;
					m_fields->badge = CCSprite::create("youtuber.png"_spr);
					m_fields->badge->setID("xanii.gd-youtubers/yt-badge");

					m_fields->badge->setScale(0.54);

					if (m_comment->m_modBadge < 1) {

						if (player_comment_big_area) {

							MultilineBitmapFont* text_holder = static_cast<MultilineBitmapFont*>(player_comment_big_area->getChildren()->objectAtIndex(0));

							for (int i = 0; i < text_holder->getChildrenCount(); i++) {

								CCLabelBMFont* player_comment_big = static_cast<CCLabelBMFont*>(text_holder->getChildren()->objectAtIndex(i));
								if (player_comment_big->getColor() == ccColor3B{255,255,255}) player_comment_big->setColor({ 255, 180, 185 });
							}

						}
						else {
							if (player_comment_small->getColor() == ccColor3B{255,255,255}) player_comment_small->setColor({ 255, 180, 185 });
						}

					}
					else m_fields->badge_x_pos = 14.f;

					if (layer->getChildByIDRecursive("username-label")) {

						CCLabelBMFont* username_btn = static_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("username-label"));
						
						m_fields->badge->setPosition(username_btn->convertToWorldSpace(username_btn->getContentSize()));
						
						if (player_comment_big_area) {
							m_fields->badge->setScale(0.665);
							m_fields->badge->setPosition({m_fields->badge->getPositionX() + 8.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 10.5f});
						}
						else {
							m_fields->badge->setPosition({m_fields->badge->getPositionX() + 8.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 8.5f});
						}

						if (layer->getChildByIDRecursive("percentage-label")) {

							CCLabelBMFont* percent = static_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("percentage-label"));
							percent->setPositionX(percent->getPositionX() + 15);
						}

						if (static_cast<CCSprite*>(layer->getChildren()->objectAtIndex(2))) { // mod button
							m_fields->badge_x_pos = 107.f;
						}

						this->addChild(m_fields->badge);

					}
					else {

						CCMenu* main_menu = static_cast<CCMenu*>(layer->getChildByIDRecursive("main-menu"));
						CCMenuItemSpriteExtra* username_btn = static_cast<CCMenuItemSpriteExtra*>(main_menu->getChildByIDRecursive("username-button"));
						CCLabelBMFont* name_label = static_cast<CCLabelBMFont*>(username_btn->getChildren()->objectAtIndex(0));

						if (layer->getChildByIDRecursive("percentage-label")) {

							CCLabelBMFont* percent = static_cast<CCLabelBMFont*>(layer->getChildByIDRecursive("percentage-label"));
							if (!player_comment_big_area) percent->setPositionX(percent->getPositionX() + 15);
							else percent->setPositionX(percent->getPositionX() + 22);
						}

						if (static_cast<CCSprite*>(layer->getChildren()->objectAtIndex(2))) {
							if (m_comment->m_modBadge > 0) {
								auto mod_btn = layer->getChildByIDRecursive("mod-badge");
								mod_btn->setVisible(false);

								m_fields->badge_mod = CCSprite::createWithSpriteFrameName(fmt::format("modBadge_0{}_001.png", m_comment->m_modBadge).c_str());
								m_fields->badge_mod->setPosition(username_btn->getContentSize());
								m_fields->badge_mod->setID("xanii.gd-youtubers/mod-badge");
								if (player_comment_big_area) {
									m_fields->badge_mod->setScale(0.75);
									m_fields->badge_mod->setPosition({m_fields->badge_mod->getPositionX() + 12, m_fields->badge_mod->getPositionY() - 10.f});
									username_btn->addChild(m_fields->badge_mod);
								}
								else {
									m_fields->badge_mod->setScale(0.55);
									m_fields->badge_mod->setPosition({m_fields->badge_mod->getPositionX() + 8, m_fields->badge_mod->getPositionY() - 7.5f});
									username_btn->addChild(m_fields->badge_mod);
								}
								m_fields->badge_mod->setPositionY(m_fields->badge_mod->getPositionY() + 4);
							}

						} 

						if (player_comment_big_area) {

							m_fields->badge->setPosition(username_btn->getContentSize());
							m_fields->badge->setScale(0.75);
							m_fields->badge->setPosition({m_fields->badge->getPositionX() + 12.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 11.f});
							if (m_comment->m_modBadge >= 1) {
								m_fields->badge->setPositionX(m_fields->badge->getPositionX() + 8.5f);
							}
							m_fields->badge->setPositionY(m_fields->badge->getPositionY() + 4);
							username_btn->addChild(m_fields->badge);
						}
						else {
							m_fields->badge->setPosition(username_btn->getContentSize());
							m_fields->badge->setScale(0.55);
							m_fields->badge->setPosition({m_fields->badge->getPositionX() + 11.425f + m_fields->badge_x_pos, m_fields->badge->getPositionY() - 8.f});
							username_btn->addChild(m_fields->badge);
						}
						m_fields->badge->setPositionY(m_fields->badge->getPositionY() + 4);
					}
				}
			}	
		}
	}
};