#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

class Animal :public cocos2d::Sprite
{
private:
	//����
	int m_type;
	//����
	int m_x;
	int m_y;

	bool m_isSwaping; // �Ƿ����ڽ���
	bool m_isCrushing; // �Ƿ���������


	int m_special;	// ���������
	bool m_isNeedClean; // �������

public:
	// ���ݶ��������Լ����괴������
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

	void crush(); //����
};

#endif