#pragma once

#include <set>

class Observer;

class Model
{
public:
	Model();
	virtual ~Model();

	void attach(Observer* observer);
	void detach(Observer* observer);
protected:
	void notify();
private:
	std::set<Observer*> m_observers;
};
