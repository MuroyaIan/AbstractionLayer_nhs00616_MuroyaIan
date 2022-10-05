﻿//==============================================================================
// Filename: Draw_CameraTest.h
// Description: テストカメラ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Draw/Draw_Camera.h>

//===== クラス定義 =====

//***** テストカメラ *****
class DrawCameraTest : public DrawCamera
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
    DrawCameraTest(
        /*[in]*/ App64& app) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~DrawCameraTest() noexcept override;

    //--------------------------------------------------------------------------
    /// 更新処理
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Update() noexcept override;

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// カメラリセット
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ResetCamera() noexcept override;

    //--------------------------------------------------------------------------
    /// 行列更新
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void UpdateMatrix() noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //INPUT_MGR& m_input;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_input     入力処理の参照先
    /// </summary>
};
