////==============================================================================
//// Filename: Draw_CameraMgr.cpp
//// Description: カメラマネージャ
//// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
////==============================================================================
//
////===== インクルード部 =====
//#include <Draw/Draw_CameraMgr.h>
//#include <Draw/Draw_CameraTest.h>
//
//namespace dx = DirectX;
//
////===== クラス実装 =====
//DrawCameraMgr::DrawCameraMgr(App64& app) noexcept : m_app(app),
//    m_mtxView(), m_mtxProj(),
//    m_aCamera(static_cast<int>(CameraID::MAX_CAMERA)), m_currentCamera(CameraID::TEST)
//{
//    //行列初期化
//    dx::XMMATRIX mtx = dx::XMMatrixIdentity();
//    dx::XMStoreFloat4x4(&m_mtxView, mtx);
//    dx::XMStoreFloat4x4(&m_mtxProj, mtx);
//
//    //カメラ初期化
//    m_aCamera[static_cast<int>(CameraID::TEST)] = std::make_unique<DrawCameraTest>(app);
//}
//
//DrawCameraMgr::~DrawCameraMgr() noexcept
//{
//}
//
////更新処理
//void DrawCameraMgr::Update() noexcept
//{
//    m_aCamera[static_cast<int>(m_currentCamera)]->Update();
//    m_mtxView = m_aCamera[static_cast<int>(m_currentCamera)]->GetViewMtx();
//    m_mtxProj = m_aCamera[static_cast<int>(m_currentCamera)]->GetProjMtx();
//}
//
////描画処理
//void DrawCameraMgr::Draw() const noexcept
//{
//    //行列情報をバインド
//    m_app.GetGfxPack().m_dx.SetViewMtx(m_mtxView);
//    m_app.GetGfxPack().m_dx.SetProjectionMtx(m_mtxProj);
//    m_app.GetGfxPack().m_shaderMgr.Bind(DrawShaderMgr::BinderID::CB_VS_MTX_VP);
//}
//
////カメラセット
//void DrawCameraMgr::SetCamera(CameraID id) noexcept
//{
//    m_currentCamera = id;
//}
//
////ワールド行列取得
//DirectX::XMFLOAT4X4 DrawCameraMgr::GetWorldMtx() const noexcept
//{
//    return m_aCamera[static_cast<int>(m_currentCamera)]->GetWorldMtx();
//}
//
////回転情報取得
//DirectX::XMFLOAT3 DrawCameraMgr::GetRotation() const noexcept
//{
//    return m_aCamera[static_cast<int>(m_currentCamera)]->GetRotation();
//}
