#include "Player.h"

bool Player::init() 
{
	if (!Node::init())
	{
		return false;
	}


	return true;
};

void Player::update(float delta) 
{

};

void Player::cardDispHand() 
{
	//手札の数だけ
	for (int i = 0; i < hand.size(); i++)
	{
		//一番右のカードを見るまで
		for (int j = 0; j+1<hand.size(); j++)
		{
			if ((int)hand.at(j)->myNumber > (int)hand.at(j + 1)->myNumber)
				{
					hand.swap(j, j + 1);
				}
		}
	}
	log("%d", hand.size());
	for (int i = 0; i < hand.size(); i++)
	{
		hand.at(i)->setMyPosition(Vec2(150 * i + getPositionX() / 3, getPositionY()));
	}
};

void Player::cardThrow(Card* &card) 
{

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
void Player::checkFame(int x)
{
	static int count = 0;
	MoveBy* move = MoveBy::create(1.0, Vec2(0, 100));

	//10回
	for (int i = 0; i < hand.size(); i++)
	{
		//最後のカードかどうか
		if (x < hand.size())
		{
			//自身でないかかどうか
			if (i != x)
			{
				//マークが一緒か
				if (hand.at(x)->myMark == hand.at(i)->myMark)
				{
					if (count >= 3)
					{
						log("SUPER");
					}
					//番号が1だけ差がある
					if (hand.at(x)->myNumber + 1 == hand.at(i)->myNumber)
					{
						hand.at(i)->runAction(move);
						hand.at(i)->setColor(Color3B::YELLOW);
						hand.at(x)->setColor(Color3B::GREEN);
						log("getRed%d", i);
						count++;
					}
				}
			}
		}
	}
	count = 0;
	//最後のカードかどうか
	if (x < hand.size())
	{
		log("%d", x);
		checkFame(x + 1);
	}//刻子(同じ番号)を見る

};

void Player::checkOrder(int num) 
{

}
