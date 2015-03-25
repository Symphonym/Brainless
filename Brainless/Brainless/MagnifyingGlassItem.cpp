#include "MagnifyingGlassItem.h"
#include "Game.h"
#include "Renderer.h"
#include "ResourceLoader.h"

MagnifyingGlassItem::MagnifyingGlassItem(int id)
:
Item("Magnifying Glass","MagnifyingGlassInv", "MagnifyingGlass", id)
{
	m_lootable = true;
	m_usable = false;
	m_useString = "I should pick it up before using.";
	m_examineString = "Childhood memories; burning ants, smelling their burnt carcasses, it's all coming back to me.";

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

