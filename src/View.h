#pragma once

#include "Observer.h"

class Model;
class Controller;

class View : public Observer
{
public:
	View(Model* model);
	virtual ~View();

	virtual void updateNotify() override;

	virtual void init();
	virtual void draw();

	Model* getModel();
	Controller* getController();
protected:
	virtual Controller* makeController();
protected:
	Model* m_model;
	Controller* m_controller;
};
