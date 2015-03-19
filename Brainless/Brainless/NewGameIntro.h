#ifndef INCLUDED_NEW_GAME_INTRO_H
#define INCLUDED_NEW_GAME_INTRO_H

#include "State.h"

class NewGameIntro : public State
{
public:

	NewGameIntro(StateMachine &machine);

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	virtual void onStop();
	virtual void onPlay();

private:

	int m_currentIndex;
	float m_curFade;
	std::vector<std::pair<sf::Text, float> > m_introTexts;

	// Half of the time is used for fade in, half for fade out
	void pushIntroText(const std::string &text, float duration);

};

#endif