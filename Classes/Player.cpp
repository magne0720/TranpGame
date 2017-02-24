#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}
	point = 50000;
	pickNumber = 0;
	pickState = STATE::HAND;
	brainHandCount = 10;
	brainCount = 10;
	brainEnd = true;
	cardSort(ROLE::EQUAL,hand);
	for (int i = 0; i < 13; i++) 
	{
		result.pushBack(Card::create(MARK::NONE, NUMBER::ZERO));
	}
	return true;
};

//true�F�\
//false�F��
void Player::cardDispHand(bool isReversed) 
{
	cardSort(sortType,hand);
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
		hand.at(i)->setState(STATE::HAND);
		hand.at(i)->setReverse(isReversed);
		if (!isReversed) 
		{
			hand.at(i)->setColor(Color3B::WHITE);
		}
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
	brainEnd = false;
	brainHandCount = hand.size();
	checkRoleNew(this);
	brainEnd = true;
};

//���̓��_�̌v�Z
int Player::calcRole(Vector<Card*> result) 
{
	int point=0;
	for (int i = 0; i < hand.size(); i++) 
	{
		//if(result.at(i)->myNumber==NUMBER::ZERO)
		{
			log("%d-plus%d", i,(int)result.at(i)->myNumber);
			point += (int)result.at(i)->myNumber;
		}
	}
	return point;
};
void Player::checkRoleNew(Player* player)
{
	//�R���Z�b�g�̑g�ݍ��킹��S�X�L����
	for (int z = 0; z < player->brainCount - 2; z++)
		for (int y = z + 1; y < player->brainCount - 1; y++)
			for (int x = y + 1; x < player->brainCount ; x++) {
				if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber) && (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber)) {//�R��������Ă���
					if (x < player->brainCount - 1 && x < player->hand.size() - 3)
						if ((player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber) //�S�����ꏏ
							)
							checkRoleNew(check(player, z, y, x, x + 1));//�S���̏����ց@�߂�l���J�[�h���Ȃ̂ŁA����ōċN����
						else checkRoleNew(check(player, z, y, x));//�R���̏���
				}
				if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && ((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) &&//���Ԃɕ���ł���
					(player->hand.at(z)->myMark == player->hand.at(y)->myMark) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)//�}�[�N������
					) checkRoleNew(check(player, z, y, x));//�R���̏���
			}
				addPoint(calcRole(player->result));
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3�܂�����������̏���
	log("3");
	Player* brain(brainPlayer);
	for (int i = 0; i < hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole==ROLE::WITHOUT) {
			//���ʂɂR���R�s�[���ĕۑ�
			brain->result.at(i)->myNumber=brainPlayer->hand.at(x)->myNumber; brain->hand.at(x)->myRole=ROLE::ROLEIN;//����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
			brain->result.at(i+1)->myNumber = brainPlayer->hand.at(y)->myNumber; brain->hand.at(y)->myRole = ROLE::ROLEIN;
			brain->result.at(i+2)->myNumber = brainPlayer->hand.at(z)->myNumber; brain->hand.at(z)->myRole = ROLE::ROLEIN;
			brain->brainCount -= 3;//�������ׂ��J�[�h�������Z
		}
	}
	////���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
	if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)brainPlayer->hand.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
		else brainPlayer->hand.at(i)->setRole(hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	}
	log("ok");
	//sort
	//sort(brain);//�\�[�g���ā[�P�̏��𖖔��ɁA�����Đ��ڂ��ď������������i�{�����j

	return brain;
};


Player* Player::check(Player *&brainHand, int x, int y, int z,int q) {//4�܂�����������̏���
	log("4");
	Player* brain(brainHand);
	//	if (brain.at(x)->myRole == ROLE::WITHOUT) {//���ʏ������ݐ�̖�����T��
	//											   //���ʂɂR���R�s�[���ĕۑ�
	//		brain.at(x)->setRole(ROLE::ROLEIN); //����t���āA�����ς݂̃J�[�h�Ƃ��ă}�[�N
	//		brain.at(y)->setRole(ROLE::ROLEIN);
	//		brain.at(z)->setRole(ROLE::ROLEIN);
	//		brain.at(q)->setRole(ROLE::ROLEIN);
	//		brainHandCount -= 4;//�������ׂ��J�[�h�������Z
	//	}
	////���ʏ��ɏ����ς݂̃J�[�h�����Ȃ����ċL�^
	//for (int i = 0; i < brain.size() || hand.at(i)>0; i++) {
	//	if (brain.at(i)->myRole == ROLE::ROLEIN)brain.at(i)->setRole(ROLE::WITHOUT);//�����ς݃J�[�h���o�@���@�|�P�ɂ���i�{���͏�̏����ł��΂����j
	//	else brain.at(i)->setRole(hand.at(i)->myRole);//���J�[�h��񂩂�J�[�h�����R�s�[(�������ɑg�ݍ��߂�j
	//}
	////sort
	//sort(brain);//�\�[�g���ā[�P�̏��𖖔��ɁA�����Đ��ڂ��ď������������i�{�����j

	return brain;
};

void Player::addPoint(int num)
{
	if (num < point) {
		point = num;
		for(int i = 0; i < hand.size(); i++) 
		{
			if(hand.at(i)->myNumber!=NUMBER::ZERO)
			{
				log("ok");
			}
		}
	}
}


//�\�[�g
void Player::sort(Vector<Card*> &temp) {
	for (int i = 0; i < hand.size() - 1; i++) {
		for (int k = i + 1; k < hand.size(); k++) {
			if (temp.at(i)->myRole < temp.at(k)->myRole) {//�召��r���Č������Ă���
				temp.swap(i, k);
			}
		}
	}
}