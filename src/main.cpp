#include <iostream>

#include "View.h"
#include "Controller.h"
#include "GraphView.h"
#include "GraphModel.h"

int main()
{
	GraphModel m;
	View* v1 = new GraphView(&m);
	v1->init();

	auto c1 = v1->getController();

	bool running = true;
	while (running)
	{
		running &= c1->update();
	}

	delete v1;

	return 0;
}