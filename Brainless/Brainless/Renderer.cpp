#include "Renderer.h"
#include <cmath>
#include "TileMap.h"
#include "Constants.h"
#include "ResourceLoader.h"

Renderer::Renderer()
:
m_shader(nullptr),
m_renderTarget(nullptr)
{

}

void Renderer::plugShader(const sf::Shader &shader)
{
	m_shader = &shader;
}
void Renderer::unplugShader()
{
	m_shader = nullptr;
}

void Renderer::setTarget(sf::RenderTarget &target)
{
	m_renderTarget = &target;
}


void Renderer::setTileMap(TileMap &tileMap)
{
	m_tileMap = &tileMap;
}
void Renderer::drawTMSpecial(const sf::Drawable &drawable, const sf::FloatRect &bounds)
{
	sf::Vector2i startIndex = m_tileMap->positionToIndex(sf::Vector2f(bounds.left, bounds.top));
	//sf::Vector2i endIndex = m_tileMap->positionToIndex(sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height));

	std::vector<sf::IntRect> boundVec;

	// TODO check if <= on TIleMap rendering/culling
	for (int x = startIndex.x; x < startIndex.x+1; x++)
	{
		for (int y = startIndex.y; y <= startIndex.y + 1; y++)
		{
			Tile& tile = m_tileMap->getTile(x, y);

			

			sf::FloatRect bounds = tile.getBounds();
			//bounds.left += -Constants::LeftTileOffset*2;
			//bounds.top += -Constants::TopTileOffset*2;

			sf::Vector2i screenBounds = m_renderTarget->mapCoordsToPixel(sf::Vector2f(bounds.left, bounds.top));

			if (tile.getType() == Tile::Nothing)
				boundVec.push_back(sf::IntRect(0, 0, 0, 0));
			else
				boundVec.push_back(sf::IntRect(screenBounds.x, screenBounds.y, bounds.width, bounds.height));
		}
	}

	sf::Shader &shader = ResourceLoader::instance().retrieveShader("TestShader");
	//if (boundVec.empty())
	//{
		m_testTasks.push_back(std::make_pair(&drawable, boundVec));
		//shader.setParameter("anything", 1);
	//}
	//else
	//{
	//	m_testTasks.push_back(std::make_pair(&drawable, boundVec[0]));
		//shader.setParameter("startPoint", boundVec[0].left, boundVec[0].top);
		//shader.setParameter("endPoint", boundVec[0].left + boundVec[0].width, boundVec[0].top + boundVec[0].height);
	//}

	//plugShader(shader);
	//m_renderTarget->draw(drawable, &shader);
	//unplugShader();

}



void Renderer::draw(const sf::Drawable &drawable, const sf::Transformable &transformable)
{
	const sf::Vector2f& pos = transformable.getPosition();
	float distanceToZero = std::sqrt(pos.x*pos.x + pos.y*pos.y);

	m_renderTasks.push_back(std::make_pair(&drawable, distanceToZero));
}

void Renderer::draw(const sf::Sprite &sprite)
{
	draw(sprite, sprite);
}

void Renderer::drawAbove(const sf::Drawable &drawable)
{
	m_renderTasks.push_back(std::make_pair(&drawable, -1));
}

void Renderer::drawHUD(const sf::Drawable &drawable)
{
	m_hudRenderTasks.push_back(&drawable);
}

void Renderer::drawBackground(const sf::Drawable &drawable)
{
	m_backgroundRenderTasks.push_back(&drawable);
}
void Renderer::drawForeground(const sf::Drawable &drawable)
{
	m_foregroundRenderTasks.push_back(&drawable);
}

void Renderer::executeDraws()
{
	// Sort draw calls based on distance to 0,0
	std::sort(m_renderTasks.begin(), m_renderTasks.end(), [](const RenderPair& a, const RenderPair &b) {
		return b.second < a.second;
	});

	// Draw background stuff
	for (std::size_t i = 0; i < m_backgroundRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_backgroundRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_backgroundRenderTasks[i]);
	}

	// Execute all draw calls order of distance
	for (std::size_t i = 0; i < m_renderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_renderTasks[i].first, m_shader);
		else
			m_renderTarget->draw(*m_renderTasks[i].first);
	}

	sf::View tempView = m_renderTarget->getView(); 
	m_renderTarget->setView(m_renderTarget->getDefaultView());

	// Draw hud with default camera
	for (std::size_t i = 0; i < m_hudRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_hudRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_hudRenderTasks[i]);
	}

	m_renderTarget->setView(tempView);

	// Draw foreground stuff
	for (std::size_t i = 0; i < m_foregroundRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_foregroundRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_foregroundRenderTasks[i]);
	}

	/*for (std::size_t i = 0; i < m_testTasks.size(); i++)
	{
		sf::Shader &shader = ResourceLoader::instance().retrieveShader("TestShader");
			//shader.setParameter("anything", 1);

			for (std::size_t c = 0; c < m_testTasks[i].second.size(); c++)
			{
				shader.setParameter("startPoint"+std::to_string(c + 1), 
					m_testTasks[i].second[c].left, m_testTasks[i].second[c].top);
				shader.setParameter("endPoint" + std::to_string(i + 1),
					m_testTasks[i].second[c].left + m_testTasks[i].second[c].width, m_testTasks[i].second[c].top + m_testTasks[i].second[c].height);
			}


		//plugShader(shader);
		//m_renderTarget->draw(drawable, &shader);
		//unplugShader();

		//if (m_shader != nullptr)
		//	m_renderTarget->draw(*m_testTasks[i].first, m_shader);
		//else
			m_renderTarget->draw(*m_testTasks[i].first, &shader);
	}

	m_testTasks.clear();
	*/
	m_foregroundRenderTasks.clear();
	m_backgroundRenderTasks.clear();
	m_hudRenderTasks.clear();
	m_renderTasks.clear();
}

Renderer& Renderer::instance()
{
	static Renderer renderInstance;
	return renderInstance;
}

	