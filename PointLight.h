#pragma once

#include <DirectXMath.h>

class PointLight
{
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
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

private:
	XMFLOAT3 lightpos = { 0,0,0 };
	XMFLOAT3 lightcolor = { 1,1,1 };
	XMFLOAT3 lightatten = { 1.0f,1.0f,1.0f };
	bool active = false;

public:
	inline void SetPos(const XMFLOAT3& pos) { lightpos = pos; };
	inline XMFLOAT3& GetPos() { return lightpos; };

	inline void SetColor(const XMFLOAT3& pos) { lightcolor = pos; };
	inline XMFLOAT3& GetColor() { return lightcolor; };

	inline void SetAtten(const XMFLOAT3& pos) { lightatten = pos; };
	inline XMFLOAT3& GetSetAtten() { return lightatten; };

	inline void SetActive(bool a) { active = a; };
	inline bool IsActive() { return active; };
};