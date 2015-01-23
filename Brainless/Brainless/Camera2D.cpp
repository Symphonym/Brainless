#include "Camera2D.h"
#include "SFML\Window\Mouse.hpp"
#include "SFML\Window\Keyboard.hpp"

Camera2D::Camera2D(sf::Vector2f center, sf::Vector2f size)
:
m_view(center, size),
m_size(size)
{
}

void Camera2D::levelEditorUpdate(sf::RenderWindow &window)
{
	sf::Vector2f velocity(0, 0);
	mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		velocity += sf::Vector2f(0, -0.1f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		velocity += sf::Vector2f(0, 0.1f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		velocity += sf::Vector2f(-0.1f, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		velocity += sf::Vector2f(0.1f, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		m_view.zoom(1.0001f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		m_view.zoom(0.9999f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		m_view.setSize(m_size);

	m_view.move(velocity);

	window.setView(m_view);
}

void Camera2D::gameUpdate(sf::RenderWindow &window, sf::Vector2f &target)
{
	m_center = target;
	m_view.setCenter(m_center);

	window.setView(m_view);
}

