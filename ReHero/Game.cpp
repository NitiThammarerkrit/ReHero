#include "stdafx.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "Card.h"
#include "Deck.h"
#include "SpriteObject.h"
#include "ClickableObject.h"


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
		if (clickObject&&deck->getMana()>= clickObject->getMana())
		{
			//cout << cardIndex;
			cout << "Use mana " << clickObject->getMana()<< endl;
			deck->loseMana(clickObject->getMana());
			deck->handToDiscardPile(cardIndex);
			//deck->drawToHand(1); 
			resetHandPos();
			if (enemies[0]->takeDamage(clickObject->effect()))
			{
				cout <<"Monster take damage:" <<clickObject->effect();
				monsterHp[0]->setSize(((float)enemies[0]->getHP() / (float)enemies[0]->getMaxHP()) * 250.0f, 20);
				cout << "\nMonster HP:" << enemies[0]->getHP();
				monsterHp[0]->translate(glm::vec3((-clickObject->effect()/2)/100*250, 0.0f, 0.0f));
			}
			else
			{
				  monsterHp[0]->setSize(0, 20);
				  enemies[0]->setActive(false);
			}
				
				
			/*if(clickObject->active==true)
				clickObject->active = false;	*/
		}
		else
			if (clickObject)
			{
				cout << "Not enough mana" << endl;
				clickObject->setIsInHand(true);
				clickObject->setPosition(glm::vec3(0, -240, 0));
				cout << "Reset"<<endl;
			}
	}
	if (clickableObject)
	{
		endTurn();
	}
	clickObject = NULL;
	clickableObject = NULL;
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
				//cout <<endl<< "Click : " << gameObject->getName()<<" "<<cardIndex << endl;
				isMouseDown = true;
				clickObject = gameObject;
				break;
			}
		}
	}
	for (int i = 0; i < clickable.size(); i++)
	{
		ClickableObject* object = dynamic_cast<ClickableObject*>(clickable[i]);
		if (object)
		{
			if (object->isClick(realX, realY))
			{
				//cardIndex = i;
				cout <<endl<< "Click JA " << endl;
				isMouseDown = true;
				clickableObject = object;
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
		case 'u': resetHandPos(); break;
		case 'd': deck->drawToHand(1); resetHandPos(); break;
		case 'e': endTurn(); break;
		case 'l': obj->translate(glm::vec3(-0.3, 0, 0)); break;
		case 'r': restartGame(); break;
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

	Hero * Hero1 = new Hero(100,"Hero2.png", 1, 6);
	Hero1->setSize(250.0f, -250.0f);
	Hero1->translate(glm::vec3(-350.0f, 60.0f, 0.0f));
	Hero1->setAnimationLoop(1, 1, 6, 800);
	objects.push_back(Hero1);
	myHero = Hero1;

	Monster * Monster1 = new Monster(100,"gob.png", 1, 2);
	Monster1->setSize(-250.0f, -250.0f);
	Monster1->translate(glm::vec3(350.0f, 60.0f, 0.0f));
	Monster1->setAnimationLoop(1, 1, 2, 400);
	objects.push_back(Monster1);
	enemies.push_back(Monster1);

	

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
	SpriteObject clickableObject("endturn.png", 1, 1);
	deck = Deck::getInstance();
	for (int i = 0; i < 12; i++)
	{
		Card * card = new Card();
		card->setId(id++);
		card->setMana(2);
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
	deck->randomMana();
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

	SpriteObject * healthBarMonster = new SpriteObject("hp.png", 1, 1);
	healthBarMonster->setSize(250.0f, 20.0f);
	healthBarMonster->translate(glm::vec3(350.0f, 200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	monsterHp.push_back(healthBarMonster);
	objects.push_back(healthBarMonster);

	SpriteObject * healthBarHero = new SpriteObject("hp.png", 1, 1);
	healthBarHero->setSize(250.0f, 20.0f);
	healthBarHero->translate(glm::vec3(-350.0f, 200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	heroHp.push_back(healthBarHero);
	objects.push_back(healthBarHero);

	ClickableObject * endTurn = new ClickableObject;
	endTurn->setSpriteClickableObject(clickableObject, 1, 1);
	if (spriteNum > 0)
	{
		endTurn->setColumn(spriteNum);
		endTurn->setRow(2);
	}
	else
	{
		endTurn->setColumn(7 + spriteNum);
		endTurn->setRow(1);
	}
	endTurn->genUV();
	spriteNum--;
	endTurn->setSize(170.0f, 130.0f);
	endTurn->translate(glm::vec3(500.0f, -250.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	clickable.push_back(endTurn);
	objects.push_back(endTurn);
	
}

void Game::render()
{
	this->getRenderer()->render(this->objects);	
	this->getRenderer()->render(*(deck->getHand()));
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
	cout<<"Mana "<<deck->getMana()<<endl;
	float k = deck->cardsOnHand() / -2;
	float cardAngel;
	if (deck->cardsOnHand() <= 5)
	{
		cardAngel = 20;
	}
	else
	{
		cardAngel = 100 / deck->cardsOnHand();
	}
		
	for (int i = 0; i < deck->cardsOnHand(); i++)
	{
		Card *card = deck->handAt(i);
		card->setIsInHand(true);
		card->setSize(200.0f, 280.0f);
		//obj->setColor(colorR[i + 2], colorG[i + 2], colorB[i + 2]);
		//card->setName(name[i + 2]);
		card->setPosition(glm::vec3(0, 0, 0));
		card->translate(glm::vec3(0, -240, i));
		
		if (deck->cardsOnHand() % 2 != 0)
		{
			card->setCardAngle(k*cardAngel);
		}
		else
		{
			card->setCardAngle((k+0.5)* cardAngel);
		}
		k++;
		//objects.push_back(card);
	}
	
}

void Game::endTurn()
{
	deck->discardHand();
	monsterTurn();
	deck->randomMana();
	deck->drawToHand(5);
	resetHandPos();
}

void Game::monsterTurn()
{
	float damage = rand() % 21 + 10;
	if (myHero->takeDamage(damage))
	{
		cout <<endl<< "Hero take Damage:" << damage<<endl;
		cout << endl <<"MaxHP: "<< myHero->getMaxHP()<<endl;
		heroHp[0]->setSize((myHero->getHP() / (float)myHero->getMaxHP()) * 250.0f, 20);
		cout << "Hero HP:" << myHero->getHP()<<endl;
		//int hpTrTemp = monsterHp[0]->getTranformX();
		heroHp[0]->translate(glm::vec3((-damage / 2) / 100 * 250, 0.0f, 0.0f));
	}
	else
	{
		heroHp[0]->setSize(0, 20);
		myHero->setActive(false);
	}
}

void Game::restartGame()
{
	cout << "Reset" << endl; 
	endTurn();
	myHero->setMaxHP(100);
	myHero->setHP(100);
	heroHp[0]->setSize((myHero->getHP() / (float)myHero->getMaxHP()) * 250.0f, 20);
	heroHp[0]->setPosition(glm::vec3(-350.0f, 200.0f, 0.0f));
	enemies[0]->setMaxHP(100);
	enemies[0]->setHP(100);
	monsterHp[0]->setSize(((float)enemies[0]->getHP() / (float)enemies[0]->getMaxHP()) * 250.0f, 20);
	monsterHp[0]->setPosition(glm::vec3(350.0f ,200.0f,0.0f));
	myHero->setActive(true);
	enemies[0]->setActive(true);	

}




Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


