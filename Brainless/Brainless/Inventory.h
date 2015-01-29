#pragma once
#include <vector>


class item;

class Inventory
{
public:
	Inventory();
	~Inventory();
private:
	std::vector<item*> m_inventory;
};

