#include "WrappedText.h"
#include "ResourceLoader.h"
#include "Renderer.h"

WrappedText::WrappedText(sf::Vector2f position, sf::Vector2f messageBoxSize, sf::Font font)
:
m_position(position),
m_messageBoxSize(messageBoxSize),
m_font(font)
{
}

void WrappedText::setPosition(const sf::Vector2f &position)
{
	m_position = position;
}
void WrappedText::setSize(const sf::Vector2f &size)
{
	m_messageBoxSize = size;
}
void WrappedText::setTextColor(const sf::Color &color)
{
	m_text.setColor(color);
}

const sf::Vector2f& WrappedText::getPosition() const
{
	return m_position;
}
const sf::Vector2f& WrappedText::getBoxSize() const
{
	return m_messageBoxSize;
}

void WrappedText::Type(std::string text, float speed, sf::Color color, int textSize)
{
	typedLength = 0;
	delay = 200;
	timer = delay;
	currentLetter = 0;
	currentRow = 0;
	m_text.setCharacterSize(textSize);
	m_thisTextRow.setCharacterSize(textSize);
	m_text.setColor(color);
	m_linesOfText.clear();
	m_linesOfText.push_back("");
	m_string = text;
	m_speed = speed;
	m_text.setString("");
	m_text.setFont(m_font);
	m_thisTextRow.setFont(m_font);
}

void WrappedText::Update(float deltaTime)
{
	m_text.setPosition(m_position);

	if (currentLetter < m_string.length())
	{
		timer -= deltaTime * m_speed;

		if (timer <= 0)
		{
			timer = delay;
			m_linesOfText[currentRow] += m_string[currentLetter];
			currentLetter++;

			//Adds all lines of text to the m_text variable
			std::string allLines = "";
			for (int i = 0; i < currentRow + 1; i++)
			{
				allLines += m_linesOfText[i];
				allLines += '\n';
			}
			m_text.setString(allLines);

			//Sets text to thisTextRow which is used to check if the current line of text is to wide for the messageBox
			m_thisTextRow.setString(m_linesOfText[currentRow]);

			//Removes the highest line if the text height is to big for the box
			while (m_text.getGlobalBounds().height > m_messageBoxSize.y)
			{
				std::string croppedText = m_text.getString();

				for (int i = 0; i <= croppedText.length(); i++)
				{
					croppedText.erase(croppedText.begin());
					if (croppedText[0] == '\n')
					{
						croppedText.erase(croppedText.begin());
						break;
					}
				}
				m_text.setString(croppedText);
			}

			//If the current line of text is to wide,
			//add the last word to the next line and delete it from the current line
			if (m_thisTextRow.getGlobalBounds().width > m_messageBoxSize.x)
			{
				m_linesOfText.push_back("");
				currentRow++;
				for (int i = m_linesOfText[currentRow - 1].length(); i > 0; i--)
				{
					if (m_linesOfText[currentRow - 1][i - 1] == ' ')
					{
						for (int j = i; j < m_linesOfText[currentRow - 1].length(); j++)
						{
							m_linesOfText[currentRow] += m_linesOfText[currentRow - 1][j];
						}
						while (m_linesOfText[currentRow - 1][m_linesOfText[currentRow - 1].length() - 1] != ' ')
						{
							m_linesOfText[currentRow - 1].pop_back();
						}
						break;
					}
				}
			}
		}
	}
}

void WrappedText::Draw(bool drawAsHud)
{
	if (drawAsHud)
		Renderer::instance().drawHUD(m_text);
	else
		Renderer::instance().drawAbove(m_text);
}


bool WrappedText::isFinished() const
{
	return currentLetter >= m_string.length();
}