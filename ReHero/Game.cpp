#include "stdafx.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "Card.h"
#include "Deck.h"
#include "SpriteObject.h"

using namespace std;

Game* Game::instance = nullptr;


Game * Game::getInstance()
{
	if (instance == nullptr) {
		instance = new Game();
	}
	return instance;
}

GLRenderer * Game::getRenderer()
{
	return this->renderer;
}

void Game::handleMouseUp(int x, int y)
{
	float realX, realY;
	realX = (gameWidth / -2.0f) + x * (gameWidth / winWidth);
	realY = (gameHeight / -2.0f) + (winHeight - y) * (gameHeight / winHeight);
	isMouseDown = false;
	if (realY < 0.0f)
	{
		if (clickObject)
		{
			clickObject->setIsInHand(true);
			clickObject->setPosition(glm::vec3(0, -240, 0));
			cout << "Reset";
		}
		
	}
	else
	{
		//do something
		if (clickObject)
		{
			if(clickObject->getId()==1)
				clickObject->effect();
			if(clickObject->active==true)
				clickObject->active = false;
		}
		
	}
	clickObject = NULL;
} 

void Game::handleMouseDown(int x, int y)
{
	float realX, realY;
	realX = (gameWidth / -2.0f) + x * (gameWidth / winWidth);
	realY = (gameHeight / -2.0f) + (winHeight - y) * (gameHeight / winHeight);

	//cout << "X : " << realX << endl << "Y : " << realY << endl;
	for (DrawableObject* obj : objects) {
		Card* gameObject = dynamic_cast<Card*>(obj);
		if (gameObject)
		{
			if (gameObject->isClick(realX, realY))
			{
				cout << "Click : " << gameObject->getName() << endl;
				isMouseDown = true;
				clickObject = gameObject;
				break;
			}
		}
	}
}

void Game::handleMouseMotion(int x, int y)
{
	if(isMouseDown)
	{
		float realX, realY;
		realX = (gameWidth / -2.0f) + x * (gameWidth / winWidth);
		realY = (gameHeight / -2.0f) + (winHeight - y) * (gameHeight / winHeight);
		cout << realX << " " << realY << endl;

		if (clickObject)
		{
			clickObject->setIsInHand(false);
			clickObject->setPosition(glm::vec3(realX, realY, 0));
			//clickObject->setPositionDrag(realX, realY);
		}
	}
	
}

void Game::handleKey(char ch)
{
	/*if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(0);
		switch (ch) {
		case 'u': obj->translate(glm::vec3(0, 0.3, 0)); break;
		case 'd': obj->translate(glm::vec3(0, -0.3, 0)); break;
		case 'l': obj->translate(glm::vec3(-0.3, 0, 0)); break;
		case 'r': obj->translate(glm::vec3(0.3, 0, 0)); break;
		}
	}*/
}

void Game::init(int width, int height)
{
	winWidth = width;
	winHeight = height;
	renderer = new GLRendererColor(width, height);
	renderer->initGL("shaders/color/vertext.shd", "shaders/color/fragment.shd");

	gameWidth = 1280;
	gameHeight = 720;
	renderer->setOrthoProjection(gameWidth / -2.0f, gameWidth / 2.0f, gameHeight / -2.0f, gameHeight / 2.0f);
	renderer->setClearColor(1.0f, 1.0f, 200.0f / 255);

	isMouseDown = false;
	clickObject = NULL;

	SquareMeshVbo * square = new SquareMeshVbo();
	square->loadData();
	renderer->addMesh(SquareMeshVbo::MESH_NAME, square);


	SpriteObject * Hero1 = new SpriteObject("Hero2.png", 1, 3);
	Hero1->setSize(250.0f, -250.0f);
	Hero1->translate(glm::vec3(-350.0f, 60.0f, 0.0f));
	Hero1->setAnimationLoop(1, 1, 3, 800);
	objects.push_back(Hero1);

	SpriteObject * Monster = new SpriteObject("monster1.png", 1, 1);
	Monster->setSize(-480.0f, -270.0f);
	Monster->translate(glm::vec3(350.0f, 90.0f, 0.0f));
	Monster->setAnimationLoop(1, 1, 15, 2000);
	objects.push_back(Monster);


	/*Card * heroHealthbar = new Card();
	heroHealthbar->setColor(1,0,0);
	heroHealthbar->setSize(250.0f, 28.0f);
	heroHealthbar->setName("Healthbar");
	heroHealthbar->translate(glm::vec3(-360.0f, 250, 0));
	//obj->setCardAngle(i * 20.0f);
	//obj->translate(glm::vec3(-120, -300, 0));
	//obj->rotate(45.0f);
	objects.push_back(heroHealthbar);

	Card * monsterHealthbar = new Card();
	monsterHealthbar->setColor(1, 0, 0);
	monsterHealthbar->setSize(250.0f, 28.0f);
	monsterHealthbar->setName("Healthbar");
	monsterHealthbar->translate(glm::vec3(360.0f, 250, 0));
	//obj->setCardAngle(i * 20.0f);
	//obj->translate(glm::vec3(-120, -300, 0));
	//obj->rotate(45.0f);
	objects.push_back(monsterHealthbar);   */

	float colorR[5] = { 1.0f,0.0f,0.0f,1.0f,1.0f };
	float colorG[5] = { 0.0f,1.0f,0.0f,1.0f,0.0f };
	float colorB[5] = { 0.0f,0.0f,1.0f,0.0f,1.0f };
	string name[5] = { "RED","GREEN","BLUE","YELLOW","PINK" };
	int spriteNum = 6;
	int allCard = 12;
	int id = 1;
	SpriteObject cardsprite("sprite.png", 2, 6);
	Deck * deck = new Deck();
	for (int i = 0; i < 12; i++)
	{
		Card * card = new Card();
		card->setId(id++);
		card->setSpriteCard(cardsprite, 2, 6);
		deck->addCardToDeck(card);
		if (spriteNum > 0)
		{
			card->setColumn(spriteNum);
			card->setRow(2);
		}
		else
		{
			card->setColumn(7 + spriteNum);
			card->setRow(1);
		}
		card->genUV();
		spriteNum--;
	}
	deck->shufflePlayerDeck();
	deck->drawToHand(5);
	for (int i = -2; i <= 2; i++)
	{
		Card *card = deck->handAt(i+2);
		card->setSize(200.0f, 280.0f);
		//obj->setColor(colorR[i + 2], colorG[i + 2], colorB[i + 2]);
		//card->setName(name[i + 2]);
		card->translate(glm::vec3(0, -240, i));
		card->setCardAngle(i * 20.0f);
		objects.push_back(card);
	}
	/*Card * obj6 = new Card();
	obj6->setColor(0.0, 0.0, 0.0);
	obj6->setSize(160.0f, 246.0f);
	obj6->setName("Monster");
	obj6->translate(glm::vec3(350, 75, 0));
	//obj6->rotate(-45.0f);
	objects.push_back(obj6);  

	/*Card * obj7 = new Card();
	obj7->setColor(0.0, 0.0, 0.0);
	obj7->setSize(160.0f, 246.0f);
	obj7->setName("Hero");
	obj7->translate(glm::vec3(-350, 75, 0));
	//obj6->rotate(-45.0f);
	objects.push_back(obj7); */
	
	

	/*SpriteObject * character = new SpriteObject("character.png", 3, 5);
	character->setSize(200, -200);
	character->translate(glm::vec3(0.0f, 30.0f, 0.0f));
	character->setAnimationLoop(1,1,15,2000);
	objects.push_back(character);	*/

	
}

void Game::render()
{
	this->getRenderer()->render(this->objects);
}

void Game::update(float deltaTime)
{
	if (isMouseDown)
	{
		
	}
	if (!isMouseDown)
	{
		
	}
	for (DrawableObject* obj : objects) {
		obj->update(deltaTime);
	}
}


Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


