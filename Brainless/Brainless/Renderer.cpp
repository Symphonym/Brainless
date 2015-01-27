#include "Renderer.h"
#include <cmath>
#include "TileMap.h"
#include "Constants.h"
#include "ResourceLoader.h"
#include "Utility.h"

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

	//sf::Shader &shader = ResourceLoader::instance().retrieveShader("TestShader");
	//if (boundVec.empty())
	//{
		//m_testTasks.push_back(std::make_pair(&drawable, boundVec));
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
#include <iostream>
void Renderer::drawByBounds(const sf::Drawable &drawable, const sf::FloatRect &bounds)
{
	const sf::Vector2f pos = sf::Vector2f(
		bounds.left + bounds.width / 2.f,
		bounds.top + bounds.height / 2.f);
	const sf::Vector2f pos1 = sf::Vector2f(
		bounds.left + bounds.width,
		bounds.top);
	const sf::Vector2f pos2 = sf::Vector2f(
		bounds.left + bounds.width,
		bounds.top + bounds.height);
	const sf::Vector2f pos3 = sf::Vector2f(
		bounds.left,
		bounds.top + bounds.height);
	const sf::Vector2f pos4 = sf::Vector2f(
		bounds.left,
		bounds.top);

	const sf::Vector2f centerTop = sf::Vector2f(
		bounds.left + bounds.width/2.f,
		0);

	//sf::Vector2f vec = centerTop - 
	sf::Vector2f angleVec = sf::Vector2f(bounds.left, bounds.top) - sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
	float angleLength = std::sqrt(angleVec.x*angleVec.x + angleVec.y*angleVec.y);
	angleVec.x /= angleLength;
	angleVec.y /= angleLength;

	float specialLength = 0;
	sf::Vector2f startPos(bounds.left + bounds.width/2.f, bounds.top+bounds.height/2.f);
	for (int i = 0;; i+= 5)
	{
		sf::Vector2f result = startPos + sf::Vector2f(angleVec.x*i, angleVec.y*i);

		if (result.x < 0 || result.y < 0)
		{
			sf::Vector2f vec(angleVec.x*i, angleVec.y*i);
			specialLength += std::sqrt(vec.x*vec.x + vec.y*vec.y);
			break;
		}
	}
	/*startPos = sf::Vector2f(bounds.left, bounds.top);
	for (int i = 0;; i += 5)
	{
		sf::Vector2f result = startPos + sf::Vector2f(angleVec.x*i, angleVec.y*i);

		if (result.x < 0 || result.y < 0)
		{
			sf::Vector2f vec(angleVec.x*i, angleVec.y*i);
			specialLength += std::sqrt(vec.x*vec.x + vec.y*vec.y);
			break;
		}
	}
	startPos = sf::Vector2f(bounds.left+bounds.width, bounds.top);
	for (int i = 0;; i += 5)
	{
		sf::Vector2f result = startPos + sf::Vector2f(angleVec.x*i, angleVec.y*i);

		if (result.x < 0 || result.y < 0)
		{
			sf::Vector2f vec(angleVec.x*i, angleVec.y*i);
			specialLength += std::sqrt(vec.x*vec.x + vec.y*vec.y);
			break;
		}
	}
	startPos = sf::Vector2f(bounds.left, bounds.top+bounds.height);
	for (int i = 0;; i += 5)
	{
		sf::Vector2f result = startPos + sf::Vector2f(angleVec.x*i, angleVec.y*i);

		if (result.x < 0 || result.y < 0)
		{
			sf::Vector2f vec(angleVec.x*i, angleVec.y*i);
			specialLength += std::sqrt(vec.x*vec.x + vec.y*vec.y);
			break;
		}
	}
	startPos = sf::Vector2f(bounds.left+bounds.width, bounds.top + bounds.height);
	for (int i = 0;; i += 5)
	{
		sf::Vector2f result = startPos + sf::Vector2f(angleVec.x*i, angleVec.y*i);

		if (result.x < 0 || result.y < 0)
		{
			sf::Vector2f vec(angleVec.x*i, angleVec.y*i);
			specialLength += std::sqrt(vec.x*vec.x + vec.y*vec.y);
			break;
		}
	}*/
	//float multiplier = 1.f / angleVec.x;
	//angleVec.x *= multiplier; // X is now 1
	//angleVec.y *= multiplier; // Y is now the height difference per X

	// This gives us a line in the form of y = xa where a is angleVec.y
	float tilt = angleVec.y;

	//std::cout << "TILT " << tilt << std::endl;
	//float length = std::sqrt(angleVec.x*angleVec.x + angleVec.y*angleVec.y);
	//angleVec.x /= length;
	//angleVec.y /= length;

	//sf::Vector2f target = -pos4;
	//target.x /= angleVec.x;
	//target.y /= angleVec.x;

	//std::cout << "A1 " << target.x << " A2 " << target.y << std::endl;
	//std::cout << "Result A1 " << 

	float distanceToZero = std::sqrt(pos.x*pos.x + pos.y*pos.y);

	float distanceToZero1 = std::sqrt(pos1.x*pos1.x + pos1.y*pos1.y);
	float distanceToZero2 = std::sqrt(pos2.x*pos2.x + pos2.y*pos2.y);
	float distanceToZero3 = std::sqrt(pos3.x*pos3.x + pos3.y*pos3.y);
	float distanceToZero4 = std::sqrt(pos4.x*pos4.x + pos4.y*pos4.y);

	float total = distanceToZero1 + distanceToZero2 + distanceToZero3 + distanceToZero + distanceToZero4;
	float newTotal = 0;
	const int divides = 10;
	float widthDivideDistance = bounds.width / static_cast<float>(divides);
	float heightDivideDistance = bounds.height / static_cast<float>(divides);
	for (int i = 0; i <= divides; i++)
	{
		sf::Vector2f vec1(bounds.left + i*widthDivideDistance, bounds.top);
		float distanceToZer1 = std::sqrt(vec1.x*vec1.x + vec1.y*vec1.y);
		newTotal += distanceToZer1;

		sf::Vector2f vec2(bounds.left + i*widthDivideDistance, bounds.top + bounds.height);
		float distanceToZer2 = std::sqrt(vec2.x*vec2.x + vec2.y*vec2.y);
		newTotal += distanceToZer2;

		sf::Vector2f vec3(bounds.left + i*widthDivideDistance, bounds.top + bounds.height/2.f);
		float distanceToZer3 = std::sqrt(vec3.x*vec3.x + vec3.y*vec3.y);
		newTotal += distanceToZer3;
	}
	for (int i = 0; i <= divides; i++)
	{
		sf::Vector2f vec1(bounds.left, bounds.top + i*heightDivideDistance);
		float distanceToZer1 = std::sqrt(vec1.x*vec1.x + vec1.y*vec1.y);
		newTotal += distanceToZer1;

		sf::Vector2f vec2(bounds.left + bounds.width, bounds.top + i*heightDivideDistance);
		float distanceToZer2 = std::sqrt(vec2.x*vec2.x + vec2.y*vec2.y);
		newTotal += distanceToZer2;

		sf::Vector2f vec3(bounds.left + bounds.width/2.f, bounds.top + i*heightDivideDistance);
		float distanceToZer3 = std::sqrt(vec3.x*vec3.x + vec3.y*vec3.y);
		newTotal += distanceToZer3;
	}
	// total + bounds.top*10.f
	float theLength = specialLength > distanceToZero ? specialLength : distanceToZero;

	// THE KEY IS HERE, SORT BY CENTER_X + CENTER_Y
	m_renderTasks.push_back(RenderData(&drawable, bounds, bounds.left+bounds.width/2.f + bounds.top+bounds.height/2.f));
}



void Renderer::draw(const sf::Drawable &drawable, const sf::Transformable &transformable)
{
	const sf::Vector2f& pos = transformable.getPosition();
	float distanceToZero = std::sqrt(pos.x*pos.x + pos.y*pos.y);

	m_renderTasks.push_back(RenderData(&drawable, sf::FloatRect(0, 0, 0, 0), distanceToZero));
}

void Renderer::draw(const sf::Sprite &sprite)
{
	draw(sprite, sprite);
}

void Renderer::drawAbove(const sf::Drawable &drawable)
{
	m_renderTasks.push_back(RenderData(&drawable, sf::FloatRect(0, 0, 0, 0), -1));
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
	std::sort(m_renderTasks.begin(), m_renderTasks.end(), [](const RenderData& a, const RenderData &b) {
		return b.lengthToOrigo < a.lengthToOrigo;
	});

	

	/*const int offset = 5;
	for (std::size_t i = 0; i < m_renderTasks.size(); i++)
	{
		int startIndex = i - offset;
		int endIndex = i + offset;

		startIndex = Utility::clampValue<int>(startIndex, 0, m_renderTasks.size() - 1);
		endIndex = Utility::clampValue<int>(endIndex, 0, m_renderTasks.size() - 1);

		float myLeft = m_renderTasks[i].bounds.left;// +m_renderTasks[i].bounds.width / 2.f;
		float myTop = m_renderTasks[i].bounds.top;// +m_renderTasks[i].bounds.height / 2.f;
		float myBottom = m_renderTasks[i].bounds.top + m_renderTasks[i].bounds.height;
		float myRight = m_renderTasks[i].bounds.left + m_renderTasks[i].bounds.width;

		for (int c = startIndex; c <= endIndex; c++)
		{
			float leftSide = m_renderTasks[c].bounds.left + Constants::LeftTileOffset*2;
			float topSide = m_renderTasks[c].bounds.top + Constants::TopTileOffset*2;
			float rightSide = m_renderTasks[c].bounds.left + m_renderTasks[c].bounds.width - Constants::LeftTileOffset * 2;
			float bottomSide = m_renderTasks[c].bounds.top + m_renderTasks[c].bounds.height - Constants::TopTileOffset * 2;

			// This object is currently being drawn on TOP of 'me'
			if (c > i)
			{
				//if (myOrigoX > rightSide || myOrigoY > bottomSide)
				if (myLeft > rightSide && myTop > bottomSide && myBottom > bottomSide && myRight > rightSide)
				{
					std::iter_swap(m_renderTasks.begin() + i, m_renderTasks.begin() + c);
					//RenderData tmpMyData = m_renderTasks[i];
					//m_renderTasks[i] = m_renderTasks[c];
					//m_renderTasks[c] = tmpMyData;
				}

			}

			// This object is currently being drawn BELOW 'me'
			else if (c < i)
			{
				if ((myLeft < leftSide && myTop < topSide))
				{
					//std::iter_swap(m_renderTasks.begin() + i, m_renderTasks.begin() + c);
				}
			}
		}
	}*/

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
			m_renderTarget->draw(*m_renderTasks[i].drawable, m_shader);
		else
			m_renderTarget->draw(*m_renderTasks[i].drawable);
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






RenderData::RenderData(const sf::Drawable *drawableParam, sf::FloatRect boundsParam, float lengthParam)
:
drawable(drawableParam),
bounds(boundsParam),
lengthToOrigo(lengthParam)
{

}
	