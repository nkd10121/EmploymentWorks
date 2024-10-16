#pragma once
#include "SceneBase.h"
class ScenePause : public SceneBase
{
public:
	ScenePause();
	virtual ~ScenePause();

	virtual void StartLoad() override;
	virtual bool IsLoaded() const override;

	virtual void Init() override;
	virtual void End() override;

	virtual void Update() override;
	virtual void Draw() override;
};

