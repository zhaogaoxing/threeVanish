#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"


#include "SystemHeader.h"
#include "AnimalGrid.h"

class GameLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnterTransitionDidFinish();
	virtual void onExit();


	CREATE_FUNC(GameLayer);

private:
	AnimalGrid* m_AnimalGrid;
	
};

#endif