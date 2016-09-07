#ifndef __ANIMAL_GRID_H__
#define __ANIMAL_GRID_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SystemHeader.h"
#include <vector>
#include "Animal.h"

USING_NS_CC;
using namespace std;

class AnimalGrid :public cocos2d::Node
{
private:
	int m_row;  //行数
	int m_col;  //列数
	Animal* m_animalSelected; // 当前选择的动物
	Animal* m_animalSwapped; // 欲交换的动物

	vector<vector<Animal*>> m_AnimalGrid; // 存放动物对象的容器
	Vector<Animal*> m_crushAnimalGrid; // 准备消除的动物容器
	Vector<Animal*> m_newAnimalGrid; // 准备加入布局的动物容器

	Vector<Animal*> m_crushAnimalH;
	Vector<Animal*> m_crushAnimalV;

	// 关于创建动物的方法
	Animal* createAnimal(int x, int y); // 根据坐标创建一个动物
	bool isAnimalLegal(Animal* animal, int x, int y); // 判断创建的动物是否合法

	//关于操作动物的方法
	void swapAnimals(Animal *animalA, Animal *animalB);//交换两个动物
	void swapAnimalToNewPos(Animal* animal); //移动到新的位置

	// 事件响应部分
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	// 游戏逻辑部分
	bool canCrush(); // 判断当前状态的动物阵列是否能消除
	void goCrush(); // 开始消除



	//// 捕捉函数，捕捉消除步骤是否完成
	void onAnimalsSwaping(float dt);


	void singeAnimal(Animal* animal); // 判断单个动物
	void specialSinged(Animal* animal);
	bool checkGridClean();


public:
	static AnimalGrid* create(int row, int col);
	bool init(int row, int col);

	void updateMap(); // 更新动物阵列
	bool isDeadMap(); // 判断当前布局是否死图

	int getRow() { return m_row; }
	int getCol() { return m_col; }

};

#endif
