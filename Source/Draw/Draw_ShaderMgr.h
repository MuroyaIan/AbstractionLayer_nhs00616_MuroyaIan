﻿//==============================================================================
// Filename: Draw_ShaderMgr.h
// Description: シェーダマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

//===== 前方宣言 =====
class GfxBinder;

//===== クラス定義 =====

//***** シェーダ管理 *****
class DrawShaderMgr
{
public:

    //バインダ一覧
    enum class BinderID
    {
        //頂点シェーダ
        VS_INSTANCE_PHONG,

        //入力レイアウト
        IL_INSTANCE_PHONG,

        //プリミティブトポロジー
        PT_LINE,
        PT_TRI,

        //サンプラー
        SAMPLER,

        //ピクセルシェーダ
        PS_PHONG,

        //定数バッファ
        CB_VS_MTX_VP,

        ID_MAX
    };

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    DrawShaderMgr(
        /*[in]*/ GfxMain& gfx);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~DrawShaderMgr() noexcept;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ BinderID id) const noexcept;

    //--------------------------------------------------------------------------
    /// バインド処理（インスタンシング＿Phongモデル）
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind_Instance_Phong() const noexcept;

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    GfxMain& m_dx;
    std::vector<std::unique_ptr<GfxBinder>> m_aBinder;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_dx        グラフィック処理の参照先
    /// m_aBinder   バインダのポインタ配列
    /// </summary>
};
