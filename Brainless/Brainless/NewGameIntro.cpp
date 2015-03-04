#include "NewGameIntro.h"
#include "Notification.h"
#include "Game.h"
#include "StateMachine.h"
#include "ResourceLoader.h"

NewGameIntro::NewGameIntro(StateMachine &machine)
:
State(machine)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_NewGameIntro.txt");
	Notification::instance().write("The game has zombies and shit, pretty crazy");
}

void NewGameIntro::events(const sf::Event &event)
{

}
void NewGameIntro::update(float deltaTime)
{
	Notification::instance().update(deltaTime, m_window);

	if (!Notification::instance().isShown())
	{
		m_machine.popState();
		m_machine.pushState<Game>();
	}
}
void NewGameIntro::draw()
{
	Notification::instance().draw();
}

void NewGameIntro::onStop()
{

}
void NewGameIntro::onPlay()
{
	
}