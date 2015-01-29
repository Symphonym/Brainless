#include "Editor.h"
#include "Game.h"

int main(int argc, const char *args[])
{
	// Run editor if "editor" was given as a commandline argument
	if (argc > 1 && std::string(args[1]) == "editor")
	{
		Editor editor;  
		editor.run();
	}

	// Otherwise start the game
	else
	{
		Game game;
		game.run();
	}

	return 0;
}