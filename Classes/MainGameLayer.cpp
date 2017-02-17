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


	dealer = Dealer::create(Vec2(designResolutionSize.width*0.3f, designResolutionSize.height*0.5f));
	addChild(dealer);

	player_one = Player::create();
	player_one->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.2f));
	addChild(player_one);

	player_two = Player::create();
	player_two->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.8f));
	addChild(player_two);

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
	NextPhase(true);

};

//�[�[�[�[�[�[�[�[�[�[�Q�[�����ɍs���֐��[�[�[�[�[�[�[�[
//�^�b�`�������ꂽ���Ƀv���C���[���s���s��
bool MainGameLayer::actionPhase() 
{
	switch (phase)
	{
	case PHASE::START:
		return false;
	case PHASE::DROW:
		if (turn == TURN::PLAY_ONE)
		{
			player_one->cardDrow(dealer->deck);
			return true;
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
			player_one->cardThrow(0);
			return true;
		}
		else if (turn == TURN::PLAY_TWO)
		{
			player_two->cardThrow(0);
			return true;
		}
		return false;
	case PHASE::KNOCK:
		return false;
	case PHASE::END:
		return false;
	default:
		return false;
	}
	return false;
};

//�^�[���`�F���W
void MainGameLayer::NextPlayerTurn() 
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

//�t�F�C�Y�`�F���W
void MainGameLayer::NextPhase(bool isAction) 
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

		phaseLabel->setString("THROW"); break;
	case PHASE::THROW:
		if (isKnock)
		{
			phase = PHASE::KNOCK;
		}
		else 
		{
			phase = PHASE::END;	
		}
		break;
	case PHASE::KNOCK:
		callKnock();
		break;
	case PHASE::END:
		phaseLabel->setString("START");
		phase = PHASE::START;
		NextPlayerTurn();

		break;
	default:

		phaseLabel->setString("DROW"); 
		break;
	}
	player_one->cardDispHand();
	player_two->cardDispHand();
};

//�m�b�N
void MainGameLayer::callKnock() 
{
	turn = TURN::WAIT;

	player_one->checkRole();
	player_two->checkRole();

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
	NextPhase(actionPhase());




	return true;
};

void MainGameLayer::onTouchMoved(const Touch * touch, Event *unused_event) 
{

};

void MainGameLayer::onTouchEnded(const Touch * touch, Event *unused_event) 
{

};

