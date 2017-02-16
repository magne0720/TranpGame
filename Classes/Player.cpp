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
	//��D�̐�����
	for (int i = 0; i < hand.size(); i++)
	{
		//��ԉE�̃J�[�h������܂�
		for (int j = 0; j+1<hand.size(); j++)
		{
			if ((int)hand.at(j)->myNumber > (int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
		}
	}
	log("%d", hand.size());
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
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
	static int count = 0;
	MoveBy* move = MoveBy::create(1.0, Vec2(0, 100));

	//10��
	for (int i = 0; i < hand.size(); i++)
	{
		//�Ō�̃J�[�h���ǂ���
		if (x < hand.size())
		{
			//���g�łȂ������ǂ���
			if (i != x)
			{
				//�}�[�N���ꏏ��
				if (hand.at(x)->myMark == hand.at(i)->myMark)
				{
					if (count >= 3)
					{
						log("SUPER");
					}
					//�ԍ���1������������
					if (hand.at(x)->myNumber + 1 == hand.at(i)->myNumber)
					{
						hand.at(i)->runAction(move);
						hand.at(i)->setColor(Color3B::YELLOW);
						hand.at(x)->setColor(Color3B::GREEN);
						log("getRed%d", i);
						count++;
					}
				}
			}
		}
	}
	count = 0;
	//�Ō�̃J�[�h���ǂ���
	if (x < hand.size())
	{
		log("%d", x);
		checkFame(x + 1);
	}//���q(�����ԍ�)������

};

void Player::checkOrder(int num) 
{

}
