#ifndef INCLUDED_EDITOR_ITEM_MODE
#define INCLUDED_EDITOR_ITEM_MODE

class EditorItemMode
{
public:

	EditorSpriteMode(std::vector<LevelSprite>& spriteVector);

	// Returns true if something changed (so editor knows if it needs to resave)
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

};

#endif