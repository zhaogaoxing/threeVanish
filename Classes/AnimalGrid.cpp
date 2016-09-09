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

	m_animalSelected = nullptr;
	m_animalSwapped = nullptr;

	// �������г�ʼ��һ���յı�ʯ������С
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	// ���ݲ��ִ�С����������
	for (int x = 0; x < m_row; x++)
	{
		for (int y = 0; y < m_col; y++)
		{
			// ͸�����ӱ���
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, NODE_ZORDER);

			// Ϊ����������䶯�����
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}

	while (isDeadMap())
	{
		updateMap();
	}

	// ���봥������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AnimalGrid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AnimalGrid::onTouchMoved, this);

	// ��Ӵ����¼�������
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

// �жϲ����Ƿ�����ͼ
bool AnimalGrid::isDeadMap()
{
	// ģ�⽻�����жϽ������Ƿ����������粻�ܣ���ô���Ǹ���ͼ
	auto swap = [](Animal** a, Animal** b)
	{
		auto temp = *a;
		*a = *b;
		*b = temp;
	};

	bool isDeadMap = true;

	// ����ÿһ������
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			// ����ߵĽ���
			if (x > 0)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x - 1][y]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x - 1][y]);
			}

			// ���ұߵĽ���
			if (x < m_col - 1)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x + 1][y]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x + 1][y]);
			}

			// ������Ľ���
			if (y < m_row - 1)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y + 1]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y + 1]);
			}

			// ������Ľ���
			if (y > 0)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y - 1]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y - 1]);
			}
		}
	}

	m_crushAnimalGrid.clear();

	return isDeadMap;
}

// ˢ�²���
void AnimalGrid::updateMap()
{
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_AnimalGrid[x][y]->removeFromParent();
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}
}

// �������һ������
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

// �����¼�
bool AnimalGrid::onTouchBegan(Touch* touch, Event* event)
{
	// ������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(touch->getLocation());

	// �Ƿ��ڲ�����
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		// �õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		// �õ���ǰѡ�еĶ���
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

	// ��ѡ���������
	int startX = m_animalSelected->getX();
	int startY = m_animalSelected->getY();

	// ������Ĳ�������
	auto pos = this->convertToNodeSpace(touch->getLocation());
	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	// ��������㲻�ڲ����ڣ����ߴ����㲼���������ѡ���ﲼ������һ������ô����
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) ||
		Vec2(touchX, touchY) == Vec2(startX, startY))
	{
		return;
	}

	// �ж���ѡ����Ĳ��������봥����Ĳ��������Ƿ�ֱ�����һ����λ
	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		return;
	}

	// ���µ����������������Ķ�����������н����Ķ���
	// ��ȡ�������Ķ���
	m_animalSwapped = m_AnimalGrid[touchX][touchY];

	// ��������
	swapAnimals(m_animalSelected, m_animalSwapped);

	// ��������״̬��׽����
	this->schedule(schedule_selector(AnimalGrid::onAnimalsSwaping));

}

// ��������
void AnimalGrid::swapAnimals(Animal *animalA, Animal *animalB)
{
	_eventDispatcher->pauseEventListenersForTarget(this); // ������ʼ���رմ�������

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

// �����ƶ�λ��
void AnimalGrid::swapAnimalToNewPos(Animal* animal)
{
	// ���ö��ｻ��״̬Ϊ�棬�ƶ�����������Ϊ��
	animal->setSwapingState(true);

	auto move = MoveTo::create(MOVE_SPEED, Vec2(animal->getX() * GRID_WIDTH, animal->getY() * GRID_WIDTH));
	auto call = CallFunc::create([animal](){
		animal->setSwapingState(false);
	});

	animal->runAction(Sequence::create(move, call, nullptr));
}

// �ж�����
bool AnimalGrid::canCrush()
{
	int count = 0; // ������
	Animal *AnimalBegin = nullptr; // ��ʼ�����Ķ���
	Animal *AnimalNext = nullptr; // ����ʼ���￪ʼ��ǰ�����Ķ���

	// ����ÿһ��
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row - 1;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x][y + 1];

			// �����������ͬ����
			while (AnimalBegin->getType() == AnimalNext->getType())
			{
				count++;
				int nextIndex = y + count;
				if (nextIndex > m_row - 1)
				{
					break;
				}
				AnimalNext = m_AnimalGrid[x][nextIndex];
			}
			// ������������ڵ���3����ô��������Щ����Ӧ�������������Ǵ��������������
			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto animal = m_AnimalGrid[x][y + n];
					m_crushAnimalGrid.pushBack(animal);
				}
			}
			y += count;
		}
	}

	// ����ÿһ��
	for (int y = 0; y < m_row; y++)
	{
		for (int x = 0; x < m_col - 1;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x + 1][y];

			while (AnimalBegin->getType() == AnimalNext->getType())
			{
				count++;
				int nextIndex = x + count;
				if (nextIndex > m_col - 1)
					break;
				AnimalNext = m_AnimalGrid[nextIndex][y];
			}

			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto animal = m_AnimalGrid[x + n][y];

					// �п����ж���ͬʱ���п���������ô�����ظ��洢������������ӣ��������һ���ж�
					if (m_crushAnimalGrid.find(animal) != m_crushAnimalGrid.end())
					{
						continue;
					}
					m_crushAnimalGrid.pushBack(animal);
				}
			}
			x += count;
		}
	}

	// �������������Ӳ�Ϊ�գ���ô˵�������п�������������
	if (!m_crushAnimalGrid.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}




//	��ʼ����
void AnimalGrid::goCrush()
{
	// ���������������
	for (auto animal : m_crushAnimalGrid)
	{
		// �����µĶ����ʼλ���ڲ�����һ��
		auto newAnimal = Animal::createByType(random(1, ANIMAL_NUM), animal->getX(), m_row);
		newAnimal->setPosition(newAnimal->getX()*GRID_WIDTH, newAnimal->getY()*GRID_WIDTH);
		this->addChild(newAnimal);

		// ���¶���ŵ��¶�������ڣ��ȴ����벼��
		m_newAnimalGrid.pushBack(newAnimal);

		// ���������Ӧ��ˢ�µĶ�����ʱ��Ϊ��
		m_AnimalGrid[animal->getX()][animal->getY()] = nullptr;

		// ԭ�ж����������
		animal->crush();
	}
}

// ˢ�²���
void AnimalGrid::refreshAnimalGrid()
{
	// �����У���������п�λ����ôӦ��ˢ��
	for (int x = 0; x < m_col; x++)
	{
		int empty_count = 0; // һ���ܵĿո�����

		for (int y = 0; y < m_row; y++)
		{
			// ��������������������ڵĶ���ָ�룬���Ϊ�գ���ô˵��������λ��Ϊ��
			auto animal = m_AnimalGrid[x][y];
			if (!animal)
			{
				empty_count++;
			}
		}

		if (empty_count)
		{
			// �ҵ��п�λ���У�ˢ�¸��еĶ���
			refreshAnimalsToNewPos(x);
		}
	}
}

// �������
void AnimalGrid::refreshAnimalsToNewPos(int col)
{
	// ˢ�¸�������Ķ���
	int n = 0; // ��ǰ�������Ŀ�λ��
	auto p_animalBox = &m_AnimalGrid; // ����һ��������ӵ�ָ�룬����Ϊ�������ܴ���lamda

	// �������еĶ�������
	for (int y = 0; y < m_row; y++)
	{
		auto animal = m_AnimalGrid[col][y];

		if (!animal)
		{
			n++;
			continue;
		}
		else if (n != 0)
		{
			animal->setY(animal->getY() - n);
			auto move = MoveBy::create(0.2, Vec2(0, -n*GRID_WIDTH));
			auto call = CallFunc::create([p_animalBox, animal](){
				// ���¶�������ڵ�����
				(*p_animalBox)[animal->getX()][animal->getY()] = animal;
			});

			animal->runAction(Sequence::create(move, call, nullptr));
		}
	}

	// �����¶�������
	int i = n;
	int delta = 1;

	for (auto animal : m_newAnimalGrid)
	{
		if (animal->getX() == col)
		{
			animal->setY(m_row - i);

			auto delay = DelayTime::create(0.2);
			// ��������ٶ�������һЩ
			auto move = MoveBy::create(0.2*delta++, Vec2(0, -i--*GRID_WIDTH));
			auto call = CallFunc::create([animal, p_animalBox, this](){
				(*p_animalBox)[animal->getX()][animal->getY()] = animal;
				// ���¶���������Ƴ��ö���
				m_newAnimalGrid.eraseObject(animal);
			});

			animal->runAction(Sequence::create(delay, move, call, nullptr));
		}
	}
}
//
// ��׽����״̬
void AnimalGrid::onAnimalsSwaping(float dt)
{
	// ��׽�������ڽ����Ķ���Ľ��������Ƿ��Ѿ�ֹͣ�����ûֹͣ�����ؼ�����׽
	if (m_animalSelected->isSwaping() || m_animalSwapped->isSwaping())
	{
		return;
	}
	else
	{
		// ֹͣ��׽
		this->unschedule(schedule_selector(AnimalGrid::onAnimalsSwaping));

		// �жϵ�ǰ״̬�Ƿ��������
		if (canCrush())
		{
			m_animalSelected = nullptr;

			SimpleAudioEngine::getInstance()->playEffect("cursh.mp3");
			goCrush();
			this->schedule(schedule_selector(AnimalGrid::onAnimalsCrushing));
		}
		else
		{
			// ����������������ȥ��������������ʱ�Ĳ�׽����
			swapAnimals(m_animalSelected, m_animalSwapped);
			this->schedule(schedule_selector(AnimalGrid::onAnimalsSwapingBack));
		}
	}
}

// ��λ
void AnimalGrid::onAnimalsSwapingBack(float dt)
{
	// ��׽�������ڽ����Ķ���Ľ��������Ƿ��Ѿ�ֹͣ�����ûֹͣ�����ؼ�����׽
	if (m_animalSelected->isSwaping() || m_animalSwapped->isSwaping())
	{
		return;
	}
	else
	{
		this->unschedule(schedule_selector(AnimalGrid::onAnimalsSwapingBack)); // ֹͣ��׽

		m_animalSelected = nullptr;

		_eventDispatcher->resumeEventListenersForTarget(this); // ���¿�ʼ��������
	}
}

void AnimalGrid::onAnimalsCrushing(float dt)
{
	// ��׽��������״̬������ж��ﻹ����������ô������׽
	for (auto animal : m_crushAnimalGrid)
	{
		if (animal->isCrushing())
		{
			return;
		}
	}

	// ���ȫ�������Ѿ�������ϣ�ֹͣ��׽����
	unschedule(schedule_selector(AnimalGrid::onAnimalsCrushing));

	// ��������������
	m_crushAnimalGrid.clear();

	// ˢ�¶������У�������ˢ��״̬��׽����
	refreshAnimalGrid();
	this->schedule(schedule_selector(AnimalGrid::onAnimalsRefreshing));
}

void AnimalGrid::onAnimalsRefreshing(float dt)
{
	// ��׽����ˢ��״̬������¶�����ӻ��ж����ô������׽
	if (m_newAnimalGrid.size() != 0)
	{
		return;
	}
	else
	{
		unschedule(schedule_selector(AnimalGrid::onAnimalsRefreshing));

		if (canCrush())
		{
			// �������������ô��������
			goCrush();
			this->schedule(schedule_selector(AnimalGrid::onAnimalsCrushing));
		}
		else
		{

			// �ж��Ƿ�Ϊ��ͼ������ǣ���ʾ�������µ�ͼ
			if (isDeadMap())
			{
				auto winSize = Director::getInstance()->getWinSize();
				auto label = Label::createWithTTF("Cant Crush Any More, Change!", "fonts/Marker Felt.ttf", 24);
				label->setTextColor(Color4B::BLACK);
				label->setPosition(winSize.width / 2, winSize.height / 2);
				label->setOpacity(0);
				this->getParent()->addChild(label, 3);

				// ��ʾ���ֵ��뵭���󣬸��µ�ͼ���ٿ�����������
				auto fadein = FadeIn::create(0.5);
				auto fadeout = FadeOut::create(0.5);

				auto call = CallFunc::create([this, label](){
					do
					{
						updateMap();
					} while (isDeadMap());

					label->removeFromParent();

					_eventDispatcher->resumeEventListenersForTarget(this);
				});

				label->runAction(Sequence::create(fadein, DelayTime::create(2), fadeout, call, nullptr));
			}
			else
			{
				// ���������ͼ����ô��ֱ�ӿ��������������ȴ���һ�ֵĽ�������
				_eventDispatcher->resumeEventListenersForTarget(this);
			}
		}
	}
}