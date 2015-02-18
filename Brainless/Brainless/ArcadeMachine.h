#ifndef INCLUDED_ARCADE_MACHINE_H
#define INCLUDED_ARCADE_MACHINE_H

#include "State.h"
#include <SFML\Graphics.hpp>
#include <array>
#include <memory>

class ArcadeGame;
class ArcadeMachine : public State
{
public:

	ArcadeMachine(StateMachine &machine);

	void exitGame();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	// Get position and size of the arcade machine
	sf::Vector2f getScreenPos() const;
	sf::Vector2i getScreenSize() const;

private:

	static const int GameCount = 5;

	typedef std::unique_ptr<ArcadeGame> GamePtr;

	sf::Text m_currentGameText;
	sf::Sprite m_arcadeBackground;

	std::size_t m_currentGameIndex;
	std::array<sf::Sprite, GameCount> m_gameSelectionButtons;
	std::array<GamePtr, GameCount> m_games;

	bool m_playingGame;
};

#endif