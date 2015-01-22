#include "Renderer.h"

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
void Renderer::draw(const sf::Drawable &drawable)
{
	if (m_shader != nullptr)
		m_renderTarget->draw(drawable, m_shader);
	else
		m_renderTarget->draw(drawable);
}

Renderer Renderer::instance()
{
	static Renderer renderInstance;
	return renderInstance;
}

	