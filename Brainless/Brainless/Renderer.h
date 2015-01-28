#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include <SFML\Graphics.hpp>
#include <vector>


struct RenderData
{
	RenderData(const sf::Drawable *drawableParam, sf::FloatRect boundsParam, float lengthParam);

	const sf::Drawable *drawable;
	sf::FloatRect bounds;
	float lengthToOrigo;
};

class TileMap;
class Renderer
{
public:

	// Allows a shader to be used when rendering
	void plugShader(const sf::Shader &shader);
	void unplugShader();

	// Set the render target of the renderer
	void setTarget(sf::RenderTarget &target);

	// Draw a drawable with depth sorting, bounds of the drawable is required
	void drawDepth(const sf::Drawable &drawable, const sf::FloatRect &bounds);
	void drawDepth(const sf::Sprite &sprite); // Bounds are grabbed from sprite

	// Draw a drawable with depth sorting, but make it's depth value so that it's in front of everything
	void drawAbove(const sf::Drawable &drawable);

	// Draws the drawable in a separate layer using the default camera view, and ontop of everything else
	void drawHUD(const sf::Drawable &drawable);

	// Draw something a separate layer super far back or super far in front, mostly for environment stuff
	void drawBackground(const sf::Drawable &drawable);
	void drawForeground(const sf::Drawable &drawable);

	// Executes all draw calls for this frame
	void executeDraws();

	static Renderer& instance();

private:



	Renderer();

	//typedef std::pair<const sf::Drawable*, float> RenderPair;
	std::vector<RenderData> m_renderTasks;
	std::vector<const sf::Drawable*> m_hudRenderTasks;

	// Foreground and background render tasks are separate from the semi depth
	// buffer since don't require the same 2.5D layering in those layers
	std::vector<const sf::Drawable*> m_foregroundRenderTasks;
	std::vector<const sf::Drawable*> m_backgroundRenderTasks;

	std::vector<std::pair<const sf::Drawable*, std::vector<sf::IntRect> > > m_testTasks;

	const sf::Shader *m_shader;
	sf::RenderTarget *m_renderTarget;
	TileMap *m_tileMap;
};

#endif