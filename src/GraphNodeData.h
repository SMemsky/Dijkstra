#pragma once

// Extra data hold in each graph node

struct GraphNodeData
{
	struct
	{
		unsigned x;
		unsigned y;
	} pos;

	GraphNodeData(unsigned x, unsigned y) :
		pos{x, y} // I can initialize anonymous struct? Yippee!
	{}
};
