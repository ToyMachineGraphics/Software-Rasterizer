#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

class Scene
{
public:
	void Render()
	{
		RenderTask();
	}

	void LeftKeyDown()
	{
		LeftKeyDownTask();
	}

	void RightKeyDown()
	{
		RightKeyDownTask();
	}

	void UpKeyDown()
	{
		UpKeyDownTask();
	}

	void DownKeyDown()
	{
		DownKeyDownTask();
	}

	void DKeyDown()
	{
		DKeyDownTask();
	}

	void CKeyDown()
	{
		CKeyDownTask();
	}
private:
	virtual void RenderTask() = 0;
	virtual void LeftKeyDownTask() = 0;
	virtual void RightKeyDownTask() = 0;
	virtual void UpKeyDownTask() = 0;
	virtual void DownKeyDownTask() = 0;
	virtual void DKeyDownTask() = 0;
	virtual void CKeyDownTask() = 0;
};

#endif