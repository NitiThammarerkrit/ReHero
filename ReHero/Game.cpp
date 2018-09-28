#include "stdafx.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "GameObject.h"

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

void Game::handleMouse(int x, int y)
{
	float realX, realY;
	realX = (gameWidth/-2.0f) + x * (gameWidth/ winWidth);
	realY = (gameHeight / -2.0f) + (winHeight - y) * (gameHeight / winHeight);
	
	//cout << "X : " << realX << endl << "Y : " << realY << endl;
	for (DrawableObject* obj : objects) {
		Card* gameObject = dynamic_cast<Card*>(obj);
		if (gameObject)
		{
			if (gameObject->isClick(realX, realY))
			{
				cout << "Click : " << gameObject->getName() << endl;
			}
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

	SquareMeshVbo * square = new SquareMeshVbo();
	square->loadData();
	renderer->addMesh(SquareMeshVbo::MESH_NAME, square);
	float colorR[5] = {1.0f,0.0f,0.0f,1.0f,1.0f};
	float colorG[5] = {0.0f,1.0f,0.0f,1.0f,0.0f};
	float colorB[5] = {0.0f,0.0f,1.0f,0.0f,1.0f};
	string name[5] = { "RED","GREEN","BLUE","YELLOW","PINK" };
	for (int i = -2; i <= 2; i++)
	{
		Card * obj = new Card();
		obj->setColor(colorR[i+2], colorG[i+2], colorB[i+2]);
		obj->setSize(200.0f, 280.0f);
		obj->setName(name[i+2]);
		obj->translate(glm::vec3(0, -240, i));
		obj->setCardAngle( i * 20.0f);
		//obj->translate(glm::vec3(-120, -300, 0));
		//obj->rotate(45.0f);
		objects.push_back(obj);
	}


	Card * obj6 = new Card();
	obj6->setColor(0.0, 0.0, 0.0);
	obj6->setSize(160.0f, 246.0f);
	obj6->setName("Monster");
	obj6->translate(glm::vec3(350, 75, 0));
	//obj6->rotate(-45.0f);
	objects.push_back(obj6);

	Card * obj7 = new Card();
	obj7->setColor(0.0, 0.0, 0.0);
	obj7->setSize(160.0f, 246.0f);
	obj7->setName("Hero");
	obj7->translate(glm::vec3(-350, 75, 0));
	//obj6->rotate(-45.0f);
	objects.push_back(obj7);

}

void Game::render()
{
	this->getRenderer()->render(this->objects);
}

Game::Game()
{
	for (DrawableObject* obj : objects) {
		delete obj;
	}
	renderer = nullptr;
}


