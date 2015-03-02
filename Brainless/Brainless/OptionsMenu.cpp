#include "OptionsMenu.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Button.h"
#include "MainMenu.h"
#include "StateMachine.h"
#include "Utility.h"
#include "SoundPlayer.h"

#define SPRITESIZE 256 

std::unordered_map<std::string, sf::Keyboard::Key> OptionsMenu::m_keybinds;

OptionsMenu::OptionsMenu(StateMachine &machine)
:
State(machine),
m_recordingKey(false),
m_currentKey(""),
m_Animation_hair(SPRITESIZE, SPRITESIZE),
m_Animation_grass1(SPRITESIZE, SPRITESIZE),
m_Animation_grass2(SPRITESIZE, SPRITESIZE),
m_Animation_grass3(SPRITESIZE, SPRITESIZE),
m_Animation_grass4(SPRITESIZE, SPRITESIZE),
m_Animation_grass5(SPRITESIZE, SPRITESIZE),
m_Animation_scarf(SPRITESIZE, SPRITESIZE)
{
	m_keybindInfoText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_keybindInfoText.setString("Up:\nDown:\nLeft:\nRight:");
	m_keybindInfoText.setPosition(100, 100);

	addKeybindOption("Up", sf::Vector2f(300, 100));
	addKeybindOption("Down", sf::Vector2f(300, 150));
	addKeybindOption("Left", sf::Vector2f(300, 200));
	addKeybindOption("Right", sf::Vector2f(300, 250));
	addKeybindOption("Inventory", sf::Vector2f(300, 300));
	addKeybindOption("Jump", sf::Vector2f(300, 350));
	addKeybindOption("Run", sf::Vector2f(300, 400));

	sf::Image volImg;
	volImg.create(1, 30, sf::Color::White);

	m_volumeTexture.loadFromImage(volImg);
	m_volumeSprite.setTexture(m_volumeTexture);

	m_volumeSprite.setPosition(200, m_window.getSize().y - m_volumeSprite.getGlobalBounds().height*2.f - 120);
	m_volumeSprite.setColor(sf::Color::Green);
	m_curVolume = SoundPlayer::instance().getVolume();
	m_volumeSprite.setScale(m_curVolume * 4, 1);



	m_backButton = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Back_Normal"),
		ResourceLoader::instance().retrieveTexture("Back_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2 + 100.f, m_window.getSize().y / 2 - 200)));


	m_volumeUpButton = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("PlusButton"),
		ResourceLoader::instance().retrieveTexture("PlusButton_Pressed"),
		sf::Vector2f(610, m_window.getSize().y - 250)));
	m_volumeDownButton = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("MinusButton"),
		ResourceLoader::instance().retrieveTexture("MinusButton_Pressed"),
		sf::Vector2f(110, m_window.getSize().y - 250)));

	m_volumeText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_volumeText.setCharacterSize(50);
	m_volumeText.setColor(sf::Color::Green);
	m_volumeText.setString("Volume: " + std::to_string(m_curVolume) + "%");
	m_volumeText.setPosition(200, m_window.getSize().y - 260);

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

void OptionsMenu::events(const sf::Event &event)
{
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));

			for (auto itr = m_keybindTexts.begin(); itr != m_keybindTexts.end(); itr++)
			{
				if (itr->second.getGlobalBounds().contains(mousePos))
				{
					m_recordingKey = true;
					m_currentKey = itr->first;
				}
				else // ADD OTHER INPUTS
				{

				}
			}

		}
	}
	else if (event.type == sf::Event::KeyReleased && m_recordingKey)
	{
		std::string keyName = keyToString(event.key.code);
		if (keyName != "Unknown")
		{
			m_keybinds[m_currentKey] = event.key.code;
			m_keybindTexts[m_currentKey].setString(m_currentKey + ": " + keyName);
		}

		
		m_recordingKey = false;
	}
}
void OptionsMenu::update(float deltaTime)
{
	m_backgroundElement_hair.setTextureRect(m_Animation_hair.getRectangle(deltaTime));
	m_backgroundElement_grass1.setTextureRect(m_Animation_grass1.getRectangle(deltaTime));
	m_backgroundElement_grass2.setTextureRect(m_Animation_grass2.getRectangle(deltaTime));
	m_backgroundElement_grass3.setTextureRect(m_Animation_grass3.getRectangle(deltaTime));
	m_backgroundElement_grass4.setTextureRect(m_Animation_grass4.getRectangle(deltaTime));
	m_backgroundElement_grass5.setTextureRect(m_Animation_grass5.getRectangle(deltaTime));
	m_backgroundElement_scarf.setTextureRect(m_Animation_scarf.getRectangle(deltaTime));



	// Return to main menu
	if (m_backButton->getReleased(m_window))
	{
		m_machine.popState();
		m_machine.pushState<MainMenu>();
	}
	// Turn up volume
	else if (m_volumeUpButton->getReleased(m_window))
	{
		m_curVolume += 10;
		m_curVolume = Utility::clampValue<int>(m_curVolume, 0, 100);
		m_volumeSprite.setScale(m_curVolume * 4, 1);
		SoundPlayer::instance().setVolume(m_curVolume);
		m_volumeText.setString("Volume: " + std::to_string(m_curVolume) + "%");
	}
	// Turn down volume
	else if (m_volumeDownButton->getReleased(m_window))
	{
		m_curVolume -= 10;
		m_curVolume = Utility::clampValue<int>(m_curVolume, 0, 100);
		m_volumeSprite.setScale(m_curVolume * 4, 1);
		SoundPlayer::instance().setVolume(m_curVolume);
		m_volumeText.setString("Volume: " + std::to_string(m_curVolume) + "%");
	}

	sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window));
	for (auto itr = m_keybindTexts.begin(); itr != m_keybindTexts.end(); itr++)
	{
		if (itr->second.getGlobalBounds().contains(mousePos))
			itr->second.setColor(sf::Color::Yellow);
		else
			itr->second.setColor(sf::Color::Black);
	}

	if (m_recordingKey)
	{
		m_keybindTexts[m_currentKey].setColor(sf::Color::Red);
	}
}
void OptionsMenu::draw()
{
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_backgroundElement_hair);
	Renderer::instance().drawHUD(m_backgroundElement_grass1);
	Renderer::instance().drawHUD(m_backgroundElement_grass2);
	Renderer::instance().drawHUD(m_backgroundElement_grass3);
	Renderer::instance().drawHUD(m_backgroundElement_grass4);
	Renderer::instance().drawHUD(m_backgroundElement_grass5);
	Renderer::instance().drawHUD(m_backgroundElement_scarf);

	//Renderer::instance().drawHUD(m_keybindInfoText);

	for (auto itr = m_keybindTexts.begin(); itr != m_keybindTexts.end(); itr++)
		Renderer::instance().drawHUD(itr->second);

	m_backButton->draw();
	m_volumeUpButton->draw();
	m_volumeDownButton->draw();
	Renderer::instance().drawHUD(m_volumeSprite);
	Renderer::instance().drawHUD(m_volumeText);
}

sf::Keyboard::Key OptionsMenu::getKeybind(const std::string &keybindName)
{
	return m_keybinds.at(keybindName);
}

std::string OptionsMenu::keyToString(sf::Keyboard::Key key)
{
	std::string keyString = "Unknown";
	switch (key)
	{
	case sf::Keyboard::A: keyString = "A"; break;
	case sf::Keyboard::B: keyString = "B"; break;
	case sf::Keyboard::C: keyString = "C"; break;
	case sf::Keyboard::D: keyString = "D"; break;
	case sf::Keyboard::E: keyString = "E"; break;
	case sf::Keyboard::F: keyString = "F"; break;
	case sf::Keyboard::G: keyString = "G"; break;
	case sf::Keyboard::H: keyString = "H"; break;
	case sf::Keyboard::I: keyString = "I"; break;
	case sf::Keyboard::J: keyString = "J"; break;
	case sf::Keyboard::K: keyString = "K"; break;
	case sf::Keyboard::L: keyString = "L"; break;
	case sf::Keyboard::M: keyString = "M"; break;
	case sf::Keyboard::N: keyString = "N"; break;
	case sf::Keyboard::O: keyString = "O"; break;
	case sf::Keyboard::P: keyString = "P"; break;
	case sf::Keyboard::Q: keyString = "Q"; break;
	case sf::Keyboard::R: keyString = "R"; break;
	case sf::Keyboard::S: keyString = "S"; break;
	case sf::Keyboard::T: keyString = "T"; break;
	case sf::Keyboard::U: keyString = "U"; break;
	case sf::Keyboard::V: keyString = "V"; break;
	case sf::Keyboard::W: keyString = "W"; break;
	case sf::Keyboard::X: keyString = "X"; break;
	case sf::Keyboard::Y: keyString = "Y"; break;
	case sf::Keyboard::Z: keyString = "Z"; break;
	case sf::Keyboard::Up: keyString = "Up key"; break;
	case sf::Keyboard::Down: keyString = "Down key"; break;
	case sf::Keyboard::Left: keyString = "Left key"; break;
	case sf::Keyboard::Right: keyString = "Right key"; break;
	case sf::Keyboard::LShift: keyString = "Left shift"; break;
	case sf::Keyboard::Space: keyString = "Space"; break;
	};

	return keyString;
}
void OptionsMenu::addKeybindOption(const std::string &keybindName, const sf::Vector2f &position)
{
	sf::Text text;
	text.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	text.setCharacterSize(50);
	text.setColor(sf::Color::Black);
	text.setString(keybindName + ": " + keyToString(m_keybinds.at(keybindName)));
	text.setPosition(position);

	m_keybindTexts[keybindName] = text;

}
void OptionsMenu::initializeKeybinds()
{
	m_keybinds["Up"] = sf::Keyboard::W;
	m_keybinds["Down"] = sf::Keyboard::S;
	m_keybinds["Left"] = sf::Keyboard::A;
	m_keybinds["Right"] = sf::Keyboard::D;
	m_keybinds["Inventory"] = sf::Keyboard::I;
	m_keybinds["Jump"] = sf::Keyboard::Space;
	m_keybinds["Run"] = sf::Keyboard::LShift;
}

std::string OptionsMenu::getKeybindKeyName(const std::string &keybindName)
{
	return keyToString(m_keybinds.at(keybindName));
}