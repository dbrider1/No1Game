#include "GameUILayer.h"
#include "GameBLL.h"

bool GameUILayer::init()
{
	if (!Layer::init())return false;

	m_RootNode = static_cast<Layout*>(CSLoader::createNode("GameUILayer.csb"));//创建和掉落宝石
	addChild(m_RootNode);

	auto chkPause = static_cast<ui::CheckBox*>(Helper::seekWidgetByName(m_RootNode,"CheckBox_Pause"));
	chkPause->addEventListener(CC_CALLBACK_2(GameUILayer::checkPause,this));

	auto btnReTry = static_cast<ui::Button*>(Helper::seekWidgetByName(m_RootNode,"Button_ReStart"));
	btnReTry->addTouchEventListener(CC_CALLBACK_2(GameUILayer::btnRetry,this));
	//const auto& A = m_RootNode->getChildren();
	//for (auto& B : A)
	//{
	//	auto C = static_cast<Widget*>(B);
	//	log("Type = %s Name = %s", C->getDescription().c_str(), C->getName().c_str());
	//}

	return true;
}

void  GameUILayer::checkPause(Ref* sender, CheckBox::EventType type)
{
	if (type == CheckBox::EventType::SELECTED)
	{
		log("Resume Game");
	}
	else{
		log("Pause Game");
	}
}

void GameUILayer::btnRetry(Ref* ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)return;
	log("restart Game");

	GameBLL::getInstance()->gameRestart();
}

