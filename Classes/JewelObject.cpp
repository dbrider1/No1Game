#include "JewelObject.h"

#define TOTAL_JEWEL (5)

static const char* JewelNormal[TOTAL_JEWEL]{
	"hongse.png",
	"huangse.png",
	"lanse.png",
	"lvse.png",
	"zise.png"
};

//随机产生jewel
JewelObject* JewelObject::createJewel(int row, int col)
{
	auto jewel = new JewelObject();
	
	if (jewel && jewel->init())
	{
		//m_row 并非是长度，而是据此得到它的坐标，就相当与结构体中的属性！这样每个宝石都知道位置和index
		jewel->m_row = row;
		jewel->m_col = col;
		//jewel->m_ImgIndex = (int)random()% TOTAL_JEWEL;可以运行，但是不能打包，因为类型不明确
		jewel->m_ImgIndex = random(0,TOTAL_JEWEL-1);
		
		
		
		//从帧缓存中拿到图片
		
		//方案1
		//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( JewelNormal[jewel->m_ImgIndex] );
		//jewel->initWithSpriteFrame(frame);
		
		//方案2
		jewel->initWithSpriteFrameName(JewelNormal[jewel->m_ImgIndex]);
		
		jewel->autorelease();
		return jewel;
	}

	CC_SAFE_DELETE(jewel);
	return nullptr;
}

float JewelObject::getJewelWidth()
{
	//加载plist文件到资源中(因为是单例，所以加入一个就是整个程序都可以访问的到，先调用此函数，所以必须在这里初始化plist文件)
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/jewels.plist");

	auto sprite = Sprite::createWithSpriteFrameName(JewelNormal[0]);

	auto width = sprite->getContentSize().width;

	return width;
}

bool JewelObject::init()
{
	if (!Sprite::init())return  false;

	

	return true;
}