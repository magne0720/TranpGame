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
//����J�[�h������
//�A��(�����}�[�N�A���̐�)��������܂Ō���
//�����o�����炻����������������
//������������������x������(�I���܂ŌJ��Ԃ�)
//�Ō�܂Ō����猩�����J�[�h���t���Őݒ肷��
//

void Player::checkRole(int i)
{
	chanceRole(hand.at(i), ROLE::ORDER);
	for (int j = i; j < hand.size(); j++)//0����n�߂�̂���{�Ȃ̂ł��̑O�̃J�[�h�͒��ׂȂ�
	{
		if (i != j&&														//���g�łȂ������ǂ���
			hand.at(i)->myMark == hand.at(j)->myMark &&						//�}�[�N���ꏏ��
			(int)hand.at(i)->myNumber + 1 == (int)hand.at(j)->myNumber)		//�ԍ���1������������
		{
			checkRole(j);
		chanceRole(nullptr, ROLE::ORDER, true);
		}
	
	}
	if (i == hand.size() - 1) 
	{
		//log("get%d", i);
	}
	else
	{
		checkRole(i+1);
	}
};

void Player::chanceRole(Card* card ,ROLE role,bool isAllCheck) 
{//�f�o�b�O�p
	//MoveBy* move = MoveBy::create(1.0, Vec2(0, 100));

	//�����ł������Ȃ��̂����Ă���
	if (card != nullptr) 
	{
		brain.pushBack(card);
		log("size=%d", brain.size());
	}
	//�܂����ׂĂ����Ă��Ȃ��Ȃ�
	if (!isAllCheck)
	{
		//������
		return;
	}
	if (brain.size() >= 3)//�O����������ł��� 
	{
		switch (role)
		{
		case WITHOUT:
			break;
		case ORDER:
			for (int i = 0; i < hand.size(); i++) 
			{
				for (int j = 0; j < brain.size(); j++)
				{
					if (hand.at(i)->myMark == brain.at(j)->myMark&&hand.at(i)->myNumber == brain.at(j)->myNumber)
					{
						hand.at(i)->setRole(ROLE::ORDER);
						hand.at(i)->setColor(Color3B::MAGENTA);
						//hand.at(i)->runAction(move);
					}
				}
			}
			break;
		case EQUAL:
			break;
		default:
			break;
		}
		//��������ۂ�
	}
	brain.clear();
};

void Player::checkOrder(int num) 
{

}
