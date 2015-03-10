#ifndef INCLUDED_WRAPPED_TEXT_H
#define INCLUDED_WRAPPED_TEXT_H

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class WrappedText
{
public:
	WrappedText(sf::Vector2f position, sf::Vector2f messageBoxSize, sf::Font font);

	void setPosition(const sf::Vector2f &position);
	void setSize(const sf::Vector2f &size);

	// Used to override color during drawing
	void setTextColor(const sf::Color &color);

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getBoxSize() const;

	void finishTextNow();

	//Put in a string that will be written
	void Type(std::string text, float speed = 1, sf::Color color = sf::Color::Black, int textSize = 18);

	void Update(float deltaTime);

	void Draw(bool drawAsHud = true);

	bool isFinished();

private:
	//The size of the box which the text will be displayed in
	sf::Vector2f m_messageBoxSize;
	std::vector<std::string> m_linesOfText;
	sf::Vector2f m_position;
	sf::Text m_text;
	sf::Text m_thisTextRow;
	sf::Font m_font;
	int typedLength = 0;
	float delay = 1000;
	float timer = delay;
	int currentLetter = 0;
	int currentRow = 0;
	float m_speed;
	std::string m_string;
	bool m_turbo;
};

#endif