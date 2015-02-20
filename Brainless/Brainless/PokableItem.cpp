#include "PokableItem.h"
#include "Notification.h"
#include "Constants.h"

PokableItem::PokableItem(const std::string &itemName, const std::string &inventoryTextureName, const std::string &textureName, int id)
:
Item(itemName,inventoryTextureName, textureName, id)
{

}

bool PokableItem::onInteractedWith(Item &otherItem, Game &game)
{
	sf::Vector2f distanceToItem =
		sf::Vector2f(
			otherItem.getPosition().x + otherItem.getSprite().getGlobalBounds().width / 2.f,
			otherItem.getPosition().y + otherItem.getSprite().getGlobalBounds().height / 2.f) -
		sf::Vector2f(
			getPosition().x + getSprite().getGlobalBounds().width / 2.f,
			getPosition().y + getSprite().getGlobalBounds().height / 2.f);

	if (otherItem.getName() == "Long Stick")
	{
		// Within reach of long stick
		if (distanceToItem.x < otherItem.getInteractDistance().x + Constants::StickInteractAddition &&
			distanceToItem.y < otherItem.getInteractDistance().y + Constants::StickInteractAddition)
		{
			if (otherItem.isUsable())
			{
				otherItem.onUse(game);
				onPoked(game);
			}
			else
				Notification::instance().write("The stick isn't going to work for that");
		}
		else
			Notification::instance().write("It's too far away, even for this stick");
			
	}

	else if (otherItem.getName() == "Stick")
	{
		if (distanceToItem.x < otherItem.getInteractDistance().x &&
			distanceToItem.y < otherItem.getInteractDistance().y)
		{
			onPoked(game);
		}
		else
			Notification::instance().write("It's too far away, I need to get closer if I want to poke it");
	}

	return false;
}
