#include "DirectionalLight.h"
#include <cassert>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実態
/// </summary>
ID3D12Device* DirectionalLight::device = nullptr;


void DirectionalLight::StaticInitialize(ID3D12Device* device) {

	//再初期化チェック
	assert(!DirectionalLight::device);

	//nullptrチェック
	assert(device);

	//っ性的メンバ変数のセット
	DirectionalLight::device = device;
}

void DirectionalLight::Initialize() {
	//// nullptrチェック
	//assert(device);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	//HRESULT result;
	//// 定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	//	IID_PPV_ARGS(&constBuffB0));
	//assert(SUCCEEDED(result));

	//// 定数バッファのマッピング
	//result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//assert(SUCCEEDED(result));

	HRESULT result;
	result = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); };

	//定数バッファへデータ転送
	TransferConstBuffer();
}


void DirectionalLight::TransferConstBuffer() {

	HRESULT result;

	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);

	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}

}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor) {
	this->lightcolor = lightcolor;
	dirty = true;
}

void DirectionalLight::Update() {
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) {
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

DirectionalLight* DirectionalLight::Create() {
	DirectionalLight* instance = new DirectionalLight();

	instance->Initialize();

	return instance;
}