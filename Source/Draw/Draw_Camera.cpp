//==============================================================================
// Filename: Draw_Camera.cpp
// Description: カメラ処理（親クラス）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_Camera.h>
#include <Tool/Tool_Math.h>

namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -10.0f };  //カメラ位置
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };   //注視点位置
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };     //アップベクトル

constexpr short START_FOV = 60;         //視野角
constexpr float START_NEAR_Z = 0.5f;
constexpr float START_FAR_Z = 40.0f;    //クリップ距離

//===== クラス実装 =====
DrawCamera::DrawCamera(App64& app) noexcept : m_app(app),
    m_pos(START_POS), m_lookAt(START_LOOK), m_vUp(START_UP),
    m_FOV(START_FOV), g_aspectRatio(), m_nearZ(START_NEAR_Z), m_farZ(START_FAR_Z),
    m_mtxWorld(), m_mtxView(), m_mtxProj(), m_rot(0.0f, 0.0f, 0.0f), m_sensitivity(1.0f)
{
    //縦横比初期化
    auto [x, y] = m_app.GetWindowProc()->GetSize();
    g_aspectRatio = static_cast<float>(x) / static_cast<float>(y);

    //行列初期化
    InitMatrix();
}

DrawCamera::~DrawCamera() noexcept
{
}

//行列初期化
void DrawCamera::InitMatrix() noexcept
{
    //ワールド行列
    dx::XMStoreFloat4x4(&m_mtxWorld,
        dx::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

    //ビュー行列
    dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixLookAtLH(
        dx::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f),
        dx::XMVectorSet(m_lookAt.x, m_lookAt.y, m_lookAt.z, 0.0f),
        dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f))
    );

    //投影行列
    dx::XMStoreFloat4x4(&m_mtxProj,
        dx::XMMatrixPerspectiveFovLH(gMath::GetRad(m_FOV), g_aspectRatio, m_nearZ, m_farZ));
}

//投影行列更新
void DrawCamera::UpdateProjMatrix() noexcept
{
    dx::XMStoreFloat4x4(&m_mtxProj,
        dx::XMMatrixPerspectiveFovLH(gMath::GetRad(m_FOV), g_aspectRatio, m_nearZ, m_farZ));
}
