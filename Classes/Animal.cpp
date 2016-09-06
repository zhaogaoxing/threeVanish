#include "Animal.h"

USING_NS_CC;
Animal* Animal::createByType(int type, int x, int y)
{
	auto animal = new Animal();

	if (animal && animal->init(type, x, y))
	{
		animal->autorelease();

		return animal;
	}
	else
	{
		CC_SAFE_DELETE(animal);

		return nullptr;
	}
}

bool Animal::init(int type, int x, int y)
{
	Sprite::init();

	m_type = type;
	m_x = x;
	m_y = y;

	m_special = 0;

	m_isSwaping = false;
	m_isCrushing = false;

	m_isNeedClean = false;

	// ������Դ����ʼ������
	char name[100] = { 0 };
	sprintf(name, "texture/animal%d.png", m_type);
	this->initWithTexture(TextureCache::getInstance()->getTextureForKey(name));

	// ���½�Ϊê��
	this->setAnchorPoint(Vec2(0, 0));

	return true;
}

void Animal::crush()
{
	// ��ʼ����������״̬Ϊ�棬ֱ�����������������������Ƴ���Ⱦ�ڵ㣬��������״̬Ϊ��
	m_isCrushing = true;
	auto action = FadeOut::create(0.2);

	auto action1 = MoveTo::create(0.1, Vec2(this->getPosition().x, this->getPosition().y + 20));
	auto action2 = MoveTo::create(0.1, Vec2(this->getPosition().x, this->getPosition().y - 40));
	auto action3 = MoveTo::create(0.1, Vec2(this->getPosition().x, this->getPosition().y + 40));
	auto action4 = MoveTo::create(0.1, Vec2(this->getPosition().x, this->getPosition().y - 20));

	auto call = CallFunc::create([this](){
		this->removeFromParent();
		m_isCrushing = false;
	});

	this->runAction(Sequence::create(action1, action2, action3, action4, action, call, nullptr));
}