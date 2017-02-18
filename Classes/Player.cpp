#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}
	pickNumber = 0;
	pickState = STATE::HAND;
	

	return true;
};

void Player::update(float delta) 
{

};

void Player::cardDispHand() 
{
	cardSort(0);
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
		hand.at(i)->setState(STATE::HAND);
	}
};
void Player::cardSort(int num)
{
	switch (num)
	{
	//�ԍ���
	case 0:
		//��D�̐�����
		for (int i = 0; i < hand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<hand.size(); j++)
			{
				if ((int)hand.at(j)->myNumber > (int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
				
			}
		}
		break;
	//�}�[�N��
	case 1:
		//��D�̐�����
		for (int i = 0; i < hand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<hand.size(); j++)
			{
				//�}�[�N���ɕ��ёւ�
				if ((int)hand.at(j)->myMark > (int)hand.at(j + 1)->myMark)
				{
					hand.swap(j, j + 1);
				}
				//�����}�[�N�Ő��������낦��
				if ((int)hand.at(j)->myMark == (int)hand.at(j + 1)->myMark && (int)hand.at(j)->myNumber >(int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
			}
		}
		break;
	default:
		break;
	}
	
};

//�h���[
void Player::cardDrow(Vector<Card*>&deck)
{
	if (deck.size() <= 0)
	{
		return;
	}
	hand.pushBack(deck.at(DECK_TOP));
	deck.at(DECK_TOP)->setState(STATE::HAND);
	deck.erase(DECK_TOP);
};

//�J�[�h���̂Ă�
void Player::cardThrow(int num,Vector<Card*>&grave) 
{
	grave.pushBack(hand.at(num));
	grave.at(GRAVE_TOP)->setState(STATE::GRAVE);
	hand.erase(num);
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

void Player::checkRole()
{
	int one, two;
	//���q�����q
	checkEqual(0);
	checkOrder(0);
	one=calcRole();
	//���q�����q
	checkOrder(0);
	checkEqual(0);
	two = calcRole();
	log("%d-%d", one, two);
	if(one>two)
	{
		log("junsi");
	}
	else
	{
		log("kokusi");
	}
};

void Player::chanceRole(Card* card ,ROLE role,bool isAllCheck) 
{
	//�����ł������Ȃ��̂����Ă���
	if (card != nullptr) 
	{
		brain.pushBack(card);
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
					}
				}
			}
			break;
		case EQUAL://��΂S���܂�
			for (int i = 0; i < hand.size(); i++)
			{
				for (int j = 0; j < brain.size(); j++)
				{
					if (hand.at(i)->myMark == brain.at(j)->myMark&&hand.at(i)->myNumber == brain.at(j)->myNumber)
					{
						hand.at(i)->setRole(ROLE::EQUAL);
						hand.at(i)->setColor(Color3B::YELLOW);
					}
				}
			}
			break;
		default:
			break;
		}
		//��������ۂ�
	}
	brain.clear();
};

//���̓��_�̌v�Z
int Player::calcRole() 
{
	int point=0;
	for (int i = 0; i < hand.size(); i++) 
	{
		if(hand.at(i)->myRole==ROLE::WITHOUT)
		{
			point += (int)hand.at(i)->myNumber;
		}
	}
	return point;
};

//�}�[�N�������A���Ԃ̃J�[�h
void Player::checkOrder(int num) 
{
	//���g����̉\���ɒǉ�
	chanceRole(hand.at(num), ROLE::ORDER);
	//�������񎩐g�������O��
	//hand.at(num)->setRole(ROLE::WITHOUT);

	for (int j = num; j < hand.size(); j++)//0����n�߂�̂���{�Ȃ̂ł��̑O�̃J�[�h�͒��ׂȂ�
	{
		if (num != j&&															//���g�łȂ������ǂ���
			hand.at(num)->myMark == hand.at(j)->myMark &&						//�}�[�N���ꏏ��
			(int)hand.at(num)->myNumber + 1 == (int)hand.at(j)->myNumber)		//�ԍ���1������������
		{
			checkOrder(j);
		}
	}
	chanceRole(nullptr, ROLE::ORDER, true);
	if (num != HAND_SIZE)
	{
		checkOrder(num + 1);
	}
};

//�}�[�N�֌W�Ȃ��A�ԍ�������
void Player::checkEqual(int num) 
{
	//���g����̉\���ɒǉ�
	chanceRole(hand.at(num), ROLE::EQUAL);
	//�������񎩐g�������O��
	//hand.at(num)->setRole(ROLE::WITHOUT);

	for (int j = num; j < hand.size(); j++)//0����n�߂�̂���{�Ȃ̂ł��̑O�̃J�[�h�͒��ׂȂ�
	{
		if (num != j &&														//���g�łȂ������ǂ���
			(int)hand.at(num)->myNumber == (int)hand.at(j)->myNumber)		//�ԍ���������
		{
			checkEqual(j);
		}
	}
	chanceRole(nullptr, ROLE::EQUAL, true);
	if (num != HAND_SIZE)
	{
		checkEqual(num + 1);
	}
};
