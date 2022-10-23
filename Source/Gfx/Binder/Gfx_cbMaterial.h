﻿//==============================================================================
// Filename: Gfx_cbMaterial.h
// Description: マテリアル情報用定数バッファ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_ConstantBuffer.h>

//===== 前方宣言 =====
struct GfxMaterialData;

//===== クラス定義 =====

//***** マテリアルバッファ *****
class GfxCBuffMaterial : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] material     マテリアル情報の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxCBuffMaterial(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ const GfxMaterialData& material);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxCBuffMaterial() noexcept override;

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

private:

    //--------------------------------------------------------------------------
    static std::unique_ptr<GfxPixelCBuffer<GfxMaterialData>> m_pPcBuff;
    static int m_refCount;
    const GfxMaterialData& m_material;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pPcBuff   定数バッファのポインタ
    /// m_refCount  定数バッファの利用数
    /// m_material  マテリアル情報の提供先
    /// </summary>
};
