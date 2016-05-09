#include "Controller.h"

#include "View.h"
#include "Model.h"

Controller::Controller(View* view) :
	m_view(view),
	m_model(view->getModel())
{
	m_model->attach(this);
}

Controller::~Controller()
{
	m_model->detach(this);
}

void Controller::updateNotify()
{}

bool Controller::update()
{
	return true;
}
