#include <SFML\Graphics.hpp>
#include "testClass.h"
#include "Object.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include <iostream>

TestClass::TestClass() : unit(0,0)
{
	ResourceLoader::instance().loadTexture("testImage", "test2.png");
	unit.setTexture(ResourceLoader::instance().retrieveTexture("testImage"));
}
void TestClass::testUpdate()
{
	//crashar i render->draw
	Renderer::instance().draw(unit.getSprite());
	std::cout << "test" << std::endl;
}