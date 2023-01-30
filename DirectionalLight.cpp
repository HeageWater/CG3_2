#include "DirectionalLight.h"
#include <cassert>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* DirectionalLight::device = nullptr;


void DirectionalLight::StaticInitialize(ID3D12Device* device) {

	//�ď������`�F�b�N
	assert(!DirectionalLight::device);

	//nullptr�`�F�b�N
	assert(device);

	//�����I�����o�ϐ��̃Z�b�g
	DirectionalLight::device = device;
}

void DirectionalLight::Initialize() {
	//// nullptr�`�F�b�N
	//assert(device);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	//HRESULT result;
	//// �萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
	//	IID_PPV_ARGS(&constBuffB0));
	//assert(SUCCEEDED(result));

	//// �萔�o�b�t�@�̃}�b�s���O
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

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}


void DirectionalLight::TransferConstBuffer() {

	HRESULT result;

	//�萔�o�b�t�@�փf�[�^�]��
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