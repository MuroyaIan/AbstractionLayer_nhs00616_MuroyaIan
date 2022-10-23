﻿//==============================================================================
// Filename: Gfx_cbMatrix_VP.h
// Description: カメラ情報用定数バッファ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_ConstantBuffer.h>

//===== クラス定義 =====

//***** 変換行列バッファ（ビュー行列、投影行列） *****
class GfxCBuffMtxVP : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxCBuffMtxVP(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxCBuffMtxVP() noexcept override;

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
    /// ビュー情報登録
    ///
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void AddViewInfo(
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo) const noexcept;

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    static std::unique_ptr<GfxVertexCBuffer<GfxCbMtxVP>> m_pVcBuff;
    static int m_refCount;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pVcBuff   定数バッファのポインタ
    /// m_refCount  定数バッファの利用数
    /// </summary>
};
