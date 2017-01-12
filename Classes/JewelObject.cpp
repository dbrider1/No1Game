#include "JewelObject.h"

#define TOTAL_JEWEL (5)

static const char* JewelNormal[TOTAL_JEWEL]{
	"hongse.png",
	"huangse.png",
	"lanse.png",
	"lvse.png",
	"zise.png"
};

//�������jewel
JewelObject* JewelObject::createJewel(int row, int col)
{
	auto jewel = new JewelObject();
	
	if (jewel && jewel->init())
	{
		//m_row �����ǳ��ȣ����Ǿݴ˵õ��������꣬���൱��ṹ���е����ԣ�����ÿ����ʯ��֪��λ�ú�index
		jewel->m_row = row;
		jewel->m_col = col;
		//jewel->m_ImgIndex = (int)random()% TOTAL_JEWEL;�������У����ǲ��ܴ������Ϊ���Ͳ���ȷ
		jewel->m_ImgIndex = random(0,TOTAL_JEWEL-1);
		
		
		
		//��֡�������õ�ͼƬ
		
		//����1
		//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName( JewelNormal[jewel->m_ImgIndex] );
		//jewel->initWithSpriteFrame(frame);
		
		//����2
		jewel->initWithSpriteFrameName(JewelNormal[jewel->m_ImgIndex]);
		
		jewel->autorelease();
		return jewel;
	}

	CC_SAFE_DELETE(jewel);
	return nullptr;
}

float JewelObject::getJewelWidth()
{
	//����plist�ļ�����Դ��(��Ϊ�ǵ��������Լ���һ�������������򶼿��Է��ʵĵ����ȵ��ô˺��������Ա����������ʼ��plist�ļ�)
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