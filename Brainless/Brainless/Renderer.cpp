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


void Renderer::drawDepth(const sf::Drawable &drawable, const sf::FloatRect &bounds)
{
	// THE KEY IS HERE, SORT BY CENTER_X + CENTER_Y
	m_renderTasks.push_back(RenderData(&drawable, bounds, bounds.left+bounds.width/2.f + bounds.top+bounds.height/2.f));
}

void Renderer::drawDepth(const sf::Sprite &sprite)
{
	drawDepth(sprite, sprite.getGlobalBounds());
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
	