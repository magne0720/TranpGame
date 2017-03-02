#ifndef __BRAIN_H__
#define __BRAIN_H__

#include "Card.h"

class Brain :public Node 
{
public:
	CREATE_FUNC(Brain);

		//“ª‚Ì’†‚ÌŒvZ
		int brainCount;
		//èD‚Æ‚»‚ê‚Ì–ğî•ñ‚ª“ü‚é‰¼‘zèD
		Vector<Card*> hand, result;
};

#endif // !__BRAIN_H__
