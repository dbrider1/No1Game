#ifndef _GAME_GROUND_
#define _GAME_GROUND_

#include "GameHeader.h"

class GameBackGroundLayer :public Layer
{
public:
	CREATE_FUNC(GameBackGroundLayer);
	
	virtual bool init()override;
public:
	Layer* m_BackGroundLayer;
private:
	Layout* m_RootNode;
};



#endif
