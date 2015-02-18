#ifndef _CHASING_ZOMBIE
#define _CHASING_ZOMBIE
#include "Zombie.h"

class ChasingZombie : public Zombie
{
public:
	ChasingZombie(sf::Vector2f startPosition, int maxLengthX);

	void updateTask(float deltaTime);
	void updateAnimation(float deltaTime);
	int getWalkLenght();

	enum AnimationState
	{
		anim_noAnimation,
		anim_idle,
		anim_walking
	};

	enum ChasingState
	{
		chase_idle,
		chase_farFromHome,
		chase_returnHome,
		chase_chase
	};

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

protected:

private:
	sf::Vector2f m_homePosition;
	AnimationState m_animState;
	int m_maxWalkLenght;
	int m_currentLength;
	Direction m_direction;
	Unit* m_target;
	ChasingState m_chaseState;
};

#endif