#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameOverLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameOverLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GameOverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//±³¾°Í¼Æ¬
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/gameover.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	//·µ»Ø°´Å¥
	auto backlabel = Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 30);
	backlabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backlabel);
	backItem->setTarget(this, menu_selector(GameOverLayer::onBackItemCallback));
	auto menu = Menu::create(backItem, NULL);
	this->addChild(menu);

	return true;

}

void GameOverLayer::onBackItemCallback(Ref* pSender)
{
	//·µ»ØÓÎÏ·
	auto scene = GameLayer::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

void GameOverLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameover.mp3", true);
}

void GameOverLayer::onExit()
{
	Layer::onExit();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}