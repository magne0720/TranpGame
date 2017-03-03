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

	phaseTimer = 0;
	phaseSpeed = 0.02f;

	commonEffect = EFFECT::DO_NOT;

	//ゲームの準備
	gameStart();

	//effectManager->phaseChange(PHASE::START);

	
	scheduleUpdate();

	return true;
};

void MainGameLayer::update(float delta)
{

	phaseTimer += phaseSpeed;

	//player_one->pickNumber = random(0, (int)player_one->hand.size() - 1);

	//if (dealer->grave.size() > 0) 
	//{
	////if (dealer->grave.at(dealer->grave.size()-1)->myMark == MARK::SPADE) 
	//	player_one->pickState = STATE::GRAVE;
	//}
	//else
	//{
	//	player_one->pickState = STATE::DECK;
	//}


	if (isGameStart)
	{
		if (phaseTimer >= 0.5f)
		{
			nextPhase(actionPhase());
			phaseTimer = 0;
		}
	}
	else if(turn==TURN::WAIT)
	{
		if (phaseTimer >= 5.0f)
		{
			gameStart();
			phaseTimer = 0;
		}
	}


	switch (commonEffect)
	{
	case DO_NOT:
		break;
	case DO_DIVISION:
		if (cardDivisionDesign())
		{
			commonEffect = EFFECT::DO_DIVISION_THROW;
		}
		break;
	case DO_DIVISION_THROW:
		if (cardDivisionThrowDesign())
		{
			commonEffect = EFFECT::DO_NOT;
			isGameStart = true;
		}
		break;
	case DO_SHUFFLE:
		if (cardShuffleDesign())
		{
			commonEffect = EFFECT::DO_DIVISION;
		}
		break;
	case DO_DRAW:
		if (cardDrowDesign())
		{
			commonEffect = EFFECT::DO_NOT;
		}
		break;
	case DO_THROW:
		if (cardThrowDesign())
		{
			commonEffect = EFFECT::DO_NOT;
		}
		break;
	case DO_SORT:
		break;
	default:
		break;
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
	one_hand = 0;
	two_hand = 0;
	//10回
	for (int i = 0; i < HAND_START_MAX; i++)
	{
		player_one->cardDrow(dealer->deck);
		player_two->cardDrow(dealer->deck);
	}
};

//カードをシャッフルする
bool MainGameLayer::cardShuffleDesign()
{
	player_one->cardDispHand(true, -1);//0から始まるため、1枚目が表示されてしまうからー1
	player_two->cardDispHand(true, -1);
	if (effectManager->shuffleCard(dealer->deckSp->getPosition(),1.0f, 30, 0.2f)) 
	{
		return true;
	}
	return false;
};

//カードを分ける演出
bool MainGameLayer::cardDivisionDesign() 
{
	player_one->cardDispHand(true, one_hand);
	player_two->cardDispHand(false, two_hand);
	if (one_hand < player_one->hand.size()-1)
		if (player_one->effect->drowCard(player_one->hand, one_hand, dealer->deckSp->getPosition(), player_one->handPos[one_hand], 0.2f))
		{
			effectManager->phaseChange(PHASE::DROW);
			one_hand++;
		}
	if (two_hand < player_two->hand.size()-1)
		if (player_two->effect->drowCard(player_two->hand, two_hand, dealer->deckSp->getPosition(), player_two->handPos[two_hand], 0.2f))
		{
			effectManager->phaseChange(PHASE::DROW);
			two_hand++;
		}
	if (one_hand >= player_one->hand.size()-1&&two_hand >= player_two->hand.size()-1) 
	{
		return true;
	}
	return false;
};

bool MainGameLayer::cardDivisionThrowDesign() 
{
	dealer->grave.at(dealer->grave.size()-1)->setState(STATE::GRAVE);
	if (effectManager->drowCard(dealer->grave, dealer->grave.size()-1, dealer->deckSp->getPosition(), dealer->graveSp->getPosition(), 0.05f))
	{
		return true;
	}
}

//カードをひく演出
bool MainGameLayer::cardDrowDesign() 
{
	if (turn == TURN::PLAY_ONE){
		if (player_one->pickState == STATE::DECK) {
			if (player_one->effect->drowCard(player_one->hand, player_one->checkLastCard(), dealer->deckSp->getPosition(), player_one->handPos[player_one->checkLastCard()], 0.05f)){
				return true;
			}
		}
		else if (player_one->pickState == STATE::GRAVE) {
			if (player_one->effect->drowCard(player_one->hand, player_one->checkLastCard(), dealer->graveSp->getPosition(), player_one->handPos[player_one->checkLastCard()], 0.05f)){
				return true;
			}
		}
	}
	else if(turn==TURN::PLAY_TWO)
	{
		if (player_two->pickState == STATE::DECK) {
			if (player_two->effect->drowCard(player_two->hand, player_two->checkLastCard(), dealer->deckSp->getPosition(), player_two->handPos[player_two->checkLastCard()], 0.05f)){
				return true;
			}
		}
		else if (player_two->pickState == STATE::GRAVE) {
			if (player_two->effect->drowCard(player_two->hand, player_two->checkLastCard(), dealer->graveSp->getPosition(), player_two->handPos[player_two->checkLastCard()], 0.05f)){
				return true;
			}
		}
	}

	return false;
}

bool MainGameLayer::cardThrowDesign()
{
	if (turn == TURN::PLAY_ONE)
	{
		if (player_one->effect->drowCard(dealer->grave, dealer->grave.size()-1, player_one->handPos[player_one->pickNumber], dealer->graveSp->getPosition(), 0.05f))
		{
			return true;
		}
	}
	else if (turn == TURN::PLAY_TWO)
	{
		if (player_two->effect->drowCard(dealer->grave, dealer->grave.size()-1, player_two->handPos[player_two->hand.size() - 1], dealer->graveSp->getPosition(), 0.05f))
		{
			return true;
		}
	}
	return false;
}
//スタート
void MainGameLayer::gameStart()
{
	//カウントの初期化
	commonEffect = EFFECT::DO_SHUFFLE;
	turn = TURN::WAIT;
	turnCount = 1;
	isGameStart = false;
	isPass = false;
	isKnock = false;
	one_hand = 0;
	two_hand = 0;
	//お互いの初期化
	player_one->RessetPlayer();
	player_two->RessetPlayer();
	player_one->sortType = ROLE::ORDER;
	player_two->sortType = ROLE::ORDER;
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
			player_two->pickState = STATE::DECK;
			player_two->cardDrow(dealer->deck);
			//player_two->checkRole();
			return true;
		}
		return false;
	case PHASE::THROW:
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
		log("effect");
		player_one->lastCard->setKind(player_one->hand.at(player_one->hand.size() - 1)->myMark, player_one->hand.at(player_one->hand.size() - 1)->myNumber);
		if (isPass)
		{
			isPass = false;
			phase = PHASE::PASS;
			phaseLabel->setString("PASS");
		}
		commonEffect = EFFECT::DO_DRAW;
		log("check");
		player_one->checkRole();
		log("end");
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
			commonEffect = EFFECT::DO_THROW;
		break;
	case PHASE::KNOCK:
		callKnock();
		break;
	case PHASE::END:
		phaseLabel->setString("START");
		phase = PHASE::START;
		player_one->pickState = STATE::HAND;
		player_two->pickState = STATE::HAND;
		player_one->pickNumber = -1;
		player_two->pickNumber = -1;
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
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
	dealer->cardDispGrave();
	dealer->checkDeckZero();
};

//ノック
void MainGameLayer::callKnock() 
{
	isGameStart = false;
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
	player_one->sortType = ROLE::WITHOUT;
	//player_one->cardDispHand(true);
	//player_two->cardDispHand(true);
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
		player_one->cardSort(button->switchRole,player_one->result);
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

