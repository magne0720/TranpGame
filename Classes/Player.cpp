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
	//番号順
	case ROLE::ORDER:
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
				//同じ数字でマーク順をそろえる
				if ((int)hand.at(j)->myMark > (int)hand.at(j + 1)->myMark && (int)hand.at(j)->myNumber ==(int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
			}
		}
		break;
	//マーク順
	case ROLE::EQUAL:
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
	case ROLE::WITHOUT:
		//出来たやくの順番
		for (int i = 0; i < hand.size(); i++)
		{
			//一番右のカードを見るまで
			for (int j = 0; j + 1<hand.size(); j++)
			{
				//マーク順に並び替え
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
		hand.at(i)->myRole = ROLE::WITHOUT;
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

int Player::checkRole()
{
	int one, two;
	//刻子→順子
	checkEqual(0);
	checkOrder(0);
	one=calcRole();
	ressetRole();
	//順子→刻子
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
	//役ができそうなものを入れておく
	if (card != nullptr)
	{
		brain.pushBack(card);
	}
	//まだすべてを見ていないなら
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
				else//3枚役
				{
					brain.at(i)->setRole(ROLE::ORDER);
					brain.at(i)->setColor(Color3B::MAGENTA);
					log("order-%d", i);
				}
			}
		}
		break;
	case EQUAL://絶対４枚まで
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
				else//3枚役
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
	//頭を空っぽに
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
			log("%d-plus%d", i,(int)hand.at(i)->myNumber);
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
			hand.at(num)->myNumber + 1 == hand.at(j)->myNumber)					//番号が1だけ差がある
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
			hand.at(num)->myNumber == hand.at(j)->myNumber)		//番号が同じか
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

//四枚刻子の一枚抜きOK検査
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

//四枚順子の一枚抜きOK検査
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
