﻿#include "GameScene.h"
#include <cassert>
#include<sstream>
#include<iomanip>
#include"Colision.h"
#include<sstream>
#include<iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete cheeseObj;
	delete sprite1;
	delete sprite2;
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

	Sprite::LoadTexture(2, L"Resources/texture.png");

	sprite1 = Sprite::Create(2, { 0,0 });
	sprite2 = Sprite::Create(2, { 500,500 }, { 1,0,0,1 }, { 0,0 }, false, true);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// 3Dオブジェクト生成
	cheeseModel = Model::StaticCreateFromOBJ("cheese");
	cheeseObj = Object3d::Create(cheeseModel);
	cheeseObj->SetPosition({ 0.f,0.f,0.f });
	cheeseObj->SetScale({ 1.f,1.f,1.f });
	cheeseObj->Update();
	kinokoModel = Model::StaticCreateFromOBJ("small");
	kinokoObj = Object3d::Create(kinokoModel);
	kinokoObj->SetPosition({ 0.f,0.f,0.f });
	kinokoObj->SetScale({ 2.f,2.f,2.f });
	kinokoObj->Update();

	//球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1);//中心点座標
	sphere.radius = 1.f;//半径

	//平面の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0);//法線ベクトル
	plane.distance = 0.f;//原点(0,0,0)からの距離

	//三角形の初期値を設定
	triangle.p0 = XMVectorSet(-1.f, 0, -1.f, 1);
	triangle.p1 = XMVectorSet(-1.f, 0, +1.f, 1);
	triangle.p2 = XMVectorSet(+1.f, 0, -1.f, 1);
	triangle.normal = XMVectorSet(0.f, 1.f, 0.f, 0);

	//レイの初期値を設定
	ray.start = XMVectorSet(0, 1, 0, 1);
	ray.dir = XMVectorSet(0, -1, 0, 0);
}

void GameScene::Update()
{

	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT3 position = cheeseObj->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { position.z -= 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.z += 1.0f; }
		/*if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }*/

		// 座標の変更を反映
		cheeseObj->SetPosition(position);
	}

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ -1.0f,0.0f,0.0f }); }
	}

	//球移動
	//{
	//	XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
	//	if (input->PushKey(DIK_NUMPAD8)) { sphere.center += moveY; }
	//	else if(input->PushKey(DIK_NUMPAD2)){ sphere.center -= moveY; }

	//	XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
	//	if (input->PushKey(DIK_NUMPAD6)) { sphere.center += moveX; }
	//	else if (input->PushKey(DIK_NUMPAD4)) { sphere.center -= moveX; }
	//}
	////stringstreamで変数の値を埋め込んで整形する
	//std::ostringstream spherestr;
	//spherestr << "Sphere:("
	//	<< std::fixed << std::setprecision(2)
	//	<< sphere.center.m128_f32[0] << ","
	//	<< sphere.center.m128_f32[1] << ","
	//	<< sphere.center.m128_f32[2] << ")";
	//
	//debugText.Print(spherestr.str(), 50, 180, 1.f);

	//レイ操作
	{
		XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f , 0);
		if (input->PushKey(DIK_NUMPAD8)) { ray.start += moveZ; }
		else if (input->PushKey(DIK_NUMPAD2)) { ray.start -= moveZ; }

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_NUMPAD6)) { ray.start += moveX; }
		else if (input->PushKey(DIK_NUMPAD4)) { ray.start -= moveX; }
	}
	//stringstreamで変数の値を埋め込んで整形する
	std::ostringstream raystr;
	raystr << "ray.start:("
		<< std::fixed << std::setprecision(2)
		<< ray.start.m128_f32[0] << ","
		<< ray.start.m128_f32[1] << ","
		<< ray.start.m128_f32[2] << ")";

	debugText.Print(raystr.str(), 50, 180, 1.f);

	//球と平面の当たり判定
	//XMVECTOR inter;
	//bool hit = Colision::CheckSphere2Plane(sphere, plane);
	//if (hit) {
	//	debugText.Print("HIT", 50, 200, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr<<"("
	//	<< std::fixed << std::setprecision(2)
	//		<< sphere.center.m128_f32[0] << ","
	//		<< sphere.center.m128_f32[1] << ","
	//		<< sphere.center.m128_f32[2] << ")";

	//	debugText.Print(spherestr.str(), 50, 220, 1.f);
	//}

	//レイと平面の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Colision::CheckRay2Plane(ray, plane,
	//&distance,&inter);
	//if (hit) {
	//	debugText.Print("HIT", 50, 260, 1.0f);
	//	//stringstreamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr<<"("
	//	<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(raystr.str(), 50, 280, 1.f);
	//}

	//レイと球の当たり判定
	XMVECTOR inter;
	float distance;
	bool hit = Colision::CheckRay2Sphere(ray, sphere,	&distance, &inter);
	if (hit) {
		debugText.Print("HIT", 50, 260, 1.0f);
		//stringstreamをリセットし、交点座標を埋め込む
		raystr.str("");
		raystr.clear();
		raystr << "inter"
			<< std::fixed << std::setprecision(2)
			<< inter.m128_f32[0] << ","
			<< inter.m128_f32[1] << ","
			<< inter.m128_f32[2] << ")";

		debugText.Print(raystr.str(), 50, 280, 1.f);

		raystr.str("");
		raystr.clear();
		raystr << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";

		debugText.Print(raystr.str(), 50, 300, 1.f);
	}

	cheeseObj->Update();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	/*sprite1->Draw();
	sprite2->Draw();*/

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	cheeseObj->Draw();
	kinokoObj->Draw();

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

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
