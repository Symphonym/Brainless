#include "ConversationBox.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Game.h"
#include "OptionsMenu.h"

ConversationBox::ConversationBox()
:
m_playerPortraitTextureName("PlayerAvatar"),
m_isShown(false),
m_conversationState(ConversationStates::NPC),
m_basePosition(0, 0),
m_dialogBox(sf::Vector2f(0, 0), sf::Vector2f(100, 100), ResourceLoader::instance().retrieveFont("DefaultFont"))
{
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("DialogBoxBackground"));
	m_dialogBox.setSize(sf::Vector2f(m_background.getGlobalBounds().width / 1.6, m_background.getGlobalBounds().height / 1.3f));
	m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_playerPortraitTextureName));

	m_headerText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_headerText.setString("Select your answer:");
	m_headerText.setCharacterSize(30);
}

void ConversationBox::setPosition(const sf::Vector2f &position)
{
	m_basePosition = position;
	m_background.setPosition(m_basePosition);
	m_dialogBox.setPosition(sf::Vector2f(
		m_background.getGlobalBounds().left + m_background.getGlobalBounds().width - m_dialogBox.getBoxSize().x - 20.f,
		m_background.getPosition().y + 10.f));
	m_portraitSprite.setPosition(
		m_background.getPosition().x + 10.f,
		m_background.getPosition().y + 10.f);

	m_headerText.setPosition(
		m_portraitSprite.getPosition().x + m_portraitSprite.getGlobalBounds().width + 10.f,
		m_background.getPosition().y);

	for (std::size_t i = 0; i < m_answers.size(); i++)
	{
		sf::Text &text = m_answers[i];
		
		text.setPosition(
			m_portraitSprite.getPosition().x + m_portraitSprite.getGlobalBounds().width + 10.f,//m_background.getGlobalBounds().left + m_background.getGlobalBounds().width - text.getGlobalBounds().width - 10.f,
			m_background.getPosition().y + 25.f + i*(text.getGlobalBounds().height + 5.f)
			);
	}
}
void ConversationBox::setShown(bool shown)
{
	m_isShown = shown;

	if (m_isShown)
		m_conversationState = ConversationStates::NPC;
}

void ConversationBox::resetCurrentDialog()
{
	m_dialogBox.Type(parseString(m_dialog.getCurrentDialog()), 5000.f, sf::Color::White);
}

void ConversationBox::events(const sf::Event &event, Game &game)
{
	if (m_isShown)
	{
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());

				// Clicking outside the conversation box will close it
				if (!m_background.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)))
				{
					m_dialogBox.finishTextNow();
					return;
				}

				if (m_conversationState == ConversationStates::Player)
				{

					// Go through answers and check if an answer was clicked
					for (std::size_t i = 0; i < m_answers.size(); i++)
					{

						sf::FloatRect textBounds = m_answers[i].getGlobalBounds();
						sf::FloatRect mouseBounds = sf::FloatRect(
							0, mousePos.y, game.getWindow().getSize().x, 1);
						sf::Text &text = m_answers[i];

						// An answer was clicked
						if (textBounds.intersects(mouseBounds) || (m_answers.size() == 1 && m_background.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))))
						{
							m_conversationState = ConversationStates::NPC;
							m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_dialog.getPortraitTextureName()), true);

							// If the next branch has a description then we change into that branch, otherwise exit conversation
							const DialogBranch &nextBranch = m_dialog.getCurrentOptions()[i].second;
							if (nextBranch.description.empty())
								setShown(false);
							else
								m_dialog.changeBranch(nextBranch);

							// Reset the dialog next and load the next answers
							resetCurrentDialog();
							loadNextOptions();
						}
					}
				}
				else if (m_conversationState == ConversationStates::NPC)
				{
					// Dialogbox has finished showing it's text, clicking will continue the dialog
					if (m_dialogBox.isFinished())
					{
						m_conversationState = ConversationStates::Player;
						m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_playerPortraitTextureName), true);

						// If there's no more dialog options, close the dialog
						if (m_dialog.getCurrentOptions().size() == 0)
							setShown(false);
					}
				}
			}
		}
	}
}
void ConversationBox::update(float deltaTime, Game &game)
{
	if (m_isShown)
	{
		sf::Vector2f pos(
			game.getWindow().getSize().x / 2.f - m_background.getGlobalBounds().width / 2.f,
			game.getWindow().getSize().y - m_background.getGlobalBounds().height);
		setPosition(pos);

		// Just highlight answers if you hover above them
		if (m_conversationState == ConversationStates::Player)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());
			sf::FloatRect mouseBounds = sf::FloatRect(
				0, mousePos.y, game.getWindow().getSize().x, 1);

			for (std::size_t i = 0; i < m_answers.size(); i++)
			{
				sf::FloatRect textBounds = m_answers[i].getGlobalBounds();
				sf::Text &text = m_answers[i];

				// Toggle colors on answers depending on click, hover, etc
				if (textBounds.intersects(mouseBounds))//contains(sf::Vector2f(mousePos.x, mousePos.y)))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						text.setColor(sf::Color::Green);
					else
						text.setColor(sf::Color::Yellow);
				}
				else
					text.setColor(sf::Color::White);

			}
		}

		// Update dialog box drawing
		else if (m_conversationState == ConversationStates::NPC)
			m_dialogBox.Update(deltaTime);
	}

}
void ConversationBox::draw()
{
	if (m_isShown)
	{

		Renderer::instance().drawHUD(m_background);
		Renderer::instance().drawHUD(m_portraitSprite);

		 //Draw answers only if it's the player's turn to speak
		if (m_conversationState == ConversationStates::Player)
		{
			Renderer::instance().drawHUD(m_headerText);

			for (std::size_t i = 0; i < m_answers.size(); i++)
				Renderer::instance().drawHUD(m_answers[i]);
		}
		 //Draw the dialog only if it's the NPC speaking
		else if (m_conversationState == ConversationStates::NPC)
			m_dialogBox.Draw();
	}

}

sf::Vector2f ConversationBox::getSize() const
{
	return sf::Vector2f(m_background.getGlobalBounds().width, m_background.getGlobalBounds().height);
}
bool ConversationBox::isShown() const
{
	return m_isShown;
}

void ConversationBox::setDialog(const DialogTree &dialog)
{
	// Copy over the dialog and set the sprite to that of the NPC
	m_dialog = dialog;
	m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_dialog.getPortraitTextureName()), true);
	resetCurrentDialog(); // Reset any existing dialog
	loadNextOptions(); // Load answers
}

std::string ConversationBox::parseString(const std::string &str)
{
	std::string result = str;
	auto replaceFunc = [](std::string &str, const std::string &targetStr, const std::string &newStr) -> void
	{
		while (str.find(targetStr) != str.npos)
		{
			std::string::size_type startPos = str.find(targetStr);
			str.replace(startPos, targetStr.size(), newStr);
		}
	};

	replaceFunc(result, "<UP>", OptionsMenu::getKeybindKeyName("Up"));
	replaceFunc(result, "<DOWN>", OptionsMenu::getKeybindKeyName("Down"));
	replaceFunc(result, "<LEFT>", OptionsMenu::getKeybindKeyName("Left"));
	replaceFunc(result, "<RIGHT>", OptionsMenu::getKeybindKeyName("Right"));

	replaceFunc(result, "<JUMP>", OptionsMenu::getKeybindKeyName("Jump"));
	replaceFunc(result, "<RUN>", OptionsMenu::getKeybindKeyName("Run"));

	return result;
}

void ConversationBox::loadNextOptions()
{
	m_answers.clear();
	for (std::size_t i = 0; i < m_dialog.getCurrentOptions().size(); i++)
	{
		sf::Text option;
		option.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		option.setString(parseString(m_dialog.getCurrentOptions()[i].first));
		option.setCharacterSize(18);
	
		// Set text position to match that of the dialog box
		/*option.setPosition(
			0,//m_dialogBox.getPosition().x,
			m_dialogBox.getPosition().y + i*(option.getGlobalBounds().height + 5.f)
			);*/
		m_answers.push_back(option);
	}
}


ConversationBox& ConversationBox::instance()
{
	static ConversationBox convBox;
	return convBox;
}