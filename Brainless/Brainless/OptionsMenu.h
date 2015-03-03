#ifndef INCLUDED_OPTIONS_MENU_H
#define INLCUDED_OPTIONS_MENU_H

#include "State.h"
#include <SFML\Graphics.hpp>
#include <unordered_map>
#include <string>
#include <array>
#include <memory>
#include "Animation.h"
#include "GUIElement.h"

class OptionsMenu : public State
{
public:

	OptionsMenu(StateMachine &machine);

	void onPlay();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	static sf::Keyboard::Key getKeybind(const std::string &keybindName);
	static std::string getKeybindKeyName(const std::string &keybindName);
	static void initializeKeybinds();

private:

	static std::unordered_map<std::string, sf::Keyboard::Key> m_keybinds;


	typedef std::unique_ptr<GUIElement> GuiPtr;
	
	GuiPtr m_volumeUpButton;
	GuiPtr m_volumeDownButton;
	int m_curVolume;
	sf::Texture m_volumeTexture;
	sf::Sprite m_volumeSprite;
	sf::Text m_volumeText;
	
	GuiPtr m_backButton;

	bool m_recordingKey;
	std::string m_currentKey;

	std::unordered_map<std::string, sf::Text> m_keybindTexts;

	sf::Text m_keybindInfoText;

	static std::string keyToString(sf::Keyboard::Key key);
	void addKeybindOption(const std::string &keybindName, const sf::Vector2f &position);

	sf::Sprite m_background;
	sf::Sprite m_backgroundElement_hair;
	Animation m_Animation_hair;
	sf::Sprite m_backgroundElement_grass1;
	Animation m_Animation_grass1;
	sf::Sprite m_backgroundElement_grass2;
	Animation m_Animation_grass2;
	sf::Sprite m_backgroundElement_grass3;
	Animation m_Animation_grass3;
	sf::Sprite m_backgroundElement_grass4;
	Animation m_Animation_grass4;
	sf::Sprite m_backgroundElement_grass5;
	Animation m_Animation_grass5;
	sf::Sprite m_backgroundElement_scarf;
	Animation m_Animation_scarf;
};

#endif