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

//true：表
//false：裏
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
	brainEnd = false;
	brainHandCount = hand.size();
	checkRoleNew(this);
	brainEnd = true;
};

//役の得点の計算
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
	//３枚セットの組み合わせを全スキャン
	for (int z = 0; z < player->brainCount - 2; z++)
		for (int y = z + 1; y < player->brainCount - 1; y++)
			for (int x = y + 1; x < player->brainCount ; x++) {
				if ((player->hand.at(z)->myNumber == player->hand.at(y)->myNumber) && (player->hand.at(y)->myNumber == player->hand.at(x)->myNumber)) {//３枚そろっている
					if (x < player->brainCount - 1 && x < player->hand.size() - 3)
						if ((player->hand.at(x)->myNumber == player->hand.at(x + 1)->myNumber) //４枚も一緒
							)
							checkRoleNew(check(player, z, y, x, x + 1));//４枚の処理へ　戻り値がカード情報なので、それで再起処理
						else checkRoleNew(check(player, z, y, x));//３枚の処理
				}
				if (((int)player->hand.at(z)->myNumber + 1 == (int)player->hand.at(y)->myNumber) && ((int)player->hand.at(z)->myNumber + 2 == (int)player->hand.at(x)->myNumber) &&//順番に並んでいる
					(player->hand.at(z)->myMark == player->hand.at(y)->myMark) && (player->hand.at(z)->myMark == player->hand.at(x)->myMark)//マークが同じ
					) checkRoleNew(check(player, z, y, x));//３枚の処理
			}
				addPoint(calcRole(player->result));
};


Player* Player::check(Player* &brainPlayer, int x, int y, int z) {//3まいそろった時の処理
	log("3");
	Player* brain(brainPlayer);
	for (int i = 0; i < hand.size() - 2; i++) {
		if (brain->result.at(i)->myRole==ROLE::WITHOUT) {
			//結果に３枚コピーして保存
			brain->result.at(i)->myNumber=brainPlayer->hand.at(x)->myNumber; brain->hand.at(x)->myRole=ROLE::ROLEIN;//役を付けて、処理済みのカードとしてマーク
			brain->result.at(i+1)->myNumber = brainPlayer->hand.at(y)->myNumber; brain->hand.at(y)->myRole = ROLE::ROLEIN;
			brain->result.at(i+2)->myNumber = brainPlayer->hand.at(z)->myNumber; brain->hand.at(z)->myRole = ROLE::ROLEIN;
			brain->brainCount -= 3;//処理すべきカード枚数減算
		}
	}
	////結果情報に処理済みのカード情報をなくして記録
	for (int i = 0; i < brainPlayer->hand.size(); i++) {
	if (brainPlayer->hand.at(i)->myRole == ROLE::ROLEIN)brainPlayer->hand.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
		else brainPlayer->hand.at(i)->setRole(hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
	}
	log("ok");
	//sort
	//sort(brain);//ソートしてー１の情報を末尾に、そして整頓して処理を効率化（本当か）

	return brain;
};


Player* Player::check(Player *&brainHand, int x, int y, int z,int q) {//4まいそろった時の処理
	log("4");
	Player* brain(brainHand);
	//	if (brain.at(x)->myRole == ROLE::WITHOUT) {//結果書き込み先の末尾を探す
	//											   //結果に３枚コピーして保存
	//		brain.at(x)->setRole(ROLE::ROLEIN); //役を付けて、処理済みのカードとしてマーク
	//		brain.at(y)->setRole(ROLE::ROLEIN);
	//		brain.at(z)->setRole(ROLE::ROLEIN);
	//		brain.at(q)->setRole(ROLE::ROLEIN);
	//		brainHandCount -= 4;//処理すべきカード枚数減算
	//	}
	////結果情報に処理済みのカード情報をなくして記録
	//for (int i = 0; i < brain.size() || hand.at(i)>0; i++) {
	//	if (brain.at(i)->myRole == ROLE::ROLEIN)brain.at(i)->setRole(ROLE::WITHOUT);//処理済みカード検出　→　－１にする（本当は上の処理でやればいい）
	//	else brain.at(i)->setRole(hand.at(i)->myRole);//元カード情報からカード情報をコピー(これも上に組み込める）
	//}
	////sort
	//sort(brain);//ソートしてー１の情報を末尾に、そして整頓して処理を効率化（本当か）

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


//ソート
void Player::sort(Vector<Card*> &temp) {
	for (int i = 0; i < hand.size() - 1; i++) {
		for (int k = i + 1; k < hand.size(); k++) {
			if (temp.at(i)->myRole < temp.at(k)->myRole) {//大小比較して交換していく
				temp.swap(i, k);
			}
		}
	}
}