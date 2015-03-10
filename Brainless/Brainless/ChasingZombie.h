#ifndef _CHASING_ZOMBIE
#define _CHASING_ZOMBIE
#include "Zombie.h"

class ChasingZombie : public Zombie
{
public:
	ChasingZombie(sf::Vector2f startPosition, int maxLengthX, int Texture, int syncID = -1);

	void updateTask(float deltaTime);
	int getWalkLength();


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
	int m_maxWalkLenght;
	int m_currentLength;
	Direction m_direction;
	Unit* m_target;
	ChasingState m_chaseState;
};

#endif