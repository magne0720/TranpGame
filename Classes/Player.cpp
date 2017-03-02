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
	myHand = Brain::create();
	addChild(myHand);
	effect = EffectManager::create();
	addChild(effect);
	point = 50000;
	pickNumber = 0;
	pickState = STATE::HAND;
	brainEnd = false;
	myHand->brainCount = 10;
	for (int i = 0; i < 11; i++)
	{
		myHand->result.pushBack(Card::create(MARK::NONE, NUMBER::ZERO));
		myHand->result.at(i)->roleNumber = 0;
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
	}	myHand = Brain::create();
	addChild(myHand);

	point = p->point;
	pickNumber = 0;
	pickState = STATE::HAND;
	myHand->brainCount = p->myHand->brainCount;
	brainEnd = false;
	for (int i = 0; i < p->myHand->hand.size(); i++)
	{
		myHand->hand = p->myHand->hand;
		myHand->result = p->myHand->result;
		//log("%d-%d", p->hand.at(i)->myMark, p->hand.at(i)->myNumber);
	}
	//lastCard = Card::create(MARK::NONE, NUMBER::ZERO);
	//lastCard->setVisible(false);
	//addChild(lastCard);

	return true;
};

//true�F�\
//false�F��
void Player::cardDispHand(bool isReversed, int max)
{
	cardSort(sortType, myHand->hand);
	for (int i = 0; i < myHand->hand.size(); i++)
	{
		myHand->hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
		handPos[i] = myHand->hand.at(i)->getMyPosition();
		myHand->hand.at(i)->setPosition(handPos[i]);
		myHand->hand.at(i)->setState(STATE::HAND);
		myHand->hand.at(i)->setReverse(isReversed);
		if (!isReversed)
		{
			myHand->hand.at(i)->setColor(Color3B::WHITE);
		}
		if (i > max)myHand->hand.at(i)->setVisible(false);
		else myHand->hand.at(i)->setVisible(true);
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
	myHand->hand.pushBack(deck.at(deck.size()-1));
	deck.at(deck.size()-1)->setState(STATE::HAND);
	deck.erase(deck.size()-1);
};

//�J�[�h���̂Ă�
void Player::cardThrow(int num,Vector<Card*>&grave) 
{
	myHand->hand.at(num)->setReverse(true);
	grave.pushBack(myHand->hand.at(num));
	grave.at(grave.size()-1)->setState(STATE::GRAVE);
	myHand->hand.erase(num);
};

void Player::handDeath() 
{
	for (int i = 0; i < myHand->hand.size();i++)
	{
		myHand->hand.at(i)->removeFromParentAndCleanup(true);
	}
	myHand->hand.clear();
};

//���̏�����
void Player::ressetRole() 
{
	for (int i = 0; i <myHand->hand.size(); i++)
	{
		myHand->hand.at(i)->setRole(ROLE::WITHOUT);
		myHand->hand.at(i)->setColor(Color3B::WHITE);
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
<<<<<<< HEAD
	point = 5000;
	result=hand;
	brainEnd = false;
	ressetRole();
	sortType = ROLE::WITHOUT;
	brainCount = hand.size();
	checkRoleNew(this);
	//log("roleSplit=%d", RoleSplit);
	if (RoleSplit >= 2) 
	{ 
		//log("count=%d", brainCount);
		brainEnd = true; 
	}
=======
	log("-----------checkStart----------");
	point = 5000;
	myHand->result= myHand->hand;
	brainEnd = false;
	ressetRole();
	log("resset----OK");
	myHand->brainCount = myHand->hand.size();
	cardDispHand(true);
	log("copy------OK");
	checkRoleNew(myHand);
	log("check-----OK");
	
	log("-----------checkEnd------------");
>>>>>>> 16580c859f3abcfde5b6e73e84f4e21c2722095c
};

//���̓��_�̌v�Z
void Player::calcRole(Vector<Card*> cResult) 
{
	int point=0;
	for (int i = 0; i < myHand->hand.size(); i++)
	{
		if (cResult.at(i)->myRole != ROLE::ROLEIN)
		{
			point += (int)cResult.at(i)->myNumber;
		}
	}
	if (addPoint(point)) 
	{
		myHand->hand = cResult;
		//cardSort(ROLE::WITHOUT, result);
		//cardSortMarge(cResult);
		setRoleColor(myHand);
	};
	//log("---------end--------");
};

//�Ō�̃J�[�h�m�F
int Player::checkLastCard() 
{
	for (int i = 0; i < myHand->hand.size(); i++)
	{
		if (myHand->hand.at(i)->myMark == lastCard->myMark&&myHand->hand.at(i)->myNumber == lastCard->myNumber)
		{
			//log("getI=%d", i);
			return i;
		}
	}
	return 0;
}


void Player::checkRoleNew(Brain* cHand)
{
	//�R���Z�b�g�̑g�ݍ��킹��S�X�L����
	for (int z = 0; z < cHand->brainCount - 2; z++)
		for (int y = z + 1; y < cHand->brainCount - 1; y++)
			for (int x = y + 1; x < cHand->brainCount; x++) {
				if ((cHand->hand.at(z)->myNumber == cHand->hand.at(y)->myNumber)) {//2��������Ă���
					if (cHand->hand.at(y)->myNumber == cHand->hand.at(x)->myNumber) {//3��������Ă���
						if (x < cHand->brainCount - 1 && (cHand->hand.at(x)->myNumber == cHand->hand.at(x + 1)->myNumber))
							checkRoleNew(check(myHand, z, y, x, x + 1));//�S���̏����ց@�߂�l���J�[�h���Ȃ̂ŁA����ōċN����
						else
							checkRoleNew(check(myHand, z, y, x));//�R���̏���
					}
					else
					{
						//chance(myHand, z, y);
					}
				}
				if (((int)cHand->hand.at(z)->myNumber + 1 == (int)cHand->hand.at(y)->myNumber) && cHand->hand.at(z)->myMark == cHand->hand.at(y)->myMark) {//2�����Ԃɕ���ł���
					if (((int)cHand->hand.at(z)->myNumber + 2 == (int)cHand->hand.at(x)->myNumber) && (cHand->hand.at(z)->myMark == cHand->hand.at(x)->myMark)) {//3��
						if (((x < cHand->brainCount - 1) && (int)cHand->hand.at(z)->myNumber + 3 == (int)cHand->hand.at(x + 1)->myNumber) && (cHand->hand.at(z)->myMark == cHand->hand.at(x + 1)->myMark))
							checkRoleNew(check(myHand, z, y, x, x + 1));//4���̏���
						else checkRoleNew(check(myHand, z, y, x));//�R���̏���
					}
					else
					{
						//chance(myHand, z, y);
					}
				}
			}
	sort(cHand);

};

//�񖇂ŎO���ڂ̌����݂����邩������Ȃ�
bool Player::chance(Brain* player, int x, int y)
{
	log("chance[%d]-[%d]", x, y);
	player->result.at(x)->setRole(ROLE::CHANCE);
	player->result.at(y)->setRole(ROLE::CHANCE);
	return false;
};

Brain* Player::check(Brain* brainPlayer, int x, int y, int z) {//3�܂�����������̏���
	//log("3");
	Brain* brain(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole != ROLE::WITHOUT)
		{
			//���ʂɂR���R�s�[���ĕۑ�
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->brainCount -= 3;//�������ׂ��J�[�h�������Z
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN) {

			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
		}
		else {
			break;
		}
	}
	//sort(brain);
	return brain;
};


Brain* Player::check(Brain* brainPlayer, int x, int y, int z,int q) {//4�܂�����������̏���
	//log("4");
	Brain* brain(brainPlayer);
	
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myRole != ROLE::WITHOUT)
		{
			//���ʂɂR���R�s�[���ĕۑ�
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->hand.at(q)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 3)->setRole(ROLE::ROLEIN);
			//log("%d-%d-%d-GetRole", hand.at(x)->myNumber, hand.at(y)->myNumber, hand.at(z)->myNumber);
			brain->brainCount -= 4;//�������ׂ��J�[�h�������Z
			break;
		}
	}
	//���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN) {
			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
		}
		else {
			break;
		}
	}
<<<<<<< HEAD
	
	//sort(brain);
=======
	sort(brain);
>>>>>>> 16580c859f3abcfde5b6e73e84f4e21c2722095c
	
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

void Player::setRoleColor(Brain* brain) 
{
	for (int i = 0; i < myHand->hand.size(); i++)
	{
		if (brain->result.at(i)->myRole == ROLE::ROLEIN)
		{
			brain->hand.at(i)->setColor(Color3B::YELLOW);
			brain->hand.at(i)->setRole(ROLE::ROLEIN);
		}
		else if (brain->result.at(i)->myRole == ROLE::WITHOUT)
		{
			brain->hand.at(i)->setColor(Color3B::WHITE);
			brain->hand.at(i)->setRole(ROLE::WITHOUT);
		}
		else if (brain->result.at(i)->myRole == ROLE::CHANCE)
		{
			brain->hand.at(i)->setColor(Color3B::RED);
			brain->hand.at(i)->setRole(ROLE::WITHOUT);
		}
	}
}

//�\�[�g
void Player::sort(Brain* &p) 
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
	myHand->brainCount = 0;
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

void Player::cardSortMarge(Vector<Card*> cResult) 
{
	for (int i = 0; i < myHand->hand.size(); i++) {
		for (int j = 0; j < myHand->hand.size(); j++) {
			//�J�[�h��񂪓����Ȃ炻�낦��
			if (myHand->hand.at(i)->myMark == cResult.at(j)->myMark&&myHand->hand.at(i)->myNumber == cResult.at(j)->myNumber)
			{
				myHand->hand.swap(i, j);
			}
		}
	}
};

int Player::checkNotRole() 
{
	int ret = 0;
	for (int i = 0; i < myHand->hand.size(); i++)
	{
		if (myHand->hand.at(i)->myNumber>ret&&myHand->hand.at(i)->myRole != ROLE::ROLEIN)
		{
			ret = i;
		}
	}
	return ret;
}
