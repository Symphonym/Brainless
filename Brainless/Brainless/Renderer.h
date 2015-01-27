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

	void setTarget(sf::RenderTarget &target);

	void setTileMap(TileMap &tileMap);
	void drawTMSpecial(const sf::Drawable &drawable, const sf::FloatRect &bounds);
	void drawByBounds(const sf::Drawable &drawable, const sf::FloatRect &bounds);

	// Transformable is used for simple depth testing
	void draw(const sf::Drawable &drawable, const sf::Transformable& transformable);
	void draw(const sf::Sprite &sprite);

	// Draws the drawable above everything else
	void drawAbove(const sf::Drawable &drawable);

	// Draws the drawable using the default camera view, and ontop of everything else
	void drawHUD(const sf::Drawable &drawable);

	// Draw something super far back or super far in front, mostly for environment stuff
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