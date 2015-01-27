#include "Renderer.h"
#include <cmath>

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




void Renderer::draw(const sf::Drawable &drawable, const sf::Transformable &transformable)
{
	const sf::Vector2f& pos = transformable.getPosition();
	float distanceToZero = std::sqrt(pos.x*pos.x + pos.y*pos.y);

	m_renderTasks.push_back(std::make_pair(&drawable, distanceToZero));
}

void Renderer::draw(const sf::Sprite &sprite)
{
	const sf::Drawable* drawable = ((sf::Drawable*) &sprite);
	const sf::Vector2f& pos = ((sf::Transformable) sprite).getPosition();
	float distanceToZero = std::sqrt(pos.x*pos.x + pos.y*pos.y);

	m_renderTasks.push_back(std::make_pair(drawable, distanceToZero));
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

	