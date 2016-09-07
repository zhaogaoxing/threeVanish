#include "AnimalGrid.h"

USING_NS_CC;
using namespace CocosDenshion;

AnimalGrid* AnimalGrid::create(int row, int col)
{
	auto animalGrid = new AnimalGrid();

	if (animalGrid&&animalGrid->init(row, col))
	{
		animalGrid->autorelease();

		return animalGrid;
	}
	else
	{
		CC_SAFE_DELETE(animalGrid);

		return nullptr;
	}
}

bool AnimalGrid::init(int row, int col)
{
	Node::init();

	m_row = row;
	m_col = col;

	//根据行列初始化一个空的宝石容器大小
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	//根据分部大小创建出阵列
	for (int x = 0; x < m_row; x++)
	{
		for (int y = 0; y < m_col; y++)
		{
			//透明格子背景
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, NODE_ZORDER);


			// 为动物容器填充动物对象
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}

	//加入触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AnimalGrid::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(AnimalGrid::onTouchMoved, this);

	//添加触摸事件监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

//随机创建一个动物
Animal* AnimalGrid::createAnimal(int x, int y)
{
	Animal* animal = nullptr;

	while (1)
	{
		animal = Animal::createByType(random(1, ANIMAL_NUM), x, y);

		if (isAnimalLegal(animal, x, y))
		{
			break;
		}
	}

	animal->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);

	this->addChild(animal, ANIMAL_ZORDER);

	return animal;
}

// 判断布局合法
bool AnimalGrid::isAnimalLegal(Animal* animal, int x, int y)
{
	bool isXLegal = true;
	bool isYLegal = true;

	if (x > 1)
	{
		if (animal->getType() == m_AnimalGrid[x - 1][y]->getType() &&
			animal->getType() == m_AnimalGrid[x - 2][y]->getType())
		{
			isXLegal = false;
		}
		else{
			isXLegal = true;
		}
	}

	if (y > 1)
	{
		if (animal->getType() == m_AnimalGrid[x][y - 1]->getType() &&
			animal->getType() == m_AnimalGrid[x][y - 2]->getType())
		{
			isYLegal = false;
		}
		else{
			isYLegal = true;
		}
	}

	return isXLegal && isYLegal;
}

//触摸事件
bool AnimalGrid::onTouchBegan(Touch* touch, Event* event)
{
	//将触摸点坐标转化为模型坐标
	auto pos = this->convertToNodeSpace(touch->getLocation());

	//是否按在布局上
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//得到布局坐标
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//得到当前选中的动物
		m_animalSelected = m_AnimalGrid[x][y];

		return true;
	}
	else
	{
		return false;
	}
}

void AnimalGrid::onTouchMoved(Touch* touch, Event* event)
{
	if (!m_animalSelected)
	{
		return;
	}

	//已选择动物的坐标
	int startX = m_animalSelected->getX();
	int startY = m_animalSelected->getY();

	//触摸点的布局坐标
	auto pos = this->convertToNodeSpace(touch->getLocation());
	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	//如果触摸点不在布局内，或者触摸点布局坐标和已选动物布局坐标一样，那么返回
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) ||
		Vec2(touchX, touchY) == Vec2(startX, startY))
	{
		return;
	}

	//判断已选择动物的布局坐标与触摸点的布局坐标是否直角间隔一个单位
	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		return;
	}

	//余下的情况，触摸点上面的动物就是进行交换的动物
	//获取交换的动物
	m_animalSwapped = m_AnimalGrid[touchX][touchY];

	//交换动物
	swapAnimals(m_animalSelected, m_animalSwapped);

	////开启交换状态的捕捉函数
	//this->schedule(schedule_selector(AnimalGrid::onAnimalsSwaping));
}

//交换动物
void AnimalGrid::swapAnimals(Animal* animalA, Animal* animalB)
{
	_eventDispatcher->pauseEventListenersForTarget(this);//交换开始，关闭触摸监听

	auto temp = m_AnimalGrid[animalA->getX()][animalA->getY()];
	m_AnimalGrid[animalA->getX()][animalA->getY()] = m_AnimalGrid[animalB->getX()][animalB->getY()];
	m_AnimalGrid[animalB->getX()][animalB->getY()] = temp;

	auto tempX = animalA->getX();
	animalA->setX(animalB->getX());
	animalB->setX(tempX);

	auto tempY = animalA->getY();
	animalA->setY(animalB->getY());
	animalB->setY(tempY);

	swapAnimalToNewPos(animalA);
	swapAnimalToNewPos(animalB);

}

//动物移动位置
void AnimalGrid::swapAnimalToNewPos(Animal* animal)
{
	//设置动物交换状态为真，移动结束再设置为假
	animal->setSwapingState(true);

	auto move = MoveTo::create(MOVE_SPEED, Vec2(animal->getX() * GRID_WIDTH, animal->getY() * GRID_WIDTH));
	auto call = CallFunc::create([animal](){
		animal->setSwapingState(false);
	});

	animal->runAction(Sequence::create(move, call, nullptr));
}