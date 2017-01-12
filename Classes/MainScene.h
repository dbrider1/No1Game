#ifndef _MAIN_SCENE_
#define _MAIN_SCENE_

#include "GameHeader.h"

class MainScene :public Scene
{
public:
	static Scene* createScene();
	CREATE_FUNC(MainScene);
	virtual bool init();
private:
	void btnBeginGame(Ref* ref, Widget::TouchEventType type);
	void btnSet(Ref* ref, Widget::TouchEventType type);
	//Ôö¼Ó¹âÃ¢
	void addSpark();
	Layout* m_RootNode;
};


#endif // !_MAIN_SCENE_
