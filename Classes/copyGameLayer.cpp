#include "copyGameLayer.h"

copyGameLayer* copyGameLayer::create(int level)
{
	copyGameLayer *pRet = new copyGameLayer();
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

bool copyGameLayer::init(int level)
{
	if (!Layer::init())
	{
		return false;
	}

	addChild(LayerColor::create(Color4B::GREEN));

	//�^�b�`����
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(copyGameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(copyGameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(copyGameLayer::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	dealer = Dealer::create(
		Vec2(designResolutionSize.width*0.4f, designResolutionSize.height*0.5f),
		Vec2(designResolutionSize.width*0.6f, designResolutionSize.height*0.5f));

	addChild(dealer, 0);

	player_one = Player::create();
	player_one->setPosition(Vec2(designResolutionSize.width*0.4f, designResolutionSize.height*0.5f));
	auto action1 = MoveTo::create(1, Vec2(designResolutionSize.width * 0.5f, designResolutionSize.height * 0.2f));
	player_one->runAction(action1);
	//player_one->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.2f));
	addChild(player_one, 1);

	player_two = Player::create();
	player_two->setPosition(Vec2(designResolutionSize.width*0.4f, designResolutionSize.height*0.5f));
	auto action2 = MoveTo::create(1,Vec2(designResolutionSize.width * 0.5f,designResolutionSize.height * 0.8f));
	player_two->runAction(action2);
	//player_two->setPosition(Vec2(designResolutionSize.width*0.5f, designResolutionSize.height*0.8f));
	addChild(player_two, 1);

	turnLabel = Label::create("gameSTART", "fonts/arial.ttf", 30);
	turnLabel->setPosition(designResolutionSize.width*0.5f, designResolutionSize.height*0.5f);
	addChild(turnLabel);

	phaseLabel = Label::create("gameSTART", "fonts/arial.ttf", 30);
	phaseLabel->setPosition(designResolutionSize.width*0.7f, designResolutionSize.height*0.5f);
	addChild(phaseLabel);

	effectManager = EffectManager::create();
	addChild(effectManager);

	button = SortButton::create(ROLE::EQUAL, Vec2(designResolutionSize.width *0.05f, designResolutionSize.height*0.5f));
	addChild(button);

	button2 = OriginalButton::create(Vec2(designResolutionSize.width * 0,designResolutionSize.height * 0),0);//�ύX
	addChild(button2);



	//�Q�[���̏���
	gameStart();

	//effectManager->phaseChange(PHASE::START);


	scheduleUpdate();

	return true;
};

void copyGameLayer::update(float delta)
{
	if (turn != TURN::WAIT)
	{
		nextPhase(actionPhase());
	}
};

//�[�[�[�[�[�[�[�[�[�[�Q�[���̏����֐��[�[�[�[�[�[�[�[�[


//�N����ԏ��߂ɃQ�[�����n�߂邩
void copyGameLayer::startPlayer()
{
	int iRand = rand() % 2 + 1;
	turn = (TURN)iRand;
	String* name = String::createWithFormat("Player_%d_start", iRand);
	turnLabel->setString(name->getCString());
};

//�J�[�h��z��(�����J�[�h�̔z�u)
void copyGameLayer::cardDivision()
{
	//10��
	for (int i = 0; i < HAND_START_MAX; i++)
	{
		player_one->cardDrow(dealer->deck);
		player_two->cardDrow(dealer->deck);
	}
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
};

//�X�^�[�g
void copyGameLayer::gameStart()
{
	//�J�E���g�̏�����
	turnCount = 1;
	isPass = false;
	//�p�X�{�^���\��
	button2->pass->setVisible(true); //�ύX
	//�t�F�C�Y�̏�����
	phase = PHASE::START;
	//�v���C���[�̃J�[�h�����ׂĎ̂Ă�
	player_one->handDeath();
	player_two->handDeath();
	//�f�b�L���č\�z
	dealer->setDeck(true);
	//�f�b�L���V���b�t��
	dealer->cardShuffle();
	//�v���C���[��10�������݂ɔz��
	cardDivision();
	//�f�b�L�̏ォ��ꖇ�̂ĎD�ɑ���
	dealer->cardDeckThrow();
	//��s�����߂�
	startPlayer();
	//�n�܂�
	nextPhase(true);
};

//�[�[�[�[�[�[�[�[�[�[�Q�[�����ɍs���֐��[�[�[�[�[�[�[�[

//�^�b�`�������ꂽ���Ƀv���C���[���s���s��
//true��Ԃ��Ƃ��F���̏������s���Ǝ����I�Ɏ��̃t�F�C�Y�ɍs���Ƃ�
//false��Ԃ��Ƃ��F�s�������������t�F�C�Y���ŉ��x�s���Ă��悢�Ƃ�
bool copyGameLayer::actionPhase()
{
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
	switch (phase)
	{
	case PHASE::START:
		return true;
	case PHASE::DROW:
		if (turn == TURN::PLAY_ONE)//�v���C���[�P
		{
			if (player_one->pickState == STATE::DECK)//�f�b�L���������I��
			{
				if (isPass)//���g����U�Ő�s���p�X���Ă�����
				{
					if (turnCount >= 3)//���݂��̃^�[����3�^�[���ڈȍ~
					{
						player_one->cardDrow(dealer->deck);
						player_one->checkRole();
						return true;
					}
				}
				else//���g����s���p�X���s��Ȃ��ꍇ
				{
					if (turnCount >= 2)//���݂��̃^�[����2�^�[���ڈȍ~
					{
						player_one->cardDrow(dealer->deck);
						player_one->checkRole();
						return true;
					}
				}
			}
			else if (player_one->pickState == STATE::GRAVE)//�̂ĎD���������I��
			{
				player_one->cardDrow(dealer->grave);
				player_one->checkRole();
				return true;
			}
			else if (player_one->pickState == STATE::FREE)
			{
				if (turnCount == 1)//1�^�[���ڂ̂� 
				{
					player_one->checkRole();
					isPass = true;
					return true;
				}
			}
		}
		else if (turn == TURN::PLAY_TWO)//�v���C���[�Q
		{
			button2->pass->setVisible(false);
			player_two->cardDrow(dealer->deck);
			player_two->checkRole();
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
	case PHASE::PASS:
		return true;
	default:
		return false;
	}
	return false;
};

//�^�[���`�F���W
void copyGameLayer::nextPlayerTurn()
{
	if (turn == TURN::PLAY_ONE)
	{
		turn = TURN::PLAY_TWO;
		turnLabel->setString("PLAYER_TWO");
	}
	else if (turn == TURN::PLAY_TWO)
	{
		turn = TURN::PLAY_ONE;
		turnLabel->setString("PLAYER_ONE");
	}
};

//�t�F�C�Y�`�F���W(�v���C���[���I��������true)
void copyGameLayer::nextPhase(bool isAction)
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

		if (isPass)
		{
			isPass = false;
			phase = PHASE::PASS;
			phaseLabel->setString("PASS");
		}
		break;
	case PHASE::THROW:
		if (isKnock || dealer->deck.size() <= 0)
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
	player_one->cardDispHand(true);
	player_two->cardDispHand(false);
	dealer->cardDispGrave();

};

//�m�b�N
void copyGameLayer::callKnock()
{
	turn = TURN::WAIT;
	player_one->checkRole();
	player_one->cardDispHand(true);
	player_two->checkRole();
	player_two->cardDispHand(true);
	if (player_one->point<player_two->point)
	{
		turnLabel->setString("PLAYER_ONE\nWIN");
	}
	else if (player_one->point>player_two->point)
	{
		turnLabel->setString("PLAYER_TWO\nWIN");
	}
	else
	{
		turnLabel->setString("DRAW");
	}
};

//�m�b�N���ɍs������̌v�Z
void copyGameLayer::callCalculation()
{

};

//�Q�[���I�����̌v�Z
void copyGameLayer::callAddTotal()
{

};

//�[�[�[�[�[�[�[�[�[�[�[�[

bool copyGameLayer::onTouchBegan(const Touch * touch, Event *unused_event)
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

void copyGameLayer::onTouchMoved(const Touch * touch, Event *unused_event)
{
	//�̂���
};

void copyGameLayer::onTouchEnded(const Touch * touch, Event *unused_event)
{
	if (button->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->cardSort(button->switchRole,player_one->result);
	}
	//���Ƀh���[����J�[�h���f�b�L����ɂ���
	if (dealer->deckSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->pickState = STATE::DECK;
	}
	//���Ƀh���[����J�[�h���̂ĎD����ɂ���
	else if (dealer->graveSp->getBoundingBox().containsPoint(touch->getLocation()))
	{
		player_one->pickState = STATE::GRAVE;
	}
	if (button2->pass->getBoundingBox().containsPoint(touch->getLocation()))//�ύX
	{
		player_one->pickState = STATE::FREE;
		button2->pass->setVisible(false);
	};
	for (int i = 0; i < player_one->hand.size(); i++)
	{
		if (player_one->hand.at(i)->getBoundingBox().containsPoint(touch->getLocation()))
		{
			player_one->pickNumber = i;
			break;
		}
		else
		{
			player_one->pickNumber = -1;
		}
	}
	if (turn == TURN::WAIT)
	{
		player_two->cardDispHand(true);
	}
	player_one->cardDispHand(true);
};

