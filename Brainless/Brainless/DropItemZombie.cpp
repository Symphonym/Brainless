#include "DropItemZombie.h"
#include "ItemDatabase.h"
#include "ConversationBox.h"

#define MAX_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 200
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50


//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02

DropItemZombie::DropItemZombie(sf::Vector2f startPosition, int Texture, Direction startDirection)
:
Zombie(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_CabinetZombie, Texture)
{
	m_itemsWithin = { 44, 11, 43 };
	m_specialSpriteDirection = true;
	m_spriteDirection = startDirection;
	m_specialSpriteDirection = true;
	animation_idle();
	m_isDamaging = false;
	m_dialog.loadDialogFile("dialogues/zombie_items.txt");
}

void DropItemZombie::serialize(std::ofstream &writer) const //N�GONSTANS FEL, ZOMBIE KAN F�RSVINNA VID SAVE PROGRESS OCH ITEMS KAN GES 2 G�NGER
{
	Unit::serialize(writer);

	writer << m_textureId << std::endl;
	
	writer << m_itemsGiven << std::endl;
}
void DropItemZombie::deserialize(std::ifstream &reader)
{
	Unit::deserialize(reader);

	reader >> m_textureId;

	m_animState = anim_noAnimation;
	animation_idle();

	reader >> m_itemsGiven;
	if (m_itemsGiven)
	{
		m_itemsWithin.clear();
	}
}

void DropItemZombie::updateTask(float deltaTime)
{
	animation_idle();
}

bool DropItemZombie::releaseItems(Level* levelPtr, Game &game)
{

	if (m_itemsGiven == false)
	{
		m_itemsGiven = true;
		sf::Vector2i onScreenPos = game.getWindow().mapCoordsToPixel(getPosition());

		m_dialog.resetDialog();
		ConversationBox::instance().setPosition(sf::Vector2f(onScreenPos.x, onScreenPos.y));
		ConversationBox::instance().setDialog(m_dialog);
		ConversationBox::instance().setShown(true);


		//autoloot works
		sf::Vector2f floorPosition(getPosition().x + getSprite().getGlobalBounds().width / 2, getPosition().y + getSprite().getGlobalBounds().height / 2);

		// Spawn items when chest is opened
		for (std::size_t i = 0; i < m_itemsWithin.size(); i++)
		{


			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(m_itemsWithin[i]));

			// Make sure items in the chest are lootable
			if (!item->isLootable())
				continue;

			item->setPosition(sf::Vector2f(floorPosition.x, floorPosition.y - item->getSprite().getGlobalBounds().height));

			int offset = std::rand() % 150;
			if (std::rand() % 2 == 0)
				offset *= -1;

			item->setPosition(sf::Vector2f(item->getPosition().x + offset, item->getPosition().y));

			game.getLevel().addItemToBuffer(std::move(item));
		}
		m_itemsWithin.clear();
		//release items
	}
	return false;
}