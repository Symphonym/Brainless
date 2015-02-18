#include "BeachParty.h"
#include "ResourceLoader.h"

BeachParty::BeachParty(ArcadeMachine &machine)
:
ArcadeGame(machine, "�ke's Beach Party"),
m_turtleAnimation(50,50),
m_crabAnimation(40,40),
m_frieAnimation(40,40)
{
	/*m_turtleSprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
	m_crabSprite.setTexture(ResourceLoader::instance().retrieveTexture("crabsheet"));
	m_frieSprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));*/
}

void BeachParty::update(float deltaTime)
{

}