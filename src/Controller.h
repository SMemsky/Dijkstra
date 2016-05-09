#pragma once

#include "Observer.h"

class View;
class Model;

class Controller : public Observer
{
public:
	Controller(View* view);
	virtual ~Controller();

	virtual void updateNotify() override;

	virtual bool update();
protected:
	View* m_view;
	Model* m_model;
};
