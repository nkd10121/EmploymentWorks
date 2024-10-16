#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void StartLoad() override;
	virtual bool IsLoaded() const override;

	virtual void Init() override;
	virtual void End() override;

	virtual void Update() override;
	virtual void Draw() override;
};

