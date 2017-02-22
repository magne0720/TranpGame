#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}
	pickNumber = 0;
	pickState = STATE::HAND;
	brainHandCount = 0;
	cardSort(ROLE::EQUAL);
	return true;
};

void Player::cardDispHand() 
{
	cardSort(sortType);
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
		hand.at(i)->setState(STATE::HAND);
	}
};
void Player::cardSort(ROLE kind)
{
	sortType = kind;
	switch (kind)
	{
	//�ԍ���
	case ROLE::ORDER:
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
				//���������Ń}�[�N�������낦��
				if ((int)hand.at(j)->myMark > (int)hand.at(j + 1)->myMark && (int)hand.at(j)->myNumber ==(int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
			}
		}
		break;
	//�}�[�N��
	case ROLE::EQUAL:
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
	case ROLE::WITHOUT:
		//�o�����₭�̏���
		for (int i = 0; i < hand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<hand.size(); j++)
			{
				//�}�[�N���ɕ��ёւ�
				if ((int)hand.at(j)->roleNumber < (int)hand.at(j + 1)->roleNumber)
				{
					hand.swap(j, j + 1);
				}
			}
		}
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

//���̏�����
void Player::ressetRole() 
{
	for (int i = 0; i < hand.size(); i++) 
	{
		hand.at(i)->myRole = ROLE::WITHOUT;
		hand.at(i)->setColor(Color3B::WHITE);
	}
}

//��������
//����J�[�h������
//�A��(�����}�[�N�A���̐�)��������܂Ō���
//�����o�����炻����������������
//������������������x������(�I���܂ŌJ��Ԃ�)
//�Ō�܂Ō����猩�����J�[�h���t���Őݒ肷��
//

int Player::checkRole()
{
	int one, two;
	//���q�����q
	checkEqual(0);
	checkOrder(0);
	one=calcRole();
	ressetRole();
	//���q�����q
	checkOrder(0);
	checkEqual(0);
	two = calcRole();
	ressetRole();
	log("order%d-equal%d", one, two);
	if(one<two)
	{
		log("junsi->kokusi");
		checkEqual(0);
		checkOrder(0);
		cardSort(ROLE::WITHOUT);
		log("playEnd");
		return one;
	}
	else
	{
		log("kokusi->junsi");
		checkOrder(0);
		checkEqual(0);
		cardSort(ROLE::WITHOUT);
		log("playEnd");
		return two;
	}
};

void Player::chanceRole(Card* card, ROLE role, bool isAllCheck)
{
	//�����ł������Ȃ��̂����Ă���
	if (card != nullptr)
	{
		brain.pushBack(card);
	}
	//�܂����ׂĂ����Ă��Ȃ��Ȃ�
	if (!isAllCheck)
	{
		return;
	}
	if (brain.size() < 3) 
	{
		brain.clear();
		return;
	}
	log("size<<<[%d]", brain.size());
	switch (role)
	{
	case WITHOUT:
		break;
	case ORDER:
		for (int i = 0; i < brain.size(); i++) 
		{
			if (brain.at(i)->myRole == ROLE::EQUAL) 
			{
				break;
			}
		}
		for (int i = 0; i < brain.size(); i++)
		{
			brain.at(i)->setRoleNumber(brainHandCount);
			if (brain.at(i)->myRole != ROLE::EQUAL)
			{
				if (brain.size() >= 4)
				{
					brain.at(i)->setRole(ROLE::ORDER_FOUR);
					brain.at(i)->setColor(Color3B::BLUE);
					log("order_four-%d", i);
				}
				else//3����
				{
					brain.at(i)->setRole(ROLE::ORDER);
					brain.at(i)->setColor(Color3B::MAGENTA);
					log("order-%d", i);
				}
			}
		}
		break;
	case EQUAL://��΂S���܂�
		for (int i = 0; i < brain.size(); i++)
		{
			if (brain.at(i)->myRole == ROLE::ORDER)
			{
				break;
			}
		}
		for (int i = 0; i < brain.size(); i++)
		{
			brain.at(i)->setRoleNumber(brainHandCount);
			if (brain.at(i)->myRole != ROLE::ORDER)
			{
				brain.at(i)->setRole(ROLE::EQUAL);
				if (brain.size() >= 4)
				{
					brain.at(i)->setRole(ROLE::EQUAL_FOUR);
					brain.at(i)->setColor(Color3B::GREEN);
					log("four_equal-%d", i);
				}
				else//3����
				{
					brain.at(i)->setColor(Color3B::YELLOW);
					log("equal-%d", i);
				}
			}
		}
		break;
	default:
		break;
	}
	brainHandCount++;
	//��������ۂ�
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
			log("%d-plus%d", i,(int)hand.at(i)->myNumber);
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
			hand.at(num)->myNumber + 1 == hand.at(j)->myNumber)					//�ԍ���1������������
		{
			checkOrder(j);
		}
	}
	checkEqualFour(hand.at(num));
	chanceRole(nullptr, ROLE::ORDER, true);
	if (num != HAND_SIZE)
	{
		if (hand.at(num + 1)->myRole == ROLE::WITHOUT)
		{
			log("next%d", num + 1);
			checkOrder(num + 1);
		}
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
			hand.at(num)->myNumber == hand.at(j)->myNumber)		//�ԍ���������
		{
			checkEqual(j);
		}
	}
	checkOrderFour(hand.at(num));
	chanceRole(nullptr, ROLE::EQUAL, true);
	if (num != HAND_SIZE)
	{
		if (hand.at(num + 1)->myRole == ROLE::WITHOUT)
		{
			checkEqual(num + 1);
		}
	}
};

//�l�����q�̈ꖇ����OK����
void Player::checkEqualFour(Card* card)
{
	for (int i = 0; i < brain.size(); i++)
	{
		if (brain.at(i)->myRole == ROLE::EQUAL_FOUR)
		{
			brain.at(i)->setRole(ROLE::EQUAL);
		}
	}
};

//�l�����q�̈ꖇ����OK����
void Player::checkOrderFour(Card* card)
{
	for (int i = 0; i < brain.size(); i++)
	{
		if (brain.at(i)->myRole == ROLE::ORDER_FOUR)
		{
			brain.at(i)->setRole(ROLE::ORDER);
		}
	}
};
