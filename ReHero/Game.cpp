#include "stdafx.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "Card.h"
#include "Deck.h"
#include "SpriteObject.h"
#include "ClickableObject.h"
#include "Audio.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>



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
	if (clickableObject)
	{
		cout << clickableObject->getTag() << endl;
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "Start"&&state==99)
		{
			restartGame();
			state = 0;
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "Quit"&&state == 99)
		{
			state = 999999;
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "ResumeButton" && state == 100)
		{
			state = 0;
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "optionButton" && state == 0)
		{
			state = 100;
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "Exit" && state == 100)
		{
			state = 99;
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "endTurn" && state == 0)
		{
			endTurn();
		}
		clickableObject->onClick(false);
	}
	if (realY < 0.0f||(realY > 0.0f&&state==2|| state == 3))
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
			showMana->setColumn(deck->getMana()+1);
			showMana->genUV();
			deck->handToDiscardPile(cardIndex);
			//deck->drawToHand(1); 
			resetHandPos();
			state = 1;
			//myHero->setTexture(effect[0]->getTexture());
			if (enemies[0]->takeDamage(clickObject->effect()))
			{
				cout <<"Monster take damage:" <<clickObject->effect();
				/*for (int i = 0; i < 6; i++)
				{
					enemies[0]->translate(glm::vec3(pow(-1, i) * 10, 0.0f, 0.0f));
					this->getRenderer()->render(this->objects);
				}		*/
			}
			else
			{
				  monsterHp[0]->setSize(0, 20);
				  enemies[0]->setActive(false);
				  HPBG[1]->setActive(false);
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
				object->onClick(true);
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
	if(state==0)
	{
		for (int i = deck->cardsOnHand() - 1; i >= 0; i--) {
			deck->handAt(i)->setState(0);
		}

		for (int i = deck->cardsOnHand()-1; i >= 0 ; i--) {
			Card* gameObject = deck->handAt(i);
			if (gameObject->isClick(realX, realY))
			{
				deck->handAt(i)->setState(10);
				previewCard->setActive(true);
				previewCard->setRow(gameObject->getRow());
				previewCard->setColumn(gameObject->getColumn());
				previewCard->genUV();
				previewCard->setAnimationLoop(gameObject->getRow(), gameObject->getColumn(), 1, 800);
				break;
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
		case 'p': if (state == 0) { state = 100; }else if (state == 100) state = 0; cout << state; break;
			
		}
	}
}

void Game::init(int width, int height)
{
	winWidth = width;
	winHeight = height;
	renderer = new GLRendererColor(width, height);
	renderer->initGL("shaders/color/vertext.shd", "shaders/color/fragment.shd");
	state = 99;

	gameWidth = 1280;
	gameHeight = 720;
	renderer->setOrthoProjection(gameWidth / -2.0f, gameWidth / 2.0f, gameHeight / -2.0f, gameHeight / 2.0f);
	renderer->setClearColor(1.0f, 1.0f, 200.0f / 255);

	isMouseDown = false;
	clickObject = NULL;

	SquareMeshVbo * square = new SquareMeshVbo();
	square->loadData();
	renderer->addMesh(SquareMeshVbo::MESH_NAME, square);

	AudioEngine audio;
	audio.init();
	Music music = audio.loadMusic("testsound.mp3");
	music.play(-1);

	SpriteObject * MenuBG = new SpriteObject("Sprite/MenuBG.jpg", 1, 1);
	MenuBG->setSize(1280.0f, 720.0f);
	MenuBG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	MenuBG->setAnimationLoop(1, 1, 3, 800);
	MenuBG->setTag("MenuBG");
	Menu.push_back(MenuBG);

	SpriteObject * gameName = new SpriteObject("Sprite/gameName.png", 1, 1);
	gameName->setSize(600.0f, 400.0f);
	gameName->translate(glm::vec3(300.0f, 200.0f, 0.0f));
	gameName->setTag("gameName");
	Menu.push_back(gameName);

	SpriteObject Play("Sprite/Menu_Play.png", 1, 1);
	SpriteObject Play_Glow("Sprite/Menu_Play_Glow.png", 1, 1);
	SpriteObject Libary("Sprite/Menu_Libary.png", 1, 1);
	SpriteObject Libary_Glow("Sprite/Menu_Libary_Glow.png", 1, 1);
	SpriteObject Setting("Sprite/Menu_Setting.png", 1, 1);
	SpriteObject Setting_Glow("Sprite/Menu_Setting_Glow.png", 1, 1);
	SpriteObject Quit("Sprite/Menu_Quit.png", 1, 1);
	SpriteObject Quit_Glow("Sprite/Menu_Quit_Glow.png", 1, 1);
	SpriteObject pauseMenu("Sprite/pasueMenu.png", 2, 3);
	SpriteObject OptionButton("Sprite/optionbutton.png", 1, 2);
	
	

	ClickableObject * PlayButton = new ClickableObject;
	PlayButton->setSpriteClickableObject(Play, 1, 1);
	PlayButton->setEffect(Play_Glow, 1, 1);
	PlayButton->setColumn(1);
	PlayButton->setRow(1);
	PlayButton->genUV();
	PlayButton->setTag("Start");
	PlayButton->setSize(250.0f, 100.0f);
	PlayButton->translate(glm::vec3(-300.0f, 150.0f, 0.0f));
	clickable.push_back(PlayButton);
	Menu.push_back(PlayButton);

	ClickableObject * LibaryButton = new ClickableObject;
	LibaryButton->setSpriteClickableObject(Libary, 1, 1);
	LibaryButton->setEffect(Libary_Glow, 1, 1);
	LibaryButton->setColumn(1);
	LibaryButton->setRow(1);
	LibaryButton->genUV();
	LibaryButton->setTag("Libary");
	LibaryButton->setSize(250.0f, 100.0f);
	LibaryButton->translate(glm::vec3(-300.0f, 50.0f, 0.0f));
	clickable.push_back(LibaryButton);
	Menu.push_back(LibaryButton);

	ClickableObject * SettingButton = new ClickableObject;
	SettingButton->setSpriteClickableObject(Setting, 1, 1);
	SettingButton->setEffect(Setting_Glow, 1, 1);
	SettingButton->setColumn(1);
	SettingButton->setRow(1);
	SettingButton->genUV();
	SettingButton->setTag("Setting");
	SettingButton->setSize(250.0f, 100.0f);
	SettingButton->translate(glm::vec3(-300.0f, -50.0f, 0.0f));
	clickable.push_back(SettingButton);
	Menu.push_back(SettingButton);

	ClickableObject * QuitButton = new ClickableObject;
	QuitButton->setSpriteClickableObject(Quit, 1, 1);
	QuitButton->setEffect(Quit_Glow, 1, 1);
	QuitButton->setColumn(1);
	QuitButton->setRow(1);
	QuitButton->genUV();
	QuitButton->setTag("Quit");
	QuitButton->setSize(250.0f, 100.0f);
	QuitButton->translate(glm::vec3(-300.0f, -150.0f, 0.0f));
	clickable.push_back(QuitButton);
	Menu.push_back(QuitButton);

	SpriteObject * PauseBG = new SpriteObject("Sprite/PauseBG.png", 1, 1);
	PauseBG->setSize(1280.0f, 720.0f);
	PauseBG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	PauseBG->setAnimationLoop(1, 1, 3, 800);
	PauseBG->setTag("PauseBG");
	Pause.push_back(PauseBG);

	ClickableObject * ResumeButton = new ClickableObject;
	ResumeButton->setSpriteClickableObject(pauseMenu, 2, 2);
	ResumeButton->setEffect(pauseMenu, 1, 2);
	ResumeButton->genUV();
	ResumeButton->setTag("ResumeButton");
	ResumeButton->setSize(250.0f, 100.0f);
	ResumeButton->translate(glm::vec3(0.0f, -150.0f, 0.0f));
	clickable.push_back(ResumeButton);
	Pause.push_back(ResumeButton);

	ClickableObject * ExitButton = new ClickableObject;
	ExitButton->setSpriteClickableObject(pauseMenu, 2, 3);
	ExitButton->setEffect(pauseMenu, 1, 3);
	ExitButton->genUV();
	ExitButton->setTag("Exit");
	ExitButton->setSize(250.0f, 100.0f);
	ExitButton->translate(glm::vec3(0.0f, -250.0f, 0.0f));
	clickable.push_back(ExitButton);
	Pause.push_back(ExitButton);

	SpriteObject * BG = new SpriteObject("Sprite/BG.png", 1, 1);
	BG->setSize(1280.0f, 720.0f);
	BG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BG->setAnimationLoop(1, 1, 3, 800);
	BG->setTag("BG");
	objects.push_back(BG);

	Monster * Monster1 = new Monster(20,"gob", 1, 2);
	Monster1->setSize(-250.0f, -250.0f);
	Monster1->translate(glm::vec3(350.0f, 60.0f, 0.0f));
	Monster1->setAnimationLoop(1, 1, 2, 400);
	Monster1->setTag("Monster");
	objects.push_back(Monster1);
	enemies.push_back(Monster1);

	myHero = new Hero(20, "Sprite/Hero2New.png", 2, 6);
	myHero->setSize(250.0f, -250.0f);
	myHero->translate(glm::vec3(-350.0f, 60.0f, 0.0f));
	myHero->setAnimationLoop(1, 1, 6, 1500);
	myHero->setTag("Hero");
	objects.push_back(myHero);

	/*SpriteObject * Effect1 = new SpriteObject("Attack.png", 1, 6);
	Effect1->setSize(-250.0f, -250.0f);
	Effect1->translate(glm::vec3(270.0f, 60.0f, 0.0f));
	Effect1->setAnimationLoop(1, 1, 4, 2000);
	Effect1->setTag("Effect1");
	//objects.push_back(Effect1);
	effect.push_back(Effect1);		 */

	/*SpriteObject * BGF = new SpriteObject("BGF.png", 1, 1);
	BGF->setSize(1280.0f, 720.0f);
	BGF->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BGF->setAnimationLoop(1, 1, 3, 800);
	BGF->setTag("BGF");
	objects.push_back(BGF);

	SpriteObject * OH = new SpriteObject("OH.png", 1, 1);
	OH->setSize(1280.0f, 720.0f);
	OH->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	OH->setAnimationLoop(1, 1, 3, 800);
	OH->setTag("OH");
	objects.push_back(OH);

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
	int spriteNum = 10;
	int allCard = 20;
	int id = 1;
	SpriteObject cardsprite1("Sprite/cardSprite1.png", 2, 10);
	SpriteObject cardsprite2("Sprite/cardSprite2.png", 2, 10);
	SpriteObject endturn("Sprite/endturn.png", 1, 1);
	SpriteObject endturneffect("Sprite/endturneffect.png", 1, 1);
	deck = Deck::getInstance();
	int howManycard;
	ifstream datafile("deck01.txt");
	if (!datafile)
	{
		cout << "fail to load deck01 " << endl;
		return;
	}
	string names;
	datafile >> howManycard;
	for (int i = 0; i < howManycard; i++)
	{
		getline(datafile, names, '\n');
		cout << names << endl;
		PlayerDeck.push_back(names);
	}

	datafile.close();

	for (int c = 0; c < PlayerDeck.size(); c++)
	{
		cout << c << endl;
		ifstream datafile("cardSpriteData.txt");
		if (!datafile)
		{
			cout << "fail to load cardEffectData " << endl;
			return;
		}
		int row;
		int column;
		datafile >> howManycard;
		for (int i = 0; i < howManycard; i++)
		{
			datafile >> names >> row >> column;
			cout << PlayerDeck[c] << " " << names << endl;
			if (PlayerDeck[c] == names)
			{
				cout << names << " " << row << " " << column << endl;
				Card * card = new Card();
				card->setId(id++);
				card->setName(names);
				card->setSpriteCard(cardsprite1, 2, 10);
				card->setEffectCard(cardsprite2, 2, 10);
				deck->addCardToDeck(card);
				card->setColumn(column);
				cout << column << endl<<row<<endl;
				card->setRow(row);
				card->genUV();
				break;
			}
		}
		datafile.close();
	}
	deck->shufflePlayerDeck();
	deck->drawToHand(5);
	deck->randomMana();
	resetHandPos();
	/*for (int i = 0; i < 20; i++)
	{
		Card * card = new Card();
		card->setId(id++);
		card->setMana(2);
		card->setSpriteCard(cardsprite1, 2, 10);
		card->setEffectCard(cardsprite2, 2, 10);
		deck->addCardToDeck(card);
		if (spriteNum > 0)
		{
			card->setColumn(spriteNum);
			card->setRow(2);
		}
		else
		{
			card->setColumn(10 + spriteNum);
			card->setRow(1);
		}
		card->genUV();
		spriteNum--;
	}	   */

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
	

	SpriteObject * healthBarMonster = new SpriteObject("Sprite/hp.png", 1, 1);
	healthBarMonster->setSize(250.0f, 20.0f);
	healthBarMonster->translate(glm::vec3(350.0f, 200.0f, 0.0f));
	healthBarMonster->setTag("healthBarMonster");
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	monsterHp.push_back(healthBarMonster);
	enemies[0]->HPBar = healthBarMonster;
	objects.push_back(healthBarMonster);

	SpriteObject * healthBarHero = new SpriteObject("Sprite/hp.png", 1, 1);
	healthBarHero->setSize(250.0f, 20.0f);
	healthBarHero->translate(glm::vec3(-350.0f, 200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	healthBarHero->setTag("healthBarHero");
	heroHp.push_back(healthBarHero);
	myHero->HPBar = healthBarHero;
	objects.push_back(healthBarHero);
	
	SpriteObject * healthBarHeroBG = new SpriteObject("Sprite/HPBar.png", 1, 1);
	healthBarHeroBG->setSize(280.0f, 50.0f);
	healthBarHeroBG->translate(glm::vec3(-360.0f, 200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	healthBarHeroBG->setTag("healthBarHeroBG");
	objects.push_back(healthBarHeroBG);
	HPBG.push_back(healthBarHeroBG);

	SpriteObject * healthBarMonsterBG = new SpriteObject("Sprite/HPBar.png", 1, 1);
	healthBarMonsterBG->setSize(280.0f, 50.0f);
	healthBarMonsterBG->translate(glm::vec3(340.0f, 200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	healthBarMonsterBG->setTag("healthBarMonsterBG");
	objects.push_back(healthBarMonsterBG);
	HPBG.push_back(healthBarMonsterBG);

	showMana = new SpriteObject("Sprite/manasprite.png", 1, 7);
	//showMana->setAnimationLoop(1, 1, 1, 800);
	showMana->setPlayAnim(false);
	showMana->setRow(1);
	showMana->setColumn(deck->getMana()+1);
	showMana->genUV();
	showMana->setSize(90.0f, 100.0f);
	showMana->translate(glm::vec3(-500.0f, -170.0f, 0.0f));
	showMana->setTag("showMana");
	objects.push_back(showMana);

	randomMana = new SpriteObject("Sprite/manasprite.png", 1, 7);
	randomMana->setAnimationLoop(1, 1, 7, 300);
	randomMana->setRow(1);
	randomMana->setColumn(deck->getMana() + 1);
	randomMana->genUV();
	randomMana->setSize(200.0f, 210.0f);
	randomMana->translate(glm::vec3(0.0f,100.0f, 0.0f));
	randomMana->setTag("randomMana");
	objects.push_back(randomMana);
	randomMana->setActive(false);

	randomManaText = new SpriteObject("Sprite/mText2.png", 1, 1);
	randomManaText->setSize(500.0f, 150.0f);
	randomManaText->translate(glm::vec3(0.0f, 270.0f, 0.0f));
	randomManaText->setTag("randomManaText");
	objects.push_back(randomManaText);
	randomManaText->setActive(false);

	ClickableObject * endTurn = new ClickableObject;
	endTurn->setSpriteClickableObject(endturn, 1, 1);
	endTurn->setEffect(endturneffect, 1, 1);
	endTurn->setColumn(1);
	endTurn->setRow(1);
	endTurn->genUV();
	endTurn->setTag("endTurn");
	endTurn->setSize(130.0f, 100.0f);
	endTurn->translate(glm::vec3(500.0f, -200.0f, 0.0f));
	//Hero1->setAnimationLoop(1, 1, 1, 800);
	clickable.push_back(endTurn);
	objects.push_back(endTurn);
	
	ClickableObject * optionButton= new ClickableObject;
	optionButton->setSpriteClickableObject(OptionButton, 1, 1);
	optionButton->setEffect(OptionButton, 1, 2);
	optionButton->genUV();
	optionButton->setTag("optionButton");
	optionButton->setSize(50.0f, 50.0f);
	optionButton->translate(glm::vec3(600.0f, 320.0f, 0.0f));
	clickable.push_back(optionButton);
	objects.push_back(optionButton);

	previewCard = new SpriteObject("Sprite/cardSprite2.png", 2, 10);
	previewCard->setSize(300.0f, 420.0f);
	previewCard->translate(glm::vec3(0.0f, 115.0f, 0.0f));
	previewCard->setTag("previewCard");
	objects.push_back(previewCard);
	previewCard->setActive(false);

}

void Game::render()
{
	if (state == 99)
	{
		this->getRenderer()->render(this->Menu);
	}
	else
	if (state == 100)
	{
		this->getRenderer()->render(this->Pause);
	}
	else
	{
		this->getRenderer()->render(this->objects);
		this->getRenderer()->render(*(deck->getHand()));
	}
	
	
}



void Game::update(float deltaTime)
{
	/*if (state == 0)
	{

	}	  */
	for (DrawableObject* obj : objects) {
		obj->update(deltaTime);
	}
	for (Card* card : *(deck->getHand()))
	{
		card->update(deltaTime);
	}
	for (DrawableObject* menu : Menu)
	{
		menu->update(deltaTime);
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
	state = 2;
	deck->discardHand();
	monsterTurn();
	deck->randomMana();
	showMana->setColumn(deck->getMana() + 1);
	showMana->genUV();
	deck->drawToHand(5);
	resetHandPos();
}

void Game::monsterTurn()
{
		if (myHero->isAlive() == true)
		{
			enemies[0]->randomUseSkill(myHero, nullptr);
			//cout << endl << "Hero take Damage:" << damage << endl;
			//cout << endl << "MaxHP: " << myHero->getMaxHP() << endl;
		}
		if (myHero->isAlive() == false)
		{
			heroHp[0]->setSize(0, 20);
			myHero->setActive(false);
			HPBG[0]->setActive(false);
		}
	 

	/*for (int i = 0; i < enemies.size; i++)
	{
		enemies[i]->randomUseSkill(myHero, nullptr);
		if (myHero->isAlive() == false)
		{
			heroHp[0]->setSize(0, 20);
			myHero->setActive(false);
			break;
		}
	}  */
	
}

void Game::getHit()
{

}

void Game::restartGame()
{
	cout << "Reset" << endl; 
	endTurn();
	myHero->setMaxHP(20);
	myHero->setHP(20);
	myHero->takeDamage(0);
	heroHp[0]->setPosition(glm::vec3(-350.0f, 200.0f, 0.0f));
	enemies[0]->setMaxHP(20);
	enemies[0]->setHP(20);
	enemies[0]->takeDamage(0);
	monsterHp[0]->setPosition(glm::vec3(350.0f, 200.0f, 0.0f));
	HPBG[0]->setActive(true);
	HPBG[1]->setActive(true);
	deck->randomMana();
	showMana->setColumn(deck->getMana()+1);
	showMana->genUV();
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


