#include "MainMenu.h"
#include "StateMachine.h"
#include "Game.h"
#include "Button.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Renderer.h"
#include "FileSave.h"
#include "OptionsMenu.h"

#define SPRITESIZE 256 

MainMenu::MainMenu(StateMachine &machine)
:
State(machine),
m_newGameConfirmWindow(false),
m_Animation_hair(SPRITESIZE, SPRITESIZE),
m_Animation_grass1(SPRITESIZE, SPRITESIZE),
m_Animation_grass2(SPRITESIZE, SPRITESIZE),
m_Animation_grass3(SPRITESIZE, SPRITESIZE),
m_Animation_grass4(SPRITESIZE, SPRITESIZE),
m_Animation_grass5(SPRITESIZE, SPRITESIZE),
m_Animation_scarf(SPRITESIZE, SPRITESIZE)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_MainMenu.txt");
	
	
	//Renderer::instance().drawHUD(sf::Sprite(ResourceLoader::instance().retrieveTexture("StartScreen")));
	//Renderer::instance().executeDraws();
	//sf::sleep(sf::seconds(1)); //ska nog inte vara här, bara testade lite snabbt innan jag skulle med bussen

	m_buttons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("NewGame_Normal"),
		ResourceLoader::instance().retrieveTexture("NewGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 200)));

	m_buttons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("LoadGame_Normal"),
		ResourceLoader::instance().retrieveTexture("LoadGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 100.f)));

	m_buttons[2] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Options_Normal"),
		ResourceLoader::instance().retrieveTexture("Options_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 + 100.f)));


	// New game confirm data
	m_confirmBackground.setTexture(ResourceLoader::instance().retrieveTexture("NewGameConfirmBG"));
	m_confirmBackground.setPosition(
		m_window.getSize().x / 2, m_window.getSize().y / 2);

	m_newGameButtons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Yes_Normal"),
		ResourceLoader::instance().retrieveTexture("Yes_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 20.f, m_confirmBackground.getPosition().y + 120.f)));

	m_newGameButtons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("No_Normal"),
		ResourceLoader::instance().retrieveTexture("No_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 210.f, m_confirmBackground.getPosition().y + 120.f)));

	//Background
	//m_background.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background"));
	m_backgroundElement_hair.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass1.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass2.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass3.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass4.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass5.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_scarf.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));

	m_backgroundElement_hair.setPosition(8, 94);
	m_backgroundElement_grass1.setPosition(1075, 569);
	m_backgroundElement_grass2.setPosition(710, 569);
	m_backgroundElement_grass3.setPosition(400, 569);
	m_backgroundElement_grass4.setPosition(140, 569);
	m_backgroundElement_grass5.setPosition(-100, 565);
	m_backgroundElement_scarf.setPosition(-15, 166);

	m_Animation_hair.loop(0, 5, 0, 4.5f);
	m_Animation_grass1.loop(0, 5, 1, 4.5f);
	m_Animation_grass2.loop(0, 5, 2, 4.5f);
	m_Animation_grass3.loop(0, 5, 3, 4.5f);
	m_Animation_grass4.loop(0, 5, 4, 4.5f);
	m_Animation_grass5.loop(0, 5, 5, 4.5f);
	m_Animation_scarf.loop(0, 5, 6, 4.5f);

}

void MainMenu::onStop()
{
	//Play music
	SoundPlayer::instance().stopMusic("MenuMusic");
}
void MainMenu::onPlay()
{
	//Play music
	SoundPlayer::instance().playMusic("MenuMusic", true, 20);
}

void MainMenu::update(float deltaTime)
{
	m_backgroundElement_hair.setTextureRect(m_Animation_hair.getRectangle(deltaTime));
	m_backgroundElement_grass1.setTextureRect(m_Animation_grass1.getRectangle(deltaTime));
	m_backgroundElement_grass2.setTextureRect(m_Animation_grass2.getRectangle(deltaTime));
	m_backgroundElement_grass3.setTextureRect(m_Animation_grass3.getRectangle(deltaTime));
	m_backgroundElement_grass4.setTextureRect(m_Animation_grass4.getRectangle(deltaTime));
	m_backgroundElement_grass5.setTextureRect(m_Animation_grass5.getRectangle(deltaTime));
	m_backgroundElement_scarf.setTextureRect(m_Animation_scarf.getRectangle(deltaTime));

	if (!m_newGameConfirmWindow)
	{

		// New game
		if (m_buttons[0]->getReleased(m_machine.getWindow()))
			m_newGameConfirmWindow = true;

		// Load game
		else if (m_buttons[1]->getReleased(m_machine.getWindow()))
		{
			m_machine.popState();
			m_machine.pushState<Game>();
		}
		// Options menu
		else if (m_buttons[2]->getReleased(m_machine.getWindow()))
		{
			m_machine.popState();
			m_machine.pushState<OptionsMenu>();
		}
	}
	else
	{
		// Yes
		if (m_newGameButtons[0]->getReleased(m_machine.getWindow()))
		{
			FileSave::wipeProgress();
			m_machine.popState();
			m_machine.pushState<Game>();
		}

		// No
		else if (m_newGameButtons[1]->getReleased(m_machine.getWindow()))
			m_newGameConfirmWindow = false;
	}
}
void MainMenu::draw()
{
	
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_backgroundElement_hair);
	Renderer::instance().drawHUD(m_backgroundElement_grass1);
	Renderer::instance().drawHUD(m_backgroundElement_grass2);
	Renderer::instance().drawHUD(m_backgroundElement_grass3);
	Renderer::instance().drawHUD(m_backgroundElement_grass4);
	Renderer::instance().drawHUD(m_backgroundElement_grass5);
	Renderer::instance().drawHUD(m_backgroundElement_scarf);

	for (auto &button : m_buttons)
		button->draw();

	if (m_newGameConfirmWindow)
	{
		Renderer::instance().drawHUD(m_confirmBackground);

		for (auto &button : m_newGameButtons)
			button->draw();
	}
	
	Renderer::instance().executeDraws();
}