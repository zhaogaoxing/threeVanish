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

	//�������г�ʼ��һ���յı�ʯ������С
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	//���ݷֲ���С����������
	for (int x = 0; x < m_row; x++)
	{
		for (int y = 0; y < m_col; y++)
		{
			//͸�����ӱ���
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, NODE_ZORDER);


			// Ϊ����������䶯�����
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}

	//���봥������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AnimalGrid::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(AnimalGrid::onTouchMoved, this);

	//��Ӵ����¼�������
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

//�������һ������
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

// �жϲ��ֺϷ�
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

//�����¼�
bool AnimalGrid::onTouchBegan(Touch* touch, Event* event)
{
	//������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(touch->getLocation());

	//�Ƿ��ڲ�����
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//�õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//�õ���ǰѡ�еĶ���
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

	//��ѡ���������
	int startX = m_animalSelected->getX();
	int startY = m_animalSelected->getY();

	//������Ĳ�������
	auto pos = this->convertToNodeSpace(touch->getLocation());
	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	//��������㲻�ڲ����ڣ����ߴ����㲼���������ѡ���ﲼ������һ������ô����
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) ||
		Vec2(touchX, touchY) == Vec2(startX, startY))
	{
		return;
	}

	//�ж���ѡ����Ĳ��������봥����Ĳ��������Ƿ�ֱ�Ǽ��һ����λ
	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		return;
	}

	//���µ����������������Ķ�����ǽ��н����Ķ���
	//��ȡ�����Ķ���
	m_animalSwapped = m_AnimalGrid[touchX][touchY];

	//��������
	swapAnimals(m_animalSelected, m_animalSwapped);

	////��������״̬�Ĳ�׽����
	//this->schedule(schedule_selector(AnimalGrid::onAnimalsSwaping));
}

//��������
void AnimalGrid::swapAnimals(Animal* animalA, Animal* animalB)
{
	_eventDispatcher->pauseEventListenersForTarget(this);//������ʼ���رմ�������

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

//�����ƶ�λ��
void AnimalGrid::swapAnimalToNewPos(Animal* animal)
{
	//���ö��ｻ��״̬Ϊ�棬�ƶ�����������Ϊ��
	animal->setSwapingState(true);

	auto move = MoveTo::create(MOVE_SPEED, Vec2(animal->getX() * GRID_WIDTH, animal->getY() * GRID_WIDTH));
	auto call = CallFunc::create([animal](){
		animal->setSwapingState(false);
	});

	animal->runAction(Sequence::create(move, call, nullptr));
}