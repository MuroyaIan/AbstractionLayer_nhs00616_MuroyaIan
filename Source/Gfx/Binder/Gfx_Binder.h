﻿//==============================================================================
// Filename: Gfx_Binder.h
// Description: パイプライン用バインド処理（親クラス）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

//===== クラス定義 =====

//***** バインダ *****
class GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxBinder() noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    virtual ~GfxBinder() noexcept;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    virtual void Bind(
        /*[in]*/ GfxMain& gfx) noexcept = 0;

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// デバイス取得
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return DXデバイスのポインタ
    //--------------------------------------------------------------------------
    static ID3D11Device* GetDevice(
        /*[in]*/ GfxMain& gfx) noexcept
    {
        return gfx.m_pDevice.Get();
    }

    //--------------------------------------------------------------------------
    /// コンテキスト取得
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return DXコンテキストのポインタ
    //--------------------------------------------------------------------------
    static ID3D11DeviceContext* GetContext(
        /*[in]*/ GfxMain& gfx) noexcept
    {
        return gfx.m_pContext.Get();
    }

    //--------------------------------------------------------------------------

};
