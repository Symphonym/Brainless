#include "ThorsHammerItem.h"
#include "Notification.h"

ThorsHammerItem::ThorsHammerItem(int id)
:
Item("Mjölner","MjolnirInv", "ThorsHammerItem", id)
{
	m_lootable = true;
	m_examineString = "The hammer of the god Thor, and now it's in my hands. The world better prepare.";
	m_useString = "The hammer of Thor is simply not used, it is worn.";
}

bool ThorsHammerItem::onInteract(Item &otherItem, Game &game)
{
	Notification::instance().write("The might of Mjölner is not be used on lowlife items such as " + otherItem.getName());
	return false;
}

Item* ThorsHammerItem::clone()
{
	return new ThorsHammerItem(*this);
}