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

	//�^�b�`����
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

	effectManager = EffectManager::create();
	addChild(effectManager);

	//�Q�[���̏���
	gameStart();

	effectManager->phaseChange(PHASE::START);

	
	scheduleUpdate();

	return true;
};

void MainGameLayer::update(float delta)
{
	if (turn != TURN::WAIT)
	{
		nextPhase(actionPhase());
	}
};

//�[�[�[�[�[�[�[�[�[�[�Q�[���̏����֐��[�[�[�[�[�[�[�[�[


//�N����ԏ��߂ɃQ�[�����n�߂邩
void MainGameLayer::startPlayer() 
{
	int iRand = rand() % 2 + 1;
	turn = (TURN)iRand;
	String* name = String::createWithFormat("Player_%d_start", iRand);
	turnLabel->setString(name->getCString());
};

//�J�[�h��z��(�����J�[�h�̔z�u)
void MainGameLayer::cardDivision() 
{
	//10��
	for (int i = 0; i < HAND_START_MAX; i++)
	{
		player_one->cardDrow(dealer->deck);
		player_two->cardDrow(dealer->deck);
	}
	player_one->cardDispHand();
	player_two->cardDispHand();
};

//�X�^�[�g
void MainGameLayer::gameStart()
{
	player_one->handDeath();
	player_two->handDeath();
	dealer->cardDispGrave();
	dealer->setDeck(true);
	dealer->cardShuffle();
	cardDivision();
	startPlayer();
	phase = PHASE::START;
	nextPhase(true);
};

//�[�[�[�[�[�[�[�[�[�[�Q�[�����ɍs���֐��[�[�[�[�[�[�[�[
//�^�b�`�������ꂽ���Ƀv���C���[���s���s��
//true��Ԃ��Ƃ��F���̏������s���Ǝ����I�Ɏ��̃t�F�C�Y�ɍs���Ƃ�
//false��Ԃ��Ƃ��F�s�������������t�F�C�Y���ŉ��x�s���Ă��悢�Ƃ�
bool MainGameLayer::actionPhase() 
{
	switch (phase)
	{
	case PHASE::START:
		return true;
	case PHASE::DROW:
		if (turn == TURN::PLAY_ONE)
		{
			if (player_one->pickState == STATE::DECK)
			{
				player_one->cardDrow(dealer->deck);
				return true;
			}
			else if (player_one->pickState == STATE::GRAVE)
			{
				player_one->cardDrow(dealer->grave);
				return true;
			}
		}
		else if (turn == TURN::PLAY_TWO)
		{
			player_two->cardDrow(dealer->deck);
			return true;
		}
		return false;
	case PHASE::THROW:
		if (turn == TURN::PLAY_ONE)
		{
			if (player_one->pickNumber >= 0)
			{
				player_one->cardThrow(player_one->pickNumber, dealer->grave);
				player_one->pickNumber = -1;
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
	default:
		return false;
	}
	player_one->cardDispHand();
	player_two->cardDispHand();
	return false;
};

//�^�[���`�F���W
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

//�t�F�C�Y�`�F���W(�v���C���[���I��������true)
void MainGameLayer::nextPhase(bool isAction) 
{
	if (!isAction) 
	{
		return;
	}
	switch (phase)
	{
	case PHASE::START:
		phase = PHASE::DROW;
		phaseLabel->setString("DROW");
		break;
	case PHASE::DROW:
		phase = PHASE::THROW;
		dealer->checkDeckZero();
		phaseLabel->setString("THROW"); 
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

		break;
	default:

		phaseLabel->setString("DROW"); 
		break;
	}
	player_one->pickState = STATE::HAND;
	player_one->cardDispHand();
	player_two->cardDispHand();	
	dealer->cardDispGrave();

};

//�m�b�N
void MainGameLayer::callKnock() 
{
	turn = TURN::WAIT;

	player_one->checkRole();

};

//�m�b�N���ɍs������̌v�Z
void MainGameLayer::callCalculation() 
{

};

//�Q�[���I�����̌v�Z
void MainGameLayer::callAddTotal() 
{

};

//�[�[�[�[�[�[�[�[�[�[�[�[

bool MainGameLayer::onTouchBegan(const Touch * touch, Event *unused_event) 
{


	if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation()))
	{

	}




	return true;
};

void MainGameLayer::onTouchMoved(const Touch * touch, Event *unused_event) 
{

};

void MainGameLayer::onTouchEnded(const Touch * touch, Event *unused_event) 
{
	bool isNext = false;
	//���Ƀh���[����J�[�h���f�b�L����ɂ���
	if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation())) 
	{
		player_one->pickState = STATE::DECK;
		isNext = true;
	}
	//���Ƀh���[����J�[�h���̂ĎD����ɂ���
	else if (dealer->graveSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->pickState = STATE::GRAVE;
		isNext = true;
	}

	for (int i = 0; i < player_one->hand.size();i++)
	{
		if (player_one->hand.at(i)->getBoundingBox().intersectsRect(dealer->graveSp->getBoundingBox()))
		{
			player_one->pickNumber = i;
			isNext = true;
			break;
		}
		else
		{
			player_one->pickNumber = -1;
		}
	}
};

