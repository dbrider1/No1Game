#ifndef _GAME_UILAYER_
#define _GAME_UILAYER_

#include "GameHeader.h"

class GameUILayer :public Layer
{
public:
	CREATE_FUNC(GameUILayer);
	virtual bool init()override;
private:
	Layout* m_RootNode;
	void checkPause(Ref* sender, CheckBox::EventType type);
	void btnRetry(Ref* ref, Widget::TouchEventType type);
};



#endif // !_GAME_UILAYER_
