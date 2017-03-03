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

//true：表
//false：裏
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
	hand.pushBack(deck.at(DECK_TOP));
	deck.at(DECK_TOP)->setState(STATE::HAND);
	deck.erase(DECK_TOP);
};

//カードを捨てる
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

//役の初期化
void Player::ressetRole() 
{
	for (int i = 0; i < hand.size(); i++) 
	{
		hand.at(i)->setRole(ROLE::WITHOUT);
		hand.at(i)->setColor(Color3B::WHITE);
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

//役の得点の計算
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
	//３枚セットの組み合わせを全スキャン
	for (int z = 0; z < player->brainCount - 2; z++)
		for (int y = z + 1; y < player->brainCount - 1; y++)
			for (int x = y + 1; x < player->brainCount; x++) {
				if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber)) {//2枚そろっている
					if (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber) {//3枚そろっている
						if (x < player->brainCount - 1 && (player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber))
							checkRoleNew(check(player, z, y, x, x + 1));//４枚の処理へ　戻り値がカード情報なので、それで再起処理
						else
							checkRoleNew(check(player, z, y, x));//３枚の処理
					}
					else
					{
						chance(player, z, y);
					}
				}
				if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && player->hand.at(z)->myMark == player->hand.at(y)->myMark) {//2枚順番に並んでいる
					if (((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)) {//3枚
						if (((x < player->brainCount - 1) && (int)player->hand.at(z)->myNumber + 3 == (int)player->hand.at(x + 1)->myNumber) && (player->hand.at(z)->myMark == player->hand.at(x + 1)->myMark))
							checkRoleNew(check(player, z, y, x, x + 1));//4枚の処理
						else checkRoleNew(check(player, z, y, x));//３枚の処理
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

//二枚で三枚目の見込みがあるかもしれない
bool Player::chance(Player* &player, int x, int y) 
{
	return false;
};

Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3まいそろった時の処理
	//log("3");
	Player* brain=Player::create(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole != ROLE::ROLEIN)
		{
			//結果に３枚コピーして保存
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//役を付けて、処理済みのカードとしてマーク
			brain->result.at(x)->setRole(ROLE::ROLEIN);
			brain->result.at(x)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(y)->setRoleNumber(brain->RoleSplit);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(z)->setRoleNumber(brain->RoleSplit);
			brain->brainCount -= 3;//処理すべきカード枚数減算
			brain->RoleSplit++;
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)
			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
	}
	//sort(brain);
	return brain;
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z,int q) {//4まいそろった時の処理
	//log("4");
	Player* brain = Player::create(brainPlayer);
	
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myRole != ROLE::ROLEIN)
		{
			//結果に３枚コピーして保存
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//役を付けて、処理済みのカードとしてマーク
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
			brain->brainCount -= 4;//処理すべきカード枚数減算
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if(brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)
		if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
		else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
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

//ソート
void Player::sort(Player* &p) 
{
	for (int i = 0; i < p->hand.size(); i++)
	{
		//一番右のカードを見るまで
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
	//頭の中の計算
	brainCount = 0;
	//得点
	point=5000;
	//選んだ番号
	pickNumber = -1;
	//選んだパイル
	pickState=STATE::HAND;
	//役の組み分け
	RoleSplit=0;
	//手札とそれの役情報が入る仮想手札
	handDeath();
	//ノックできるか
	brainEnd=false;
};
