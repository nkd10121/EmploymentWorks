#pragma once
#include "SceneBase.h"
//テスト用
#include <list>

class SceneGame : public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void StartLoad() override;
	virtual bool IsLoaded() const override;

	virtual void Init() override;
	virtual void End() override;

	virtual void Update() override;
	virtual void Draw() override;

private:
	std::list<int> temp_handle;
};

