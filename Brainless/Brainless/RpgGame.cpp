#include "RpgGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "ParticleSystem.h"

RpgGame::RpgGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "World of Åkecraft")
{
	for (std::size_t x = 0; x < m_tiles.size(); x++)
	{
		for (std::size_t y = 0; y < m_tiles[x].size(); y++)
		{
			Tile &tile = m_tiles[x][y];
			tile.sprite.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeGrassTile"));
			tile.sprite.setPosition(
				m_machine.getScreenPos().x + 10.f + x*tile.sprite.getGlobalBounds().width,
				m_machine.getScreenPos().y + 10.f + y*tile.sprite.getGlobalBounds().height);
			tile.unit = nullptr;
		}
	}
}

void RpgGame::onGameStart()
{
	m_units.clear();
	for (std::size_t x = 0; x < m_tiles.size(); x++)
	{
		for (std::size_t y = 0; y < m_tiles[x].size(); y++)
		{
			m_tiles[x][y].unit = nullptr;
		}
	}

	createWave();
	spawnPlayer();
}

void RpgGame::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased && m_player)
	{
		if (event.key.code == sf::Keyboard::W)
		{
			playerInputToTile(m_player->x, m_player->y - 1);
		}

		else if (event.key.code == sf::Keyboard::S)
		{
			playerInputToTile(m_player->x, m_player->y + 1);
		}

		else if (event.key.code == sf::Keyboard::D)
		{
			playerInputToTile(m_player->x + 1, m_player->y);
		}

		else if (event.key.code == sf::Keyboard::A)
		{
			playerInputToTile(m_player->x - 1, m_player->y);
		}
	}
}
void RpgGame::update(float deltaTime)
{
	ParticleSystem::instance().update(deltaTime);
}
void RpgGame::draw()
{
	for (std::size_t x = 0; x < m_tiles.size(); x++)
	{
		for (std::size_t y = 0; y < m_tiles[x].size(); y++)
		{
			Renderer::instance().drawHUD(m_tiles[x][y].sprite);
		}
	}

	for (auto &unit : m_units)
		Renderer::instance().drawHUD(unit->sprite);

	ParticleSystem::instance().draw(true);
}


void RpgGame::spawnPlayer()
{
	TileUnit &player = spawnUnit("ArcadeRpgPlayer");
	m_player = &player;
}
void RpgGame::createWave()
{
	for (int i = 0; i < 10; i++)
	{
		spawnUnit("ArcadeRpgEnemy");
	}
}

bool RpgGame::canMove(int x, int y)
{
	return x >= 0 && x < MapWidth && y >= 0 && y < MapHeight && m_tiles[x][y].unit == nullptr;
}
bool RpgGame::canAttack(int x, int y)
{
	return x >= 0 && x < MapWidth && y >= 0 && y < MapHeight && m_tiles[x][y].unit != nullptr && m_tiles[x][y].unit != m_player;
}
void RpgGame::playerInputToTile(int x, int y)
{
	if (canMove(x, y))
	{
		placeUnitOnTile(m_player, x, y);
		tickEnemies();
	}
	else if (canAttack(x, y))
	{
		removeUnit(m_tiles[x][y].unit);
		tickEnemies();
	}
}
void RpgGame::placeUnitOnTile(RpgGame::TileUnit *unit, int x, int y)
{
	Tile &tile = m_tiles[x][y];
	tile.unit = unit;

	if (unit->x >= 0 && unit->x < MapWidth && unit->y >= 0 && unit->y < MapHeight)
		m_tiles[unit->x][unit->y].unit = nullptr; // Clear unit of old tile, if it exists

	// Moved to the left
	if (unit->x - 1 == x)
		unit->sprite.setRotation(270);

	// Move to the right
	else if (unit->x + 1 == x)
		unit->sprite.setRotation(90);

	// Move up
	else if (unit->y - 1 == y)
		unit->sprite.setRotation(0);

	// Move down
	else if (unit->y + 1 == y)
		unit->sprite.setRotation(180);

	unit->x = x;
	unit->y = y;
	unit->sprite.setPosition(
		m_machine.getScreenPos().x + 10.f + x*tile.sprite.getGlobalBounds().width + tile.sprite.getGlobalBounds().width / 2.f - unit->sprite.getGlobalBounds().width / 2.f,
		m_machine.getScreenPos().y + 10.f + y*tile.sprite.getGlobalBounds().height + +tile.sprite.getGlobalBounds().height / 2.f - unit->sprite.getGlobalBounds().height / 2.f);
}
RpgGame::TileUnit& RpgGame::spawnUnit(const std::string &textureName)
{
	UnitPtr unit = UnitPtr(new TileUnit());
	unit->sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));
	unit->x = -1;
	unit->y = -1;

	while (true)
	{
		int x = std::rand() % MapWidth;
		int y = std::rand() % MapHeight;

		Tile &tile = m_tiles[x][y];
		if (tile.unit == nullptr)
		{
			tile.unit = unit.get();
			placeUnitOnTile(unit.get(), x, y);
			break;
		}
	}

	TileUnit &tmpUnit = *unit.get();
	m_units.push_back(std::move(unit));
	return tmpUnit;
}
std::vector<sf::Vector2i> RpgGame::getAdjacentIndices(int x, int y) const
{
	std::vector<sf::Vector2i> result;

	// Tile to the left
	if (x - 1 >= 0)
		result.push_back(sf::Vector2i(x - 1, y));

	// Tile to the right
	if (x + 1 < MapWidth)
		result.push_back(sf::Vector2i(x + 1, y));

	// Tile above
	if (y - 1 >= 0)
		result.push_back(sf::Vector2i(x, y - 1));

	// Tile below
	if (y + 1 < MapHeight)
		result.push_back(sf::Vector2i(x, y + 1));

	return result;
}
void RpgGame::removeUnit(TileUnit *unit)
{
	ParticleSystem::instance().addParticles(50,
		sf::Vector2f(
		unit->sprite.getPosition().x + unit->sprite.getGlobalBounds().width / 2.f,
		unit->sprite.getPosition().y + unit->sprite.getGlobalBounds().height / 2.f), sf::Color::Red);

	m_tiles[unit->x][unit->y].unit = nullptr;
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		if (m_units[i].get() == unit)
		{
			m_units.erase(m_units.begin() + i);
			break;
		}
	}
}

void RpgGame::tickEnemies()
{
	bool killPlayer = false;
	for (auto &unit : m_units)
	{
		// Do not control the player
		if (unit.get() == m_player)
			continue;

		std::vector<sf::Vector2i> adjacents = getAdjacentIndices(unit->x, unit->y);

		bool hasAttacked = false;
		for (auto &adjacentIndex : adjacents)
		{
			Tile &tile = m_tiles[adjacentIndex.x][adjacentIndex.y];

			if (tile.unit != nullptr && tile.unit == m_player)
			{
				killPlayer = true;
				hasAttacked = true;
				break;
			}
		}

		if (!hasAttacked)
		{
			int moveDirection = std::rand() % 4;

			// Move to the left
			if (moveDirection == 0 && canMove(unit->x - 1, unit->y))
				placeUnitOnTile(unit.get(), unit->x - 1, unit->y);

			// Move to the right
			if (moveDirection == 1 && canMove(unit->x + 1, unit->y))
				placeUnitOnTile(unit.get(), unit->x + 1, unit->y);

			// Move up
			if (moveDirection == 2 && canMove(unit->x, unit->y - 1))
				placeUnitOnTile(unit.get(), unit->x, unit->y - 1);

			// Move down
			if (moveDirection == 3 && canMove(unit->x, unit->y + 1))
				placeUnitOnTile(unit.get(), unit->x, unit->y + 1);
		}
	}

	if (killPlayer)
	{
		removeUnit(m_player);
		m_player = nullptr;
	}
}