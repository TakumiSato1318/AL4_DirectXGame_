#pragma once
#include<DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)からの距離
	float distance = 0.f;
};

/// <summary>
/// レイ(半直線)
/// </summary>
struct Ray
{
	//始点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

class ColisionPrimitive
{
};

class Triangle 
{
public:
	//頂点座標三つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;

	void CompuleNormal();

};

