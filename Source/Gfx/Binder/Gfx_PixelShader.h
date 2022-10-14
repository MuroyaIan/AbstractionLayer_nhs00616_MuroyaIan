﻿//==============================================================================
// Filename: Gfx_PixelShader.h
// Description: ピクセルシェーダ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

#if DX_11

//***** ピクセルシェーダ *****
class GfxPixelShader : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] path     シェーダファイルのディレクトリ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxPixelShader(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::wstring& path);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxPixelShader() noexcept override;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override;

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pPixelShader  シェーダのポインタ
    /// </summary>
};

#elif DX_12


#endif // GFX
