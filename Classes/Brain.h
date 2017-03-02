#ifndef __BRAIN_H__
#define __BRAIN_H__

#include "Card.h"

class Brain :public Node 
{
public:
	CREATE_FUNC(Brain);

		//頭の中の計算
		int brainCount;
		//手札とそれの役情報が入る仮想手札
		Vector<Card*> hand, result;
};

#endif // !__BRAIN_H__
