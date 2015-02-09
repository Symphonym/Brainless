#include "MagnifyingGlassItem.h"
#include "Game.h"
#include "Renderer.h"
#include "ResourceLoader.h"

MagnifyingGlassItem::MagnifyingGlassItem(int id)
:
Item("Magnifying Glass", "MagnifyingGlass", id)
{
	m_lootable = true;
	m_usable = false;
	m_useString = "Don't think I can use that, just like that.";
	m_examineString = "It's a magnifying glass.";

	m_magnifyingView = sf::View(sf::FloatRect(0, 0, 0.25f, 0.25f));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("MagnifyingGlassBG"));
}

void MagnifyingGlassItem::heldUpdate(float deltaTime, Game &game)
{
	m_background.setPosition(
		game.getWindow().getSize().x - m_background.getGlobalBounds().width,
		game.getWindow().getSize().y - m_background.getGlobalBounds().height);

	m_magnifyingView = game.getWindow().getView();
	m_magnifyingView.setViewport(sf::FloatRect(0.75f, 0.75f, 0.25f, 0.25f));
	m_magnifyingView.zoom(0.05f);
	sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));
	m_magnifyingView.setCenter(mousePos);

	game.addCamera(m_magnifyingView);
}

void MagnifyingGlassItem::heldDraw()
{
	Renderer::instance().drawHUD(m_background);
}

Item* MagnifyingGlassItem::clone()
{
	return new MagnifyingGlassItem(*this);
}

