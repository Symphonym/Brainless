#ifndef INCLUDED_PAUSE_MENU_H
#define INCLUDED_PAUSE_MENU_H

#include "State.h"
#include "GUIElement.h"
#include <memory>
#include <array>

class Game;
class PauseMenu : public State
{
public:

	PauseMenu(StateMachine &machine);

	virtual void update(float deltaTime);
	virtual void draw();

	void giveGame(Game &game);

private:

	typedef std::unique_ptr<GUIElement> GuiPtr;

	Game *m_game;
	std::array<GuiPtr, 3> m_buttons;

	// Complete dark texture covering screen, generated in code and thus not loaded
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
};

#endif