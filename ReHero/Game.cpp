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
#include "TextMeshVbo.h"
#include "FloatText.h"

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
		//cout << clickableObject->getTag() << endl;
		cout << clickableObject->getName() << endl;
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "Map"&&state == State::GAME_MAP)
		{
			bool canGo = false;
			int clickedMap = stoi(clickableObject->getName());
			vector<int> connectedMaps = DungeonMap::getInstance()->getConnectedID(currentMap);
			for (int i = 0; i < connectedMaps.size(); i++)
			{
				if (clickedMap == connectedMaps[i])
				{
					canGo = true;
					TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
					TransitionPic->onetime = true;
					TransitionPic->delay = 3000;
					cout <<"tranPos" <<TransitionPic->delay << endl;
					break;
				}
			}
			if (canGo == false) return;
			
			
			
			//check point type
			int mon_id = 0;
			mapPointType Type = DungeonMap::getInstance()->getPointType(clickedMap);
			if (Type == mapPointType::POINT_WEAK_MONSTER)
			{
				mon_id = 3 + rand() % 3;
			}
			if (Type == mapPointType::POINT_STRONG_MONSTER)
			{
				mon_id = 6 + rand() % 3;
			}
			if (Type == mapPointType::POINT_HEALING_WELL)
			{
				mon_id = 9 + rand() % 3;
			}

			//change monster
			switch (mon_id)
			{
				case 0: enemies[0]->changeMonster("Goblin", 10); break;
				case 1: enemies[0]->changeMonster("GoblinElite", 14); break;
				case 2: enemies[0]->changeMonster("GoblinRare", 18); break;
				case 3: enemies[0]->changeMonster("Wasp", 10); break;
				case 4: enemies[0]->changeMonster("WaspElite", 14); break;
				case 5: enemies[0]->changeMonster("WaspRare", 18); break;
				case 6: enemies[0]->changeMonster("Skeleton", 14); break;
				case 7: enemies[0]->changeMonster("SkeletonElite", 20); break;
				case 8: enemies[0]->changeMonster("SkeletonRare", 28); break;
				case 9: enemies[0]->changeMonster("Centipede", 14); break;
				case 10: enemies[0]->changeMonster("CentipedeElite", 20); break;
				case 11: enemies[0]->changeMonster("CentipedeRare", 24); break;
			}
			
			currentMap = clickedMap;
			nextState = State::PLAYER_PLAY;
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
			
		}
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Start"&&state == State::GAME_MAINMENU)
		{
			nextState = State::GAME_CITY;//To City
			//restartGame();
			TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			TransitionPic->onetime = true;
			TransitionPic->delay = 3000;
			
			//BGD->changeSprite("Sprite/City.png", 1, 1);
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Building3"&& state == State::GAME_CITY)
		{
			nextState = State::PLAYER_PLAY;
			TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			TransitionPic->onetime = true;
			TransitionPic->delay =3000;
			//BGD->changeSprite("Sprite/BGD1.png", 1, 1);
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Building1"&& state == State::GAME_CITY)
		{
			nextState = State::GAME_MAP; //Map
			TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			TransitionPic->onetime = true;
			TransitionPic->delay = 3000;
			//BGD->changeSprite("Sprite/MapBG1.png", 1, 1);
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = true;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Quit"&&state == State::GAME_MAINMENU/*in Mainmenu*/)
		{
			state = State::GAME_QUIT; //Exit Game
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "ResumeButton")
		{
			//TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			//TransitionPic->onetime = true;
			//TransitionPic->delay = 3000;
			state = State::PLAYER_PLAY; //Resume Game
			//BGD->changeSprite("Sprite/BGD1.png", 1, 1);
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "optionButton")
		{
			//TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			//TransitionPic->onetime = true;
			//TransitionPic->delay = 3000;
			state = State::GAME_PAUSE; //Pause Game
			//BGD->changeSprite("Sprite/PauseBG.png", 1, 1);
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "Exit" && state == State::GAME_PAUSE/*in Pause*/)
		{
			TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			TransitionPic->onetime = true;
			TransitionPic->delay = 3000;
			nextState = State::GAME_MAINMENU; /*To mainMenu*/
			//BGD->changeSprite("Sprite/MenuBG.png", 1, 1);
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
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = false;
			}
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getName() == "endTurn" && state == State::PLAYER_PLAY)
		{
			endTurn();
		}
		else
		if (clickableObject->isClick(realX, realY) && clickableObject->getTag() == "cardDrop" && state == State::ENEMY_DIE)
		{
			//myHero->victory->setActive(true);
			state = State::TRANSITION;
			cardDropList[0]->active = false;
			cardDropList[1]->active = false;
			//TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			//TransitionPic->onetime = true;
			//TransitionPic->delay = 3000;
			 //Map
			addNewCardToDeck(clickableObject->getName());
			//setMonster(40, "wasp", 4, 5, 400);
			enemies[0]->setHP(1);
			HPBG[0]->setActive(false);
			HPBG[1]->setActive(false);
			statusIcon[0]->setActive(false);
			statusIcon[1]->setActive(false);
			statusIcon[2]->setActive(false);
			statusIcon[3]->setActive(false);
			
			//restartGame();
			//BGD->changeSprite("Sprite/MapBG1.png", 1, 1);
			for (int i = 0; i < clickable.size(); i++)
			{
				if (clickable[i]->getTag() == "Menu")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "City")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "InFight")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Pause")
					clickable[i]->active = false;
				if (clickable[i]->getTag() == "Map")
					clickable[i]->active = true;
			}
			
		}

		clickableObject->onClick(false);
	}
	if (realY < 0.0f||(realY > 0.0f&&state!= State::PLAYER_PLAY))
	{
		if (clickObject)
		{
			clickObject->setIsInHand(true);
			clickObject->setPosition(glm::vec3(0, -240, 0));
			cout << "Reset";
		}
	}
	else if (realY > 0.0f&&state == State::PLAYER_PLAY)
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
			clickObject->effect(myHero, enemies[0]);
			resetHandPos();
			deck->playACard();
			if (state == State::ENEMY_DIE)
			{
				//cout<<endl << "enemy die" << endl;
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
	//for (int i = 0; i < deck->cardsOnHand(); i++) {
	for (int i = deck->cardsOnHand()-1; i >= 0; i--) {
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
				//cout <<endl<< "Click JA " << endl;
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
	if(state==State::PLAYER_PLAY)
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
	else
	if (state == State::ENEMY_DIE)
	{
		for (int i = 1; i >= 0; i--) {
			cardDropList[i]->onClick(false);
		}

		for (int i = 1; i >= 0; i--) {
			ClickableObject* gameObject = cardDropList[i];
			if (gameObject->isClick(realX, realY))
			{
				cardDropList[i]->onClick(true);
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
		//case 'p': if (state == 0) { state = 100; }else if (state == 100) state = 0; cout << state; break;
			
		}
	}
}

void Game::init(int width, int height)
{
	winWidth = width;
	winHeight = height;
	renderer = new GLRendererColor(width, height);
	renderer->initGL("shaders/color/vertext.shd", "shaders/color/fragment.shd");
	state = State::GAME_MAINMENU;
	nextState = State::GAME_CITY;

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
	Music music = audio.loadMusic("Sound_fighting_monster.wav");
	music.play(-1);

	level = 1;
	/////////////////////////////////////////////////////////////////MainMenu/////////////////////////////////////////////////////////////
	SpriteObject * MenuBG = new SpriteObject("Sprite/MenuBG.png", 1, 1);
	MenuBG->setSize(1280.0f, 720.0f);
	MenuBG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//MenuBG->setAnimationLoop(1, 1, 3, 800);
	//MenuBG->setTag("MenuBG");
	//Menu.push_back(MenuBG);
	BGD.push_back(MenuBG);
	BG.push_back(MenuBG);
	
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
	PlayButton->translate(glm::vec3(-300.0f, 100.0f, 0.0f));
	clickable.push_back(PlayButton);
	Menu.push_back(PlayButton);

	/*ClickableObject * LibaryButton = new ClickableObject;
	LibaryButton->setSpriteClickableObject(Libary, 1, 1);
	LibaryButton->setEffect(Libary_Glow, 1, 1);
	LibaryButton->setColumn(1);
	LibaryButton->setRow(1);
	LibaryButton->genUV();
	LibaryButton->setTagAndName("Menu","Libary");
	LibaryButton->setSize(250.0f, 100.0f);
	LibaryButton->translate(glm::vec3(-300.0f, 50.0f, 0.0f));
	clickable.push_back(LibaryButton);
	Menu.push_back(LibaryButton);*/

	ClickableObject * SettingButton = new ClickableObject;
	SettingButton->setSpriteClickableObject(Setting, 1, 1);
	SettingButton->setEffect(Setting_Glow, 1, 1);
	SettingButton->setColumn(1);
	SettingButton->setRow(1);
	SettingButton->genUV();
	SettingButton->setTagAndName("Menu","Setting");
	SettingButton->setSize(250.0f, 100.0f);
	SettingButton->translate(glm::vec3(-300.0f, 0.0f, 0.0f));
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
	QuitButton->translate(glm::vec3(-300.0f, -100.0f, 0.0f));
	clickable.push_back(QuitButton);
	Menu.push_back(QuitButton);

	/*SpriteObject * PauseBG = new SpriteObject("Sprite/PauseBG.png", 1, 1);
	PauseBG->setSize(1280.0f, 720.0f);
	PauseBG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	PauseBG->setAnimationLoop(1, 1, 3, 800);
	PauseBG->setTag("PauseBG");
	//Pause.push_back(PauseBG);		*/

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
	//City.push_back(CityBG);		

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
	
	/////////////////////////////////////////////////////////////////Map/////////////////////////////////////////////////////////////
	SpriteObject mapPoint("Sprite/mapPoint.png", 1, 1);
	SpriteObject mapPointe("Sprite/mapPoint_press.png", 1, 1);
	//SpriteObject mapPointBoss("Sprite/mapPointB.png", 1, 1);
	//SpriteObject mapPointBossP("Sprite/mapPointB_press.png", 1, 1);
	SpriteObject mapPointStart("Sprite/mapPointS.png", 1, 1);
	SpriteObject mapPointStartP("Sprite/mapPointS_press.png", 1, 1);
	
	ClickableObject ** MapForest = new ClickableObject*[15];
	for (int i = 0; i < 15; i++)
	{
		MapForest[i] = new ClickableObject;
		MapForest[i]->setSpriteClickableObject(mapPoint, 1, 1);
		MapForest[i]->setEffect(mapPointe, 1, 1);
		MapForest[i]->genUV();
		MapForest[i]->setTagAndName("Map", to_string(i));
		MapForest[i]->setSize(100.0f, 100.0f);
		MapForest[i]->active = false;
		clickable.push_back(MapForest[i]);
		Map.push_back(MapForest[i]);
	}
	
	MapForest[0]->translate(glm::vec3(-494.0f, -18.0f, 0.0f));
	MapForest[0]->setSpriteClickableObject(mapPointStart, 1, 1);
	MapForest[0]->setEffect(mapPointStartP, 1, 1);
	MapForest[0]->genUV();
	MapForest[1]->translate(glm::vec3(-300.0f, 140.0f, 0.0f));
	MapForest[2]->translate(glm::vec3(-300.0f, -10.0f, 0.0f));
	MapForest[3]->translate(glm::vec3(-300.0f, -160.0f, 0.0f));
	MapForest[4]->translate(glm::vec3(-120.0f, 230.0f, 0.0f));
	MapForest[5]->translate(glm::vec3(-93.0f, -12.0f, 0.0f));
	MapForest[6]->translate(glm::vec3(-68.0f, -260.0f, 0.0f));
	MapForest[7]->translate(glm::vec3(-2.0f, 140.0f, 0.0f));
	MapForest[8]->translate(glm::vec3(3.0f, -132.0f, 0.0f));
	MapForest[9]->translate(glm::vec3(307.0f, -190.0f, 0.0f));
	MapForest[10]->translate(glm::vec3(149.0f, 91.0f, 0.0f));
	MapForest[11]->translate(glm::vec3(165.0f, -110.0f, 0.0f));
	MapForest[12]->translate(glm::vec3(305.0f, 135.0f, 0.0f));
	MapForest[13]->translate(glm::vec3(297.0f, -19.0f, 0.0f));
	MapForest[14]->translate(glm::vec3(497.0f, -17.0f, 0.0f));
	MapForest[14]->setSize(150.0f, 150.0f);
	//MapForest[14]->setSpriteClickableObject(mapPointBoss, 1, 1);
	//MapForest[14]->setEffect(mapPointBossP, 1, 1);
	MapForest[14]->genUV();	
	

	/////////////////////////////////////////////////////////////////Map/////////////////////////////////////////////////////////////

	/*SpriteObject * BG = new SpriteObject("Sprite/BGD1.png", 1, 1);
	BG->setSize(1280.0f, 720.0f);
	BG->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	BG->setAnimationLoop(1, 1, 3, 800);
	BG->setTag("BG");
	objects.push_back(BG); */

	setMonster(10,"Goblin", 4, 5,400);
	//delete enemies[0];
	//setMonster(20, "wasp", 1, 5,1000);

	myHero = new Hero(20, "Sprite/Hero2.png", 6, 8);
	myHero->setSize(450.0f, -300.0f);
	myHero->translate(glm::vec3(-350.0f, 80.0f, 0.0f));
	myHero->setAnimationLoop(1, 1, 7, 1500);
	myHero->setTag("Hero");
	objects.push_back(myHero);

	effectOnPlayer = new SpriteObject("Sprite/effect.png", 11, 9);
	effectOnPlayer->setSize(650.0f, 650.0f);
	effectOnPlayer->translate(glm::vec3(-400.0f, 200.0f, 0.0f));
	effectOnPlayer->setAnimationLoop(1, 1, 7, 900);
	effectOnPlayer->setActive(false);
	effectOnPlayer->setTag("Effect");
	objects.push_back(effectOnPlayer); 

	effectOnEnemy= new SpriteObject("Sprite/effect.png", 11, 9);
	effectOnEnemy->setSize(650.0f, 650.0f);
	effectOnEnemy->translate(glm::vec3(400.0f, 200.0f, 0.0f));
	effectOnEnemy->setAnimationLoop(1, 1, 7, 900);
	effectOnEnemy->setActive(false);
	effectOnEnemy->setTag("Effect");
	objects.push_back(effectOnEnemy);

	float colorR[5] = { 1.0f,0.0f,0.0f,1.0f,1.0f };
	float colorG[5] = { 0.0f,1.0f,0.0f,1.0f,0.0f };
	float colorB[5] = { 0.0f,0.0f,1.0f,0.0f,1.0f };
	string name[5] = { "RED","GREEN","BLUE","YELLOW","PINK" };
	int spriteNum = 10;
	int allCard = 20;
	cardsprite1 = new SpriteObject("Sprite/cardSprite1.png", cardSpriteRow, 10);
	cardsprite2 = new SpriteObject("Sprite/cardSprite2.png", cardSpriteRow, 10);
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
		//cout << names << endl;
		PlayerDeck.push_back(names);
	}

	datafile.close();

	for (int c = 0; c < PlayerDeck.size(); c++)
	{
		//cout << c << endl;
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
			//getline(datafile, names, '\t');
			datafile >> names >> row >> column;
			//cout << PlayerDeck[c] << " " << names << endl;
			if (PlayerDeck[c] == names)
			{
				//cout << names << " " << row << " " << column << endl;
				Card * card = new Card();
				card->setId(id++);
				card->setName(names);
				card->setSpriteCard(cardsprite1, cardSpriteRow, 10);
				card->setEffectCard(cardsprite2, cardSpriteRow, 10);
				deck->addCardToDeck(card);
				card->setColumn(column);
				//cout << column << endl<<row<<endl;
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

	SpriteObject * shieldHero = new SpriteObject("Sprite/metal_shield.png", 1, 1);
	shieldHero->setSize(60.0f, 60.0f);
	shieldHero->translate(glm::vec3(-470.0f, 200.0f, 0.0f));
	shieldHero->setTag("shieldHero");
	objects.push_back(shieldHero);
	statusIcon.push_back(shieldHero);

	SpriteObject * shieldMonster = new SpriteObject("Sprite/metal_shield.png", 1, 1);
	shieldMonster->setSize(60.0f, 60.0f);
	shieldMonster->translate(glm::vec3(230.0f, 200.0f, 0.0f));
	shieldMonster->setTag("shieldMonster");
	objects.push_back(shieldMonster);
	statusIcon.push_back(shieldMonster);

	SpriteObject * poisonHero = new SpriteObject("Sprite/icon_poison.png", 1, 1);
	poisonHero->setSize(40.0f, 40.0f);
	poisonHero->translate(glm::vec3(-200.0f, 200.0f, 0.0f));
	poisonHero->setTag("poisonHero");
	objects.push_back(poisonHero);
	statusIcon.push_back(poisonHero);

	SpriteObject * poisonMonster = new SpriteObject("Sprite/icon_poison.png", 1, 1);
	poisonMonster->setSize(40.0f, 40.0f);
	poisonMonster->translate(glm::vec3(500.0f, 200.0f, 0.0f));
	poisonMonster->setTag("poisonMonster");
	objects.push_back(poisonMonster);
	statusIcon.push_back(poisonMonster);

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

	previewCard = new SpriteObject("Sprite/cardSprite2.png", cardSpriteRow, 10);
	previewCard->setSize(300.0f, 420.0f);
	previewCard->translate(glm::vec3(0.0f, 115.0f, 0.0f));
	previewCard->setTag("previewCard");
	objects.push_back(previewCard);
	previewCard->setActive(false);

	SpriteObject cardDrop("Sprite/cardSprite1.png", cardSpriteRow, 10);
	SpriteObject cardDropeffect("Sprite/cardSprite2.png", cardSpriteRow, 10);
	

	ClickableObject * cardDrop1 = new ClickableObject;
	cardDrop1->setSpriteClickableObject(cardDrop, cardSpriteRow, 10);
	cardDrop1->setEffect(cardDropeffect, cardSpriteRow, 10);
	cardDrop1->setColumn(1);
	cardDrop1->setRow(1);
	cardDrop1->genUV();
	cardDrop1->setTagAndName("cardDrop", "one");
	cardDrop1->setSize(280.0f, 400.0f);
	cardDrop1->translate(glm::vec3(-200.0f, -50.0f, 0.0f));
	cardDrop1->active = false;
	clickable.push_back(cardDrop1);
	objects.push_back(cardDrop1);
	cardDropList.push_back(cardDrop1);

	ClickableObject * cardDrop2 = new ClickableObject;
	cardDrop2->setSpriteClickableObject(cardDrop, cardSpriteRow, 10);
	cardDrop2->setEffect(cardDropeffect, cardSpriteRow, 10);
	cardDrop2->setColumn(1);
	cardDrop2->setRow(1);
	cardDrop2->genUV();
	cardDrop2->setTagAndName("cardDrop", "two");
	cardDrop2->setSize(280.0f, 400.0f);
	cardDrop2->translate(glm::vec3(200.0f, -50.0f, 0.0f));
	cardDrop2->active = false;
	clickable.push_back(cardDrop2);
	objects.push_back(cardDrop2);
	cardDropList.push_back(cardDrop2);


	for (int i = 0; i < 5; i++)
	{
		FloatText* tempTexts = new FloatText();
		tempTexts->setFontName("Damaged.ttf");
		tempTexts->setFontSize(400);
		tempTexts->setTextColor(SDL_Color{ 0,0,255 });
		tempTexts->loadText(" ");
		tempText.push_back(tempTexts);	
	}

	TextObject* ShieldTextH = new TextObject();
	ShieldTextH->setFontName("Damaged.ttf");
	ShieldTextH->setPosition(glm::vec3(-470.0f,200.0f,0.0f));
	ShieldTextH->setFontSize(50);
	ShieldTextH->setTextColor(SDL_Color{ 0,0,0 });	 //bgr
	ShieldTextH->loadText(" ");
	ShieldTextHe = ShieldTextH;
	objects.push_back(ShieldTextH);

	TextObject* ShieldTextM = new TextObject();
	ShieldTextM->setFontName("Damaged.ttf");
	ShieldTextM->setPosition(glm::vec3(230.0f, 200.0f, 0.0f));
	ShieldTextM->setFontSize(50);
	ShieldTextM->setTextColor(SDL_Color{ 0,0,0 });
	ShieldTextM->loadText(" ");
	ShieldTextMo = ShieldTextM;
	objects.push_back(ShieldTextM);

	TextObject* PoisionTextH = new TextObject();
	PoisionTextH->setFontName("Damaged.ttf");
	PoisionTextH->setPosition(glm::vec3(-200.0f, 200.0f, 0.0f));
	PoisionTextH->setFontSize(50);
	PoisionTextH->setTextColor(SDL_Color{ 0,0,0 });	 //bgr
	PoisionTextH->loadText(" ");
	PoisionTextHe = PoisionTextH;
	objects.push_back(PoisionTextH);

	TextObject* PoisionTextM = new TextObject();
	PoisionTextM->setFontName("Damaged.ttf");
	PoisionTextM->setPosition(glm::vec3(500.0f, 200.0f, 0.0f));
	PoisionTextM->setFontSize(50);
	PoisionTextM->setTextColor(SDL_Color{ 0,0,0 });
	PoisionTextM->loadText(" ");
	PoisionTextMo = PoisionTextM;
	objects.push_back(PoisionTextM); 


	SpriteObject * Victory = new SpriteObject("Sprite/victory.png", 1, 1);
	Victory->setSize(800.0f, 400.0f);
	Victory->translate(glm::vec3(0.0f, 220.0f, 0.0f));
	Victory->setTag("Victory");
	Victory->setActive(false);
	enemies[0]->victory = Victory;
	objects.push_back(Victory);

	SpriteObject * defeated = new SpriteObject("Sprite/defeated.png", 1, 1);
	defeated->setSize(800.0f, 400.0f);
	defeated->translate(glm::vec3(0.0f, 220.0f, 0.0f));
	defeated->setTag("Victory");
	defeated->setActive(false);
	myHero->lose = defeated;
	objects.push_back(defeated);
	
	TransitionPic = new Transition("transition", 1, 1);
	TransitionPic->setSize(3000.0f, 720.0f);
	TransitionPic->translate(glm::vec3(-2180.0f, 0.0f, 0.0f));
	transitionsss.push_back(TransitionPic); 

	SpriteObject * Map1 = new SpriteObject("Sprite/BGD2.png", 1, 1);
	Map1->setSize(1280.0f, 720.0f);
	Map1->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	SpriteObject * Map2 = new SpriteObject("Sprite/PauseBG.png", 1, 1);
	Map2->setSize(1280.0f, 720.0f);
	Map2->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	SpriteObject * Map3 = new SpriteObject("Sprite/City.png", 1, 1);
	Map3->setSize(1280.0f, 720.0f);
	Map3->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	SpriteObject * Map4 = new SpriteObject("Sprite/MapBG1.png", 1, 1);
	Map4->setSize(1280.0f, 720.0f);
	Map4->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	
	BGD.push_back(Map1);
	BGD.push_back(Map2);
	BGD.push_back(Map3);
	BGD.push_back(Map4);
}

void Game::render()	 //Change game scene
{
	this->getRenderer()->render(this->transitionsss);
	//cout << "tranpos" << TransitionPic->getTranformX() << endl;
	if (state == State::GAME_MAINMENU)
	{
		this->getRenderer()->Clear();
		BG[0] = BGD[0];
		this->getRenderer()->render(this->BG);
		this->getRenderer()->render(this->Menu);
		this->getRenderer()->render(this->transitionsss);
	}
	else
	if (state == State::GAME_PAUSE)
	{
		this->getRenderer()->Clear();
		BG[0] = BGD[2];
		this->getRenderer()->render(this->BG);
		this->getRenderer()->render(this->Pause);
		this->getRenderer()->render(this->transitionsss);
	}
	else
	if (state == State::GAME_CITY)
	{
		this->getRenderer()->Clear();
		BG[0] = BGD[3];
		this->getRenderer()->render(this->BG);
		this->getRenderer()->render(this->City);
		this->getRenderer()->render(this->transitionsss);
	}
	else
	if (state == State::GAME_MAP)
	{
		this->getRenderer()->Clear();
		BG[0] = BGD[4];
		this->getRenderer()->render(this->BG);
		this->getRenderer()->render(this->Map);
		this->getRenderer()->render(this->transitionsss);
	}
	else
	{
		this->getRenderer()->Clear();
		BG[0] = BGD[1];
		this->getRenderer()->render(this->BG);
		this->getRenderer()->render(this->objects);
		this->getRenderer()->render(this->tempText);
		if(state != State::ENEMY_DIE && state != State::TRANSITION)
		this->getRenderer()->render(*(deck->getHand()));
		this->getRenderer()->render(this->transitionsss);
	}
}



void Game::update(float deltaTime)
{
	for (DrawableObject* tran : transitionsss)
	{
		tran->update(deltaTime);
	}
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
	for (DrawableObject* map : Map) {
		map->update(deltaTime);
	}
	for (DrawableObject* obj : objects) {
		obj->update(deltaTime);
	}
	for (SpriteObject* obj : statusIcon) {
		if (state == State::PLAYER_DIE || state == State::ENEMY_DIE || state == State::TRANSITION)
		{
			obj->setActive(false);
			continue;
		}
		if (obj->getTag() == "shieldHero" && myHero->getArmor() <= 0) obj->setActive(false);
		else if (obj->getTag() == "shieldMonster" && enemies[0]->getArmor() <= 0) obj->setActive(false);
		else if (obj->getTag() == "poisonHero" && myHero->getPoison() <= 0) obj->setActive(false);
		else if (obj->getTag() == "poisonMonster" && enemies[0]->getPoison() <= 0) obj->setActive(false);
		else obj->setActive(true);
	}
	for (Card* card : *(deck->getHand()))
	{
		card->update(deltaTime);
	}
	for (DrawableObject* text : tempText) {
		text->update(deltaTime);
	}

	if (Game::getInstance()->state == State::PLAYER_RANDOM_MANA)
	{
		deck->randomMana();
		showMana->setColumn(deck->getMana() + 1);
		showMana->genUV();
		Game::getInstance()->state = State::PLAYER_SHOW_RANDOM_MANA;
	}
	if (Game::getInstance()->state == State::PLAYER_DRAW)
	{
		deck->drawToHand(5);
		resetHandPos();
		myHero->startTurn();
	}
	if (state == State::ENEMY_DIE)
	{
		//cout << endl << "enemy die" << endl;
		monsterHp[0]->setSize(0, 20);
		enemies[0]->setActive(false);
		HPBG[1]->setActive(false);
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
			if (i == drop1)
				dropList.push_back(names);
			if (i == drop2)
				dropList.push_back(names);
		}
		datafile.close();

		ifstream datafiles("cardSpriteData.txt");
		if (!datafiles)
		{
			cout << "fail to load cardEffectData " << endl;
			return;
		}
		int row;
		int column;
		datafiles >> howManycard;
		for (int k = 0; k < 2; k++)
		{
			for (int i = 0; i < howManycard; i++)
			{
				//getline(datafile, names, '\t');
				datafiles >> names >> row >> column;
				//cout << PlayerDeck[c] << " " << names << endl;
				if (dropList[k] == names)
				{
					cardDropList[k]->setTagAndName("cardDrop", names);
					cardDropList[k]->setColumn(column);
					cardDropList[k]->setRow(row);
					cardDropList[k]->genUV();
					cardDropList[k]->active = true;
					break;
				}
			}
		}
		dropList.clear();
		
		datafiles.close();
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
	state = State::PLAYER_END_TURN;
	deck->resetPlayedCard();
	deck->discardHand();
	enemies[0]->startTurn();
	monsterTurn();
}

void Game::monsterTurn()
{
		if (myHero->isAlive() == true)
		{
			enemies[0]->randomUseSkill(myHero, nullptr);
		}	
		//cout << endl << "Hero take Damage:" << damage << endl;
		//cout << endl << "MaxHP: " << myHero->getMaxHP() << endl;
		if (myHero->isAlive() == false)
		{
			heroHp[0]->setSize(0, 20);
			//myHero->setActive(false);
			HPBG[0]->setActive(false);
		}
	
}

void Game::addNewCardToDeck(string cardName)
{
	ifstream datafile("cardSpriteData.txt");
	if (!datafile)
	{
		cout << "fail to load cardEffectData " << endl;
		return;
	}
	string names;
	int row;
	int column;
	int howManyCard;
	datafile >> howManyCard;
	for (int i = 0; i < howManyCard; i++)
	{
		//getline(datafile, names, '\t');
		datafile >> names >> row >> column;
		//cout << PlayerDeck[c] << " " << names << endl;
		if (cardName == names)
		{
			//cout << names << " " << row << " " << column << endl;
			Card * card = new Card();
			card->setId(id++);
			card->setName(names);
			card->setSpriteCard(cardsprite1, cardSpriteRow, 10);
			card->setEffectCard(cardsprite2, cardSpriteRow, 10);
			deck->addCardToDeck(card);
			card->setColumn(column);
			//cout << column << endl<<row<<endl;
			card->setRow(row);
			card->genUV();
			break;
		}
	}
	datafile.close();
}

void Game::addNewCardToDeck(Card * c) {
	deck->addCardToDeck(c);
}

void Game::restartGame()
{
	drop1 = rand() % 40;
	drop2 = rand() % 40;
	cout << "Reset" << endl; 
	//endTurn();
	/*if (state != State::ENEMY_DIE)
	{
		myHero->setMaxHP(20);
		myHero->setHP(20);
		myHero->takeDamage(0);
		heroHp[0]->setPosition(glm::vec3(-350.0f, 200.0f, 0.0f));
	}  */
	//enemies[0]->setMaxHP(20);
	//enemies[0]->setHP(20);
	myHero->takeDamage(0);
	enemies[0]->takeDamage(0);
	monsterHp[0]->setPosition(glm::vec3(350.0f, 200.0f, 0.0f));
	HPBG[0]->setActive(true);
	HPBG[1]->setActive(true);
	deck->randomMana();
	showMana->setColumn(deck->getMana()+1);
	showMana->genUV();
	deck->resetPlayedCard();
	deck->discardHand();
	deck->drawToHand(5);
	resetHandPos();
	myHero->setActive(true);
	enemies[0]->setActive(true);
	
	myHero->curePoison();
	enemies[0]->curePoison();
	myHero->startTurn();
	//enemies[0]->startTurn();
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

void Game::drawText(string text, glm::vec3 pos, int fontSize, int color)
{
	/*FloatText* tempText = new FloatText();
	tempText->setFontName("Damaged.ttf");
	tempText->setFontSize(fontSize);
	tempText->setTextColor(SDL_Color{ 255,0,0 });
	tempText->loadText(text);
	tempText->setPosition(pos);
	Text.push_back(tempText);*/
	for (int i = 0; i < tempText.size(); i++)
	{
		FloatText *Text = dynamic_cast<FloatText *>(tempText[i]);
		if (Text->isRunning == false)
		{
			if (color == 0)
			{
				Text->setTextColor(SDL_Color{ 255,0,0 });
			}
			else
			if (color == 1)
			{
				Text->setTextColor(SDL_Color{ 0,255,0 });
			}	
			else if (color == 2)
			{
				Text->setTextColor(SDL_Color{ 0,0,255 });
			}
			else if (color == 3)
			{
				Text->setTextColor(SDL_Color{ 255,255,255 });
			}
			else 
			{
				Text->setTextColor(SDL_Color{ 0,0,0 });
			}
			Text->setPosition(pos);
			Text->setFontSize(300.0f);
			Text->changeSize(fontSize);
			Text->loadText(text);
			Text->isRunning = true;

			break;
		}
	}

}

void Game::drawEffectText(string text, int type, int color, bool isActive)
{
	// type 0 = ShieldTextHero
	// type 1 = PoisonTextHero
	// type 2 = ShieldTextMonster
	// type 3 = PoisonTextMonster
	if (type == 0)
	{
		TextObject *Text = dynamic_cast<TextObject *>(ShieldTextHe);
		Text->loadText(text);
		if (color == 0)
		{
			Text->setTextColor(SDL_Color{ 255,0,0 });
		}
		else
			if (color == 1)
			{
				Text->setTextColor(SDL_Color{ 0,255,0 });
			}
			else if (color == 2)
			{
				Text->setTextColor(SDL_Color{ 0,0,255 });
			}
			else if (color == 3)
			{
				Text->setTextColor(SDL_Color{ 255,255,255 });
			}
			else
			{
				Text->setTextColor(SDL_Color{ 0,0,0 });
			}
		if (isActive == true)
		{
			Text->setActive(true);
		}
		else
			Text->setActive(false);
	}

	if (type == 1)
	{
		TextObject *Text = dynamic_cast<TextObject *>(PoisionTextHe);
		Text->loadText(text);
		if (color == 0)
		{
			Text->setTextColor(SDL_Color{ 255,0,0 });
		}
		else
			if (color == 1)
			{
				Text->setTextColor(SDL_Color{ 0,255,0 });
			}
			else
			{
				Text->setTextColor(SDL_Color{ 0,0,255 });
			}
		if (isActive == true)
		{
			Text->setActive(true);
		}
		else
			Text->setActive(false);
	}

	if (type == 1)
	{
		TextObject *Text = dynamic_cast<TextObject *>(PoisionTextHe);
		Text->loadText(text);
		if (color == 0)
		{
			Text->setTextColor(SDL_Color{ 255,0,0 });
		}
		else
			if (color == 1)
			{
				Text->setTextColor(SDL_Color{ 0,255,0 });
			}
			else
			{
				Text->setTextColor(SDL_Color{ 0,0,255 });
			}
		if (isActive == true)
		{
			Text->setActive(true);
		}
		else
			Text->setActive(false);
	}

	if (type == 2)
	{
		TextObject *Text = dynamic_cast<TextObject *>(ShieldTextMo);
		Text->loadText(text);
		if (color == 0)
		{
			Text->setTextColor(SDL_Color{ 255,0,0 });
		}
		else
			if (color == 1)
			{
				Text->setTextColor(SDL_Color{ 0,255,0 });
			}
			else
			{
				Text->setTextColor(SDL_Color{ 0,0,255 });
			}
		if (isActive == true)
		{
			Text->setActive(true);
		}
		else
			Text->setActive(false);
	}

	if (type == 3)
	{
		TextObject *Text = dynamic_cast<TextObject *>(PoisionTextMo);
		Text->loadText(text);
		if (color == 0)
		{
			Text->setTextColor(SDL_Color{ 255,0,0 });
		}
		else
		if (color == 1)
		{
			Text->setTextColor(SDL_Color{ 0,255,0 });
		}
		else
		{
			Text->setTextColor(SDL_Color{ 0,0,255 });
		}
		if (isActive == true)
		{
			Text->setActive(true);
		}
		else
			Text->setActive(false);
	}

}

//---------------------------------- for Hero --------------------------------------

void Game::doDamage(int damage) {
	if (enemies[0] != nullptr)
	{
		enemies[0]->takeDamage(damage);
		cout << "Hero deal " << damage << " damage to monster" << endl;
	}
	else cout << "Hero deal " << damage << " damage, but no target!" << endl;

}

void Game::heal(int amount) {
	myHero->getHeal(amount);
	cout << "Hero heal " << amount << endl;
}

void Game::usePoison(int amount) {
	if (enemies[0] != nullptr)
	{
		enemies[0]->takePoison(amount);
		cout << "Hero use poison " << amount << " to monster" << endl;
	}
	else cout << "Hero use poison " << amount << ", but no target!" << endl;
}

void Game::gainArmor(int amount) {
	myHero->gainArmor(amount);
	cout << "Hero gain " << amount << " armor" << endl;
}

void Game::drawCard(int amount) {
	Deck * playDeck = Deck::getInstance();
	if (playDeck != nullptr)
	{
		playDeck->drawToHand(amount);
		resetHandPos();
		cout << "Hero draw " << amount << " card" << endl;
	}
	else cout << "Deck cannot be found" << endl;
}

void Game::ChangeState(int i)
{
	if (i == 1)
	{
		nextState = State::GAME_CITY;//To City
		TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
		TransitionPic->onetime = true;
		TransitionPic->delay = 3000;
		//restartGame();

		for (int i = 0; i < clickable.size(); i++)
		{
			if (clickable[i]->getTag() == "Menu")
				clickable[i]->active = false;
			if (clickable[i]->getTag() == "City")
				clickable[i]->active = true;
			if (clickable[i]->getTag() == "InFight")
				clickable[i]->active = false;
			if (clickable[i]->getTag() == "Pause")
				clickable[i]->active = false;
			if (clickable[i]->getTag() == "Map")
				clickable[i]->active = false;
		}
	}

}


//----------------------------------------------------------------------------------

Game::~Game() {
	delete cardsprite1;
	delete cardsprite2;
	delete Deck::getInstance();
}


Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


