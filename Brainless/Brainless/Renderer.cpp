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

void Renderer::plugShader(sf::Shader &shader)
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

sf::Shader* Renderer::getCurrentShader()
{
	return m_shader;
}

void Renderer::drawDepth(const sf::Drawable &drawable, const sf::FloatRect &bounds)
{
	// THE KEY IS HERE, SORT BY CENTER_X + CENTER_Y
	m_renderTasks.push_back(std::make_pair(&drawable, bounds.left + bounds.width / 2.f + bounds.top + bounds.height / 2.f));
}

void Renderer::drawDepth(const sf::Sprite &sprite)
{
	drawDepth(sprite, sprite.getGlobalBounds());
}

void Renderer::drawAbove(const sf::Drawable &drawable)
{
	m_aboveRenderTasks.push_back(&drawable);
}
void Renderer::drawBehind(const sf::Drawable &drawable)
{
	m_behindRenderTasks.push_back(&drawable);
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
void Renderer::drawPopUp(const sf::Drawable &drawable)
{
	m_popUpRenderTasks.push_back(&drawable);
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

	// Draw "behind" stuff
	for (std::size_t i = 0; i < m_behindRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_behindRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_behindRenderTasks[i]);
	}

	// Draw depth sorted stuff
	for (std::size_t i = 0; i < m_renderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_renderTasks[i].first, m_shader);
		else
			m_renderTarget->draw(*m_renderTasks[i].first);
	}

	// Draw foreground stuff
	for (std::size_t i = 0; i < m_foregroundRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_foregroundRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_foregroundRenderTasks[i]);
	}

	// Draw "above" stuff
	for (std::size_t i = 0; i < m_aboveRenderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_aboveRenderTasks[i], m_shader);
		else
			m_renderTarget->draw(*m_aboveRenderTasks[i]);
	}

	// Draw "popup" stuff without shader
	for (std::size_t i = 0; i < m_popUpRenderTasks.size(); i++)
	{
			m_renderTarget->draw(*m_popUpRenderTasks[i]);
	}
	


	// Store a temp variable of the current camera so we can revert back to it
	sf::View tempView = m_renderTarget->getView();
	m_renderTarget->setView(m_renderTarget->getDefaultView());

	// Draw hud with default camera
	for (std::size_t i = 0; i < m_hudRenderTasks.size(); i++)
	{
		m_renderTarget->draw(*m_hudRenderTasks[i]);
	}

	m_renderTarget->setView(tempView);

	m_popUpRenderTasks.clear();
	m_aboveRenderTasks.clear();
	m_behindRenderTasks.clear();
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

