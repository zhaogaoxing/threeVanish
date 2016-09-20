#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();
	
	auto layer = GameLayer::create();

	scene->addChild(layer);

	return scene;
}

LoadingBar* GameLayer::m_bonusbar = nullptr;
Label* GameLayer::m_scorelabel = nullptr;
int GameLayer::m_score = 0;



bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto textureCache = TextureCache::getInstance();

	//���ر���ͼ
	auto bg = Sprite::createWithTexture(textureCache->getTextureForKey("texture/background.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	// ��ʼ����������
	m_AnimalGrid = AnimalGrid::create(ROW, COL);
	m_AnimalGrid->setAnchorPoint(Vec2(0, 0));
	m_AnimalGrid->setPosition((visibleSize.width - ROW*GRID_WIDTH) / 2, GRID_WIDTH + 40);
	this->addChild(m_AnimalGrid, 1);

	//����ʱ�����
	auto bounusbar_frame = Sprite::createWithTexture(textureCache->getTextureForKey("texture/bonusbar.png"));
	bounusbar_frame->setPosition(Vec2(visibleSize.width / 2, bounusbar_frame->getContentSize().height / 2 + 40));
	this->addChild(bounusbar_frame);

	//����ʱ��
	m_bonusbar = LoadingBar::create("texture/bonusbar_fill.png");
	m_bonusbar->setPercent(TIME_PERCENT);
	m_bonusbar->setPosition(bounusbar_frame->getPosition());
	this->addChild(m_bonusbar, 1);

	//��������ʱ
	this->schedule(schedule_selector(GameLayer::onReducingBonus), 0.1);

	//������
	auto bonus = Sprite::createWithTexture(textureCache->getTextureForKey("texture/bonus.png"));
	bonus->setPosition(Vec2(visibleSize.width - bonus->getContentSize().width / 2 - 10, 80));
	this->addChild(bonus, 1);

	//����
	m_scorelabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
	m_scorelabel->setAnchorPoint(Vec2(1, 0));
	m_scorelabel->setPosition(visibleSize.width - 10, 35);
	m_scorelabel->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_scorelabel, 1);

	//��ʼ����ͣ��ť
	auto pauseSprite = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/pause.png"));
	auto pauseMenuItem = MenuItemSprite::create(pauseSprite, pauseSprite, CC_CALLBACK_1(GameLayer::menuPauseCallback, this));
	auto pauseMenu = Menu::create(pauseMenuItem, NULL);
	pauseMenu->setPosition(Vec2(40, visibleSize.height - 28));
	this->addChild(pauseMenu, 20);



	return true;
}

void GameLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true);
}

void GameLayer::onExit()
{
	Layer::onExit();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//����ʱ
void GameLayer::onReducingBonus(float dt)
{
	m_bonusbar->setPercent(m_bonusbar->getPercent() - 0.2);

	//����ʱ��������Ϸ������������Ϸ����
	if (m_bonusbar->getPercent() == 0)
	{
		this->unschedule(schedule_selector(GameLayer::onReducingBonus));

		publishScore();

		auto scene = GameOverLayer::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));

	}

}

// �洢����
void GameLayer::publishScore()
{
	auto userdefault = UserDefault::getInstance();

	// �洢������Ϸ����
	char score_str[100] = { 0 };
	sprintf(score_str, "%d", m_score);
	userdefault->setStringForKey("LastScore", score_str);

	// �洢�����Ϸ����
	auto bestscore = userdefault->getStringForKey("BestScore");
	if (m_score > atoi(bestscore.c_str()))
	{
		userdefault->setStringForKey("BestScore", score_str);
	}
}

// �ӷ�
void GameLayer::addBonus(int bonus)
{
	m_score += 20 * bonus;
	char buf[100] = { 0 };
	sprintf(buf, "%d", m_score);
	m_scorelabel->setString(buf);

	m_bonusbar->setPercent(m_bonusbar->getPercent() + bonus);
}

//��ͣ��Ϸ
void GameLayer::menuPauseCallback(Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//��ͣ��ǰ���е�node
	this->pause();

	for (const auto&node : this->getChildren())
	{
		node->pause();
	}

	//�������˵�
	auto backNormal = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/back.png"));
	auto backSelected = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/back-on.png"));
	auto backMenuItem = MenuItemSprite::create(backNormal, backSelected,
		CC_CALLBACK_1(GameLayer::menuBackCallback, this));

	//������Ϸ
	auto resumeNormal = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/resume.png"));
	auto resumeSelected = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/resume-on.png"));
	auto resumeMenuItem = MenuItemSprite::create(resumeNormal, resumeSelected,
		CC_CALLBACK_1(GameLayer::menuResumeCallback, this));

	menu = Menu::create(backMenuItem, resumeMenuItem, NULL);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(menu, 20);
}

//�������˵�
void GameLayer::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

//������Ϸ
void GameLayer::menuResumeCallback(Ref* pSender)
{
	this->resume();
	for (const auto&node : this->getChildren())
	{
		node->resume();
	}

	this->removeChild(menu);
}