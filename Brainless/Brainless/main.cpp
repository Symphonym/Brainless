#include "Editor.h"
#include "MainMenu.h"
#include "ResourceLoader.h"
#include "StateMachine.h"
#include "StartProgramIntro.h"
#include "OptionsMenu.h"

int main(int argc, const char *args[])
{
	// Seed random generator
	std::srand(std::time(NULL));

	// Load resources that are used pretty much everywhere
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_Main.txt");

	// Run editor if "editor" was given as a commandline argument
	if (argc > 1 && std::string(args[1]) == "editor")
	{
		Editor editor;  
		editor.run();
	}

	// Otherwise start the game
	else
	{
		OptionsMenu::initializeKeybinds();

		StateMachine machine;
		machine.pushState<StartProgramIntro>();
		machine.run();
	}

	return 0;
}