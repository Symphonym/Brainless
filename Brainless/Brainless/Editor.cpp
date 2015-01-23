#include "Editor.h"

Editor::Editor()
:
m_editor(sf::VideoMode(800, 600, sf::Style::Close), "Brainless Editor")
{
	
}

void Editor::run()
{
	loop();
}

void Editor::loop()
{
	while (m_editor.isOpen())
	{
		sf::Event event;
		while (m_editor.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_editor.close();

		}



		m_editor.clear(sf::Color::Black);
		//map.draw(defView);
		//window.draw(circle);
		m_editor.display();
	}
}