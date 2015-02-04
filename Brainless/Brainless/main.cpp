#include "Editor.h"
#include "Game.h"
#include "ResourceLoader.h"

int main(int argc, const char *args[])
{
	// Seed random generator
	std::srand(std::time(NULL));

	// Load resources that are used pretty much everywhere
	ResourceLoader::instance().loadFromFile("loadfiles/ResourceLoad_Main.txt");

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