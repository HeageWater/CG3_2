#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	assert(!LightGroup::device);

	assert(device);

	LightGroup::device = device;
}

void LightGroup::Initialize()
{
	DefaultSetting();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

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

void LightGroup::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex) {
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;

	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);

	if (SUCCEEDED(result))
	{
		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++)
		{
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].active = false;
			}

			constBuff->Unmap(0, nullptr);
		}

		//constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightdir);
	dirty = true;

}

void LightGroup::DefaultSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[0].SetLightDir({ 0.0f,-1.0f,0.0f,0 });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ +0.5f,+0.1f,0.2f,0 });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ -0.5f,+0.1f,-0.2f,0 });

}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup;

	instance->Initialize();

	return instance;
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}
