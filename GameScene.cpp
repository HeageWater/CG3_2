#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete objSkydome;
	delete objGround;
	delete objFighter;
	delete modelSkydome;
	delete modelGround;
	delete modelFighter;
	delete camera;

	delete objSphere;
	delete modelSphere;

	//safe_delete(light);
	//safe_delete(lightGroup);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	// カメラ生成
	camera = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight, input);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 1, 0 });
	camera->SetDistance(3.0f);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();

	objSphere = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objFighter->SetModel(modelFighter);

	modelSphere = Model::CreateFromOBJ("sphere", true);
	//modelSphere = Model::CreateFromOBJ("enemy");
	objSphere->SetModel(modelSphere);

	objFighter->SetPosition({ +1, 0, 0 });
	objSphere->SetPosition({ -1, 1, 0 });

	//ライト生成
	light = DirectionalLight::Create();
	light->SetLightColor({ 1,1,1 });
	Object3d::SetLight(light);

	//lightGroup = LightGroup::Create();
	//Object3d::SetLightGroup(lightGroup);
}

void GameScene::Update()
{
	{
		XMFLOAT3 rot = objSphere->GetRotation();
		rot.y += 1.0f;
		objSphere->SetRotation(rot);
		objFighter->SetRotation(rot);
	}

	{
		static XMVECTOR lightDir = { 0,1,5,0 };

		if (input->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; };
		if (input->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; };
		if (input->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; };
		if (input->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; };

		light->SetLightDir(lightDir);
		//light->SetLight(light);
		//objFighter->SetLight(light);

		std::ostringstream debugstr;
		debugstr << "lightDirFactor("
			<< std::fixed << std::setprecision(2)
			<< lightDir.m128_f32[0]<<","
			<< lightDir.m128_f32[1]<<","
			<< lightDir.m128_f32[2]<<")";
		debugText.Print(debugstr.str(),50,50,1.0f);
	}

	{
		/*lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
		lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2],0 }));
		lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));

		lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2],0 }));
		lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor1));

		lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2],0 }));
		lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor2));*/

	}

	light->Update();
	//light2->Update();
	camera->Update();

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();

	objSphere->Update();

	debugText.Print("AD: move camera LeftRight", 50, 80, 1.0f);
	debugText.Print("WS: move camera UpDown", 50, 100, 1.0f);
	debugText.Print("ARROW: move camera FrontBack", 50, 120, 1.0f);
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//objSkydome->Draw();
	//objGround->Draw();
	objFighter->Draw();

	objSphere->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
