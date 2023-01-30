#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <vector>
#include <wrl.h>
#include <unordered_map>

class DirectionalLight {
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
  // �萔�o�b�t�@�p�f�[�^�\����
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
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// 
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ���C�g�̕����Z�b�g
	/// </summary>
	/// <param name="lightdir"></param>
	void SetLightDir(const XMVECTOR& lightdir);
	XMVECTOR GetLightDir() { return lightdir; };
	/// <summary>
	/// ���C�g�̐F�Z�b�g
	/// </summary>
	/// <param name="lightdir"></param>
	void SetLightColor(const XMFLOAT3& lightcolor);
	XMFLOAT3 GetLightColor() { return lightcolor; };

	/// <summary>
	///	�X�V
	/// </summary>
	/// <returns></returns>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	static DirectionalLight* Create();

	_inline void SetActive(bool active) { this->active = active; };
	_inline bool IsActive() { return active; };

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	//���C�g������(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 1,0,0,0 };

	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };

	//�_�[�e�B�t���O
	bool dirty = false;


	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@

	// �萔�o�b�t�@�̃}�b�v
	ConstBufferDataB0* constMap = nullptr;
};