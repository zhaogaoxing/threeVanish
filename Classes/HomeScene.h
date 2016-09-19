#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "GameScene.h"

using namespace CocosDenshion;

class HomeLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	CREATE_FUNC(HomeLayer);
};

#endif