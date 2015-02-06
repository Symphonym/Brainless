#include "Inventory.h"
#include "Renderer.h"
#include "ItemDatabse.h"
#include "Level.h"
#include "ResourceLoader.h"
#include "Game.h"

Inventory::Inventory()
:
m_isOpen(false)
{
	for (std::size_t x = 0; x < m_slots.size(); x++)
	{
		for (std::size_t y = 0; y < m_slots[x].size(); y++)
		{
			sf::Sprite& slotSprite = m_slots[x][y].second;
			slotSprite.setTexture(ResourceLoader::instance().retrieveTexture("InventorySlot"));
			slotSprite.setPosition(x*slotSprite.getGlobalBounds().width, y*slotSprite.getGlobalBounds().height);
		}
	}
}

void Inventory::addItem(ItemPtr item)
{
	for (std::size_t x = 0; x < m_slots.size(); x++)
	{
		for (std::size_t y = 0; y < m_slots[x].size(); y++)
		{
			if (!m_slots[x][y].first)
			{
				m_slots[x][y].first = std::move(item);
				if (m_slots[x][y].first)
					int b = 3;
				return;
			}
		}
	}
}

void Inventory::events(const sf::Event &event, const sf::RenderWindow &gameWindow, Level &level)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);

	// Toggle inventory
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::I)
			m_isOpen = !m_isOpen;
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

			if (invPair)
			{
				// A place action was done with an item already selected
				if (m_mouseItem)
				{
					// There's an item in the slot that the player pressed, initiate a combine action
					if (invPair->first)
					{
						// Matching (and valid) target and product ID, then the action can be done
						if (m_mouseItem->getCombineData().targetID >= 0 && invPair->first->getCombineData().targetID >= 0 &&
							m_mouseItem->getCombineData().targetID == invPair->first->getID() && // ID of selected item matches the targetID of inventory item
							invPair->first->getCombineData().targetID == m_mouseItem->getID() && //ID of inventory item matches the targetID of selected item
							m_mouseItem->getCombineData().productItemID == invPair->first->getCombineData().productItemID) // Just make sure they have the same product
						{
							int newItemID = m_mouseItem->getCombineData().productItemID;

							delete m_mouseItem.release();
							delete invPair->first.release();

							addItem(std::move(ItemDatabase::instance().extractItem(newItemID)));
						}
						// Otherwise put item back into inventory
						else
							addItem(std::move(m_mouseItem));
					}

					// No item in the slot, then just place the item in that slot
					else
						invPair->first = std::move(m_mouseItem);
				}

				// Select new item, the item might be null (empty slot) but doesn't matter since we use null to determine if an item is selected
				else
					m_mouseItem = std::move(invPair->first);
			}

			// The user did not click on the inventory, try with world interaction
			else if (m_mouseItem)
			{
				

				// If the item interacted with a unit this frame, don't let it interact with any items as well
				// Since this could mean that an item is used twice in a single frame
				bool interactedWithUnit = false;

				// Try unit interaction
				for (std::size_t i = 0; i < level.getUnits().size(); i++)
				{
					Unit& unit = *level.getUnits()[i].get();
					/*if (m_mouseItem.getSprite().getGlobalBounds().intersects(unit.getCollisionRect()))
					{
						if(m_mouseItem->onInteract(unit))
							delete m_mouseItem.release();
						unit.onInteractedWith(*m_mouseItem.get());

						interactedWithUnit = true;
					}*/
				}

				// Try item interaction
				for (std::size_t i = 0; i < level.getItems().size(); i++)
				{
					if (!m_mouseItem || interactedWithUnit)
						break;

					Item& item = *level.getItems()[i].get();
					if (m_mouseItem->getSprite().getGlobalBounds().intersects(item.getSprite().getGlobalBounds()))
					{
						// Invoke interaction on world item
						if (item.onInteractedWith(*m_mouseItem.get()))
							level.removeItem(i);

						// Invoke interaction handling on mouse item
						if (m_mouseItem->onInteract(item))
							delete m_mouseItem.release();

						break;
					}
				}


				// First time click on world will just close the inventory
				if (m_isOpen)
					m_isOpen = false;

				// Second time click on world, without the item being destroyed from interaction, will put it back into the inventory
				else if (m_mouseItem && !m_isOpen)
					addItem(std::move(m_mouseItem));

				// Loop through level units
				// if placement failed, put back into inventory
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

			if (invPair)
			{
				// TODO examine action
			}
		}
	}
}
void Inventory::update(float deltaTime, Game &game)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());

	if (m_mouseItem)
	{
		// Place selected item in HUD space
		if (m_isOpen)
		{
			// Held item gets special update calls in HUD space
			m_mouseItem->heldUpdate(deltaTime, game);

			m_mouseItem->getSprite().setPosition(
				sf::Vector2f(mousePos.x - m_mouseItem->getSprite().getGlobalBounds().width / 2.f, mousePos.y - m_mouseItem->getSprite().getGlobalBounds().height / 2.f));
		}
			

		// Place selected item in WORLD space
		else
		{
			sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(mousePos);
			worldPos.x -= m_mouseItem->getSprite().getGlobalBounds().width / 2.f;
			worldPos.y -= m_mouseItem->getSprite().getGlobalBounds().height / 2.f;

			m_mouseItem->getSprite().setPosition(worldPos);
		}
	}
}

void Inventory::draw()
{
	// Draw inventory
	if (m_isOpen)
	{
		for (std::size_t x = 0; x < m_slots.size(); x++)
		{
			for (std::size_t y = 0; y < m_slots[x].size(); y++)
			{
				sf::Sprite& slotSprite = m_slots[x][y].second;

				// Draw inventory slot
				Renderer::instance().drawHUD(slotSprite);

				if (m_slots[x][y].first)
				{
					sf::Sprite &itemSprite = m_slots[x][y].first->getSprite();
					itemSprite.setPosition(
						slotSprite.getPosition().x + slotSprite.getGlobalBounds().width / 2.f - itemSprite.getGlobalBounds().width / 2.f,
						slotSprite.getPosition().y + slotSprite.getGlobalBounds().height / 2.f - itemSprite.getGlobalBounds().height / 2.f);

					// Draw item
					Renderer::instance().drawHUD(itemSprite);
				}
			}
		}
	}

	// Draw selected item on hud or in-game depending on if inventory is open or not
	if (m_mouseItem)
	{
		if (m_isOpen)
		{
			Renderer::instance().drawHUD(m_mouseItem->getSprite());
			m_mouseItem->heldDraw();
		}
		else
			Renderer::instance().drawAbove(m_mouseItem->getSprite());
	}
}


bool Inventory::holdingItem() const
{
	if (m_mouseItem)
		return true;
	else
		return false;
}

std::vector<const Item*> Inventory::getInventoryItems() const
{
	std::vector<const Item*> items;
	for (std::size_t x = 0; x < m_slots.size(); x++)
	{
		for (std::size_t y = 0; y < m_slots[x].size(); y++)
		{
			const InventoryPair& invPair = m_slots[x][y];
			if (invPair.first)
				items.push_back(invPair.first.get());
		}
	}

	if (m_mouseItem)
		items.push_back(m_mouseItem.get());

	return items;
}

Inventory::InventoryPair* Inventory::getSlotAt(const sf::Vector2f &position)
{
	// No slot available if inventory is closed
	if (!m_isOpen)
		return nullptr;

	for (std::size_t x = 0; x < m_slots.size(); x++)
	{
		for (std::size_t y = 0; y < m_slots[x].size(); y++)
		{
			InventoryPair& invPair = m_slots[x][y];

			// If mouse is on top of an inventory slot
			if (invPair.second.getGlobalBounds().contains(position.x, position.y))
				return &invPair;
		}
	}
	return nullptr;
}