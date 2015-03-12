#include "LeverItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"

LeverItem::LeverItem(bool down, int id)
:
Item("Lever", "LeverDown", "LeverDown", id),
m_isDown(down)
{
	m_usable = true;
}


void LeverItem::onUse(Game &game)
{
	m_isDown = !m_isDown;
		
	if (m_isDown)
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverDown"));
	}
	else
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverUp"));
	}
}
void LeverItem::onExamine()
{
	if (m_isDown)
		m_examineString = "It's a lever, it looks a bit down.";
	else
	{
		m_examineString = "It's a lever, up up and away!";
	}
}

void LeverItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isDown << std::endl;
}
void LeverItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isDown;

	if (m_isDown)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverDown"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverUp"));
}

void LeverItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* LeverItem::clone()
{
	return new LeverItem(*this);
}
