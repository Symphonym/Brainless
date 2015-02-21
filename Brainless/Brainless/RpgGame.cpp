#include "RpgGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "ParticleSystem.h"

RpgGame::RpgGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "Turtlerain Survival"),
m_score(0),
m_hunger(m_hungerMax),
m_spawnDelayCur(0)
{
	for (std::size_t x = 0; x < m_tiles.size(); x++)
	{
		for (std::size_t y = 0; y < m_tiles[x].size(); y++)
		{
			Tile &tile = m_tiles[x][y];
			tile.sprite.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeGrassTile"));
			tile.sprite.setPosition(
				m_machine.getScreenPos().x + 5.f + x*tile.sprite.getGlobalBounds().width,
				m_machine.getScreenPos().y + 5.f + y*tile.sprite.getGlobalBounds().height);
			tile.unit = nullptr;
		}
	}

	// Initialize hunger bar
	sf::Image barImg;
	barImg.create(1, 30, sf::Color::White);
	m_hungerBarTexture.loadFromImage(barImg);

	m_hungerBar.setTexture(m_hungerBarTexture);
	m_hungerBar.setPosition(
		m_machine.getScreenPos().x + 200.f,
		m_machine.getScreenPos().y + m_machine.getScreenSize().y - 35.f);

	// Initialize score text
	m_scoreText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setString("Score: 0");
	m_scoreText.setPosition(m_machine.getScreenPos().x + 10.f, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 35.f);
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

	m_hunger = m_hungerMax;
	m_spawnDelayCur = 0;
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
	m_hungerBar.setScale(m_hunger, 1);

	float hungerPercent = static_cast<float>(m_hunger) / static_cast<float>(m_hungerMax);
	sf::Color hungerColor = sf::Color(
		255 + (0 - 255) * hungerPercent,
		0 + (255 - 0) * hungerPercent,
		0);
	m_hungerBar.setColor(hungerColor);

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

	Renderer::instance().drawHUD(m_scoreText);
	Renderer::instance().drawHUD(m_hungerBar);

	ParticleSystem::instance().draw(true);
}


void RpgGame::spawnPlayer()
{
	UnitPtr unit = UnitPtr(new TileUnit());
	unit->sprite.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeRpgPlayer"));
	unit->sprite.setOrigin(
		unit->sprite.getGlobalBounds().width / 2.f,
		unit->sprite.getGlobalBounds().height / 2.f);
	unit->x = MapWidth/2;
	unit->y = MapHeight-1;

	placeUnitOnTile(unit.get(), unit->x, unit->y);
	m_player = unit.get();

	m_units.push_back(std::move(unit));

}
void RpgGame::spawnEnemy(const std::string &textureName)
{
	UnitPtr unit = UnitPtr(new TileUnit());
	unit->sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));
	unit->sprite.setOrigin(
		unit->sprite.getGlobalBounds().width / 2.f,
		unit->sprite.getGlobalBounds().height / 2.f);
	unit->x = m_player->x;
	unit->y = 0;
	unit->sprite.setRotation(180); // Looking down by default
	placeUnitOnTile(unit.get(), unit->x, unit->y);

	m_units.push_back(std::move(unit));
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
		m_hunger -= 10;
		tickGame();
	}
	else if (canAttack(x, y))
	{
		removeUnit(m_tiles[x][y].unit, sf::Color::Red);
		placeUnitOnTile(m_player, x, y);

		// Killing enemies feeds your hunger bar
		m_hunger += 50;
		if (m_hunger >= m_hungerMax)
			m_hunger = m_hungerMax;

		tickGame();
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
		m_machine.getScreenPos().x + 5.f + x*tile.sprite.getGlobalBounds().width + tile.sprite.getGlobalBounds().width / 2.f,
		m_machine.getScreenPos().y + 5.f + y*tile.sprite.getGlobalBounds().height + +tile.sprite.getGlobalBounds().height / 2.f);
}


void RpgGame::removeUnit(TileUnit *unit, const sf::Color &particleColor)
{
	ParticleSystem::instance().addParticles(50,
		sf::Vector2f(
			unit->sprite.getPosition().x,
			unit->sprite.getPosition().y), 
			particleColor);

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

void RpgGame::tickGame()
{
	// Update units
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		TileUnit *unit = m_units[i].get();

		// Do not control the player
		if (unit == m_player)
			continue;

		if (canMove(unit->x, unit->y+1))
			placeUnitOnTile(unit, unit->x, unit->y + 1);
		else if (m_player->x == unit->x && m_player->y == unit->y + 1)
		{
			removeUnit(m_player, sf::Color::Red);
			placeUnitOnTile(unit, unit->x, unit->y + 1);
			m_player = nullptr;
			break; // Killing player ends game, and thus enemy input
		}
	}

	

	if (m_player != nullptr)
	{
		++m_spawnDelayCur;

		if (m_spawnDelayCur >= m_spawnDelayMax)
		{
			spawnEnemy("ArcadeRpgEnemy");
			m_spawnDelayCur = 0;
		}

		// Check a row consists entirely of enemies
		for (std::size_t y = 0; y < m_tiles[0].size(); y++)
		{
			bool isUnit = true;
			for (std::size_t x = 0; x < m_tiles.size(); x++)
			{
				Tile &tile = m_tiles[x][y];

				// Check if an enemy isn't in a tile on the row
				if (tile.unit == nullptr || tile.unit == m_player)
				{
					isUnit = false;
					break;
				}
			}

			if (isUnit)
			{
				for (std::size_t x = 0; x < m_tiles.size(); x++)
					removeUnit(m_tiles[x][y].unit, sf::Color::Cyan);

				++m_score;
				m_scoreText.setString("Score: " + std::to_string(m_score));
			}
		}

		// Check if player died from hunger
		if (m_hunger <= 0)
		{
			m_hunger = 0;
			removeUnit(m_player, sf::Color::Yellow);
		}
	}
}