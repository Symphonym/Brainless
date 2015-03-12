#include "LeverSideItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"

LeverSideItem::LeverSideItem(bool left, int id)
:
Item("Lever", "LeverLeft", "LeverLeft", id),
m_isLeft(left)
{
	m_usable = true;
}


void LeverSideItem::onUse(Game &game)
{
	m_isLeft = !m_isLeft;

	if (m_isLeft)
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverLeft"));
	}
	else
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverRight"));
	}
}
void LeverSideItem::onExamine()
{
	if (m_isLeft)
		m_examineString = "It's a lefty lever.";
	else
	{
		m_examineString = "It's a lever, Righto.";
	}
}

void LeverSideItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isLeft << std::endl;
}
void LeverSideItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isLeft;

	if (m_isLeft)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverLeft"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("LeverRight"));
}

void LeverSideItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* LeverSideItem::clone()
{
	return new LeverSideItem(*this);
}
