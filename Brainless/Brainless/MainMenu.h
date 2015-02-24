#ifndef INCLUDED_MAIN_MENU_H
#define INCLUDED_MAIN_MENU_H

#include <array>
#include <memory>
#include "State.h"
#include "GUIElement.h"
#include "Animation.h"

class MainMenu : public State
{
public:

	MainMenu(StateMachine &machine);

	virtual void update(float deltaTime);
	virtual void draw();

private:

	typedef std::unique_ptr<GUIElement> GuiPtr;

	std::array<GuiPtr, 2> m_buttons;

	// Whether or not the confirm window for a new game is shown
	bool m_newGameConfirmWindow;
	std::array<GuiPtr, 2> m_newGameButtons;
	sf::Sprite m_confirmBackground;
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