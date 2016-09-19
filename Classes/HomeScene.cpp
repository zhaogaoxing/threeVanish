#include "HomeScene.h"

USING_NS_CC;

Scene* HomeLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = HomeLayer::create();
	scene->addChild(layer);
	return scene;
}

bool HomeLayer::init()
{
	if (!Layer:: init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	//����ͼƬ
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/home.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	//��ʾ��ǩ
	auto label = Label::createWithTTF("start", "fonts/arial.ttf", 40);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 6));
	this->addChild(label, 1);

	label->runAction(RepeatForever::create(Blink::create(1, 1)));

	//ע�ᴥ���¼�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [](Touch* touch, Event* event)
	{
		auto scene = TransitionFade::create(1.0f, GameLayer::createScene());
		Director::getInstance()->pushScene(scene);
		return false;
	};

	//��Ӵ����¼�����
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HomeLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("WBGM.mp3", true);
}

void HomeLayer::onExit()
{
	Layer::onExit();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}