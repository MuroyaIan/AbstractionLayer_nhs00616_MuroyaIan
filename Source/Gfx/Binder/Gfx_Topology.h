//==============================================================================
// Filename: Gfx_Topology.h
// Description: トポロジー処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

//***** トポロジー *****
class GfxTopology : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] type     トポロジータイプ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxTopology(
        /*[in]*/ D3D11_PRIMITIVE_TOPOLOGY type) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxTopology() noexcept override;

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
    D3D11_PRIMITIVE_TOPOLOGY m_type;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_type  トポロジータイプ
    /// </summary>
};
