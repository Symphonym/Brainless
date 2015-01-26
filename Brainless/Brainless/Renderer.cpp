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
void Renderer::executeDraws()
{
	// Sort draw calls based on distance to 0,0
	std::sort(m_renderTasks.begin(), m_renderTasks.end(), [](const RenderPair& a, const RenderPair &b) {
		return b.second < a.second;
	});

	// Execute all draw calls order of distance
	for (std::size_t i = 0; i < m_renderTasks.size(); i++)
	{
		if (m_shader != nullptr)
			m_renderTarget->draw(*m_renderTasks[i].first, m_shader);
		else
			m_renderTarget->draw(*m_renderTasks[i].first);
	}

	m_renderTasks.clear();
}

Renderer& Renderer::instance()
{
	static Renderer renderInstance;
	return renderInstance;
}

	