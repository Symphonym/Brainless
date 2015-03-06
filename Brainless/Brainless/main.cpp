#include "Editor.h"
#include "MainMenu.h"
#include "ResourceLoader.h"
#include "StateMachine.h"
#include "StartProgramIntro.h"
#include "OptionsMenu.h"

// https://docs.google.com/document/d/1nog2e27vsrZsYKw-mIb_lyVyjMO-0aa4DxjMhRYTRGw/edit?pli=1#
/*
Game intro flavour text





The change was slow…

Peoples’ minds just shut off and no one knew what to do…

Society slowed down, stuttered, and stopped…

No one was safe, and no one cared...

A few had secluded themselves in hope of avoiding the devastating change…








Game starts, Maeve monologue






...don’t leave here soon I’ll become one of them, one of those brainless… things. My

motivation is already waning, I need to stop this while I still can. I can’t become like them.

Don’t really know where to start, where to go. Guess I don’t really have a choice either

way, either chasing a rumour of a cure or wandering aimlessly. If I gave up now, mom

and dad would never forgive me. I could use this place as a base, I’ve been hunkered

down here for far too long, it’s time to get out there and explore what’s become of this

place. Let’s see how much further our society has decayed, shall we?

*/
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