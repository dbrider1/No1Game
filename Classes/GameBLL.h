#ifndef _GAME_BLL_
#define _GAME_BLL_

#include "GameHeader.h"


class GameBLL :public Ref
{
public:
	static GameBLL* getInstance();
	static void destoryInstance();
	//������Ϸ
	Scene* createGame(void* level);
	//���¿�ʼ
	void gameRestart();
	//��ͣ��Ϸ
	void gamePause();
	//������Ϸ
	void gameResume();
	void gameWin();
	void gameOver();
	//�뿪��Ϸ
	void gameQuit();
	
private:
	GameBLL();
	~GameBLL();
private:

};




#endif // !_GAME_BLL_
