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
	//ע�ᴥ��
	bool touchBegin(Touch* touch, Event* event);
	void touchMoved(Touch* touch, Event* event);
	EventListenerTouchOneByOne* pListener;

	//��ʼ����Ϸ����Ԫ��(��ʼ�����飬ԭʼλ�õ�)
	void initGameBase();
	//��ʼ����ͼ����ʯ����
	void initMap();
	//���ɺ����䱦ʯ
	void createAndDropJewel(int row, int col);
	//��ñ�ʯ������
	Point positionOfJewel(int row, int col);
	//��д��ʱ��
	void update(float fT);
	//��ȡ��ǰ�������ķ���
	JewelObject* jewelOfPoint(Point point);
	//������ʯ
	void swapJewel();
	//����� ��ͬ�ı�ʯ
	void checkRowSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels);
	//����� ��ͬ�ı�ʯ
	void checkColSameLink(JewelObject* jewel, Vector<JewelObject*> &m_VecJewels);
	//������ʯ
	void removeJewel(Vector<JewelObject*> &m_VecJewels);
	//��Ⲣɾ����ʯ
	void checkAndRemoveJewel();
	//������ʯ
	void fillJewels();
	//ɾ����ʯ��Ч
	void explodeJewel(JewelObject* jewel);
	//ɾ��Ч����ɺ���õĺ����������߼���ɾ����ʯ��
	void actionEndCallBack(Node* node);
private:
	//��ԭʼx��yλ��ֵ
	float m_leftBottomX;
	float m_LeftBottomY;
	//������ ����
	int m_widthNum;
	int m_HightNum;
	//2d array
	JewelObject** m_Map;
	//�����Ƿ������� true:������
	bool m_bIsActioning;
	//�Ƿ���Խ��д������� true:OK
	bool m_bIsTouchable;
	//�״ε���ı�ʯ
	JewelObject* m_SrcJewel;
	JewelObject* m_DestJewel;
	//�Ƿ��п�ȱ��Ҫ�
	bool m_isNeedFill;
	


	//���� ��ǩ
	Label* numLabel;
};



#endif // !_GAME_SCENE_
