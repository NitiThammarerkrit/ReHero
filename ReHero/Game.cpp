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
#include "TextObject.h"
#include "TextMeshVbo.h"						             
											         



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
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Start"&&state==99/*in Mainmenu*/)
		{
			restartGame();
			state = 50;	  //To City
			for (int i = 0; i < clickable.size(); i++)
			{
				if(clickable[i]->getTag()== "Menu")
				clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
				clickable[i]->active = true;
				if (clickable[i]->getTag() == "InFight")
				clickable[i]->active = false;
				if (clickable[i]->getTag() == "Pause")
				clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Building3"&&state == 50/*in Mainmenu*/)
		{
			state = 0;
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = true;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Quit"&&state == 99/*in Mainmenu*/)
		{
			state = 999999; //Exit Game
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "ResumeButton" && state == 100/*in Pause*/)
		{
			state = 0;	   //To fight
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = true;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "optionButton" && state == 0/*inFight*/)
		{
			state = 100;	 //To pause
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = true;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Exit" && state == 100/*in Pause*/)
		{
			state = 99; /*To mainMenu*/
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = true;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "endTurn" && state == 0)
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
			state = 1;
			//myHero->setTexture(effect[0]->getTexture());
			clickObject->effect(myHero, enemies[0]);
			resetHandPos();
			deck->playACard();
			if (enemies[0]->isAlive() == false)
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

	TextMeshVbo * text = new TextMeshVbo();
	text->loadData();
	renderer->addMesh(TextMeshVbo::MESH_NAME, text);

	AudioEngine audio;
	audio.init();
	Music music = audio.loadMusic("testsound.mp3");
	music.play(-1);
	/////////////////////////////////////////////////////////////////MainMenu/////////////////////////////////////////////////////////////
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
	PlayButton->setTagAndName("Menu","Start");
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
	LibaryButton->setTagAndName("Menu","Libary");
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
	SettingButton->setTagAndName("Menu","Setting");
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
	QuitButton->setTagAndName("Menu", "Quit");
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
	ResumeButton->setTagAndName("Pause", "ResumeButton");
	ResumeButton->setSize(250.0f, 100.0f);
	ResumeButton->translate(glm::vec3(0.0f, -150.0f, 0.0f));
	ResumeButton->active = false;
	clickable.push_back(ResumeButton);
	Pause.push_back(ResumeButton);

	ClickableObject * ExitButton = new ClickableObject;
	ExitButton->setSpriteClickableObject(pauseMenu, 2, 3);
	ExitButton->setEffect(pauseMenu, 1, 3);
	ExitButton->genUV();
	ExitButton->setTagAndName("Pause", "Exit");
	ExitButton->setSize(250.0f, 100.0f);
	ExitButton->translate(glm::vec3(0.0f, -250.0f, 0.0f));
	ExitButton->active = false;
	clickable.push_back(ExitButton);
	Pause.push_back(ExitButton);
	/////////////////////////////////////////////////////////////////MainMenu/////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////City/////////////////////////////////////////////////////////////////
	SpriteObject * CityBG = new SpriteObject("Sprite/City.png", 1, 1);
	CityBG->setSize(1280.0f, 720.0f);
	CityBG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	CityBG->setAnimationLoop(1, 1, 3, 800);
	CityBG->setTag("CityBG");
	City.push_back(CityBG);

	SpriteObject Building1_Normal("Sprite/Building1.png", 1, 1);
	SpriteObject Building1_Glow("Sprite/Building1Glow.png", 1, 1);
	SpriteObject Building2_Normal("Sprite/Building2.png", 1, 1);
	SpriteObject Building2_Glow("Sprite/Building2Glow.png", 1, 1);
	SpriteObject Building3_Normal("Sprite/Building3.png", 1, 1);
	SpriteObject Building3_Glow("Sprite/Building3Glow.png", 1, 1);
	SpriteObject Building4_Normal("Sprite/Building4.png", 1, 1);
	SpriteObject Building4_Glow("Sprite/Building4Glow.png", 1, 1);

	ClickableObject * Building1 = new ClickableObject;
	Building1->setSpriteClickableObject(Building1_Normal, 1, 1);
	Building1->setEffect(Building1_Glow, 1, 1);
	Building1->setColumn(1);
	Building1->setRow(1);
	Building1->genUV();
	Building1->setTagAndName("City", "Building1");
	Building1->setSize(480.0f, 430.0f);
	Building1->translate(glm::vec3(-393.0f, 140.0f, 0.0f));
	Building1->active = false;
	clickable.push_back(Building1);
	City.push_back(Building1);

	ClickableObject * Building2 = new ClickableObject;
	Building2->setSpriteClickableObject(Building2_Normal, 1, 1);
	Building2->setEffect(Building2_Glow, 1, 1);
	Building2->setColumn(1);
	Building2->setRow(1);
	Building2->genUV();
	Building2->setTagAndName("City", "Building2");
	Building2->setSize(200.0f, 250.0f);
	Building2->translate(glm::vec3(30.0f, 140.0f, 0.0f));
	Building2->active = false;
	clickable.push_back(Building2);
	City.push_back(Building2);

	ClickableObject * Building3 = new ClickableObject;
	Building3->setSpriteClickableObject(Building3_Normal, 1, 1);
	Building3->setEffect(Building3_Glow, 1, 1);
	Building3->setColumn(1);
	Building3->setRow(1);
	Building3->genUV();
	Building3->setTagAndName("City", "Building3");
	Building3->setSize(300.0f, 300.0f);
	Building3->translate(glm::vec3(491, 163, 0.0f));
	Building3->active = false;
	clickable.push_back(Building3);
	City.push_back(Building3);

	ClickableObject * Building4 = new ClickableObject;
	Building4->setSpriteClickableObject(Building4_Normal, 1, 1);
	Building4->setEffect(Building4_Glow, 1, 1);
	Building4->setColumn(1);
	Building4->setRow(1);
	Building4->genUV();
	Building4->setTagAndName("City", "Building4");
	Building4->setSize(320.0f, 260.0f);
	Building4->translate(glm::vec3(115.0f, -225.0f, 0.0f));
	Building4->active = false;
	clickable.push_back(Building4);
	City.push_back(Building4);

	////////////////////////////////////////////////////////////////City/////////////////////////////////////////////////////////////////
	SpriteObject * BG = new SpriteObject("Sprite/BG.png", 1, 1);
	BG->setSize(1280.0f, 720.0f);
	BG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BG->setAnimationLoop(1, 1, 3, 800);
	BG->setTag("BG");
	objects.push_back(BG);

	setMonster(20,"gob", 3, 5,400);
	//delete enemies[0];
	//setMonster(20, "wasp", 1, 5,1000);

	myHero = new Hero(20, "Sprite/Hero2New.png", 2, 6);
	myHero->setSize(250.0f, -250.0f);
	myHero->translate(glm::vec3(-350.0f, 60.0f, 0.0f));
	myHero->setAnimationLoop(1, 1, 6, 1500);
	myHero->setTag("Hero");
	objects.push_back(myHero);

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
	

	SpriteObject * healthBarMonster = new SpriteObject("Sprite/hp.png", 1, 1);
	healthBarMonster->setSize(250.0f, 20.0f);
	healthBarMonster->translate(glm::vec3(350.0f, 200.0f, 0.0f));
	healthBarMonster->setTag("healthBarMonster");
	monsterHp.push_back(healthBarMonster);
	enemies[0]->HPBar = healthBarMonster;
	objects.push_back(healthBarMonster);

	SpriteObject * healthBarHero = new SpriteObject("Sprite/hp.png", 1, 1);
	healthBarHero->setSize(250.0f, 20.0f);
	healthBarHero->translate(glm::vec3(-350.0f, 200.0f, 0.0f));
	healthBarHero->setTag("healthBarHero");
	heroHp.push_back(healthBarHero);
	myHero->HPBar = healthBarHero;
	objects.push_back(healthBarHero);
	
	SpriteObject * healthBarHeroBG = new SpriteObject("Sprite/HPBar.png", 1, 1);
	healthBarHeroBG->setSize(280.0f, 50.0f);
	healthBarHeroBG->translate(glm::vec3(-360.0f, 200.0f, 0.0f));
	healthBarHeroBG->setTag("healthBarHeroBG");
	objects.push_back(healthBarHeroBG);
	HPBG.push_back(healthBarHeroBG);

	SpriteObject * healthBarMonsterBG = new SpriteObject("Sprite/HPBar.png", 1, 1);
	healthBarMonsterBG->setSize(280.0f, 50.0f);
	healthBarMonsterBG->translate(glm::vec3(340.0f, 200.0f, 0.0f));
	healthBarMonsterBG->setTag("healthBarMonsterBG");
	objects.push_back(healthBarMonsterBG);
	HPBG.push_back(healthBarMonsterBG);

	showMana = new SpriteObject("Sprite/manasprite.png", 1, 7);
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
	endTurn->setTagAndName("InFight", "endTurn");
	endTurn->setSize(130.0f, 100.0f);
	endTurn->translate(glm::vec3(500.0f, -200.0f, 0.0f));
	endTurn->active = false;
	clickable.push_back(endTurn);
	objects.push_back(endTurn);
	
	ClickableObject * optionButton= new ClickableObject;
	optionButton->setSpriteClickableObject(OptionButton, 1, 1);
	optionButton->setEffect(OptionButton, 1, 2);
	optionButton->genUV();
	optionButton->setTagAndName("InFight", "optionButton");
	optionButton->setSize(50.0f, 50.0f);
	optionButton->translate(glm::vec3(600.0f, 320.0f, 0.0f));
	optionButton->active = false;
	clickable.push_back(optionButton);
	objects.push_back(optionButton);

	previewCard = new SpriteObject("Sprite/cardSprite2.png", 2, 10);
	previewCard->setSize(300.0f, 420.0f);
	previewCard->translate(glm::vec3(0.0f, 115.0f, 0.0f));
	previewCard->setTag("previewCard");
	objects.push_back(previewCard);
	previewCard->setActive(false);

	TextObject * HerodamageText = new TextObject();
	HerodamageText->setFontSize(40);
	HerodamageText->setFontName("Damaged.ttf");
	HerodamageText->setPosition(glm::vec3(-400.0f, 60.0f, 0.0f));
	HerodamageText->setTextColor(SDL_Color{ 0,0,255 });
	HerodamageText->loadText("999");
	//myHero->DamageText = HerodamageText;
	objects.push_back(HerodamageText);

	TextObject * EnemydamageText = new TextObject();
	EnemydamageText->setFontSize(40);
	EnemydamageText->setFontName("Damaged.ttf");
	EnemydamageText->setPosition(glm::vec3(400.0f, 60.0f, 0.0f));
	EnemydamageText->setTextColor(SDL_Color{ 0,0,255 });
	EnemydamageText->loadText("999");
	//enemies[0]->DamageText = EnemydamageText;
	objects.push_back(EnemydamageText);
}

void Game::render()	 //Change game scene
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
	if (state == 50)
	{
		this->getRenderer()->render(this->City);
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
	for (DrawableObject* menu : Menu)
	{
		menu->update(deltaTime);
	}
	for (DrawableObject* city : City)
	{
		city->update(deltaTime);
	}
	for (DrawableObject* pause : Pause)
	{
		pause->update(deltaTime);
	}
	for (DrawableObject* obj : objects) {
		obj->update(deltaTime);
	}
	for (Card* card : *(deck->getHand()))
	{
		card->update(deltaTime);
	}

}

void Game::resetHandPos()
{
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
	}

	
}

void Game::endTurn()
{
	state = 2;
	deck->resetPlayedCard();
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

		}								   			//cout << endl << "Hero take Damage:" << damage << endl;
			//cout << endl << "MaxHP: " << myHero->getMaxHP() << endl;
		if (myHero->isAlive() == false)
		{
			heroHp[0]->setSize(0, 20);
			myHero->setActive(false);
			HPBG[0]->setActive(false);
		}
	
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

void Game::setMonster(int HP, string name, int row, int column,int speed)
{
	Monster * Monster1 = new Monster(HP, name, row, column);
	Monster1->setSize(350.0f, -300.0f);
	Monster1->translate(glm::vec3(350.0f, 80.0f, 0.0f));
	Monster1->setAnimationLoop(1, 1, column, speed);
	Monster1->setTag("Monster");
	objects.push_back(Monster1);
	enemies.push_back(Monster1);
}




Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


