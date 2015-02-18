#ifndef INCLUDED_ARCADE_GAME_H
#define INCLUDED_ARCADE_GAME_H

#include <SFML\Graphics.hpp>

class ArcadeMachine;
class ArcadeGame
{
public:

	ArcadeGame(ArcadeMachine &machine, const std::string &gameName);

	virtual void events(const sf::Event &event) {};
	virtual void update(float deltaTime) {};
	virtual void draw() {};

	// Get the name of the game
	std::string getName() const;

protected:

	ArcadeMachine &m_machine;

private:

	std::string m_gameName;
};

#endif