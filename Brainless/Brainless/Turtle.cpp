#include "Turtle.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"
#include "OptionsMenu.h"
#include <iostream>
//plz no hate on uggly code, much lazy, very kappa

Object::Object(stuffType _type, int _x, int _y)
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

Directions::Directions(direction _dirr, Directions* _next) : dirr(_dirr), next(_next){}

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
	
	direction = new Directions(up, nullptr);
	direction_tail = direction;
}

Turtle::~Turtle()
{
	Directions* del;
	while (direction != nullptr)
	{
		del = direction;
		direction = direction->next;
		delete(del);
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
			{
				delete(map[i][j]); 
			}
		}
	}
}

void Turtle::onGameStart()
{
	scoreText.setPosition(screenPos.x + 20, screenPos.y + 630);
	score = 0;
	time = 0;
	isDead = false;
	infoShowing = true;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
			{
				delete(map[i][j]); 
				map[i][j] = nullptr;
			}
		}
	}

	while (direction->next != nullptr)
	{
		Directions* del;
		del = direction;
		direction = direction->next;
		delete(del);
	}
	direction->dirr = up;

	head = new Object(turtle, 5, 9);
	head->before = nullptr;
	head->next = nullptr;
	tail = head;
	map[5][9] = head;
	map[5][2] = new Object(frie, 5, 2);

}


void Turtle::update(float deltaTime)
{

	time += deltaTime;

	if (!isDead)
	{
		/* super ai for debug */
		/*if (head->x == 9 && head->y == 0)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 2)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 4)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 6)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 8)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		if (head->x == 1 && head->y == 1)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 3)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 5)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 7)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		else if (head->x == 0 && head->y == 9)
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(up, nullptr);
				direction_tail = direction_tail->next;
			}
		}


		else if (head->x == 9 && head->y == 1)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 3)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 5)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 7)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 9 && head->y == 9)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		else if (head->x == 0 && head->y == 0)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 2)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 4)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 6)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (head->x == 1 && head->y == 8)
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}*/

		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Up")))
		{
			if (direction_tail->dirr != down  && direction_tail->dirr != up)
			{
				direction_tail->next = new Directions(up, nullptr);
				direction_tail = direction_tail->next;
			}
		}
		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Down")))
		{
			if (direction_tail->dirr != up && direction_tail->dirr != down)
			{
				direction_tail->next = new Directions(down, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left")))
		{
			if (direction_tail->dirr != right  && direction_tail->dirr != left)
			{
				direction_tail->next = new Directions(left, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right")))
		{
			if (direction_tail->dirr != left && direction_tail->dirr != right)
			{
				direction_tail->next = new Directions(right, nullptr);
				direction_tail = direction_tail->next;
			}
		}

		if (time > 0.1f)
		{
			if (direction->next != nullptr)
			{
				Directions* del;
				del = direction;
				direction = direction->next;
				delete(del);
			}

			time -= 0.1f;
			int x = 0;
			int y = 0;
			if (direction->dirr == up)
			{
				y = -1;
				head->sprite.setRotation(0);
			}
			else if (direction->dirr == down)
			{
				y = 1;
				head->sprite.setRotation(180);
			}
			else if (direction->dirr == left)
			{
				x = -1;
				head->sprite.setRotation(270);
			}
			else if (direction->dirr == right)
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
				Object* ptr = tail;
				if (ptr->before != nullptr)ptr->before->next = nullptr;
				map[ptr->x][ptr->y] = nullptr;
				ptr->x = newPosX;
				ptr->y = newPosY;
				map[newPosX][newPosY] = ptr;
				if (ptr != head)
				{
					tail = ptr->before;
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
						rndPosX = (int) Utility::randomValueBetween(0, 9);
						rndPosY = (int) Utility::randomValueBetween(0, 9);
						if (map[rndPosX][rndPosY] == nullptr) movedFrie = true;
					}
					map[rndPosX][rndPosY] = map[newPosX][newPosY];

					Object* ptr = new Object(turtle, newPosX, newPosY);
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

			if (direction->dirr == up)
			{
				head->sprite.setRotation(0);
			}
			else if (direction->dirr == down)
			{
				head->sprite.setRotation(180);
			}
			else if (direction->dirr == left)
			{
				head->sprite.setRotation(270);
			}
			else if (direction->dirr == right)
			{
				head->sprite.setRotation(90);
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
			scoreText.setString("Score: " + std::to_string(score));
		}
	}
	else
	{
		scoreText.setPosition(screenPos.x + 180, screenPos.y + 320);
		scoreText.setString("Final Score: " + std::to_string(score) + "\n\nPress ESC to exit");
		infoShowing = false;
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