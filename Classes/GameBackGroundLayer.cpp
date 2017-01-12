#include "GameBackGroundLayer.h"

bool GameBackGroundLayer::init()
{
	if (!Layer::init())return false;

	m_RootNode = static_cast<Layout*>(CSLoader::createNode("GameScene.csb"));
	addChild(m_RootNode);



	return true;
}