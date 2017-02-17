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

void Player::checkRole(int i)
{
	chanceRole(hand.at(i), ROLE::ORDER);
	for (int j = i; j < hand.size(); j++)//0から始めるのが基本なのでその前のカードは調べない
	{
		if (i != j&&														//自身でないかかどうか
			hand.at(i)->myMark == hand.at(j)->myMark &&						//マークが一緒か
			(int)hand.at(i)->myNumber + 1 == (int)hand.at(j)->myNumber)		//番号が1だけ差がある
		{
			checkRole(j);
		chanceRole(nullptr, ROLE::ORDER, true);
		}
	
	}
	if (i == hand.size() - 1) 
	{
		//log("get%d", i);
	}
	else
	{
		checkRole(i+1);
	}
};

void Player::chanceRole(Card* card ,ROLE role,bool isAllCheck) 
{//デバッグ用
	//MoveBy* move = MoveBy::create(1.0, Vec2(0, 100));

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
						//hand.at(i)->runAction(move);
					}
				}
			}
			break;
		case EQUAL:
			break;
		default:
			break;
		}
		//頭を空っぽに
	}
	brain.clear();
};

void Player::checkOrder(int num) 
{

}
