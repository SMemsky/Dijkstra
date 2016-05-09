#include "View.h"

#include <iostream>

#include "Model.h"
#include "Controller.h"

View::View(Model* model) :
	m_model(model),
	m_controller(nullptr)
{
	m_model->attach(this);
}

View::~View()
{
	if (m_controller != nullptr)
	{
		delete m_controller;
	}

	m_model->detach(this);
}

void View::updateNotify()
{}

void View::init()
{
	m_controller = makeController();
}

void View::draw()
{}

Controller* View::makeController()
{
	return new Controller(this);
}

Model* View::getModel()
{
	return m_model;
}

Controller* View::getController()
{
	return m_controller;
}
