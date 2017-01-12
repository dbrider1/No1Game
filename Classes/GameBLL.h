#ifndef _GAME_BLL_
#define _GAME_BLL_

#include "GameHeader.h"


class GameBLL :public Ref
{
public:
	static GameBLL* getInstance();
	static void destoryInstance();
	//创建游戏
	Scene* createGame(void* level);
	//重新开始
	void gameRestart();
	//暂停游戏
	void gamePause();
	//继续游戏
	void gameResume();
	void gameWin();
	void gameOver();
	//离开游戏
	void gameQuit();
	
private:
	GameBLL();
	~GameBLL();
private:

};




#endif // !_GAME_BLL_
