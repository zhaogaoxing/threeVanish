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

	}

}