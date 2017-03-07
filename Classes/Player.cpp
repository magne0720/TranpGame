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

//true：表
//false：裏
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
	//番号順
	case ROLE::ORDER:
		//手札の数だけ
		for (int i = 0; i < cardhand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				if ((int)cardhand.at(j)->myNumber > (int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
				//同じ数字でマーク順をそろえる
				if ((int)cardhand.at(j)->myMark > (int)cardhand.at(j + 1)->myMark && (int)cardhand.at(j)->myNumber ==(int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
			}
		}
		break;
	//マーク順
	case ROLE::EQUAL:
		//手札の数だけ
		for (int i = 0; i < cardhand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				//マーク順に並び替え
				if ((int)cardhand.at(j)->myMark > (int)cardhand.at(j + 1)->myMark)
				{
					cardhand.swap(j, j + 1);
				}
				//同じマークで数字をそろえる
				if ((int)cardhand.at(j)->myMark == (int)cardhand.at(j + 1)->myMark && (int)cardhand.at(j)->myNumber >(int)cardhand.at(j + 1)->myNumber)
				{
					cardhand.swap(j, j + 1);
				}
			}
		}
		break;
	case ROLE::WITHOUT:
		//出来たやくの順番
		for (int i = 0; i < cardhand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<cardhand.size(); j++)
			{
				//マーク順に並び替え
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

//ドロー
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

//カードを捨てる
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

//役の初期化
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

//役を見る
//一つずつカードを見る
//連番(同じマーク、次の数)を見つけるまで検査
//発見出来たらそこからもう一つ見つける
//もう一つあったらもう一度見つける(終わるまで繰り返す)
//最後まで見たら見つけたカードを役付きで設定する
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

//役の得点の計算
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

//最後のカード確認
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
	//３枚セットの組み合わせを全スキャン
	for (int z = 0; z < player->hand.size() - 2; z++)
		for (int y = z + 1; y < player->hand.size() - 1; y++)
			for (int x = y + 1; x < player->hand.size(); x++) {
				if (player->hand.at(x)->myNumber>ZERO) {
					if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber)) {//2枚そろっている
						if (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber) {//3枚そろっている
							if (x < player->brainCount - 1 && (player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber))
								checkRoleNew(check(player, z, y, x, x + 1));//４枚の処理へ　戻り値がカード情報なので、それで再起処理
							else
								checkRoleNew(check(player, z, y, x));//３枚の処理
						}
						else
						{
							//chance(player, z, y);
						}
					}
					if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && player->hand.at(z)->myMark == player->hand.at(y)->myMark) {//2枚順番に並んでいる
						if (((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)) {//3枚
							for (int q = x + 1; q < hand.size(); q++)
								if (((int)player->hand.at(z)->myNumber + 3 == (int)player->hand.at(q)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(q)->myMark)) {
									checkRoleNew(check(player, z, y, x, q));//4枚の処理
									isFourCard = true;
								}
							if (!isFourCard)
								checkRoleNew(check(player, z, y, x));//３枚の処理
						}
						else
						{
							//chance(player, z, y);
						}
					}
				}
			}
	//残りのカードをコピー
	copyResultForHand(player);

		calcRole(player->result);
};

//二枚で三枚目の見込みがあるかもしれない
void Player::check(Player* &player, int x, int y) 
{

};

Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3まいそろった時の処理
	//log("3");
	Player* brain=Player::create(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myNumber <= NUMBER::ZERO)
		{
			log("%d.%d.%d", brain->hand.at(x)->myNumber, brain->hand.at(y)->myNumber, brain->hand.at(z)->myNumber);

			//結果に３枚コピーして保存
			brain->result.at(i)->setKind(brainPlayer->hand.at(x));	brain->hand.at(x)->setKind(NONE, ZERO);//0にして、処理済みのカードとしてマーク
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
			brain->brainCount -= 3;//処理すべきカード枚数減算
			brain->roleCount++;
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myMark == MARK::NONE&&brainPlayer->hand.at(i)->myNumber == ZERO)
		{
			break;
		}
		if (brain->hand.at(i)->myNumber == ZERO)brain->hand.at(i)->setKind(NONE, ZERO);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
		else brain->hand.at(i)->setKind(brainPlayer->hand.at(i));//元カード情報からカード情報をコピー(これも上に組み込める）
	}
	sort(brain);
	return brain;
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z,int q) {//4まいそろった時の処理
	//log("4");

	Player* brain = Player::create(brainPlayer); 
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myNumber <= NUMBER::ZERO)
		{
			log("%d.%d.%d.%d", brain->hand.at(x)->myNumber, brain->hand.at(y)->myNumber, brain->hand.at(z)->myNumber, brain->hand.at(q)->myNumber);

			//結果に３枚コピーして保存
			//結果に３枚コピーして保存
			brain->result.at(i)->setKind(brainPlayer->hand.at(x));		brain->hand.at(x)->setKind(NONE, ZERO);//0にして、処理済みのカードとしてマーク
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
			brain->brainCount -= 4;//処理すべきカード枚数減算
			brain->roleCount++;
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myMark == MARK::NONE&&brainPlayer->hand.at(i)->myNumber == NUMBER::ZERO)
		{
			break;
		}
		if (brain->hand.at(i)->myNumber == ZERO)brain->hand.at(i)->setKind(NONE, ZERO);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
		else brain->hand.at(i)->setKind(brainPlayer->hand.at(i));//元カード情報からカード情報をコピー(これも上に組み込める）
	}
	
	sort(brain);
	
	return brain;
};

bool Player::addPoint(int num)
{
		if (num <= point) {
		point = num;
		if (point <= 10)brainEnd = true;//ノック可能
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

//ソート
void Player::sort(Player* &p) 
{
	for (int i = 0; i < p->hand.size(); i++)
	{
		//一番右のカードを見るまで
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
	//頭の中の計算
	brainCount = 10;
	//得点
	point=5000;
	//選んだ番号
	pickNumber = -1;
	//選んだパイル
	pickState=STATE::HAND;
	//手札とそれの役情報が入る仮想手札
	handDeath();
	//ノックできるか
	brainEnd=false;
};

//残りのカードをリザルトに追加
void Player::copyResultForHand(Player* &p) 
{
	for (int i = 0; i < hand.size(); i++) {
		if (p->hand.at(i)->myNumber>NUMBER::ZERO&&p->brainCount>=0) {
			p->brainCount--;
			p->result.at(p->brainCount)->setKind(hand.at(i));
		}
	}
};

//役ができなかったカードの中で一番数が大きいものを特定する
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
