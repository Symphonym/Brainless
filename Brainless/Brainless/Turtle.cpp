#include "Turtle.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"
#include "OptionsMenu.h"
//plz no hate on uggly code, much lazy, very kappa

stuff::stuff(stuffType _type, int _x, int _y)
:type(_type),x(_x),y(_y),oldX(_x),oldY(_y),sprite(),animation()
{
	
	if (type == turtle)
	{
		sprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
		animation.setWidth(50);
		animation.setHeight(50);
		sprite.setOrigin(25, 25);
		animation.loop(0, 1, 0, 1.5f);
	}
	else
	{
		sprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));
		animation.setWidth(40);
		animation.setHeight(40);
		sprite.setOrigin(20, 20);
		animation.loop(0, 1, 0, 1.5f);
	}

}

Turtle::Turtle(ArcadeMachine &machine)
:
ArcadeGame(machine, "Turtle Conga"),
screenPos(m_machine.getScreenPos()),
score(0)
{
	screenPos += sf::Vector2f(20, 20);
	background.setTexture(ResourceLoader::instance().retrieveTexture("TurtleBackground"));

	background.setPosition(m_machine.getScreenPos());

	scoreText = sf::Text("Score: " + score, ResourceLoader::instance().retrieveFont("DefaultFont"));

	infoText = sf::Text("Collect the Curly Fries", ResourceLoader::instance().retrieveFont("DefaultFont"));
	infoText.setPosition(screenPos.x + 140, screenPos.y + 325);
	infoText.setColor(sf::Color(0, 120, 0, 255));

	scoreText.setColor(sf::Color(0, 120, 0, 255));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = nullptr;
		}
	}
}

Turtle::~Turtle()
{
	//std::cout << "körsinte" << std::endl;
}

void Turtle::onGameStart()
{
	scoreText.setPosition(screenPos.x + 20, screenPos.y + 630);
	score = 0;
	isDead = false;
	infoShowing = true;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
			{
				delete(map[i][j]); //kallas inte ifall man stänger av arcademachine helt och startar igen, liten memory leak
			}
		}
	}

	direction = up;

	head = new stuff(turtle,5,9);
	head->before = nullptr;
	head->next = nullptr;
	map[5][9] = head;
	map[5][2] = new stuff(frie,5,2);
}

void Turtle::update(float deltaTime)
{

	time += deltaTime;

	if (!isDead)
	{
		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Up")))
		{
			direction = up;
		}
		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Down")))
		{
			direction = down;
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left")))
		{
			direction = left;
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right")))
		{
			direction = right;
		}

		if (time > 0.1f)
		{
			time-=0.1f;
			int x = 0;
			int y = 0;
			if (direction == up)
			{
				y = -1;
				head->sprite.setRotation(0);
			}
			else if (direction == down)
			{
				y = 1;
				head->sprite.setRotation(180);
			}
			else if (direction == left)
			{
				x = -1;
				head->sprite.setRotation(270);
			}
			else if (direction == right)
			{
				x = 1;
				head->sprite.setRotation(90);
			}
			int newPosX = head->x + x;
			int newPosY = head->y + y;
			if (newPosX < 0 || newPosX > 9 || newPosY < 0 || newPosY > 9)
			{
				SoundPlayer::instance().playSound("ArcadeFail", screenPos, 20);
				isDead = true;
			}
			else if (map[newPosX][newPosY] == nullptr)
			{
				stuff* ptr = head;
				while (ptr->next != nullptr)
				{
					ptr = ptr->next;
				}
				if(ptr->before != nullptr)ptr->before->next = nullptr;
				map[ptr->x][ptr->y] = nullptr;
				ptr->x = newPosX;
				ptr->y = newPosY;
				map[newPosX][newPosY] = ptr;
				if (ptr != head)
				{
					ptr->before = nullptr;
					ptr->next = head;
					head->before = ptr;
					head = ptr;
				}
			}
			else if (map[head->x + x][head->y + y]->type == frie)
			{
				score++;
				if (score != 99)
				{


					bool movedFrie = false;

					int rndPosX;
					int rndPosY;
					while (!movedFrie) //fulaste trial and error search
					{
						rndPosX = Utility::randomValueBetween(0, 9);
						rndPosY = Utility::randomValueBetween(0, 9);
						if (map[rndPosX][rndPosY] == nullptr) movedFrie = true;
					}
					map[rndPosX][rndPosY] = map[newPosX][newPosY];

					stuff* ptr = new stuff(turtle, newPosX, newPosY);
					map[newPosX][newPosY] = ptr;
					ptr->next = head;
					ptr->before = nullptr;
					head->before = ptr;
					head = ptr;
				}
				else isDead = true;
				SoundPlayer::instance().playSound("ArcadeLight", screenPos, 20);
			}
			else if (map[head->x + x][head->y + y]->type == turtle)
			{
				isDead = true;
				SoundPlayer::instance().playSound("ArcadeFail", screenPos, 20);
			}

			if (direction == up)
			{
				head->sprite.setRotation(0);
			}
			else if (direction == down)
			{
				head->sprite.setRotation(180);
			}
			else if (direction == left)
			{
				head->sprite.setRotation(270);
			}
			else if (direction == right)
			{
				head->sprite.setRotation(90);
			}
		}
		
		scoreText.setString("Score: " + std::to_string(score));
	}
	
	else
	{
		scoreText.setPosition(screenPos.x + 180, screenPos.y + 320);
		scoreText.setString("Final Score: " + std::to_string(score) + "\n\nPress ESC to exit");
		infoShowing = false;
	}
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
				map[i][j]->sprite.setTextureRect(map[i][j]->animation.getRectangle(deltaTime));
		}
	}
}

void Turtle::draw()
{
	Renderer::instance().drawHUD(background);
	if (infoShowing)
		Renderer::instance().drawHUD(infoText);

	

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else if (map[i][j]->type == frie)
			{
				map[i][j]->sprite.setPosition(screenPos + sf::Vector2f(i * 70 + 10, j * 70 + 15) );
				Renderer::instance().drawHUD(map[i][j]->sprite);
			}
			else if (map[i][j]->type == turtle)
			{
				map[i][j]->sprite.setPosition(screenPos + sf::Vector2f(i * 70 + 5, j * 70+10));
				Renderer::instance().drawHUD(map[i][j]->sprite);
			}
		}
	}

	Renderer::instance().drawHUD(scoreText);
}