#include "Inventory.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Level.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Game.h"
#include "Notification.h"
#include "Player.h"

Inventory::Inventory()
:
m_isOpen(false),
m_showHighlighText(false)
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

	m_highlightText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));

	sf::Image bgImg;
	bgImg.create(1, 1, sf::Color::White);

	m_highlightBackground.loadFromImage(bgImg);
	m_highlightBGSprite.setTexture(m_highlightBackground);
	m_highlightBGSprite.setColor(sf::Color::Color(0, 0, 0, 128));
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

void Inventory::events(const sf::Event &event, Game &game)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());

	// Toggle inventory
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::I)
		{
			m_isOpen = !m_isOpen;
			SoundPlayer::instance().playSound("inventory_open",game.getWindow().getView().getCenter());
		}
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

						bool endCombining = false;

						// Go through combinations of selected item
						for (std::size_t i = 0; i < m_mouseItem->getCombinations().size(); i++)
						{
							// Go through combinations of inventory item
							const CombineData &mouseCombine = m_mouseItem->getCombinations()[i];
							for (std::size_t j = 0; j < invPair->first->getCombinations().size(); j++)
							{
								const CombineData &invCombine = invPair->first->getCombinations()[j];

								// The items have combinations for eachother
								if (mouseCombine.targetID == invPair->first->getID() &&
									invCombine.targetID == m_mouseItem->getID() && // They have eachother as required item for crafting
									mouseCombine.productItemID == invCombine.productItemID) // They have the same product item
								{
									int newItemID = mouseCombine.productItemID;

									// Check if the items are consumed when used in this combination
									if (mouseCombine.consumedOnCraft)
										delete m_mouseItem.release();
									if (invCombine.consumedOnCraft)
										delete invPair->first.release();

									// Add new item to inventory
									addItem(std::move(ItemDatabase::instance().extractItem(newItemID)));

									endCombining = true;
									break;
								}

							}

							if (endCombining)
								break;
						}

						// If mouse item wasen't consumed by the combination, put it back into inventory
						if (m_mouseItem)
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
				sf::Vector2f playerCenter = sf::Vector2f(
					game.getPlayer().getCollisionRect().left + game.getPlayer().getCollisionRect().width / 2.f,
					game.getPlayer().getCollisionRect().top + game.getPlayer().getCollisionRect().height / 2.f);


				// If the item interacted with a unit this frame, don't let it interact with any items as well
				// Since this could mean that an item is used twice in a single frame
				bool interactedWithUnit = false;

				// Try unit interaction
				for (std::size_t i = 0; i < game.getLevel().getUnits().size(); i++)
				{
					Unit& unit = *game.getLevel().getUnits()[i].get();

					sf::Vector2f unitCenter = sf::Vector2f(
						unit.getCollisionRect().left + unit.getCollisionRect().width / 2.f,
						unit.getCollisionRect().top + unit.getCollisionRect().height / 2.f);

					sf::Vector2f distVec = unitCenter - playerCenter;
					distVec.x = std::abs(distVec.x);
					distVec.y = std::abs(distVec.y);

					if (distVec.x > m_mouseItem->getInteractDistance().x || distVec.y > m_mouseItem->getInteractDistance().y)
						continue;

					if (m_mouseItem->getSprite().getGlobalBounds().intersects(unit.getCollisionRect()))
					{
						// Invoke interaction handling on item
						if(m_mouseItem->onInteractUnit(unit))
							delete m_mouseItem.release();

						// Invoke interaction on unit
						if (unit.onInteractedWith(*m_mouseItem.get()))
							game.getLevel().removeUnit(i);

						interactedWithUnit = true;
						break;
					}
				}

				// Try item interaction
				for (std::size_t i = 0; i < game.getLevel().getItems().size(); i++)
				{
					if (!m_mouseItem || interactedWithUnit)
						break;

					Item& item = *game.getLevel().getItems()[i].get();

					sf::Vector2f itemCenter = sf::Vector2f(
						item.getSprite().getGlobalBounds().left + item.getSprite().getGlobalBounds().width / 2.f,
						item.getSprite().getGlobalBounds().top + item.getSprite().getGlobalBounds().height / 2.f);

					sf::Vector2f distVec = itemCenter - playerCenter;
					distVec.x = std::abs(distVec.x);
					distVec.y = std::abs(distVec.y);

					if (distVec.x > item.getInteractDistance().x || distVec.y > item.getInteractDistance().y)
						continue;

					if (m_mouseItem->getSprite().getGlobalBounds().intersects(item.getSprite().getGlobalBounds()))
					{
						// Invoke interaction on world item
						if (item.onInteractedWith(*m_mouseItem.get(), game))
							game.getLevel().removeItem(i);

						// Invoke interaction handling on mouse item
						if (m_mouseItem->onInteract(item, game))
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
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

			if (invPair && invPair->first)
				Notification::instance().write(invPair->first->getExamineString());
		}
	}
}
void Inventory::update(float deltaTime, Game &game)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(game.getWindow());

	if (m_mouseItem)
	{
		// Don't show item name when you're moving items around
		m_showHighlighText = false;

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
	else
	{
		InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

		if (invPair && invPair->first)
		{
			m_showHighlighText = true;
			m_highlightText.setString(invPair->first->getName());
			m_highlightText.setPosition(
				invPair->first->getPosition().x + invPair->first->getSprite().getGlobalBounds().width/2.f - m_highlightText.getGlobalBounds().width/2.f,
				invPair->first->getPosition().y + 10.f);

			if (m_highlightText.getPosition().x < 0)
					m_highlightText.setPosition(0, m_highlightText.getPosition().y);

			m_highlightBGSprite.setScale(m_highlightText.getGlobalBounds().width + 6.f, m_highlightText.getGlobalBounds().height + 6.f);
			m_highlightBGSprite.setPosition(m_highlightText.getPosition().x - 3.f, m_highlightText.getPosition().y + 3.f);
		}
		else
			m_showHighlighText = false;
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
	
	if (m_showHighlighText)
	{
		Renderer::instance().drawHUD(m_highlightBGSprite);
		Renderer::instance().drawHUD(m_highlightText);
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