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

	void validateNoteInput(std::vector<sf::Sprite> &noteList, const sf::Sprite &baseNote);
	void createHitText(const std::string &text, const sf::Color &color);

	// How many seconds the game session is
	static const int GameSession = 50;

	sf::Text m_scoreText;
	int m_score;

	float m_noteSpeed;

	// Data to make delays between notes
	float m_curDelay, m_maxDelay;
	std::string m_spawnString;

	// Text for good note hits
	std::vector<std::pair<float, sf::Text> > m_hitTexts;

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