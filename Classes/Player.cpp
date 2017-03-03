#include "Player.h"

Player* Player::create(Player* &p)
{
	Player *pRet = new Player();
	if (pRet && pRet->init(p))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
};
Player* Player::create()
{
	Player *pRet = new Player();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
};


bool Player::init()
{
	if (!Node::init())
	{
		return false;
	}
	effect = EffectManager::create();
	addChild(effect);
	point = 50000;
	pickNumber = 0;
	pickState = STATE::HAND;
	brainCount = 10;
	RoleSplit = 0;
	brainEnd = false;
	cardSort(ROLE::EQUAL, hand);
	for (int i = 0; i < 11; i++)
	{
		result.pushBack(Card::create(MARK::NONE, NUMBER::ZERO));
		result.at(i)->roleNumber = 0;
	}	
	lastCard = Card::create(MARK::NONE, NUMBER::ZERO);
	lastCard->setVisible(false);
	addChild(lastCard);

	return true;
};

bool Player::init(Player* &p)
{
	if (!Node::init())
	{
		return false;
	}
	point = p->point;
	pickNumber = 0;
	pickState = STATE::HAND;
	brainCount = p->brainCount;
	RoleSplit = p->RoleSplit;
	brainEnd = false;
	for (int i = 0; i < p->hand.size(); i++)
	{
		result = p->result;
		hand = p->hand;
		//log("%d-%d", p->hand.at(i)->myMark, p->hand.at(i)->myNumber);
	}
	lastCard = Card::create(MARK::NONE, NUMBER::ZERO);
	lastCard->setVisible(false);
	addChild(lastCard);

	return true;
};

//true�F�\
//false�F��
void Player::cardDispHand(bool isReversed, int max)
{
	cardSort(sortType, hand);
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
		handPos[i] = hand.at(i)->getMyPosition();
		hand.at(i)->setPosition(handPos[i]);
		hand.at(i)->setState(STATE::HAND);
		hand.at(i)->setReverse(isReversed);
		if (!isReversed)
		{
			hand.at(i)->setColor(Color3B::WHITE);
		}
		if (i > max)hand.at(i)->setVisible(false);
		else hand.at(i)->setVisible(true);
	}
};

void Player::cardSort(ROLE kind,Vector<Card*>&cardhand)
{
	sortType = kind;
	switch (kind)
	{
	//�ԍ���
	case ROLE::ORDER:
		//��D�̐�����
		for (int i = 0; i < cardhand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				if ((int)cardhand.at(j)->myNumber > (int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
				//���������Ń}�[�N�������낦��
				if ((int)cardhand.at(j)->myMark > (int)cardhand.at(j + 1)->myMark && (int)cardhand.at(j)->myNumber ==(int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
			}
		}
		break;
	//�}�[�N��
	case ROLE::EQUAL:
		//��D�̐�����
		for (int i = 0; i < cardhand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				//�}�[�N���ɕ��ёւ�
				if ((int)cardhand.at(j)->myMark > (int)cardhand.at(j + 1)->myMark)
				{
					cardhand.swap(j, j + 1);
				}
				//�����}�[�N�Ő��������낦��
				if ((int)cardhand.at(j)->myMark == (int)cardhand.at(j + 1)->myMark && (int)cardhand.at(j)->myNumber >(int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
			}
		}
		break;
	case ROLE::WITHOUT:
		//�o�����₭�̏���
		for (int i = 0; i < cardhand.size(); i++)
		{
			//��ԉE�̃J�[�h������܂�
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				//�}�[�N���ɕ��ёւ�
				if ((int)cardhand.at(j)->roleNumber < (int)cardhand.at(j + 1)->roleNumber)
				{
					cardhand.swap(j, j + 1);
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
	hand.at(num)->setReverse(true);
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
		hand.at(i)->setRole(ROLE::WITHOUT);
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

void Player::checkRole()
{
	result=hand;
	brainEnd = false;
	ressetRole();
	brainCount = hand.size();
	checkRoleNew(this);
	//log("roleSplit=%d", RoleSplit);
	if (RoleSplit >= 2) 
	{ 
		//log("count=%d", brainCount);
		brainEnd = true; 
	}
};

//���̓��_�̌v�Z
void Player::calcRole(Vector<Card*> cResult) 
{
	int point=0;
	//log("---------start------");
	for (int i = 0; i < hand.size(); i++)
	{
		if(cResult.at(i)->myRole==ROLE::WITHOUT)
		{
			//log("%d-plus%d", i,(int)cResult.at(i)->myNumber);
			point += (int)cResult.at(i)->myNumber;
		}else{
			//log("%d-[%d]", i, cResult.at(i)->myNumber);
		}
	}
	if (addPoint(point)) 
	{
		cardSort(ROLE::WITHOUT, result);
		setRoleColor(cResult);
	};
	//log("---------end--------");
};

//�Ō�̃J�[�h�m�F
int Player::checkLastCard() 
{
	for (int i = 0; i < hand.size(); i++) 
	{
		if (hand.at(i)->myMark == lastCard->myMark&&hand.at(i)->myNumber == lastCard->myNumber)
		{
			//log("getI=%d", i);
			return i;
		}
	}
	return 0;
}


void Player::checkRoleNew(Player* player)
{
	//�R���Z�b�g�̑g�ݍ��킹��S�X�L����
	for (int z = 0; z < player->brainCount - 2; z++)
		for (int y = z + 1; y < player->brainCount - 1; y++)
			for (int x = y + 1; x < player->brainCount; x++) {
				if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber)) {//2��������Ă���
					if (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber) {//3��������Ă���
						if (x < player->brainCount - 1 && (player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber))
							checkRoleNew(check(player, z, y, x, x + 1));//�S���̏����ց@�߂�l���J�[�h���Ȃ̂ŁA����ōċN����
						else
							checkRoleNew(check(player, z, y, x));//�R���̏���
					}
					else
					{
						chance(player, z, y);
					}
				}
				if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && player->hand.at(z)->myMark == player->hand.at(y)->myMark) {//2�����Ԃɕ���ł���
					if (((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)) {//3��
						if (((x < player->brainCount - 1) && (int)player->hand.at(z)->myNumber + 3 == (int)player->hand.at(x + 1)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x + 1)->myMark))
							checkRoleNew(check(player, z, y, x, x + 1));//4���̏���
						else checkRoleNew(check(player, z, y, x));//�R���̏���
					}
					else
					{
						chance(player, z, y);
					}
				}
			}
//	if (player->RoleSplit >= 2) {
		calcRole(player->result);
//	}
};

//�񖇂ŎO���ڂ̌����݂����邩������Ȃ�
bool Player::chance(Player* &player, int x, int y) 
{
	return false;
};

Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3�܂�����������̏���
	//log("3");
	Player* brain=Player::create(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole != ROLE::ROLEIN)
		{
			//���ʂɂR���R�s�[���ĕۑ�
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(x)->setRole(ROLE::ROLEIN);
			brain->result.at(x)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRoleNumber(brain->RoleSplit);
			brain->brainCount -= 3;//�������ׂ��J�[�h�������Z
			brain->RoleSplit++;
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)
			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	}
	//sort(brain);
	return brain;
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z,int q) {//4�܂�����������̏���
	//log("4");
	Player* brain = Player::create(brainPlayer);
	
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myRole != ROLE::ROLEIN)
		{
			//���ʂɂR���R�s�[���ĕۑ�
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(x)->setRole(ROLE::ROLEIN);
			brain->result.at(x)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(q)->setRole(ROLE::ROLEIN);
			brain->result.at(q)->setRole(ROLE::ROLEIN);
			brain->result.at(q)->setRoleNumber(brain->RoleSplit);
			//log("%d-%d-%d-GetRole", hand.at(x)->myNumber, hand.at(y)->myNumber, hand.at(z)->myNumber);
			brain->brainCount -= 4;//�������ׂ��J�[�h�������Z
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if(brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)
		if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
		else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	}
	
	//sort(brain);
	
	return brain;
};

bool Player::addPoint(int num)
{
	if (num < point) {
		point = num;
		return true;
	}
	return false;
}

void Player::setRoleColor(Vector<Card*>cResult) 
{
	for (int i = 0; i < hand.size(); i++) 
	{
		if (cResult.at(i)->myRole == ROLE::ROLEIN)
		{
			hand.at(i)->setColor(Color3B::YELLOW);
		}
		else
		{
			hand.at(i)->setColor(Color3B::WHITE);
		}
	}
}

//�\�[�g
void Player::sort(Player* &p) 
{
	for (int i = 0; i < p->hand.size(); i++)
	{
		//��ԉE�̃J�[�h������܂�
		for (int j = 0; j + 1 < p->hand.size(); j++)
		{
			if ((int)p->result.at(j)->roleNumber < (int)p->result.at(j + 1)->roleNumber)
			{
				p->hand.swap(j, j + 1);
				p->result.swap(j, j + 1);
			}
		}
	}
};

void Player::RessetPlayer() 
{
	//���̒��̌v�Z
	brainCount = 0;
	//���_
	point=5000;
	//�I�񂾔ԍ�
	pickNumber = -1;
	//�I�񂾃p�C��
	pickState=STATE::HAND;
	//���̑g�ݕ���
	RoleSplit=0;
	//��D�Ƃ���̖���񂪓��鉼�z��D
	handDeath();
	//�m�b�N�ł��邩
	brainEnd=false;
};
