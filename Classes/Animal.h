#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

class Animal :public cocos2d::Sprite
{
private:
	//类型
	int m_type;
	//坐标
	int m_x;
	int m_y;

	bool m_isSwaping; // 是否正在交换
	bool m_isCrushing; // 是否正在消除


	int m_special;	// 特殊消标记
	bool m_isNeedClean; // 消除标记

public:
	// 根据动物类型以及坐标创建动物
	static Animal* createByType(int type, int x, int y);
	bool init(int type, int x, int y);

	int getType() { return m_type; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }


	int getSpecial(){ return m_special; }
	bool isNeedClean() { return m_isNeedClean; }

	void setClean(bool state) { m_isNeedClean = state; }
	void setSpeicial(int special){ m_special = special; }


	bool isSwaping() { return m_isSwaping; }
	bool isCrushing() { return m_isCrushing; }

	void setSwapingState(bool state) { m_isSwaping = state; }

	void crush(); //消除
};

#endif