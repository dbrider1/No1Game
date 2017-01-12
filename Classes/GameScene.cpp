#include "GameScene.h"
#include "GameBackGroundLayer.h"
#include "JewelObject.h"

#define MAP_WIDTH (7)
#define MAP_HIGHT (7)
#define JEWEL_GAP (1) //��ʯ��� �����������
#define GAME_SCENE_TAG (2)
#define FALL_SPEED (1)

//���캯������ֱ���������ʼ������
GameScene::GameScene() :
	m_leftBottomX(0),
	m_LeftBottomY(0),
	m_widthNum(0),
	m_HightNum(0),
	m_Map(NULL),
	m_bIsActioning(true),
	m_bIsTouchable(true),
	m_SrcJewel(NULL),
	m_DestJewel(NULL),
	m_isNeedFill(false)
{

}

GameScene::~GameScene()
{
	if (m_Map) {
		free(m_Map);
	}

	if (pListener != nullptr) {
		_eventDispatcher->removeEventListener(pListener);
	}
	
	//if (m_VecJewels.size()!=0) {
	//	//�������
	//	m_VecJewels.clear();
	//}
	
}

Scene * GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())return false;
	
	auto backLayer = GameBackGroundLayer::create();
	addChild(backLayer);
	
	//ע�ᴥ��
	pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegin,this);
	pListener->onTouchMoved = CC_CALLBACK_2(GameScene::touchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	initGameBase();
	
	//����ϵͳ��ʱ��
	scheduleUpdate();
	
	return true;
}

//��ʼ����Ϸ����Ԫ��(��ʼ�����飬ԭʼλ�õ�)
void GameScene::initGameBase()
{
	m_HightNum = MAP_HIGHT;
	m_widthNum = MAP_WIDTH;

	auto s = Director::getInstance()->getWinSize();

	////���ñ�ʯ����λ��
	m_leftBottomX = (s.width - m_widthNum*JewelObject::getJewelWidth() - (m_widthNum - 1)*JEWEL_GAP) / 2;
	m_LeftBottomY = (s.height - m_HightNum*JewelObject::getJewelWidth() - (m_HightNum - 1)*JEWEL_GAP) / 2;
	
	////���� �����С
	auto arraySize = sizeof(JewelObject*) *m_HightNum*m_widthNum;
	//��������(��������Ҫѧϰ)
	//mallocǰһ��Ҫָ������ ����С����sizeof���
	m_Map = (JewelObject**)malloc(arraySize);
	//��ʼ������
	memset(m_Map,0, arraySize);

	//��ʼ����Ϸ��ͼ
	initMap();
}

//��ʼ����ͼ����ʯ����
void GameScene::initMap()
{
	for (int row = 0; row < m_HightNum; row++)
	{
		for (int col = 0; col <  m_widthNum; col++) 
		{
			createAndDropJewel(row, col); //�����͵��䱦ʯ
		}
	}
}

//���ɺ����䱦ʯ
void GameScene::createAndDropJewel(int row, int col)
{
	auto s = Director::getInstance()->getWinSize();

	auto jewel = JewelObject::createJewel(row, col);

	//֮����ʹ��positionOfJewel()����Ϊ�˿�����չ ,Ϊ���Ժ�������ȱ
	auto endPosition = positionOfJewel(row, col);
	auto startPosition = Vec2(endPosition.x, endPosition.y + s.height);

	jewel->setPosition(startPosition);
	jewel->runAction(MoveTo::create(FALL_SPEED, endPosition));
	jewel->setGlobalZOrder(2);

	//���뵽������
	addChild(jewel);
	

	////��Ӽ���label
	//auto i = row*m_widthNum + col;
	///*auto num = StringUtils::format("%d", i);*/
	//auto num = StringUtils::format("(%d, %d)", row,col);
	//numLabel = Label::create();
	//numLabel->setString(num);
	//numLabel->setScale(2);
	//numLabel->setPosition(endPosition);
	//numLabel->setColor(Color3B::BLACK);
	//numLabel->setGlobalZOrder(100);
	//addChild(numLabel);

	//��ʼ�� ����
	m_Map[row*m_widthNum + col] = jewel;
}

//��ñ�ʯ������
Point GameScene::positionOfJewel(int row, int col)
{
	auto initwidth = 35.f;

	auto x = JewelObject::getJewelWidth()*col + m_leftBottomX + JEWEL_GAP + initwidth;
	auto y = JewelObject::getJewelWidth()*row + m_LeftBottomY + JEWEL_GAP;
	return Vec2(x, y);
}

void GameScene::update(float fT)
{
	if (m_bIsActioning) 
	{
		m_bIsActioning = false;//��������У�����˺�����������Ϊfalse��һ��δ��ɣ��ͱ���ֵ��true��һ��ȫ����ɣ���ȫ������Ϊfalse��
		
	    //��⶯���������ʯ���ж���������δ���/δ�ƶ���ϣ���������������֮����
		for (int i = 0; i < m_HightNum*m_widthNum; i++)
		{
			auto jewels = m_Map[i];
			if (jewels && jewels->getNumberOfRunningActions()>0)  {
				m_bIsActioning = true;
				break;
			}
		}
		
		m_bIsTouchable = !m_bIsActioning;
	}

	if (!m_bIsActioning) 
	{
		if (m_isNeedFill)
		{
			//TODO: fill
			fillJewels();
			m_isNeedFill = false;
		}
		else
		{
			checkAndRemoveJewel();
		}
	}
}

bool GameScene::touchBegin(Touch* touch, Event* event)
{
	//ÿ�ε��֮ǰ�������ʼ�������������ڹ��캯����
	m_SrcJewel = NULL;
	m_DestJewel = NULL;

	if (!m_bIsTouchable)return false;

	//log("touch ok");
	auto point = touch->getLocation();

	m_SrcJewel = jewelOfPoint(point);//λ����ȷ

	return true;
}

void GameScene::touchMoved(Touch* touch, Event* event)
{
	if (!m_bIsTouchable || !m_SrcJewel)return;

	//1.��ȡ�ĸ�����ķ��� 2.�жϵ��Ƿ������� ˼·��1.����ʯ�Դ����ԣ�row��col�����ң�����
	auto row = m_SrcJewel->getRow();
	auto col = m_SrcJewel->getCol();

	auto size = m_SrcJewel->getContentSize();
	
	auto location = touch->getLocation();

	//up
	auto upRect = Rect(
		m_SrcJewel->getPositionX()-size.width/2,
		m_SrcJewel->getPositionY()+size.height/2,
		m_SrcJewel->getContentSize().width,
		m_SrcJewel->getContentSize().height);

	if (upRect.containsPoint(location)) 
	{
		//log("UP TAUCH");
		row++;
		if (row < m_HightNum) {//�������ܵ���num�����ҲС����1����λ
			//���ý�Ҫ�ƶ�����ʯ
			m_DestJewel = m_Map[row * m_widthNum + col ];
			//m_DestJewel->setColor(Color3B::RED);
			//TODO:�ƶ���ʯ�����ô�ֱ�ƶ����ԣ�bool���ͣ�
			swapJewel();
		}
		return;
	}

	//down 
	auto dowmRect = Rect(
		m_SrcJewel->getPositionX() - size.width / 2,
		m_SrcJewel->getPositionY() - (size.height / 2 * 3),//����뷨������ ��*3��
		m_SrcJewel->getContentSize().width,
		m_SrcJewel->getContentSize().height);

	if (dowmRect.containsPoint(location)) {
		//log("DOWN TAUCH");
		row--;
		if (row >= 0) {
			//���ý�Ҫ�ƶ�����ʯ
			m_DestJewel = m_Map[row * m_widthNum + col];
			//TODO:�ƶ���ʯ�����ô�ֱ�ƶ����ԣ�bool���ͣ�
			swapJewel();
			/*m_DestJewel->setColor(Color3B::GREEN);*/
		}
		return;
	}

	//left
	auto leftRect = Rect(
		m_SrcJewel->getPositionX() - (size.width / 2) * 3,
		m_SrcJewel->getPositionY() - size.height / 2,
		m_SrcJewel->getContentSize().width,
		m_SrcJewel->getContentSize().height);

	if (leftRect.containsPoint(location)) {
		//log("LEFT TAUCH");
		col--;
		if (col >= 0) {
			//���ý�Ҫ�ƶ�����ʯ
			m_DestJewel = m_Map[row * m_widthNum + col];
			swapJewel();
			//TODO:�ƶ���ʯ�����ô�ֱ�ƶ����ԣ�bool���ͣ�
		}
		return;

	}

	//right
	auto rightRect = Rect(
		m_SrcJewel->getPositionX() + size.width / 2,
		m_SrcJewel->getPositionY() - size.height / 2,
		m_SrcJewel->getContentSize().width,
		m_SrcJewel->getContentSize().height);

	if (rightRect.containsPoint(location)) {
		//log("RIGHT TAUCH");
		col++;
		if (col<m_widthNum) {
			//���ý�Ҫ�ƶ�����ʯ
			m_DestJewel = m_Map[row * m_widthNum + col];
			//TODO:�ƶ���ʯ�����ô�ֱ�ƶ����ԣ�bool���ͣ�
			swapJewel();
		}
		return;
	}
}

JewelObject* GameScene::jewelOfPoint(Point point)

{
	Rect rect = Rect(0,0,0,0);
	JewelObject* jewel = NULL;

	for (int i = 0; i < m_HightNum*m_widthNum; i++) 
	{
		jewel = m_Map[i];
		if (jewel) {
			//rect ��Ҫ����ԭ�������size����	
			rect.origin.x = jewel->getPositionX() - (jewel->getContentSize().width / 2);
			rect.origin.y = jewel->getPositionY() - (jewel->getContentSize().height / 2);
			rect.size = jewel->getContentSize();
		}

		if (rect.containsPoint(point)) 
		{
			//log(" touch Point(%f %f)", rect.origin.x, rect.origin.y);
		/*	log(" jewel Array(%d)",i);*/
			/*jewel->setVisible(false);*/
			return jewel;
		}
	}

	return nullptr;
}

void GameScene::swapJewel()
{
	m_bIsActioning = true;//һ���������
	m_bIsTouchable = false;//����һ�ξ�����������������������һֱ������ȥ
	
	if (!m_DestJewel || !m_SrcJewel)return;
	
	//1.change jewels
	auto srcPos = m_SrcJewel->getPosition();
	auto desPos = m_DestJewel->getPosition();
	auto pos = srcPos;

	//swap point
	auto srcRow = m_SrcJewel->getRow();
	auto srcCol = m_SrcJewel->getCol();
	auto desRow = m_DestJewel->getRow();
	auto desCol = m_DestJewel->getCol();

	log("Old:  src(%d %d) ,desc(%d %d)", m_SrcJewel->getRow(), m_SrcJewel->getCol(), m_DestJewel->getRow(), m_DestJewel->getCol());
	
	//if ok num>=3
	m_Map[srcRow*m_widthNum + srcCol] = m_DestJewel;
	m_Map[desRow*m_widthNum + desCol] = m_SrcJewel;

	m_SrcJewel->setRow(desRow);
	m_SrcJewel->setCol(desCol);
	m_DestJewel->setRow(srcRow);
	m_DestJewel->setCol(srcCol);


	log("New:  src(%d %d) ,desc(%d %d)", m_SrcJewel->getRow(), m_SrcJewel->getCol(), m_DestJewel->getRow(), m_DestJewel->getCol());

	//�ֱ����ƶ��� ������ʯ��src�� dest�������������ĸ�����
	Vector<JewelObject*> checkRowFirst;
	checkRowSameLink(m_SrcJewel, checkRowFirst);//����˺�������&���ͣ���ô���޷���ֵ������size������

	Vector<JewelObject*> checkColFirst;
	checkColSameLink(m_SrcJewel, checkColFirst);

	Vector<JewelObject*> checkRowSecond;
	checkRowSameLink(m_DestJewel, checkRowSecond);

	Vector<JewelObject*> checkColSecond;
	checkColSameLink(m_DestJewel, checkColSecond);

	if (checkRowFirst.size() >= 3
		|| checkRowSecond.size() >= 3
		|| checkColFirst.size() >= 3
		|| checkColSecond.size() >= 3
		) //������������ͬ
	{
		auto time = 0.2;
		m_DestJewel->runAction(MoveTo::create(time, srcPos));
		m_SrcJewel->runAction(MoveTo::create(time, desPos));//λ�û��ˣ�����ͼƬû����
		return;
	}
	else 
	{
		auto srcRow = m_SrcJewel->getRow();
		auto srcCol = m_SrcJewel->getCol();
		auto desRow = m_DestJewel->getRow();
		auto desCol = m_DestJewel->getCol();

		m_Map[srcRow*m_widthNum + srcCol] = m_DestJewel;
		m_Map[desRow*m_widthNum + desCol] = m_SrcJewel;
		
		m_SrcJewel->setRow(desRow);
		m_SrcJewel->setCol(desCol);
		m_DestJewel->setRow(srcRow);
		m_DestJewel->setCol(srcCol);

		auto time = 0.2;
		auto move1 = MoveTo::create(time, srcPos);
		auto move1Back = MoveTo::create(time, desPos);

		auto move2 = MoveTo::create(time, desPos);
		auto move2Back = MoveTo::create(time, srcPos);
		m_DestJewel->runAction( Sequence::create(move1, move1Back,NULL) );//û��NULL�����ͻ����
		m_SrcJewel->runAction( Sequence::create(move2, move2Back,NULL) );
	}
	//swap �ɹ�������λ�� ��������λ�ò��䣬������λ��ҲӦ�øı� 

	//1��index��ͬ���ɣ�Ҫ�ж�ȫ�ֵ� 
	//2.�ĸ�����Ҫȥ�ж� 
	//3.��� ��ǰ�ƶ� >=3�����������������   <3������������
	//4.����λ���� ��ֱ��ɾ��

	//1.����ȫ��check������ok��change | no��back �����м�飬�м��
	//2.����ɾ��������delete������
	//3.���ò��亯�� fill������
}

//����� ��ͬ�ı�ʯ
void GameScene::checkRowSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels)
{
	m_VecJewels.pushBack(jewel);//�����������ã��ȼ�����˵�������д˲��裬����������һ��������ȡ���յĸ�����
	//��һ��
	auto neighbor = jewel->getRow() - 1;
	
	while (neighbor>=0)//���õݹ飬����ѭ��Ҳ����
	{
		auto jewelDown = m_Map[neighbor*m_widthNum + jewel->getCol()];

		if (jewelDown  //��������������Ҫ�ǵü�� Ҳ�����ö��� TODO::����Ƿ���Ҫ�Ƴ�������Ϊ���ĸ���
			&& jewelDown->getImgIndex() == jewel->getImgIndex() ) 
		{
			m_VecJewels.pushBack(jewelDown);
			neighbor--;
		}else {
			break;//if not �����˳�������ʯ������������ֱ��������
		}
	}

	//��һ��
	neighbor = jewel->getRow() + 1;

	while (neighbor < m_HightNum)
	{
		auto col = jewel->getCol();
		auto jewelsUp = m_Map[neighbor*m_widthNum + jewel->getCol()];
		if ( jewelsUp && jewelsUp->getImgIndex() == jewel->getImgIndex() )
		{
			m_VecJewels.pushBack(jewelsUp);
			neighbor++;
		}
		else {
			break;
		}
	}

	log("%d", m_VecJewels.size());
}

//����� ��ͬ�ı�ʯ
void GameScene::checkColSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels)
{
	m_VecJewels.pushBack(jewel);

	auto neighbor = jewel->getCol() - 1;
	
	//Left
	while (neighbor >= 0) {
		auto jewelLeft = m_Map[jewel->getRow() * m_widthNum + neighbor];
		if (jewelLeft && jewelLeft->getImgIndex()==jewel->getImgIndex())
		{
			m_VecJewels.pushBack(jewelLeft);
			neighbor--;
		}
		else {
			break;
		}
	}

	//Right
	neighbor = jewel->getCol() + 1;

	while (neighbor<m_widthNum) {
		auto jewelRight = m_Map[jewel->getRow() * m_widthNum + neighbor];
		if (jewelRight 
			&& (jewelRight->getImgIndex()==jewel->getImgIndex()) )
		{
			m_VecJewels.pushBack(jewelRight);
			neighbor++;
		}
		else {
			break;
		}
	}
}


void GameScene::checkAndRemoveJewel()
{
	for (int i = 1; i < m_HightNum * m_widthNum; i++)
	{
		auto jewel = m_Map[i];
		if (!jewel) continue;
		
		Vector<JewelObject*> checkRowFirst;
		checkRowSameLink(jewel, checkRowFirst);

		Vector<JewelObject*> checkColFirst;
		checkColSameLink(jewel, checkColFirst);
		
		auto maxNumVec = checkRowFirst.size() > checkColFirst.size() ? checkRowFirst : checkColFirst;
		if (maxNumVec.size() < 3)continue;
		
		//���������������ͬ����>=3��,����
		removeJewel(maxNumVec);
	}
}

void GameScene::removeJewel(Vector<JewelObject*> &m_VecJewels)
{
	m_bIsActioning = true;

	for (auto &A : m_VecJewels)
	{
		auto jewel = (JewelObject*)A;
		if (jewel) {
			m_isNeedFill = true;
			explodeJewel(jewel);
		}
	}
	/*fillJewels();*/
}

void GameScene::explodeJewel(JewelObject* jewel)
{
	auto time = 0.3;
	auto action = Sequence::create( ScaleTo::create(time, 0), CallFuncN::create(CC_CALLBACK_1( GameScene::actionEndCallBack,this) ), NULL);
	jewel->runAction(action);

}

void GameScene::actionEndCallBack(Node* node)
{
	auto jewel = (JewelObject*)node;

	//���������и�ֵΪ0
	m_Map[jewel->getRow() * m_widthNum + jewel->getCol()] = NULL;
	
	//Ȼ���ڸ��ڵ���ɾ���Լ�
	jewel->removeFromParent();								     
}

void GameScene::fillJewels()
{
	m_bIsActioning = true;
	
	//˼·:ֻ�ܴ��ϱ�fill������ֻ�ı�row

	//����һά���飬����ÿһ�п�ȱ�ı�ʯ��Ŀ��int *һά��int** ��ά��
	auto colEmpty = (int*)malloc(sizeof(int*) * m_widthNum);
	memset((void*)colEmpty,0, sizeof(int*)* m_widthNum);

	JewelObject* jewel = NULL;

	//����ߣ������һ��һ��ͳ��
	for (int col = 0; col < m_widthNum; col++)
	{
		int rowEmptyNum = 0; //ÿһ�� �ձ�ʯ�ĸ���
		for (int row = 0; row < m_HightNum; row++)
		{
			jewel = m_Map[row*m_widthNum + col];
			if (NULL==jewel)
			{
				rowEmptyNum++;
			}
			else
			{
				if (rowEmptyNum > 0)//����п�ȱ ���ұ����� ��jewel ���ǿ�ȱ�ĵ�һ����ʯ
				{
					//�ײ�ı䣺�����иı丳ֵ
					auto newRow = row - rowEmptyNum;//��Ҫ����ı�ʯ ����
					m_Map[newRow * m_widthNum + col] = jewel;
					m_Map[row*m_widthNum + col] = NULL;
					//����ʹ��jewel��֮����Ҫʹ�� 
					//����m_Map��ֻ�������Ϊ0����ʯ��ͼƬ����Ϣ����
					//���jewel��ֵΪ0����������ʯ��������

					//���ı䣺sprite�ı�
					auto endPosition = positionOfJewel(newRow, col);
					auto startPos = jewel->getPosition();
					jewel->stopAllActions();

					//�ٶȱ������٣������ü򵥵����֣�һ�����䱦ʯ����Ͳ������ˣ�
					auto s = Director::getInstance()->getWinSize();
					auto speed = (startPos.y - endPosition.y) / s.height;

					jewel->runAction(MoveTo::create(speed, endPosition));
					jewel->setRow(newRow);
				}
			}
			colEmpty[col] = rowEmptyNum;
		}
	}

	for (int col = 0; col < m_widthNum; col++)
	{
		for (int row = m_HightNum - colEmpty[col]; row < m_HightNum; row++)
		{
			createAndDropJewel(row,col);
		}

	}

	//��malloc���������ж�Ӧ��free������
	free(colEmpty);
}