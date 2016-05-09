#include "Model.h"

#include <cassert>

#include "Observer.h"

Model::Model() :
	m_observers()
{}

Model::~Model()
{}

void Model::attach(Observer* observer)
{
	assert(observer != nullptr);

	m_observers.insert(observer);
}

void Model::detach(Observer* observer)
{
	m_observers.erase(observer);
}

void Model::notify()
{
	auto end = m_observers.end();
	for (auto it = m_observers.begin(); it != end; ++it)
	{
		(*it)->updateNotify();
	}
}
