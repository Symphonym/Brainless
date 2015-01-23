#include "Editor.h"
#include "Renderer.h"

Editor::Editor()
:
m_editor(sf::VideoMode(800, 600, sf::Style::Close), "Brainless Editor"),
m_camera(m_editor.getDefaultView())
{
	Renderer::instance().setTarget(m_editor);
}

void Editor::run()
{
	loop();
}

void Editor::loop()
{
	sf::Clock tickClock;
	while (m_editor.isOpen())
	{
		float deltaTime = tickClock.restart().asSeconds();
		const float cameraSpeed = deltaTime*100.f;
		const float zoomSpeed = deltaTime;

		sf::Event event;
		while (m_editor.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_editor.close();

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			m_camera.move(0, -cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			m_camera.move(0, cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			m_camera.move(-cameraSpeed, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			m_camera.move(cameraSpeed, 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			m_camera.zoom(1.f + zoomSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			m_camera.zoom(1.f - zoomSpeed);


		m_editor.clear(sf::Color::Black);
		//map.draw(defView);
		//window.draw(circle);
		m_editor.display();
	}
}