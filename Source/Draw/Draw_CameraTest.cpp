//==============================================================================
// Filename: Draw_CameraTest.cpp
// Description: テストカメラ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_CameraTest.h>
#include <Tool/Tool_Math.h>

namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr dx::XMFLOAT3 START_POS = { 0.0f, 0.0f, -20.0f };  //カメラ位置
constexpr dx::XMFLOAT3 START_LOOK = { 0.0f, 0.0f, 0.0f };   //注視点位置
constexpr dx::XMFLOAT3 START_UP = { 0.0f, 1.0f, 0.0f };     //アップベクトル

constexpr short START_FOV = 60;         //視野角
constexpr float START_NEAR_Z = 0.5f;
constexpr float START_FAR_Z = 40.0f;    //クリップ距離

//===== クラス実装 =====
DrawCameraTest::DrawCameraTest(App64& app) noexcept : DrawCamera(app),
    m_input(m_app.GetInputMgr())
{
    //親メンバ初期化
    m_pos = START_POS;
    m_lookAt = START_LOOK;
    m_vUp = START_UP;
    m_FOV = START_FOV;
    m_nearZ = START_NEAR_Z;
    m_farZ = START_FAR_Z;

    //行列初期化
    DrawCamera::InitMatrix();
}

DrawCameraTest::~DrawCameraTest() noexcept
{
}

//更新処理
void DrawCameraTest::Update() noexcept
{
    //回転制御
    if (m_input.m_kb.GetPress(VK_W))
        m_rot.x += gMath::GetRad(1);
    else if (m_input.m_kb.GetPress(VK_S))
        m_rot.x -= gMath::GetRad(1);
    if (m_input.m_kb.GetPress(VK_A))
        m_rot.y += gMath::GetRad(1);
    else if (m_input.m_kb.GetPress(VK_D))
        m_rot.y -= gMath::GetRad(1);
    if (m_input.m_kb.GetTrigger(VK_R))
        ResetCamera();

    //回転角度制限
    if (m_rot.x > F_PI)
        m_rot.x -= F_2PI;
    else if (m_rot.x < -F_PI)
        m_rot.x += F_2PI;
    if (m_rot.y > F_PI)
        m_rot.y -= F_2PI;
    else if (m_rot.y < -F_PI)
        m_rot.y += F_2PI;

    //行列更新
    UpdateMatrix();
}

//カメラリセット
void DrawCameraTest::ResetCamera() noexcept
{
    //データ初期化
    m_rot.x = 0.0f;
    m_rot.y = 0.0f;
}

//行列更新
void DrawCameraTest::UpdateMatrix() noexcept
{
    //ワールド行列
    dx::XMStoreFloat4x4(&m_mtxWorld,
        dx::XMMatrixTranslation(START_POS.x, START_POS.y, START_POS.z)
        * dx::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z)
    );

    //ビュー行列
    m_pos = { m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43 };
    m_vUp = { m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 };
    dx::XMStoreFloat4x4(&m_mtxView, dx::XMMatrixLookAtLH(
        dx::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f),
        dx::XMVectorSet(m_lookAt.x, m_lookAt.y, m_lookAt.z, 0.0f),
        dx::XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, 0.0f))
    );
}
