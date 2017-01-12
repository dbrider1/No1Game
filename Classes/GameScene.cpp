#include "GameScene.h"
#include "GameBackGroundLayer.h"
#include "JewelObject.h"

#define MAP_WIDTH (7)
#define MAP_HIGHT (7)
#define JEWEL_GAP (1) //宝石间距 用括号括起宏
#define GAME_SCENE_TAG (2)
#define FALL_SPEED (1)

//构造函数可以直接在外面初始化变量
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
	//	//清空容器
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
	
	//注册触摸
	pListener = EventListenerTouchOneByOne::create();
	pListener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegin,this);
	pListener->onTouchMoved = CC_CALLBACK_2(GameScene::touchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	initGameBase();
	
	//调用系统定时器
	scheduleUpdate();
	
	return true;
}

//初始化游戏基本元素(初始化数组，原始位置等)
void GameScene::initGameBase()
{
	m_HightNum = MAP_HIGHT;
	m_widthNum = MAP_WIDTH;

	auto s = Director::getInstance()->getWinSize();

	////设置宝石基础位置
	m_leftBottomX = (s.width - m_widthNum*JewelObject::getJewelWidth() - (m_widthNum - 1)*JEWEL_GAP) / 2;
	m_LeftBottomY = (s.height - m_HightNum*JewelObject::getJewelWidth() - (m_HightNum - 1)*JEWEL_GAP) / 2;
	
	////设置 数组大小
	auto arraySize = sizeof(JewelObject*) *m_HightNum*m_widthNum;
	//创建数组(二次数组要学习)
	//malloc前一定要指定类型 而大小就用sizeof获得
	m_Map = (JewelObject**)malloc(arraySize);
	//初始化数组
	memset(m_Map,0, arraySize);

	//初始化游戏地图
	initMap();
}

//初始化地图（宝石矩阵）
void GameScene::initMap()
{
	for (int row = 0; row < m_HightNum; row++)
	{
		for (int col = 0; col <  m_widthNum; col++) 
		{
			createAndDropJewel(row, col); //创建和掉落宝石
		}
	}
}

//生成和下落宝石
void GameScene::createAndDropJewel(int row, int col)
{
	auto s = Director::getInstance()->getWinSize();

	auto jewel = JewelObject::createJewel(row, col);

	//之所以使用positionOfJewel()就是为了可以扩展 ,为了以后可以填补空缺
	auto endPosition = positionOfJewel(row, col);
	auto startPosition = Vec2(endPosition.x, endPosition.y + s.height);

	jewel->setPosition(startPosition);
	jewel->runAction(MoveTo::create(FALL_SPEED, endPosition));
	jewel->setGlobalZOrder(2);

	//加入到场景中
	addChild(jewel);
	

	////添加计数label
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

	//初始化 数组
	m_Map[row*m_widthNum + col] = jewel;
}

//获得宝石的坐标
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
		m_bIsActioning = false;//如果运行中，进入此函数，先设置为false，一旦未完成，就被赋值成true。一旦全部完成，安全出来（为false）
		
	    //检测动作，如果宝石还有动作（下落未完成/未移动完毕），不允许触摸，反之允许
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
	//每次点击之前都必须初始化，而不仅仅在构造函数中
	m_SrcJewel = NULL;
	m_DestJewel = NULL;

	if (!m_bIsTouchable)return false;

	//log("touch ok");
	auto point = touch->getLocation();

	m_SrcJewel = jewelOfPoint(point);//位置正确

	return true;
}

void GameScene::touchMoved(Touch* touch, Event* event)
{
	if (!m_bIsTouchable || !m_SrcJewel)return;

	//1.获取四个方向的方块 2.判断点是否在其中 思路：1.用钻石自带属性（row，col）来找！灵性
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
		if (row < m_HightNum) {//绝不可能等于num，最大也小于其1个单位
			//设置将要移动的钻石
			m_DestJewel = m_Map[row * m_widthNum + col ];
			//m_DestJewel->setColor(Color3B::RED);
			//TODO:移动钻石，设置垂直移动属性（bool类型）
			swapJewel();
		}
		return;
	}

	//down 
	auto dowmRect = Rect(
		m_SrcJewel->getPositionX() - size.width / 2,
		m_SrcJewel->getPositionY() - (size.height / 2 * 3),//这个想法很灵性 （*3）
		m_SrcJewel->getContentSize().width,
		m_SrcJewel->getContentSize().height);

	if (dowmRect.containsPoint(location)) {
		//log("DOWN TAUCH");
		row--;
		if (row >= 0) {
			//设置将要移动的钻石
			m_DestJewel = m_Map[row * m_widthNum + col];
			//TODO:移动钻石，设置垂直移动属性（bool类型）
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
			//设置将要移动的钻石
			m_DestJewel = m_Map[row * m_widthNum + col];
			swapJewel();
			//TODO:移动钻石，设置垂直移动属性（bool类型）
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
			//设置将要移动的钻石
			m_DestJewel = m_Map[row * m_widthNum + col];
			//TODO:移动钻石，设置垂直移动属性（bool类型）
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
			//rect 需要增加原点坐标和size即可	
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
	m_bIsActioning = true;//一定产生活动了
	m_bIsTouchable = false;//交换一次就立即不允许触摸。否则它会一直触摸下去
	
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

	//分别检查移动后 两个钻石（src和 dest）的上下左右四个方向
	Vector<JewelObject*> checkRowFirst;
	checkRowSameLink(m_SrcJewel, checkRowFirst);//如果此函数不是&类型，那么就无法赋值给容器size（）？

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
		) //有三个以上相同
	{
		auto time = 0.2;
		m_DestJewel->runAction(MoveTo::create(time, srcPos));
		m_SrcJewel->runAction(MoveTo::create(time, desPos));//位置换了，但是图片没换！
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
		m_DestJewel->runAction( Sequence::create(move1, move1Back,NULL) );//没有NULL参数就会崩溃
		m_SrcJewel->runAction( Sequence::create(move2, move2Back,NULL) );
	}
	//swap 成功：交换位置 但是最终位置不变，数组中位置也应该改变 

	//1：index相同即可，要判断全局的 
	//2.四个方向都要去判断 
	//3.如果 当前移动 >=3，交换，消除，填充   <3：交换，返回
	//4.其他位置有 就直接删除

	//1.调用全局check（），ok：change | no：back 包括行检查，列检查
	//2.调用删除函数。delete（）；
	//3.调用补充函数 fill（）；
}

//检查行 相同的宝石
void GameScene::checkRowSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels)
{
	m_VecJewels.pushBack(jewel);//不管有无想用，先加入再说（必须有此步骤，两个方向公用一个容器，取最终的个数）
	//下一行
	auto neighbor = jewel->getRow() - 1;
	
	while (neighbor>=0)//不用递归，用死循环也可以
	{
		auto jewelDown = m_Map[neighbor*m_widthNum + jewel->getCol()];

		if (jewelDown  //凡创建个变量都要记得检测 也可以用断言 TODO::添加是否需要移除函数（为了四个）
			&& jewelDown->getImgIndex() == jewel->getImgIndex() ) 
		{
			m_VecJewels.pushBack(jewelDown);
			neighbor--;
		}else {
			break;//if not 立刻退出（此钻石不符合条件，直接跳过）
		}
	}

	//上一行
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

//检查列 相同的宝石
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
		
		//如果可以消除（相同个数>=3）,消除
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

	//先在数组中赋值为0
	m_Map[jewel->getRow() * m_widthNum + jewel->getCol()] = NULL;
	
	//然后在父节点中删除自己
	jewel->removeFromParent();								     
}

void GameScene::fillJewels()
{
	m_bIsActioning = true;
	
	//思路:只能从上边fill，所以只改变row

	//创建一维数组，保存每一列空缺的宝石数目（int *一维，int** 二维）
	auto colEmpty = (int*)malloc(sizeof(int*) * m_widthNum);
	memset((void*)colEmpty,0, sizeof(int*)* m_widthNum);

	JewelObject* jewel = NULL;

	//最外边：最后再一列一列统计
	for (int col = 0; col < m_widthNum; col++)
	{
		int rowEmptyNum = 0; //每一列 空宝石的个数
		for (int row = 0; row < m_HightNum; row++)
		{
			jewel = m_Map[row*m_widthNum + col];
			if (NULL==jewel)
			{
				rowEmptyNum++;
			}
			else
			{
				if (rowEmptyNum > 0)//如果有空缺 并且遍历的 此jewel 不是空缺的第一个钻石
				{
					//底层改变：数组中改变赋值
					auto newRow = row - rowEmptyNum;//需要下落的宝石 行数
					m_Map[newRow * m_widthNum + col] = jewel;
					m_Map[row*m_widthNum + col] = NULL;
					//不能使用jewel，之后还需要使用 
					//区别：m_Map是只有数组变为0，宝石的图片等信息还有
					//如果jewel赋值为0，则整个宝石都不存在

					//表层改变：sprite改变
					auto endPosition = positionOfJewel(newRow, col);
					auto startPos = jewel->getPosition();
					jewel->stopAllActions();

					//速度必须匀速，不能用简单的数字（一旦下落宝石增多就不匀速了）
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

	//有malloc（），就有对应的free（）；
	free(colEmpty);
}