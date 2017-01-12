#ifndef _JEWEL_OBJECT_
#define _JEWEL_OBJECT_

#include "GameHeader.h"

class JewelObject:public Sprite
{
public:
	static JewelObject* createJewel(int row,int col);
	static float getJewelWidth();
	bool init();

	// Ù–‘∫Í øÌ£¨∏ﬂ£¨Õº∆¨Index
	CC_SYNTHESIZE(int ,m_row ,Row);
	CC_SYNTHESIZE(int ,m_col ,Col);
	CC_SYNTHESIZE(int ,m_ImgIndex ,ImgIndex);
private:

};

#endif // !_JEWEL_OBJECT_


