#include "Inventory.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Level.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Game.h"
#include "Notification.h"
#include "Player.h"
#include "Button.h"
#include "CraftingDatabase.h"
#include "OptionsMenu.h"

Inventory::Inventory()
:
m_isOpen(false),
m_mouseItemSlot(nullptr),
m_showHighlighText(false),
m_craftingModeEnabled(false)
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

	m_craftButton = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Crafting_Normal"),
		ResourceLoader::instance().retrieveTexture("Crafting_Pressed"),
		sf::Vector2f(10, m_slots[0][0].second.getGlobalBounds().height * Constants::InventoryHeight + 10.f)));
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
		if (event.key.code == OptionsMenu::getKeybind("Inventory"))
		{
			m_isOpen = !m_isOpen;
			SoundPlayer::instance().playSound("inventory_open",game.getWindow().getView().getCenter());

			if (!m_isOpen)
				setCraftingMode(false);
		}
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Items can not be moved around in crafting mode
		if (event.mouseButton.button == sf::Mouse::Left && !m_craftingModeEnabled)
		{
			InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

			if (invPair)
			{
				// A place action was done with an item already selected
				if (m_mouseItem)
				{
					// There's an item in the slot that the player pressed, switch slots
					if (invPair->first)
					{
						m_mouseItemSlot->first = std::move(invPair->first);
						invPair->first = std::move(m_mouseItem);
					}

					// No item in the slot, then just place the item in that slot
					else
						invPair->first = std::move(m_mouseItem);
				}

				// Select new item, the item might be null (empty slot) but doesn't matter since we use null to determine if an item is selected
				else
				{
					m_mouseItemSlot = invPair;
					m_mouseItem = std::move(invPair->first);
				}
			}

			// The user did not click on the inventory, try with world interaction
			else if (m_mouseItem)
			{
				if (!m_isOpen)
				{
					sf::Vector2f playerCenter = sf::Vector2f(
						game.getPlayer().getCollisionRect().left + game.getPlayer().getCollisionRect().width / 2.f,
						game.getPlayer().getCollisionRect().top + game.getPlayer().getCollisionRect().height / 2.f);


					// If the item interacted with an item this frame, don't let it interact with any units as well
					// Since this could mean that an item is used twice in a single frame
					bool interactedWithItem = false;
					bool itemTooFarAway = false;

					// Try item interaction
					for (std::size_t i = 0; i < game.getLevel().getItems().size(); i++)
					{
						if (!m_mouseItem)
							break;

						Item& item = *game.getLevel().getItems()[i].get();

						sf::Vector2f itemCenter = sf::Vector2f(
							item.getInteractBounds().left + item.getInteractBounds().width / 2.f,
							item.getInteractBounds().top + item.getInteractBounds().height / 2.f);

						sf::Vector2f distVec = itemCenter - playerCenter;
						distVec.x = std::abs(distVec.x);
						distVec.y = std::abs(distVec.y);

						// Item too far away
						if (distVec.x > item.getInteractDistance().x || distVec.y > item.getInteractDistance().y)
						{
							itemTooFarAway = true;
							continue;
						}

						if (m_mouseItem->getInteractBounds().intersects(item.getInteractBounds()))
						{
							bool deleteOtherItem = false;
							bool deleteMouseItem = false;

							// Invoke interaction handling on mouse item
							if (m_mouseItem->onInteract(item, game))
								deleteMouseItem = true;

							// Invoke interaction on world item
							if (item.onInteractedWith(*m_mouseItem.get(), game))
								deleteOtherItem = true;

							if (deleteOtherItem)
								game.getLevel().removeItem(i);

							if (deleteMouseItem)
								delete m_mouseItem.release();

							interactedWithItem = true;
							break;
						}
					}

					bool interactedWithUnit = false;
					bool unitTooFarAway = false;

					// Try unit interaction
					for (std::size_t i = 0; i < game.getLevel().getUnits().size(); i++)
					{
						if (interactedWithItem)
							break;

						Unit& unit = *game.getLevel().getUnits()[i].get();

						sf::Vector2f unitCenter = sf::Vector2f(
							unit.getCollisionRect().left + unit.getCollisionRect().width / 2.f,
							unit.getCollisionRect().top + unit.getCollisionRect().height / 2.f);

						sf::Vector2f distVec = unitCenter - playerCenter;
						distVec.x = std::abs(distVec.x);
						distVec.y = std::abs(distVec.y);

						if (distVec.x > m_mouseItem->getInteractDistance().x || distVec.y > m_mouseItem->getInteractDistance().y)
						{
							itemTooFarAway = true;
							continue;
						}

						if (m_mouseItem->getInteractBounds().intersects(unit.getCollisionRect()))
						{
							bool deleteMouseItem = false;
							bool deleteUnit = false;

							// Invoke interaction handling on item
							if (m_mouseItem->onInteractUnit(unit))
								deleteMouseItem = true;

							// Invoke interaction on unit
							if (unit.onInteractedWith(*m_mouseItem.get(), game))
								deleteUnit = true;

							if (deleteMouseItem)
								delete m_mouseItem.release();
							if (deleteUnit)
								game.getLevel().removeUnit(i);

							interactedWithUnit = true;
							break;
						}
					}


					if (!interactedWithItem && !interactedWithUnit)
					{
						if (unitTooFarAway || itemTooFarAway)
							Notification::instance().write("It's too far away");
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
		// Selecting crafting components
		else if (event.mouseButton.button == sf::Mouse::Left && m_craftingModeEnabled)
		{
			for (std::size_t x = 0; x < m_slots.size(); x++)
			{
				for (std::size_t y = 0; y < m_slots[x].size(); y++)
				{
					InventoryPair& invPair = m_slots[x][y];

					// If mouse is on top of an occupied inventory slot
					if (invPair.first && invPair.second.getGlobalBounds().contains(mousePos.x, mousePos.y))
						m_selectedSlots.push_back(sf::Vector2i(x, y));
				}
			}

			highlightSelected();
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

			m_mouseItem->setPosition(
				sf::Vector2f(mousePos.x - m_mouseItem->getSprite().getGlobalBounds().width / 2.f, mousePos.y - m_mouseItem->getSprite().getGlobalBounds().height / 2.f));
		}
			

		// Place selected item in WORLD space
		else
		{
			sf::Vector2f worldPos = game.getWindow().mapPixelToCoords(mousePos);
			worldPos.x -= m_mouseItem->getSprite().getGlobalBounds().width / 2.f;
			worldPos.y -= m_mouseItem->getSprite().getGlobalBounds().height / 2.f;

			m_mouseItem->setPosition(worldPos);
		}
	}
	else
	{
		InventoryPair* invPair = getSlotAt(sf::Vector2f(mousePos.x, mousePos.y));

		// Show highlight text for slots you hover over
		if (invPair && invPair->first)
		{
			m_showHighlighText = true;
			m_highlightText.setString(invPair->first->getName());
			m_highlightText.setPosition(
				invPair->first->getPosition().x + invPair->first->getSprite().getGlobalBounds().width/2.f - m_highlightText.getGlobalBounds().width/2.f,
				invPair->first->getPosition().y + 10);

			if (m_highlightText.getPosition().x < 0)
					m_highlightText.setPosition(0, m_highlightText.getPosition().y);

			m_highlightBGSprite.setScale(m_highlightText.getGlobalBounds().width + 6.f, m_highlightText.getGlobalBounds().height + 6.f);
			m_highlightBGSprite.setPosition(m_highlightText.getPosition().x - 3.f, m_highlightText.getPosition().y + 10.f);
		}
		else
			m_showHighlighText = false;

		// Check for crafting mode button input
		if (m_craftButton->getReleased(game.getWindow()))
		{
			if (m_craftingModeEnabled && !m_selectedSlots.empty())
				craft();

			// Doesn't work atm if (m_selectedSlots.empty() || !m_craftingModeEnabled)
			setCraftingMode(!m_craftingModeEnabled);

		}
	}
}

void Inventory::draw()
{
	// Draw inventory
	if (m_isOpen)
	{
		//Renderer::instance().drawHUD(m_background);
		for (std::size_t x = 0; x < m_slots.size(); x++)
		{
			for (std::size_t y = 0; y < m_slots[x].size(); y++)
			{
				sf::Sprite& slotSprite = m_slots[x][y].second;

				// Draw inventory slot
				Renderer::instance().drawHUD(slotSprite);

				if (m_slots[x][y].first)
				{
					sf::Sprite &itemSprite = m_slots[x][y].first->getInventorySprite();
					m_slots[x][y].first->setPosition(sf::Vector2f(
						slotSprite.getPosition().x + slotSprite.getGlobalBounds().width / 2.f - itemSprite.getGlobalBounds().width / 2.f,
						slotSprite.getPosition().y + slotSprite.getGlobalBounds().height / 2.f - itemSprite.getGlobalBounds().height / 2.f));

					// Draw item
					Renderer::instance().drawHUD(itemSprite);
				}
			}
		}

		if (m_showHighlighText)
		{
			Renderer::instance().drawHUD(m_highlightBGSprite);
			Renderer::instance().drawHUD(m_highlightText);
		}

		m_craftButton->draw();
	}

	// Draw selected item on hud or in-game depending on if inventory is open or not
	if (m_mouseItem)
	{
		if (m_isOpen)
		{
			m_mouseItem->inventoryDraw();
			m_mouseItem->heldDraw();
		}
		else
			m_mouseItem->inventoryDraw(true);
	}
	

}

void Inventory::emptyInventory()
{
	for (auto &rows : m_slots)
	{
		for (auto &slot : rows)
			delete slot.first.release();
	}
}

void Inventory::toggleVisible()
{
	m_isOpen = !m_isOpen;
}

bool Inventory::holdingItem() const
{
	if (m_mouseItem)
		return true;
	else
		return false;
}

void Inventory::recolorSlots(const sf::Color &color)
{
	for (std::size_t x = 0; x < m_slots.size(); x++)
	{
		for (std::size_t y = 0; y < m_slots[x].size(); y++)
		{
			InventoryPair& invPair = m_slots[x][y];
			invPair.second.setColor(color);
		}
	}
}
void Inventory::highlightSelected()
{
	for (auto &index : m_selectedSlots)
		m_slots[index.x][index.y].second.setColor(sf::Color::Yellow);
}

void Inventory::setCraftingMode(bool enabled)
{
	m_craftingModeEnabled = enabled;

	if (m_craftingModeEnabled)
	{
		m_selectedSlots.clear();
		recolorSlots(sf::Color::Green);
	}

	else
	{
		recolorSlots(sf::Color::White);
	}
}
void Inventory::craft()
{
	std::vector<int> idVector;

	for (auto &index : m_selectedSlots)
	{
		if (m_slots[index.x][index.y].first)
			idVector.push_back(m_slots[index.x][index.y].first->getID());
	}

	CraftingDatabase::ItemPtr item = CraftingDatabase::instance().craftItem(idVector);

	// An item was crafted
	if (item)
	{
		// Remove all ingredients from inventory
		for (auto &index : m_selectedSlots)
			delete m_slots[index.x][index.y].first.release();

		Notification::instance().write("\"" + item->getName() + "\" was crafted successfully!");

		// Add item to inventory
		addItem(std::move(item));
	}
	else
	{
		Notification::instance().write("That doesn't seem to work");
	}


	m_selectedSlots.clear();
	recolorSlots(sf::Color::Green);
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