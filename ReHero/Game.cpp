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
			cout << cardIndex;
			deck->handToDiscardPile(cardIndex);
			deck->drawToHand(1); 
			resetHandPos();
			if(clickObject->getId()==1)
				clickObject->effect();
			float hpTemp = hp[0]->getSizeX();
			hp[0]->setSize(hpTemp - 50, 20);
			float hpTrTemp = hp[0]->getTranformX();
			cout << hpTrTemp;
			hp[0]->setPosition(glm::vec3(hpTrTemp-25, 250.0f, 0.0f));
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
	for (int i = 0; i < deck->cardsOnHand(); i++) {
		Card* gameObject = dynamic_cast<Card*>(deck->handAt(i));
		if (gameObject)
		{
			if (gameObject->isClick(realX, realY))
			{
				cardIndex = i;
				cout << "Click : " << gameObject->getName()<<" "<<cardIndex << endl;
				isMouseDown = true;
				clickObject = gameObject;
				break;
			}
		}
	}
}

void Game::handleMouseMotion(int x, int y)
{
	previewCard->setActive(false);
	float realX, realY;
	realX = (gameWidth / -2.0f) + x * (gameWidth / winWidth);
	realY = (gameHeight / -2.0f) + (winHeight - y) * (gameHeight / winHeight);
	if(isMouseDown)
	{
		
		//cout << realX << " " << realY << endl;

		if (clickObject)
		{
			clickObject->setIsInHand(false);
			clickObject->setPosition(glm::vec3(realX, realY, 0));
			//clickObject->setPositionDrag(realX, realY);
		}
	}
	else
	{
		for (int i = 0; i < deck->cardsOnHand(); i++) {
			Card* gameObject = dynamic_cast<Card*>(deck->handAt(i));
			if (gameObject)
			{
				if (gameObject->isClick(realX, realY))
				{
					previewCard->setActive(true);
					previewCard->setRow(gameObject->getRow());
					previewCard->setColumn(gameObject->getColumn());
					previewCard->genUV();
					break;
				}
			}
		}
	}
	
}

void Game::handleKey(char ch)
{
	if (this->objects.size() > 0) {
		DrawableObject *obj = this->objects.at(0);
		switch (ch) {
		case 'u': obj->translate(glm::vec3(0, 0.3, 0)); break;
		case 'd': deck->drawToHand(1); resetHandPos(); break;
		case 'l': obj->translate(glm::vec3(-0.3, 0, 0)); break;
		case 'r': obj->translate(glm::vec3(0.3, 0, 0)); break;
		}
	}
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

	SpriteObject * BG = new SpriteObject("BG.png", 1, 1);
	BG->setSize(1280.0f, 720.0f);
	BG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BG->setAnimationLoop(1, 1, 3, 800);
	objects.push_back(BG);

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

	SpriteObject * healthBarMonster = new SpriteObject("hp.png", 1, 1);
	healthBarMonster->setSize(250.0f, 20.0f);
	healthBarMonster->translate(glm::vec3(350.0f, 250.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	hp.push_back(healthBarMonster);
	objects.push_back(healthBarMonster);

	SpriteObject * healthBarHero = new SpriteObject("hp.png", 1, 1);
	healthBarHero->setSize(250.0f, 20.0f);
	healthBarHero->translate(glm::vec3(-350.0f, 250.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	hp.push_back(healthBarHero);
	objects.push_back(healthBarHero);

	SpriteObject * BGF = new SpriteObject("BGF.png", 1, 1);
	BGF->setSize(1280.0f, 720.0f);
	BGF->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BGF->setAnimationLoop(1, 1, 3, 800);
	objects.push_back(BGF);

	SpriteObject * OH = new SpriteObject("OH.png", 1, 1);
	OH->setSize(1280.0f, 720.0f);
	OH->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	OH->setAnimationLoop(1, 1, 3, 800);
	objects.push_back(OH);

	previewCard = new SpriteObject("sprite.png", 2, 6);
	previewCard->setSize(300.0f,420.0f);
	previewCard->translate(glm::vec3(0.0f, 80.0f, 0.0f));
	previewCard->setAnimationLoop(1, 1, 1, 800);
	objects.push_back(previewCard);
	previewCard->setActive(false);

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
	deck = new Deck();
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
	resetHandPos();
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

void Game::resetHandPos()
{
	//if(deck->cardsOnHand()%2!=0)
	float k = deck->cardsOnHand() / -2;
	float cardAngel;
	if (deck->cardsOnHand() <= 5)
	{
		cardAngel = 20;
	}
	else
		cardAngel = 100 / deck->cardsOnHand();
	for (int i = 0; i < deck->cardsOnHand(); i++)
	{
		Card *card = deck->handAt(i);
		card->setSize(200.0f, 280.0f);
		//obj->setColor(colorR[i + 2], colorG[i + 2], colorB[i + 2]);
		//card->setName(name[i + 2]);
		card->setPosition(glm::vec3(0, 0, 0));
		card->translate(glm::vec3(0, -240, i));
		
		if(deck->cardsOnHand()%2!=0)
		card->setCardAngle(k*cardAngel);
		else
		if (deck->cardsOnHand() % 2 == 0)
		{
			card->setCardAngle((k+0.5)* cardAngel);
		}
		k++;
		objects.push_back(card);
	}
	
}


Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


