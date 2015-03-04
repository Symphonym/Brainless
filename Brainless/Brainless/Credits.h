#ifndef INCLUDED_CREDITS_H
#define INCLUDED_CREDITS_H

#include "State.h"
#include "Animation.h"

class Credits : public State
{
public:

	Credits(StateMachine &machine);

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	virtual void onStop();
	virtual void onPlay();

private:

	sf::Sprite m_creditsBG;

	std::vector<sf::Text> m_creditText;

	sf::Sprite m_background;
	sf::Sprite m_backgroundElement_hair;
	Animation m_Animation_hair;
	sf::Sprite m_backgroundElement_grass1;
	Animation m_Animation_grass1;
	sf::Sprite m_backgroundElement_grass2;
	Animation m_Animation_grass2;
	sf::Sprite m_backgroundElement_grass3;
	Animation m_Animation_grass3;
	sf::Sprite m_backgroundElement_grass4;
	Animation m_Animation_grass4;
	sf::Sprite m_backgroundElement_grass5;
	Animation m_Animation_grass5;
	sf::Sprite m_backgroundElement_scarf;
	Animation m_Animation_scarf;

	void pushCreditsParagraph(const std::string &text, unsigned int charSize = 60);
	void resetCreditsPosition();
};

#endif