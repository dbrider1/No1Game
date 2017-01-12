#ifndef _GAME_SCENE_
#define _GAME_SCENE_

#include "GameHeader.h"
#include "GameBackGroundLayer.h"

class JewelObject;
class GameScene :public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene();
	CREATE_FUNC(GameScene);
private:
	virtual bool init();
	//注册触摸
	bool touchBegin(Touch* touch, Event* event);
	void touchMoved(Touch* touch, Event* event);
	EventListenerTouchOneByOne* pListener;

	//初始化游戏基本元素(初始化数组，原始位置等)
	void initGameBase();
	//初始化地图（宝石矩阵）
	void initMap();
	//生成和下落宝石
	void createAndDropJewel(int row, int col);
	//获得宝石的坐标
	Point positionOfJewel(int row, int col);
	//重写定时器
	void update(float fT);
	//获取当前触摸到的方块
	JewelObject* jewelOfPoint(Point point);
	//交换钻石
	void swapJewel();
	//检查行 相同的宝石
	void checkRowSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels);
	//检查列 相同的宝石
	void checkColSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels);
	//消除钻石
	void removeJewel(Vector<JewelObject*> &m_VecJewels);
	//检测并删除钻石
	void checkAndRemoveJewel();
	//补齐钻石
	void fillJewels();
	//删除钻石特效
	void explodeJewel(JewelObject* jewel);
	//删除效果完成后调用的函数（彻底逻辑中删除宝石）
	void actionEndCallBack(Node* node);
private:
	//最原始x、y位置值
	float m_leftBottomX;
	float m_LeftBottomY;
	//横纵向 个数
	int m_widthNum;
	int m_HightNum;
	//2d array
	JewelObject** m_Map;
	//动作是否在运行 true:运行中
	bool m_bIsActioning;
	//是否可以进行触摸操作 true:OK
	bool m_bIsTouchable;
	//首次点击的宝石
	JewelObject* m_SrcJewel;
	JewelObject* m_DestJewel;
	//是否有空缺需要填补
	bool m_isNeedFill;
	


	//计数 标签
	Label* numLabel;
};



#endif // !_GAME_SCENE_
