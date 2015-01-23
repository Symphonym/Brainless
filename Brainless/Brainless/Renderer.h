#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <SFML\Graphics.hpp>
#include <vector>

class Renderer
{
public:

	// Allows a shader to be used when rendering
	void plugShader(const sf::Shader &shader);
	void unplugShader();

	void setTarget(sf::RenderTarget &target);

	// Transformable is used for simple depth testing
	void draw(const sf::Drawable &drawable, const sf::Transformable& transformable);
	// Draws the drawable above everything else, usefull for drawing HUD and interfaces
	void drawAbove(const sf::Drawable &drawable);

	// Executes all draw calls for this frame
	void executeDraws();

	static Renderer& instance();

private:

	Renderer();

	typedef std::pair<const sf::Drawable*, float> RenderPair;
	std::vector<RenderPair> m_renderTasks;

	const sf::Shader *m_shader;
	sf::RenderTarget *m_renderTarget;
};

#endif