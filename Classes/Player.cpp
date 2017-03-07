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
	roleCount = 0;
	pickNumber = 0;
	pickState = STATE::HAND;
	brainCount = 10;
	brainEnd = false;
	isFourCard = false;
	isDeside = false;
	cardSort(ROLE::EQUAL, hand);
	for (int i = 0; i < 11; i++)
	{
		result.pushBack(Card::create(MARK::NOMARK, NUMBER::NONUMBER));
		result.at(i)->setRole(ROLE::WITHOUT);
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
	roleCount = p->roleCount;
	pickState = STATE::HAND;
	brainCount = p->brainCount;
	brainEnd = false;
	isFourCard = false;	
	isDeside = false;
	for (int i = 0; i < p->hand.size(); i++)
	{
		result.pushBack(Card::create(p->result.at(i)->myMark, p->result.at(i)->myNumber));
		hand.pushBack(Card::create(p->hand.at(i)->myMark, p->hand.at(i)->myNumber));
		result.at(i)->setRole(p->result.at(i)->myRole);
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
	cardSort(ROLE::ORDER, hand);
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 4, getPositionY()));
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
	hand.pushBack(deck.at(deck.size()-1));
	deck.at(deck.size()-1)->setState(STATE::HAND);
	deck.erase(deck.size()-1);
};

//�J�[�h���̂Ă�
void Player::cardThrow(int num,Vector<Card*>&grave) 
{
	hand.at(num)->setReverse(true);
	grave.pushBack(hand.at(num));
	grave.at(grave.size()-1)->setState(STATE::GRAVE);
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
		result.at(i)->setRole(ROLE::WITHOUT);
		hand.at(i)->setColor(Color3B::WHITE);
		result.at(i)->setKind(MARK::NONE,NUMBER::ZERO);
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
	roleCount = 0;
	cardDispHand(true);
	point = 5000;
	brainEnd = false;
	ressetRole();
	brainCount = hand.size();
	checkRoleNew(this);
	cardDispHand(true);
	//log("roleSplit=%d", RoleSplit);
	
};

//���̓��_�̌v�Z
void Player::calcRole(Vector<Card*> cResult) 
{
	int iPoint=0;
	setKnockThrowCard(cResult);
	for (int i = 0; i < hand.size(); i++)
	{
		if (cResult.at(i)->myRole != ROLE::ROLEIN)
		{
			if(i!=pickNumber)
				iPoint += (int)cResult.at(i)->myNumber;
		}
		else {
		}
	}
	if (addPoint(iPoint)) 
	{
		cardSort(ROLE::ORDER, result);
		setRoleColor(cResult);
	};
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
	isFourCard = false;
	//�R���Z�b�g�̑g�ݍ��킹��S�X�L����
	for (int z = 0; z < player->hand.size() - 2; z++)
		for (int y = z + 1; y < player->hand.size() - 1; y++)
			for (int x = y + 1; x < player->hand.size(); x++) {
				if (player->hand.at(x)->myNumber>ZERO) {
					if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber)) {//2��������Ă���
						if (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber) {//3��������Ă���
							if (x < player->brainCount - 1 && (player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber))
								checkRoleNew(check(player, z, y, x, x + 1));//�S���̏����ց@�߂�l���J�[�h���Ȃ̂ŁA����ōċN����
							else
								checkRoleNew(check(player, z, y, x));//�R���̏���
						}
						else
						{
							//chance(player, z, y);
						}
					}
					if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && player->hand.at(z)->myMark == player->hand.at(y)->myMark) {//2�����Ԃɕ���ł���
						if (((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)) {//3��
							for (int q = x + 1; q < hand.size(); q++)
								if (((int)player->hand.at(z)->myNumber + 3 == (int)player->hand.at(q)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(q)->myMark)) {
									checkRoleNew(check(player, z, y, x, q));//4���̏���
									isFourCard = true;
								}
							if (!isFourCard)
								checkRoleNew(check(player, z, y, x));//�R���̏���
						}
						else
						{
							//chance(player, z, y);
						}
					}
				}
			}
	//�c��̃J�[�h���R�s�[
	copyResultForHand(player);

		calcRole(player->result);
};

//�񖇂ŎO���ڂ̌����݂����邩������Ȃ�
void Player::check(Player* &player, int x, int y) 
{

};

Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3�܂�����������̏���
	//log("3");
	Player* brain=Player::create(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myNumber <= NUMBER::ZERO)
		{
			log("%d.%d.%d", brain->hand.at(x)->myNumber, brain->hand.at(y)->myNumber, brain->hand.at(z)->myNumber);

			//���ʂɂR���R�s�[���ĕۑ�
			brain->result.at(i)->setKind(brainPlayer->hand.at(x));	brain->hand.at(x)->setKind(NONE, ZERO);//0�ɂ��āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(i+1)->setKind(brainPlayer->hand.at(y)); brain->hand.at(y)->setKind(NONE, ZERO);
			brain->result.at(i+2)->setKind(brainPlayer->hand.at(z));	brain->hand.at(z)->setKind(NONE, ZERO);
			brain->result.at(i)->roleNumber		=brain->roleCount;
			brain->result.at(i+1)->roleNumber	= brain->roleCount;
			brain->result.at(i+2)->roleNumber	= brain->roleCount;
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->hand.at(x)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->brainCount -= 3;//�������ׂ��J�[�h�������Z
			brain->roleCount++;
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myMark == MARK::NONE&&brainPlayer->hand.at(i)->myNumber == ZERO)
		{
			break;
		}
		if (brain->hand.at(i)->myNumber == ZERO)brain->hand.at(i)->setKind(NONE, ZERO);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
		else brain->hand.at(i)->setKind(brainPlayer->hand.at(i));//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	}
	sort(brain);
	return brain;
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z,int q) {//4�܂�����������̏���
	//log("4");

	Player* brain = Player::create(brainPlayer); 
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myNumber <= NUMBER::ZERO)
		{
			log("%d.%d.%d.%d", brain->hand.at(x)->myNumber, brain->hand.at(y)->myNumber, brain->hand.at(z)->myNumber, brain->hand.at(q)->myNumber);

			//���ʂɂR���R�s�[���ĕۑ�
			//���ʂɂR���R�s�[���ĕۑ�
			brain->result.at(i)->setKind(brainPlayer->hand.at(x));		brain->hand.at(x)->setKind(NONE, ZERO);//0�ɂ��āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(i + 1)->setKind(brainPlayer->hand.at(y));	brain->hand.at(y)->setKind(NONE, ZERO);
			brain->result.at(i + 2)->setKind(brainPlayer->hand.at(z));	brain->hand.at(z)->setKind(NONE, ZERO);
			brain->result.at(i + 3)->setKind(brainPlayer->hand.at(q));	brain->hand.at(q)->setKind(NONE, ZERO);
			brain->result.at(i)->roleNumber = brain->roleCount;
			brain->result.at(i + 1)->roleNumber = brain->roleCount;
			brain->result.at(i + 2)->roleNumber = brain->roleCount;
			brain->result.at(i + 3)->roleNumber = brain->roleCount;
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 3)->setRole(ROLE::ROLEIN);
			brain->hand.at(x)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->hand.at(q)->setRole(ROLE::ROLEIN);
			brain->brainCount -= 4;//�������ׂ��J�[�h�������Z
			brain->roleCount++;
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myMark == MARK::NONE&&brainPlayer->hand.at(i)->myNumber == NUMBER::ZERO)
		{
			break;
		}
		if (brain->hand.at(i)->myNumber == ZERO)brain->hand.at(i)->setKind(NONE, ZERO);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
		else brain->hand.at(i)->setKind(brainPlayer->hand.at(i));//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	}
	
	sort(brain);
	
	return brain;
};

bool Player::addPoint(int num)
{
		if (num <= point) {
		point = num;
		if (point <= 10)brainEnd = true;//�m�b�N�\
		return true;
	}
	return false;
}

void Player::setRoleColor(Vector<Card*>cResult)
{
	for (int i = 0; i < hand.size(); i++)
	{
		for (int j = 0; j < hand.size(); j++) {
			if (cResult.at(i)->myMark == hand.at(j)->myMark&&cResult.at(i)->myNumber == hand.at(j)->myNumber) {
				if (cResult.at(i)->myRole == ROLE::ROLEIN)
				{
					if (hand.at(j)->roleNumber % 3 == 2) {
						hand.at(j)->setColor(Color3B::YELLOW);
						hand.at(j)->setRole(ROLE::ROLEIN);
					}
					else if (hand.at(j)->roleNumber % 3 == 1) {
						hand.at(j)->setColor(Color3B::RED);
						hand.at(j)->setRole(ROLE::ROLEIN);
					}
					else if (hand.at(j)->roleNumber % 3 == 0) {
						hand.at(j)->setColor(Color3B::GREEN);
						hand.at(j)->setRole(ROLE::ROLEIN);
					}
				}
				else
				{
					hand.at(j)->setColor(Color3B::WHITE);
					hand.at(j)->setRole(ROLE::WITHOUT);
				}
			}
		}
	}
};

//�\�[�g
void Player::sort(Player* &p) 
{
	for (int i = 0; i < p->hand.size(); i++)
	{
		//��ԉE�̃J�[�h������܂�
		for (int j = 0; j  < p->hand.size()-1; j++)
		{
			if ((int)p->result.at(j)->roleNumber < (int)p->result.at(j + 1)->roleNumber)
			{
				p->result.swap(j, j + 1); 
			}
		}
	}
};

void Player::ressetPlayer() 
{
	//���̒��̌v�Z
	brainCount = 10;
	//���_
	point=5000;
	//�I�񂾔ԍ�
	pickNumber = -1;
	//�I�񂾃p�C��
	pickState=STATE::HAND;
	//��D�Ƃ���̖���񂪓��鉼�z��D
	handDeath();
	//�m�b�N�ł��邩
	brainEnd=false;
};

//�c��̃J�[�h�����U���g�ɒǉ�
void Player::copyResultForHand(Player* &p) 
{
	for (int i = 0; i < hand.size(); i++) {
		if (p->hand.at(i)->myNumber>NUMBER::ZERO&&p->brainCount>=0) {
			p->brainCount--;
			p->result.at(p->brainCount)->setKind(hand.at(i));
		}
	}
};

//�����ł��Ȃ������J�[�h�̒��ň�Ԑ����傫�����̂���肷��
void Player::setKnockThrowCard(Vector<Card*> cResult)
{
	pickNumber = 0;
	for (int i = 0; i < hand.size(); i++)
	{
		if (cResult.at(i)->myRole != ROLE::ROLEIN)
		{
			if (cResult.at(pickNumber)->myNumber < cResult.at(i)->myNumber)
			{
				pickNumber = i;
			}
		}
	}
};
