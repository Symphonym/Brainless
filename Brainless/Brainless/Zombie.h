#ifndef _ZOMBIE
#define _ZOMBIE
#include "Unit.h"
//#include "ZombieScript.h"

class Zombie : public Unit
{
public:
	Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID, int Texture, int syncID = -1);

	virtual bool onInteractedWith(Item &otherItem, Game &game);
	virtual void onCollideWith(Unit *unit);
	//virtual void onCollisionWithItem(Item &item) final;
	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		anim_noAnimation,
		anim_idle,
		anim_walking,
		anim_idleSlow,
		anim_walkingSlow,
	};

	// If set to false, this zombie will not deal damage
	void setDamaging(bool damaging);

	
	void incrementTexture();
	int getTextureID();
	int getSyncID() override;

	bool collide(Unit *unit);
protected:
	int m_textureId;
	AnimationState m_animState;
	void animation_idle();
	void animation_walking();
	void animation_idleSlow();
	void animation_walkingSlow();
	bool m_isDamaging;
private:
};

#endif