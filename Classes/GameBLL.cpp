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
	//创建 根Scene
	auto scene = Scene::create();

	//加入 游戏场景	
	auto m_GameScene = GameScene::create();
	scene->addChild(m_GameScene);

	//加入 UI层
	auto uiLayer = GameUILayer::create();
	scene->addChild(uiLayer,1);
	uiLayer->setGlobalZOrder(1); //为了让宝石出现在UI层之上


	return scene;
}

void GameBLL::gameRestart()
{
	destoryInstance();

	auto scene = createGame(nullptr);
	Director::getInstance()->replaceScene(scene);
}

//暂停游戏
void GameBLL::gamePause()
{

}

//继续游戏
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