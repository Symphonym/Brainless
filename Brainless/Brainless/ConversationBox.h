#ifndef INCLUDED_CONVERSATION_BOX_H
#define INCLUDED_CONVERSATION_BOX_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "DialogTree.h"
#include "WrappedText.h"

class Game;
class ConversationBox
{
public:

	void setPosition(const sf::Vector2f &position);
	void setShown(bool shown);

	// Reset the current dialog being displayed
	void resetCurrentDialog();

	void events(const sf::Event &event, Game &game);
	void update(float deltaTime, Game &game);
	void draw();

	// Change the dialog in use by the box
	void setDialog(const DialogTree &dialog);

	sf::Vector2f getSize() const;
	bool isShown() const;

	static ConversationBox& instance();

private:

	static bool HelpDialogShown;
	void checkForHelpTrigger(Game &game);

	ConversationBox();

	std::string parseString(const std::string &str);

	void loadNextOptions();

	enum ConversationStates
	{
		Player,
		NPC
	};

	const std::string m_playerPortraitTextureName;
	bool m_isShown;
	ConversationStates m_conversationState;

	sf::Vector2f m_basePosition;

	sf::Text m_headerText;

	// List of answers to choose from
	std::vector<sf::Text> m_answers;

	DialogTree m_dialog;
	WrappedText m_dialogBox;
	sf::Sprite m_portraitSprite;
	sf::Sprite m_background;
};

#endif