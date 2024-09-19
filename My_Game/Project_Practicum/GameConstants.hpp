class GC
{
public:
	GC(){};
	~GC(){};

	enum Direction
	{
		UP = -1,
		DOWN = 1,
		LEFT = -1,
		RIGHT = 1,
		IDLE = 0,
	};

	static const unsigned int FPS_LIMIT = 60;
private:

};