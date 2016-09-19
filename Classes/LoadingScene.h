#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "GameScene.h"
#include "HomeScene.h"


class LoadingLayer : public cocos2d::Layer
{
private:
	int m_texture_num;	// ��ǰ���ص�������

	void onTextureLoading(float dt);
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LoadingLayer);
};


#endif