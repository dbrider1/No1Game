#include "MainScene.h"
#include "GameBLL.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::create();

	auto layer = Layer::create();

	scene->addChild(layer);

	return scene;
}

bool  MainScene::init()
{
	if (!Scene::init())return false;

	auto s = Director::getInstance()->getWinSize();

	m_RootNode = static_cast<Layout*>(CSLoader::createNode("LogoScene.csb"));
	this->addChild(m_RootNode);
	
	//读取数据
	const auto& A = m_RootNode->getChildren();
	for (auto& B : A)
	{
		auto C = static_cast<Widget*>(B);
		log("Type = %s Name = %s", C->getDescription().c_str(), C->getName().c_str());
	}

	auto btnSet = static_cast<ui::Button*>(Helper::seekWidgetByName(m_RootNode,"Button_Set"));
	btnSet->addTouchEventListener(CC_CALLBACK_2(MainScene::btnSet,this));

	auto btnBegin = Button::create();
	btnBegin->setPosition(Vec2(240.f,150.f));
	btnBegin->loadTextureNormal("images/huang_1.png");
	btnBegin->loadTexturePressed("images/huang_2.png");
	addChild(btnBegin,1);
	btnBegin->addTouchEventListener( CC_CALLBACK_2(MainScene::btnBeginGame,this) );

	addSpark();

	return true;
}

void MainScene::btnBeginGame(Ref* ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)return;

	auto scene = GameBLL::getInstance()->createGame(nullptr);
	Director::getInstance()->replaceScene(scene);
}

void MainScene::btnSet(Ref* ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)return;
	log("进入设置界面");
}


void  MainScene::addSpark()
{
	auto light = Sprite::create("images/guangyun.png");
	addChild(light,0);
	light->setPosition(Vec2(280.f,150.f));
	//light->setScale(0.3);

	auto rotate = RotateBy::create(3,360);
	auto action = RepeatForever::create(rotate);

	light->runAction(action);
}