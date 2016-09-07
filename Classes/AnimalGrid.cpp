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
	this->schedule(schedule_selector(AnimalGrid::onAnimalsSwaping));
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

//�ж�����
bool AnimalGrid::canCrush()
{
	int count = 0;//������
	Animal *AnimalBegin = nullptr;//��ʼ�����Ķ���
	Animal *AnimalNext = nullptr;//����ʼ���￪ʼ��������Ķ���

	//����ÿһ��
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row - 1;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x][y + 1];

			//�����������ͬ����
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
			//������������ڵ���3����ô��������Щ����Ӧ�������������Ǵ��������������
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

	//����ÿһ��
	for (int y = 0; y < m_row; y++)
	{
		for (int x = 0; x < m_col;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x + 1][y];

			while (AnimalBegin->getType() == AnimalNext->getType())
			{
				count++;
				int nextIndex = x + count;
				if (nextIndex>m_col)
				{
					break;
				}
				AnimalNext = m_AnimalGrid[nextIndex][y];
			}

			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto animal = m_AnimalGrid[x + n][y];

					//�п����ж���ͬʱ���п��������������ظ����浽����������ӣ��������һ���ж�
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

	//�������������Ӳ�Ϊ�գ���ô˵�������п���������������
	if (!m_crushAnimalGrid.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//��Ǵ���Ķ�������״̬
void AnimalGrid::singeAnimal(Animal* animal)
{
	int row = animal->getX();
	int col = animal->getY();

	m_crushAnimalH.pushBack(animal);
	m_crushAnimalV.pushBack(animal);

	//�ж϶�������Ƿ���ͬ
	while (col > 1)
	{
		col = col - 1;

		auto animal_left = m_AnimalGrid[animal->getX()][col];

		if (animal_left&&animal_left->getType() == animal->getType())
		{
			m_crushAnimalH.pushBack(animal_left);
		}
		else
		{
			break;
		}
	}

	//�ж϶����ұ�
	if (animal->getY() != m_col)
	{
		for (int i = animal->getY() + 1; i < m_col; i++)
		{
			auto animal_right = m_AnimalGrid[animal->getX()][i];

			if (animal_right && animal_right->getType() == animal->getType())
			{
				m_crushAnimalH.pushBack(animal_right);
			}
			else
			{
				break;
			}
		}
	}

	// �ж϶�������
	if (animal->getX() != m_row)
	{
		for (int i = animal->getX() + 1; i < m_row; i++)
		{
			auto animal_up = m_AnimalGrid[i][animal->getY()];

			if (animal_up && animal_up->getType() == animal->getType())
			{
				m_crushAnimalV.pushBack(animal_up);
			}
			else
			{
				break;
			}
		}

	}

	// �ж϶�������
	while (row > 1)
	{
		row = row - 1;

		auto animal_down = m_AnimalGrid[row][animal->getY()];

		if (animal_down && animal_down->getType() == animal->getType())
		{
			m_crushAnimalV.pushBack(animal_down);
		}
		else
		{
			break;
		}
	}

	// �жϺ�����������Ǵ���������
	if (m_crushAnimalH.size() < 3)
	{
		m_crushAnimalH.clear();
	}
	else
	{

		for (auto animalH : m_crushAnimalH)
		{
			animalH->setClean(true);
		}
	}
	// �ж�������������Ǵ���������
	if (m_crushAnimalV.size() < 3)
	{
		m_crushAnimalV.clear();
	}
	else
	{

		for (auto animalV : m_crushAnimalV)
		{
			animalV->setClean(true);
		}
	}
	// �ж�4��
	if (m_crushAnimalH.size() == 4 || m_crushAnimalV.size() == 4)
	{
		log("4xiao");
		bool crush = true;

		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				crush = false;
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				crush = false;
			}
		}

		if (crush)
		{
			if (m_crushAnimalH.size() == 4 && m_crushAnimalV.size() < 4)
			{
				animal->setSpeicial(1);
			}
			else if (m_crushAnimalV.size() == 4 && m_crushAnimalH.size() < 4)
			{
				animal->setSpeicial(2);
			}
		}
	}
	// �ж�T,L����
	if (m_crushAnimalH.size() + m_crushAnimalV.size() >= 6)
	{
		log("Txiao");
		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				animalH->setSpeicial(0);
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				animalV->setSpeicial(0);
			}

		}

		animal->setSpeicial(3);
	}

	// �ж�5��
	if (m_crushAnimalH.size() == 5 || m_crushAnimalV.size() == 5)
	{
		log("5xiao");
		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				animalH->setSpeicial(0);
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				animalV->setSpeicial(0);
			}

		}

		animal->setSpeicial(4);
	}
	// ��������������Ϊ����������ǵĶ���������
	for (auto animalH : m_crushAnimalH)
	{
		if (animalH->isNeedClean() && (animalH->getSpecial() > 0))
		{
			this->specialSinged(animalH);
		}
	}
	// ��������������Ϊ����������ǵĶ���������
	for (auto animalV : m_crushAnimalV)
	{
		if (animalV->isNeedClean() && (animalV->getSpecial() > 0))
		{
			this->specialSinged(animalV);
		}
	}

	m_crushAnimalH.clear();
	m_crushAnimalV.clear();
}

// ���������
void AnimalGrid::specialSinged(Animal* animal)
{
	// ����һ����
	if (animal->getSpecial() == 1)
	{
		for (int i = 0; i < m_row; i++)
		{
			auto animal_V = m_AnimalGrid[i][animal->getY()];
			animal_V->setClean(true);

			if (animal_V->getSpecial()>0)
			{
				specialSinged(animal_V);
			}
		}
	}

	// ����һ����
	if (animal->getSpecial() == 2)
	{
		for (int i = 0; i < m_col; i++)
		{
			auto animal_H = m_AnimalGrid[animal->getX()][i];
			animal_H->setClean(true);

			if (animal_H->getSpecial() > 0)
			{
				specialSinged(animal_H);
			}
		}
	}

	// ��������
	if (animal->getSpecial() == 3)
	{
		for (int x = 0; x < m_row; x++)
		{
			for (int y = 0; y < m_col; y++)
			{
				auto animal_L = m_AnimalGrid[x][y];

				if ((abs(animal_L->getX() - animal->getX()) +
					abs(animal_L->getY() - animal->getY())) < 3)
				{
					animal_L->setClean(true);

					if (animal_L->getSpecial() > 0)
					{
						specialSinged(animal_L);
					}
				}
			}
		}
	}

	// ����ͬһ����
	if (animal->getSpecial() == 4)
	{
		for (int x = 0; x < m_row; x++)
		{
			for (int y = 0; y < m_col; y++)
			{
				auto animalF = m_AnimalGrid[x][y];

				if (animalF && animalF->getType() == animal->getType())
				{
					animalF->setClean(true);
				}
			}
		}
	}
}

// ����ȫ�֣��Ƿ���������ǵĶ���
bool AnimalGrid::checkGridClean()
{
	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (m_AnimalGrid[i][j]->isNeedClean())
			{
				return true;
			}
		}
	}

	return false;
}


//��ʼ����
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
			/*this->schedule(schedule_selector(AnimalGrid::onAnimalsCrushing));*/
		}
		//else
		//{
		//	// ����������������ȥ��������������ʱ�Ĳ�׽����
		//	swapAnimals(m_animalSelected, m_animalSwapped);
		//	this->schedule(schedule_selector(AnimalGrid::onAnimalsSwapingBack));
		//}
	}
}

