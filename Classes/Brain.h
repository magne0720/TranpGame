#ifndef __BRAIN_H__
#define __BRAIN_H__

#include "Card.h"

class Brain :public Node 
{
public:
	CREATE_FUNC(Brain);

		//���̒��̌v�Z
		int brainCount;
		//��D�Ƃ���̖���񂪓��鉼�z��D
		Vector<Card*> hand, result;
};

#endif // !__BRAIN_H__
