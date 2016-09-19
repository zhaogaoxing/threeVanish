#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"


#include "SystemHeader.h"
#include "AnimalGrid.h"

using namespace ui;

class GameLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	static void addBonus(int bonus);


	CREATE_FUNC(GameLayer);

private:
	AnimalGrid* m_AnimalGrid;
	
	static LoadingBar* m_bonusbar;//计时条

	static Label* m_scorelabel;
	static int m_score; // 分数

	void onReducingBonus(float dt);//开启倒计时
	void publishScore(); // 存储游戏分数

};

#endif