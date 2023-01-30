#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <wrl.h>
#include <unordered_map>

class DirectionalLight {
private: // エイリアス
// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
  // 定数バッファ用データ構造体
	struct ConstBufferData {
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};

private:
	static ID3D12Device* device;
	bool active = false;

public:
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ライトの方向セット
	/// </summary>
	/// <param name="lightdir"></param>
	void SetLightDir(const XMVECTOR& lightdir);
	XMVECTOR GetLightDir() { return lightdir; };
	/// <summary>
	/// ライトの色セット
	/// </summary>
	/// <param name="lightdir"></param>
	void SetLightColor(const XMFLOAT3& lightcolor);
	XMFLOAT3 GetLightColor() { return lightcolor; };

	/// <summary>
	///	更新
	/// </summary>
	/// <returns></returns>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	static DirectionalLight* Create();

	_inline void SetActive(bool active) { this->active = active; };
	_inline bool IsActive() { return active; };

private:
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	//ライト交戦方向(単位ベクトル)
	XMVECTOR lightdir = { 1,0,0,0 };

	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };

	//ダーティフラグ
	bool dirty = false;


	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ

	// 定数バッファのマップ
	ConstBufferDataB0* constMap = nullptr;
};