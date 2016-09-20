#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"

class GameOverLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();

	CREATE_FUNC(GameOverLayer);

private:
	void onBackItemCallback(cocos2d::Ref* pSender);

};

#endif