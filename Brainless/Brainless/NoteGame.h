#ifndef INCLUDED_NOTE_GAME_H
#define INCLUDED_NOTE_GAME_H

#include "ArcadeGame.h"
#include <vector>
#include <SFML\Graphics.hpp>

class NoteGame : public ArcadeGame
{
public:

	NoteGame(ArcadeMachine &machine);

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

private:

	sf::Text m_scoreText;
	int m_score;

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