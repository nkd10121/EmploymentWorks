#pragma once
#include "DxLib.h"
#include "MyLib.h"
#include "Vec3.h"
#include <memory>

/// <summary>
/// オブジェクトの基底クラス
/// </summary>
class ObjectBase : public MyLib::Collidable
{
public:
	/// <summary>
	///	コンストラクタ
	/// </summary>
	/// <param name="priority">優先度</param>
	/// <param name="tag">タグ</param>
	ObjectBase(Priority priority, GameObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectBase();

	///// <summary>
	///// 初期化
	///// </summary>
	//virtual void Init() {};
	///// <summary>
	///// 更新
	///// </summary>
	//virtual void Update() {};
	///// <summary>
	///// 描画
	///// </summary>
	//virtual void Draw() {};

	/// <summary>
	/// モデルのロードを申請する
	/// </summary>
	virtual void LoadModel() {};

protected:
	int m_modelHandle;	//モデルハンドル
};