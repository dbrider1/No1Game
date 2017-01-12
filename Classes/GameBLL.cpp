#include "GameBLL.h"
#include "GameWinLayer.h"
#include "GameOverLayer.h"
#include "GameScene.h"
#include "GameUILayer.h"





static GameBLL* pInStance = nullptr;

GameBLL::GameBLL()
{
	
}

GameBLL::~GameBLL()
{

}

GameBLL * GameBLL::getInstance()
{
	if (pInStance==nullptr)
	{
		pInStance = new GameBLL();
	}

	return pInStance;
}

void  GameBLL::destoryInstance()
{
	if (pInStance)
	{
		CC_SAFE_DELETE(pInStance);
	}
}

Scene * GameBLL::createGame(void* level)
{
	//���� ��Scene
	auto scene = Scene::create();

	//���� ��Ϸ����	
	auto m_GameScene = GameScene::create();
	scene->addChild(m_GameScene);

	//���� UI��
	auto uiLayer = GameUILayer::create();
	scene->addChild(uiLayer,1);
	uiLayer->setGlobalZOrder(1); //Ϊ���ñ�ʯ������UI��֮��


	return scene;
}

void GameBLL::gameRestart()
{
	destoryInstance();

	auto scene = createGame(nullptr);
	Director::getInstance()->replaceScene(scene);
}

//��ͣ��Ϸ
void GameBLL::gamePause()
{

}

//������Ϸ
void GameBLL::gameResume()
{

}

void GameBLL::gameWin()
{

}

void GameBLL::gameOver()
{

}

void GameBLL::gameQuit()
{


}