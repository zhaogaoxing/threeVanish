#include "LoadingScene.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* LoadingLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = LoadingLayer::create();

	scene->addChild(layer);

	return scene;
}

bool LoadingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origing = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithSystemFont("Loading... ", "Arial",50);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(label);

	//��ʼ������������
	m_texture_num = 0;
	int *ptexture_num = &m_texture_num;

	//������ϻص�
	auto addTextureCallback = [ptexture_num](Texture2D* texture)
	{
		(*ptexture_num)++;
	};

	//�첽Ԥ��������ͼƬ
	TextureCache::getInstance()->addImageAsync("texture/background.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/grid.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal1.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal4.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal5.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal6.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal7.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal8.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/bonus.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/bonusbar.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/bonusbar_fill.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/gameover.png", addTextureCallback);

	//�������ؽ��ȼ��
	this->schedule(schedule_selector(LoadingLayer::onTextureLoading));

	//Ԥ������Ч
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.mp3");

	SimpleAudioEngine::getInstance()->preloadEffect("cursh.mp3");

	return true;
}

void LoadingLayer::onTextureLoading(float dt)
{
	//һ������ͼƬ��ϣ���ô������Ϸ����
	if (m_texture_num == 14)
	{
		this->unschedule(schedule_selector(LoadingLayer::onTextureLoading));

		auto call = CallFunc::create([](){
			auto scene = GameLayer::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		//�ȴ�һ���������
		this->runAction(Sequence::create(DelayTime::create(0.51), call, nullptr));
	}
}