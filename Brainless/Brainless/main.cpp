#include <SFML\Graphics.hpp>
#include "ResourceLoader.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Editor.h"
#include "Constants.h"
#include "Animation.h"
#include "TestClass.h"

/*
Lyckades inte hitta hur man återställde en commit, 
testBranch är ok
mainBranch kan inte kompilera, råkade få en merge commit (trodde jag)
Men det felet kan även ha uppstått i en tidigare commit.

Här är felet jag får när jag försöker kompilera:
1>c:\program files (x86)\microsoft visual studio 12.0\vc\include\xmemory0(593): error C2280: 'std::unique_ptr<Item,std::default_delete<_Ty>>::unique_ptr(const std::unique_ptr<_Ty,std::default_delete<_Ty>> &)' : attempting to reference a deleted function
1>          with
1>          [
1>              _Ty=Item
1>          ]
1>          c:\program files (x86)\microsoft visual studio 12.0\vc\include\memory(1486) : see declaration of 'std::unique_ptr<Item,std::default_delete<_Ty>>::unique_ptr'
1>          with
1>          [
1>              _Ty=Item
1>          ]
1>          c:\program files (x86)\microsoft visual studio 12.0\vc\include\xmemory0(592) : while compiling class template member function 'void std::allocator<_Ty>::construct(_Ty *,const _Ty &)'
1>          with
1>          [
1>              _Ty=Level::ItemPtr
1>          ]
1>          c:\program files (x86)\microsoft visual studio 12.0\vc\include\xmemory0(723) : see reference to function template instantiation 'void std::allocator<_Ty>::construct(_Ty *,const _Ty &)' being compiled
1>          with
1>          [
1>              _Ty=Level::ItemPtr
1>          ]
1>          c:\program files (x86)\microsoft visual studio 12.0\vc\include\type_traits(572) : see reference to class template instantiation 'std::allocator<_Ty>' being compiled
1>          with
1>          [
1>              _Ty=Level::ItemPtr
1>          ]
1>          c:\program files (x86)\microsoft visual studio 12.0\vc\include\vector(650) : see reference to class template instantiation 'std::is_empty<_Alloc>' being compiled
1>          with
1>          [
1>              _Alloc=std::allocator<Level::ItemPtr>
1>          ]
1>          c:\users\a14tobme.hs\source\repos\brainlessmain\brainless\brainless\level.h(32) : see reference to class template instantiation 'std::vector<Level::ItemPtr,std::allocator<_Ty>>' being compiled
1>          with
1>          [
1>              _Ty=Level::ItemPtr
1>          ]
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
*/

int main(int argc, const char *args[])
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Brainless");
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);

	//if (argc == 1) // ONLY UNCOMMEN THESE IF's WHEN DEPLOYING PROGRAM, editor can then be accessed by running "game.exe editor"
	//{
	//	if (args[0] == "editor")
	//	{
			Editor editor;  
			editor.run();
	//	}
	//}

	/*	TestClass test;
		test.testUpdate();*/

	/*Renderer::instance().setTarget(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		window.clear(sf::Color::Black);
		window.draw(circle);
		window.display();
	}*/
	

	return 0;
}