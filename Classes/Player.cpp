#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}


	return true;
};

void Player::update(float delta) 
{

};

void Player::cardDispHand() 
{
	for (int i = 0; i < hand.size(); i++) 
	{
		hand.at(i)->setMyPosition(Vec2(150 * i+getPositionX()/3, getPositionY()));
	}
};

void Player::cardThrow(Card* &card) 
{

};

void Player::handDeath() 
{
	for (int i = 0; i < hand.size();i++)
	{
		hand.at(i)->removeFromParentAndCleanup(true);
	}
	hand.clear();
};

//��������
void Player::checkFame(int x)
{
	hand.at(x)->isFamed = true;
	//10��
	for (int i=0; i < hand.size(); i++)
	{
		//�Ō�̃J�[�h���ǂ���
		if ( x + i  < hand.size()) 
		{
			//�}�[�N���ꏏ��
			if (hand.at(x)->myMark == hand.at(x+i)->myMark)
			{
				log("[x,i]=[%d,%d]", x, i);
				//���̔ԍ���
				if ((int)hand.at(x)->myNumber + 1 == (int)hand.at(x + i)->myNumber)
				{
					log("check[%d]_[%d]<[%d]_[%d]", x,(int)hand.at(x)->myNumber, i, (int)hand.at(x + i)->myNumber);
					checkFame(i);
					hand.at(x+i)->setColor(Color3B::GREEN);
					hand.at(x)->setColor(Color3B::GREEN);
				}
				else
				{
					log("none");
				}
			}
		}
	}
	//���q(�����ԍ�)������



};

void Player::checkOrder(int num) 
{

}
