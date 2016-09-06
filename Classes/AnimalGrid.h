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



public:
	static AnimalGrid* create(int row, int col);
	bool init(int row, int col);

	void updateMap(); // 更新动物阵列
	bool isDeadMap(); // 判断当前布局是否死图

	int getRow() { return m_row; }
	int getCol() { return m_col; }

};

#endif
