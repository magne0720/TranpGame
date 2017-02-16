#include "MainGameLayer.h"

MainGameLayer* MainGameLayer::create(int level)
{
	MainGameLayer *pRet = new MainGameLayer();
	if (pRet && pRet->init(level))
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

bool MainGameLayer::init(int level) 
{
	if (!Layer::init()) 
	{
		return false;
	}

	//タッチ判定
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MainGameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainGameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainGameLayer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	dealer = Dealer::create(Vec2(designResolutionSize.width*0.3f, designResolutionSize.height*0.5f));
	addChild(dealer);

	player_one = Player::create();
	player_one->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.2f));
	addChild(player_one);

	player_two = Player::create();
	player_two->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.8f));
	addChild(player_two);

	//カードを配る
	dealer->cardShuffle();
	cardDivision();
	startPlayer();
	gameStart();
	scheduleUpdate();

	return true;
};

void MainGameLayer::update(float delta) 
{



};

//ーーーーーーーーーーゲームの準備関数ーーーーーーーーー


//誰が一番初めにゲームを始めるか
void MainGameLayer::startPlayer() 
{
	int iRand = rand() % 2 + 1;
	turn = (TURN)iRand;
};

//カードを配る(初期カードの配置)
void MainGameLayer::cardDivision() 
{
	//10回
	for (int i = 0; i < HAND_START_MAX; i++)
	{
		dealer->cardDrow(player_one->hand);
		dealer->cardDrow(player_two->hand);
		player_one->cardDispHand();
		player_two->cardDispHand();
	}
};

//スタート
void MainGameLayer::gameStart() 
{

};

//ーーーーーーーーーーゲーム中に行う関数ーーーーーーーー
//ターンチェンジ
void MainGameLayer::NextPlayerTurn() 
{
	if (turn == TURN::PLAY_ONE) 
	{
		turn = TURN::PLAY_TWO;
	}
	else if(turn==TURN::PLAY_TWO)
	{
		turn = TURN::PLAY_ONE;
	}
};

//フェイズチェンジ
void MainGameLayer::NextPhase() 
{
	switch (phase)
	{
	case MainGameLayer::START:
		phase = PHASE::DROW;
		break;
	case MainGameLayer::DROW:
		if (turn == TURN::PLAY_ONE)dealer->cardDrow(player_one->hand);
		else if (turn == TURN::PLAY_TWO)dealer->cardDrow(player_two->hand);
		phase = PHASE::THROW;
		break;
	case MainGameLayer::THROW:
		if (isKnock)phase == PHASE::KNOCK;
		else phase == PHASE::END;
		break;
	case MainGameLayer::KNOCK:
		callKnock();
		break;
	case MainGameLayer::END:
		phase == PHASE::START;
		NextPlayerTurn();
		break;
	default:
		break;
	}
};

//ノック
void MainGameLayer::callKnock() 
{
	turn = TURN::WAIT;
	for (int i = 0; i < player_one->hand.size(); i++) 
	{

	}
	for (int i = 0; i < player_two->hand.size(); i++) 
	{

	}
};

//ノック時に行われる役の計算
void MainGameLayer::callCalculation() 
{

};

//ゲーム終了時の計算
void MainGameLayer::callAddTotal() 
{

};

//ーーーーーーーーーーーー

bool MainGameLayer::onTouchBegan(const Touch * touch, Event *unused_event) 
{


	if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
//		if (dealer->deck.size() <= 0)
		{

			player_one->handDeath();
			player_two->handDeath();
			dealer->setDeck(true);
			dealer->cardShuffle();
			cardDivision();
			player_one->checkFame(0);
		}
		//else
		//{
		//	dealer->cardDrow(player_one->hand);
		//	dealer->cardDrow(player_two->hand);
		//	player_one->cardDispHand();
		//	player_two->cardDispHand();

		//}

	}




	return true;
};

void MainGameLayer::onTouchMoved(const Touch * touch, Event *unused_event) 
{

};

void MainGameLayer::onTouchEnded(const Touch * touch, Event *unused_event) 
{

};

