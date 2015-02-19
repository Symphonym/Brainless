#ifndef INCLUDED_NOTE_GAME_H
#define INCLUDED_NOTE_GAME_H

#include "ArcadeGame.h"
#include <vector>
#include <SFML\Graphics.hpp>

class NoteGame : public ArcadeGame
{
public:

	NoteGame(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

private:

	// Returns true if score, false otherwise
	bool handleDistanceScore(float distance);

	void penalty();

	// How many seconds the game session is
	static const int GameSession = 50;

	sf::Text m_scoreText;
	int m_score;

	float m_speedAddition;

	// Data to make delays between notes
	float m_aCurDelay;
	float m_wCurDelay;
	float m_dCurDelay;
	float m_aMaxDelay;
	float m_wMaxDelay;
	float m_dMaxDelay;

	// Notes
	std::vector<sf::Sprite> m_aNotes;
	std::vector<sf::Sprite> m_wNotes;
	std::vector<sf::Sprite> m_dNotes;

	// Base notes at bottom of screen
	sf::Sprite m_baseANote;
	sf::Sprite m_baseWNote;
	sf::Sprite m_baseDNote;
};

#endif