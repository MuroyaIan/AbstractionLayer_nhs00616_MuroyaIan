﻿//==============================================================================
// Filename: Gfx_Inputlayout.h
// Description: 入力レイアウト処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

//***** 入力レイアウト *****
class GfxInputLayout : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx                      グラフィック処理の参照先
    /// \param[in] layout                   レイアウト情報
    /// \param[in] pVertexShaderBytecode    頂点シェーダの情報
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxInputLayout(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
        /*[in]*/ ID3DBlob* pVertexShaderBytecode);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxInputLayout() noexcept override;

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
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pInputLayout  レイアウトのポインタ
    /// </summary>
};
