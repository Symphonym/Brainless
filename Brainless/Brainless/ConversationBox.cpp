#include "ConversationBox.h"
#include "ResourceLoader.h"
#include "Renderer.h"

ConversationBox::ConversationBox()
:
m_playerPortraitTextureName("TestItem5"),
m_isShown(false),
m_conversationState(ConversationStates::NPC),
m_basePosition(0, 0),
m_dialogBox(sf::Vector2f(0, 0), sf::Vector2f(100, 100), ResourceLoader::instance().retrieveFont("DefaultFont"))
{
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("BackgroundTest"));
	m_dialogBox.setSize(sf::Vector2f(m_background.getGlobalBounds().width / 1.6, m_background.getGlobalBounds().height / 1.3f));
	m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_playerPortraitTextureName));
}

void ConversationBox::setPosition(const sf::Vector2f &position)
{
	m_basePosition = position;
	m_background.setPosition(m_basePosition);
	m_dialogBox.setPosition(sf::Vector2f(
		m_background.getGlobalBounds().left + m_background.getGlobalBounds().width - m_dialogBox.getBoxSize().x - 10.f,
		m_background.getPosition().y + 10.f));
	m_portraitSprite.setPosition(
		m_background.getPosition().x + 10.f,
		m_background.getPosition().y + 10.f);

	for (std::size_t i = 0; i < m_answers.size(); i++)
	{
		sf::Text &text = m_answers[i];
		
		text.setPosition(
			m_background.getGlobalBounds().left + m_background.getGlobalBounds().width - text.getGlobalBounds().width - 10.f,
			m_background.getPosition().y + 10.f + i*(text.getGlobalBounds().height + 5.f)
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
	m_dialogBox.Type(m_dialog.getCurrentDialog(), 5000.f, sf::Color::White);
}

void ConversationBox::events(const sf::Event &event, const sf::RenderWindow &gameWindow)
{
	if (m_isShown)
	{
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (m_conversationState == ConversationStates::Player)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);

					// Go through answers and check if an answer was clicked
					for (std::size_t i = 0; i < m_answers.size(); i++)
					{

						sf::FloatRect textBounds = m_answers[i].getGlobalBounds();
						sf::Text &text = m_answers[i];

						// An answer was clicked
						if (textBounds.contains(sf::Vector2f(mousePos.x, mousePos.y)))
						{
							m_conversationState = ConversationStates::NPC;
							m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_dialog.getPortraitTextureName()));

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
						m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_playerPortraitTextureName));

						// If there's no more dialog options, close the dialog
						if (m_dialog.getCurrentOptions().size() == 0)
							setShown(false);
					}
				}
			}
		}
	}
}
void ConversationBox::update(float deltaTime, const sf::RenderWindow &gameWindow)
{
	if (m_isShown)
	{
		// Just highlight answers if you hover above them
		if (m_conversationState == ConversationStates::Player)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);

			for (std::size_t i = 0; i < m_answers.size(); i++)
			{
				sf::FloatRect textBounds = m_answers[i].getGlobalBounds();
				sf::Text &text = m_answers[i];

				// Toggle colors on answers depending on click, hover, etc
				if (textBounds.contains(sf::Vector2f(mousePos.x, mousePos.y)))
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

		// Draw answers only if it's the player's turn to speak
		if (m_conversationState == ConversationStates::Player)
		{
			for (std::size_t i = 0; i < m_answers.size(); i++)
				Renderer::instance().drawHUD(m_answers[i]);
		}
		// Draw the dialog only if it's the NPC speaking
		else if (m_conversationState == ConversationStates::NPC)
			m_dialogBox.Draw();
	}

}

sf::Vector2f ConversationBox::getSize() const
{
	return sf::Vector2f(m_background.getGlobalBounds().width, m_background.getGlobalBounds().height);
}

void ConversationBox::setDialog(const DialogTree &dialog)
{
	// Copy over the dialog and set the sprite to that of the NPC
	m_dialog = dialog;
	m_portraitSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_dialog.getPortraitTextureName()));
	resetCurrentDialog(); // Reset any existing dialog
	loadNextOptions(); // Load answers
}

void ConversationBox::loadNextOptions()
{
	m_answers.clear();
	for (std::size_t i = 0; i < m_dialog.getCurrentOptions().size(); i++)
	{
		sf::Text option;
		option.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		option.setString(m_dialog.getCurrentOptions()[i].first);
		option.setCharacterSize(15);
	
		// Set text position to match that of the dialog box
		option.setPosition(
			m_dialogBox.getPosition().x,
			m_dialogBox.getPosition().y + i*(option.getGlobalBounds().height + 5.f)
			);
		m_answers.push_back(option);
	}
}


ConversationBox& ConversationBox::instance()
{
	static ConversationBox convBox;
	return convBox;
}