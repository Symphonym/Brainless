#include "ArcadeGame.h"

ArcadeGame::ArcadeGame(ArcadeMachine &machine, const std::string &gameName)
:
m_machine(machine),
m_gameName(gameName)
{

}

std::string ArcadeGame::getName() const
{
	return m_gameName;
}