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

	// Stores a new game in the machine
	void loadArcadeGame(ArcadeGame *game);

	void exitGame();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	// Get position and size of the arcade machine
	sf::Vector2f getScreenPos() const;
	sf::Vector2i getScreenSize() const;

private:

	void refreshMenu();

	static const int GameCount = 5;

	typedef std::unique_ptr<ArcadeGame> GamePtr;

	sf::Text m_noGameText;

	sf::Text m_currentGameText;
	sf::Text m_infoText;
	sf::Text m_escText;
	sf::Sprite m_arcadeForeground;
	sf::Sprite m_arcadeBackground;

	std::size_t m_currentGameIndex;
	std::vector<sf::Text> m_gameSelectionButtons;
	std::vector<GamePtr> m_games;

	bool m_playingGame;
};

#endif