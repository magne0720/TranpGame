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

//true：表
//false：裏
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
					hand.swap(j, j + 1);
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
	myHand->hand.pushBack(deck.at(deck.size()-1));
	deck.at(deck.size()-1)->setState(STATE::HAND);
	deck.erase(deck.size()-1);
};

//カードを捨てる
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

//役の初期化
void Player::ressetRole() 
{
	for (int i = 0; i <myHand->hand.size(); i++)
	{
		myHand->hand.at(i)->setRole(ROLE::WITHOUT);
		myHand->hand.at(i)->setColor(Color3B::WHITE);
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

//役の得点の計算
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

//最後のカード確認
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
	//３枚セットの組み合わせを全スキャン
	for (int z = 0; z < cHand->brainCount - 2; z++)
		for (int y = z + 1; y < cHand->brainCount - 1; y++)
			for (int x = y + 1; x < cHand->brainCount; x++) {
				if ((cHand->hand.at(z)->myNumber == cHand->hand.at(y)->myNumber)) {//2枚そろっている
					if (cHand->hand.at(y)->myNumber == cHand->hand.at(x)->myNumber) {//3枚そろっている
						if (x < cHand->brainCount - 1 && (cHand->hand.at(x)->myNumber == cHand->hand.at(x + 1)->myNumber))
							checkRoleNew(check(myHand, z, y, x, x + 1));//４枚の処理へ　戻り値がカード情報なので、それで再起処理
						else
							checkRoleNew(check(myHand, z, y, x));//３枚の処理
					}
					else
					{
						//chance(myHand, z, y);
					}
				}
				if (((int)cHand->hand.at(z)->myNumber + 1 == (int)cHand->hand.at(y)->myNumber) && cHand->hand.at(z)->myMark == cHand->hand.at(y)->myMark) {//2枚順番に並んでいる
					if (((int)cHand->hand.at(z)->myNumber + 2 == (int)cHand->hand.at(x)->myNumber) && (cHand->hand.at(z)->myMark == cHand->hand.at(x)->myMark)) {//3枚
						if (((x < cHand->brainCount - 1) && (int)cHand->hand.at(z)->myNumber + 3 == (int)cHand->hand.at(x + 1)->myNumber) && (cHand->hand.at(z)->myMark == cHand->hand.at(x + 1)->myMark))
							checkRoleNew(check(myHand, z, y, x, x + 1));//4枚の処理
						else checkRoleNew(check(myHand, z, y, x));//３枚の処理
					}
					else
					{
						//chance(myHand, z, y);
					}
				}
			}
	sort(cHand);

};

//二枚で三枚目の見込みがあるかもしれない
bool Player::chance(Brain* player, int x, int y)
{
	log("chance[%d]-[%d]", x, y);
	player->result.at(x)->setRole(ROLE::CHANCE);
	player->result.at(y)->setRole(ROLE::CHANCE);
	return false;
};

Brain* Player::check(Brain* brainPlayer, int x, int y, int z) {//3まいそろった時の処理
	//log("3");
	Brain* brain(brainPlayer);
	//log("brainCount=%d", brain->brainCount);
	for (int i = 0; i < brain->hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole != ROLE::WITHOUT)
		{
			//結果に３枚コピーして保存
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//役を付けて、処理済みのカードとしてマーク
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->brainCount -= 3;//処理すべきカード枚数減算
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN) {

			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
		}
		else {
			break;
		}
	}
	//sort(brain);
	return brain;
};


Brain* Player::check(Brain* brainPlayer, int x, int y, int z,int q) {//4まいそろった時の処理
	//log("4");
	Brain* brain(brainPlayer);
	
	for (int i = 0; i < brain->hand.size() - 3; i++) {
		if (brain->result.at(i)->myRole != ROLE::WITHOUT)
		{
			//結果に３枚コピーして保存
			brain->hand.at(x)->setRole(ROLE::ROLEIN);//役を付けて、処理済みのカードとしてマーク
			brain->result.at(i)->setRole(ROLE::ROLEIN);
			brain->hand.at(y)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 1)->setRole(ROLE::ROLEIN);
			brain->hand.at(z)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 2)->setRole(ROLE::ROLEIN);
			brain->hand.at(q)->setRole(ROLE::ROLEIN);
			brain->result.at(i + 3)->setRole(ROLE::ROLEIN);
			//log("%d-%d-%d-GetRole", hand.at(x)->myNumber, hand.at(y)->myNumber, hand.at(z)->myNumber);
			brain->brainCount -= 4;//処理すべきカード枚数減算
			break;
		}
	}
	//結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
		if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN) {
			if (brain->hand.at(i)->myRole == ROLE::ROLEIN)brain->hand.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
			else brain->hand.at(i)->setRole(brainPlayer->hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
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

//ソート
void Player::sort(Brain* &p) 
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
	myHand->brainCount = 0;
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

void Player::cardSortMarge(Vector<Card*> cResult) 
{
	for (int i = 0; i < myHand->hand.size(); i++) {
		for (int j = 0; j < myHand->hand.size(); j++) {
			//カード情報が同じならそろえる
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
