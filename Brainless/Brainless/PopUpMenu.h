#ifndef INCLUDED_POP_UP_MENU_H
#define INCLUDED_POP_UP_MENU_H

#include "WorldButton.h"
#include <array>

class PopUpMenu
{
public:
	PopUpMenu(WorldButton &examine, WorldButton &use, WorldButton &pickUp, sf::Vector2f position);

	//returns a number for which button is pressed
	//1 = examine,  2 = use, 3 = pickUp, 0 = none
	int getButtonPressed();

	//activates the menu
	//it deactivates when the player clicks outside the pop up menu
	void activate();

	void setPosition(sf::Vector2f pos);

	void update(sf::RenderWindow *window);
	void draw();

private:
	sf::Vector2f m_position;
	std::array<WorldButton&, 3> m_buttons;
	sf::IntRect m_hitbox;
	bool m_isActive;
};

#endif