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

	addChild(LayerColor::create(Color4B::GREEN));

	//タッチ判定
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MainGameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainGameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainGameLayer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	dealer = Dealer::create(
		Vec2(designResolutionSize.width*0.4f, designResolutionSize.height*0.5f),
		Vec2(designResolutionSize.width*0.6f, designResolutionSize.height*0.5f));

	addChild(dealer,0);

	player_one = Player::create();
	player_one->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.2f));
	addChild(player_one,1);

	player_two = Player::create();
	player_two->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.8f));
	addChild(player_two,1);

	turnLabel = Label::create("gameSTART", "fonts/arial.ttf", 30);
	turnLabel->setPosition(designResolutionSize.width*0.5f, designResolutionSize.height*0.5f);
	addChild(turnLabel);

	phaseLabel = Label::create("gameSTART", "fonts/arial.ttf", 30);
	phaseLabel->setPosition(designResolutionSize.width*0.7f, designResolutionSize.height*0.5f);
	addChild(phaseLabel);

	P_ONE_LABEL = Label::create("point", "fonts/arial.ttf", 30);
	P_ONE_LABEL->setPosition(designResolutionSize.width*0.3f, designResolutionSize.height*0.4f);
	addChild(P_ONE_LABEL);

	P_TWO_LABEL = Label::create("point", "fonts/arial.ttf", 30);
	P_TWO_LABEL->setPosition(designResolutionSize.width*0.3f, designResolutionSize.height*0.6f);
	addChild(P_TWO_LABEL);

	effectManager = EffectManager::create();
	addChild(effectManager);

	button = SortButton::create(ROLE::EQUAL,Vec2(designResolutionSize.width *0.05f, designResolutionSize.height*0.5f));
	addChild(button);

	//ゲームの準備
	gameStart();

	//effectManager->phaseChange(PHASE::START);

	
	scheduleUpdate();

	return true;
};

void MainGameLayer::update(float delta)
{
	static float timer = 0;
	timer += delta;
	if (dealer->grave.size() > 0) 
	{
	//if (dealer->grave.at(dealer->GRAVE_TOP)->myMark == MARK::SPADE) 
		player_one->pickState = STATE::GRAVE;
	}
	else
	{
		player_one->pickState = STATE::DECK;
	}


	if (turn != TURN::WAIT)
	{
		nextPhase(actionPhase());
		timer = 0;
	}
	else
	{
		if(timer>=3.0f)
		{
			gameStart();
			timer = 0;
		}
	}
};

//ーーーーーーーーーーゲームの準備関数ーーーーーーーーー


//誰が一番初めにゲームを始めるか
void MainGameLayer::startPlayer() 
{
	int iRand = rand() % 2 + 1;
	//turn = (TURN)iRand;
	turn = TURN::PLAY_ONE;
	String* name = String::createWithFormat("Player_%d_start", iRand);
	turnLabel->setString(name->getCString());
};

//カードを配る(初期カードの配置)
void MainGameLayer::cardDivision() 
{
	//10回
	for (int i = 0; i < HAND_START_MAX; i++)
	{
		player_one->cardDrow(dealer->deck);
		player_two->cardDrow(dealer->deck);
	}
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
};

//スタート
void MainGameLayer::gameStart()
{
	//カウントの初期化
	turnCount = 1;
	isPass = false;
	//フェイズの初期化
	phase = PHASE::START;
	//お互いの初期化
	player_one->RessetPlayer();
	player_two->RessetPlayer();
	//デッキを再構築
	dealer->setDeck(true);
	//デッキをシャッフル
	dealer->cardShuffle();
	//プレイヤーに10枚ずつ交互に配る
	cardDivision();
	//デッキの上から一枚捨て札に送る
	dealer->cardDeckThrow();
	//先行を決める
	startPlayer();
	//始まる
	nextPhase(true);
};

//ーーーーーーーーーーゲーム中に行う関数ーーーーーーーー

//タッチが離された時にプレイヤーが行う行動
//trueを返すとき：その処理を行うと自動的に次のフェイズに行くとき
//falseを返すとき：行う処理が同じフェイズ内で何度行ってもよいとき
bool MainGameLayer::actionPhase() 
{
	switch (phase)
	{
	case PHASE::START:
		return true;
	case PHASE::DROW:
		if (turn == TURN::PLAY_ONE)//プレイヤー１
		{
			if (player_one->pickState == STATE::DECK)//デッキから引くを選ぶ
			{
				if (isPass)//自身が後攻で先行がパスしていたら
				{
					if (turnCount >= 3)//お互いのターンが3ターン目以降
					{
						player_one->cardDrow(dealer->deck); 
						return true;
					}
				}
				else//自身が先行かパスを行わない場合
				{
					if(turnCount>=2)//お互いのターンが2ターン目以降
					{
						player_one->cardDrow(dealer->deck);
						return true;
					}
				}
			}
			else if (player_one->pickState == STATE::GRAVE)//捨て札から引くを選ぶ
			{
				player_one->cardDrow(dealer->grave);
				return true;
			}
			else if (player_one->pickState == STATE::FREE)
			{
				if (turnCount==1)//1ターン目のみ 
				{
					isPass = true;
					return true;
				} 
			}
		}
		else if (turn == TURN::PLAY_TWO)//プレイヤー２
		{
			player_two->cardDrow(dealer->deck);
			//player_two->checkRole();
			return true;
		}
		return false;
	case PHASE::THROW:	
		player_one->pickNumber = random(0, (int)player_one->hand.size()-1);
		player_two->pickNumber = random(0, (int)player_one->hand.size()-1);

		if (turn == TURN::PLAY_ONE)
		{
			if(player_one->brainEnd)
			{
				isKnock = true;
				return true;
			}
			if (player_one->pickNumber >= 0)
			{
				player_one->cardThrow(player_one->pickNumber, dealer->grave);
				return true;
			}
		}
		else if (turn == TURN::PLAY_TWO)
		{
			if (player_two->pickNumber >= 0)
			{
				player_two->cardThrow(player_two->pickNumber, dealer->grave);
				return true;
			}
		}
		return false;
	case PHASE::KNOCK:
		return true;
	case PHASE::END:
		return true;
	case PHASE::PASS:
		return true;
	default:
		return false;
	}
	return false;
};

//ターンチェンジ
void MainGameLayer::nextPlayerTurn() 
{
	if (turn == TURN::PLAY_ONE) 
	{
		turn = TURN::PLAY_TWO;
		turnLabel->setString("PLAYER_TWO");
	}
	else if(turn==TURN::PLAY_TWO)
	{
		turn = TURN::PLAY_ONE;
		turnLabel->setString("PLAYER_ONE");
	}
};

//フェイズチェンジ(プレイヤーが選択を行いtrueが引数に渡されたら発動)
//フェイズごとに一度行う行動
void MainGameLayer::nextPhase(bool isAction) 
{
	if (!isAction) 
	{
		return;
	}
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
	effectManager->phaseChange(phase);
	switch (phase)
	{
	case PHASE::START:
		phase = PHASE::DROW;
		phaseLabel->setString("DROW");
		break;
	case PHASE::DROW:
		phase = PHASE::THROW;
		phaseLabel->setString("THROW");

		if (isPass)
		{
			isPass = false;
			phase = PHASE::PASS;
			phaseLabel->setString("PASS");
		}
		player_one->cardDispHand(true);
		player_two->cardDispHand(false);
		//player_one->checkRole();
			break;
	case PHASE::THROW:
			if (isKnock||dealer->deck.size()<=0)
			{
				phase = PHASE::KNOCK;
				phaseLabel->setString("KNOCK");
			}
			else
			{
				phase = PHASE::END;
				phaseLabel->setString("END");
			}
		break;
	case PHASE::KNOCK:
		callKnock();
		break;
	case PHASE::END:
		phaseLabel->setString("START");
		phase = PHASE::START;
		nextPlayerTurn();
		turnCount++;
		break;
	case PHASE::PASS:
		phaseLabel->setString("END");
		phase = PHASE::END;
		break;
	default:
		phaseLabel->setString("DROW"); 
		break;
	}
	player_one->pickState = STATE::HAND;
	dealer->cardDispGrave();
	dealer->checkDeckZero();
};

//ノック
void MainGameLayer::callKnock() 
{
	turn = TURN::WAIT;
	player_one->cardDispHand(true);
	player_two->cardDispHand(true);
	player_one->checkRole();
	player_two->checkRole();
	String* name1 = String::createWithFormat("%d", player_one->point);
	String* name2 = String::createWithFormat("%d", player_two->point);
	P_ONE_LABEL->setString(name1->getCString());
	P_TWO_LABEL->setString(name2->getCString());
	if (player_one->point < player_two->point)
	{
		turnLabel->setString("PLAYER_ONE\nWIN");
	}
	else if(player_one->point>player_two->point)
	{
		turnLabel->setString("PLAYER_TWO\nWIN");
	}
	else
	{
		turnLabel->setString("DRAW");
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

	}

	if (touch->getLocation().y > designResolutionSize.height*0.9f)
	{
		gameStart();
	};
	

	return true;
};

void MainGameLayer::onTouchMoved(const Touch * touch, Event *unused_event) 
{

};

void MainGameLayer::onTouchEnded(const Touch * touch, Event *unused_event) 
{


	if (button->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->cardSort(button->switchRole,player_one->hand);
	}
	//次にドローするカードをデッキからにする
	if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation())) 
	{
		player_one->pickState = STATE::DECK;
	}
	//次にドローするカードを捨て札からにする
	else if (dealer->graveSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->pickState = STATE::GRAVE;
	}
	if (touch->getLocation().x > designResolutionSize.width*0.95f)
	{
		player_one->pickState = STATE::FREE;
	};
	for (int i = 0; i < player_one->hand.size();i++)
	{
		if(player_one->hand.at(i)->getBoundingBox().containsPoint(touch->getLocation()))
		{
			player_one->pickNumber = i;
			break;
		}
		else
		{
			player_one->pickNumber = -1;
		}
	}
	if(turn==TURN::WAIT)
	{
		player_two->cardDispHand(true);
	}
	player_one->cardDispHand(true);
};

