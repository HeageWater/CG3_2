#pragma once
#include "DirectionalLight.h"

class LightGroup {
private: // エイリアス
// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int DirLightNum = 3;

public: // サブクラス
  // 定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT3 ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
	};

private:
	static ID3D12Device* device;

	ComPtr<ID3D12Resource> constBuff;

	XMFLOAT3 ambientColor = { 1,1,1 };

	DirectionalLight dirLights[DirLightNum];

	bool dirty = false;
public:
	static void StaticInitialize(ID3D12Device* device);

	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetAmbientColor(const XMFLOAT3& color);
	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	void SetDirLightColor(int index, const XMFLOAT3& lightdir);

	void DefaultSetting();

	static LightGroup* Create();
};