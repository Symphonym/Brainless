#include "LighterItem.h"
#include "Game.h"
#include "Renderer.h"

LighterItem::LighterItem(int id)
:
Item("Lighter","LighterInv", "LighterItem", id)
{
	m_lootable = true;
	m_examineString = "This lighter still works.";
	m_useString = "I should probably just pick it up instead.";
}

void LighterItem::heldUpdate(float deltaTime, Game &game)
{
	sf::Shader *shader = Renderer::instance().getCurrentShader();

	if (shader)
	{
		shader->setParameter("enableLightSource", 1);
		shader->setParameter("lightPos", sf::Mouse::getPosition(game.getWindow()).x, game.getWindow().getSize().y - sf::Mouse::getPosition(game.getWindow()).y);
		shader->setParameter("lightReachDistance", 300);
	}
}
void LighterItem::update(float deltaTime, Game &game)
{
	sf::Shader *shader = Renderer::instance().getCurrentShader();

	if (shader)
	{
		sf::Vector2i screenPos = game.getWindow().mapCoordsToPixel(getPosition());

		shader->setParameter("enableLightSource", 1);
		shader->setParameter("lightPos", 
			screenPos.x+getSprite().getGlobalBounds().width/2.f, game.getWindow().getSize().y - screenPos.y+getSprite().getGlobalBounds().height/2.f);
		shader->setParameter("lightReachDistance", 300);
	}
}

Item* LighterItem::clone()
{
	return new LighterItem(*this);
}