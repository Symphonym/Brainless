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

};

#endif