#pragma once
class GameManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

};