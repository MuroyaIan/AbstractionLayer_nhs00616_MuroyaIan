﻿//==============================================================================
// Filename: Draw_Camera.h
// Description: カメラ処理（親クラス）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Draw/Draw_CameraMgr.h>

//===== クラス定義 =====

//***** メインカメラ *****
class DrawCamera
{
public:

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	/// コンストラクタ
	///
	/// \param[in] app  App参照
	///
	/// \return void
	//--------------------------------------------------------------------------
	DrawCamera(
		/*[in]*/ App64& app) noexcept;

	//--------------------------------------------------------------------------
	/// デストラクタ
	///
	/// \return void
	//--------------------------------------------------------------------------
	virtual ~DrawCamera() noexcept;

	//--------------------------------------------------------------------------
	/// 更新処理
	///
	/// \return void
	//--------------------------------------------------------------------------
	virtual void Update() noexcept = 0;

	//--------------------------------------------------------------------------
	/// ワールド行列取得
	///
	/// \return ワールド行列
	//--------------------------------------------------------------------------
	DirectX::XMFLOAT4X4 GetWorldMtx() const noexcept
	{
		return m_mtxWorld;
	}

	//--------------------------------------------------------------------------
	/// ビュー行列取得
	///
	/// \return ビュー行列
	//--------------------------------------------------------------------------
	DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
	{
		return m_mtxView;
	}

	//--------------------------------------------------------------------------
	/// 投影行列取得
	///
	/// \return 投影行列
	//--------------------------------------------------------------------------
	DirectX::XMFLOAT4X4 GetProjMtx() const noexcept
	{
		return m_mtxProj;
	}

	//--------------------------------------------------------------------------
	/// 回転情報取得
	///
	/// \return 回転情報
	//--------------------------------------------------------------------------
	DirectX::XMFLOAT3 GetRotation() const noexcept
	{
		return m_rot;
	}

	//--------------------------------------------------------------------------

protected:

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	/// 行列初期化
	///
	/// \return void
	//--------------------------------------------------------------------------
	void InitMatrix() noexcept;

	//--------------------------------------------------------------------------
	/// 投影行列更新
	///
	/// \return void
	//--------------------------------------------------------------------------
	void UpdateProjMatrix() noexcept;

	//--------------------------------------------------------------------------
	/// カメラリセット
	///
	/// \return void
	//--------------------------------------------------------------------------
	virtual void ResetCamera() noexcept = 0;

	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	App64& m_app;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_lookAt;
	DirectX::XMFLOAT3 m_vUp;
	short m_FOV;
	float g_aspectRatio;
	float m_nearZ;
	float m_farZ;
	DirectX::XMFLOAT4X4 m_mtxWorld;
	DirectX::XMFLOAT4X4 m_mtxView;
	DirectX::XMFLOAT4X4 m_mtxProj;
	DirectX::XMFLOAT3 m_rot;
	float m_sensitivity;
	//--------------------------------------------------------------------------

	/// <summary>
	/// m_app			App参照
	/// m_pos			座標
	/// m_lookAt		注視点
	/// m_vUp			アップベクトル
	/// m_FOV			視野角
	/// g_aspectRatio   縦横比
	/// m_nearZ			クリップ距離(視点に近い側)
	/// m_farZ			クリップ距離(視点に遠い側)
	/// m_mtxWorld		ワールド行列
	/// m_mtxView		ビュー行列
	/// m_mtxProj		投影行列
	/// m_rot			回転
	/// m_sensitivity	感度
	/// </summary>
};
