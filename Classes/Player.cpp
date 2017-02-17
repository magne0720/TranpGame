#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}
	pickNumber = 0;
	

	return true;
};

void Player::update(float delta) 
{

};

void Player::cardDispHand() 
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
	}
};
void Player::cardSort(int num)
{
	switch (num)
	{
	//番号順
	case 0:
		//手札の数だけ
		for (int i = 0; i < hand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<hand.size(); j++)
			{
				if ((int)hand.at(j)->myNumber > (int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
			}
		}
		break;
	//マーク順
	case 1:
		//手札の数だけ
		for (int i = 0; i < hand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<hand.size(); j++)
			{
				//マーク順に並び替え
				if ((int)hand.at(j)->myMark > (int)hand.at(j + 1)->myMark)
				{
					hand.swap(j, j + 1);
				}
				//同じマークで数字をそろえる
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

//ドロー
void Player::cardDrow(Vector<Card*>&deck)
{
	if (deck.size() <= 0)
	{
		return;
	}
	hand.pushBack(deck.at(0));
	deck.at(0)->setGlobalZOrder(3);
	deck.at(0)->setState(STATE::HAND);
	deck.erase(0);
};

Card* Player::cardThrow(int num,Vector<Card*>&grave) 
{
	Card* card = Card::create(hand.at(num)->myMark, hand.at(num)->myNumber);
	grave.pushBack(hand.at(num));
	hand.at(num)->removeFromParentAndCleanup(true);
	hand.erase(num);
	return card;
};

void Player::handDeath() 
{
	for (int i = 0; i < hand.size();i++)
	{
		hand.at(i)->removeFromParentAndCleanup(true);
	}
	hand.clear();
};

//役を見る
//一つずつカードを見る
//連番(同じマーク、次の数)を見つけるまで検査
//発見出来たらそこからもう一つ見つける
//もう一つあったらもう一度見つける(終わるまで繰り返す)
//最後まで見たら見つけたカードを役付きで設定する
//

void Player::checkRole()
{
	int one, two;
	//刻子→順子
	checkEqual(0);
	checkOrder(0);
	one=calcRole();
	//順子→刻子
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
	//役ができそうなものを入れておく
	if (card != nullptr) 
	{
		brain.pushBack(card);
	}
	//まだすべてを見ていないなら
	if (!isAllCheck)
	{
		//抜ける
		return;
	}
	if (brain.size() >= 3)//三枚から役ができる 
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
		case EQUAL:
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
		//頭を空っぽに
	}
	brain.clear();
};

//役の得点の計算
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

//マークが同じ、順番のカード
void Player::checkOrder(int num) 
{
	//自身を役の可能性に追加
	chanceRole(hand.at(num), ROLE::ORDER);
	//いったん自身を役から外す
	//hand.at(num)->setRole(ROLE::WITHOUT);

	for (int j = num; j < hand.size(); j++)//0から始めるのが基本なのでその前のカードは調べない
	{
		if (num != j&&															//自身でないかかどうか
			hand.at(num)->myMark == hand.at(j)->myMark &&						//マークが一緒か
			(int)hand.at(num)->myNumber + 1 == (int)hand.at(j)->myNumber)		//番号が1だけ差がある
		{
			checkOrder(j);
		}
	}
	chanceRole(nullptr, ROLE::ORDER, true);
	if (num != hand.size() - 1)
	{
		checkOrder(num + 1);
	}
};

//マーク関係なし、番号が同じ
void Player::checkEqual(int num) 
{
	//自身を役の可能性に追加
	chanceRole(hand.at(num), ROLE::EQUAL);
	//いったん自身を役から外す
	//hand.at(num)->setRole(ROLE::WITHOUT);

	for (int j = num; j < hand.size(); j++)//0から始めるのが基本なのでその前のカードは調べない
	{
		if (num != j &&														//自身でないかかどうか
			(int)hand.at(num)->myNumber == (int)hand.at(j)->myNumber)		//番号が同じか
		{
			checkEqual(j);
		}
	}
	chanceRole(nullptr, ROLE::EQUAL, true);
	if (num != hand.size() - 1)
	{
		checkEqual(num + 1);
	}
};
