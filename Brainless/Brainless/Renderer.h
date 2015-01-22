#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <SFML\Graphics.hpp>

class Renderer
{
public:

	// Allows a shader to be used when rendering
	void plugShader(const sf::Shader &shader);
	void unplugShader();

	void setTarget(sf::RenderTarget &target);
	void draw(const sf::Drawable &drawable);

	static Renderer instance();

private:

	Renderer();

	const sf::Shader *m_shader;
	sf::RenderTarget *m_renderTarget;
};

#endif