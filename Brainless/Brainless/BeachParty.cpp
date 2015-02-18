#include "BeachParty.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"

BeachParty::BeachParty(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åke's Beach Party"),
m_turtleAnimation(50,50),
m_crabAnimation(40,40),
m_frieAnimation(40,40)
{
	m_turtleSprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
	m_crabSprite.setTexture(ResourceLoader::instance().retrieveTexture("crabsheet"));
	m_frieSprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("beach"));

	m_turtleSprite.setPosition(m_machine.getScreenPos().x + 325, m_machine.getScreenPos().y + 325);
}

void BeachParty::update(float deltaTime)
{

}