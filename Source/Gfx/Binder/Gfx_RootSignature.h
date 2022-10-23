//==============================================================================
// Filename: Gfx_RootSignature.h
// Description: ルートシグネチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>
#include <Gfx/Binder/Gfx_HeapMgr.h>
#include <Gfx/Binder/Gfx_Sampler.h>

//===== クラス定義 =====

//***** テクスチャ *****
class GfxRootSignature : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] heapInfo     ヒープ登録用情報の参照先
    /// \param[in] Sampler      サンプラー情報の参照先
    /// \param[in] heapMgr      ヒープマネージャの参照先
    /// \param[in] ppRef        自身のダブルポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxRootSignature(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo& heapInfo,
        /*[in]*/ GfxSampler& sampler,
        /*[in]*/ GfxHeapMgr& heapMgr,
        /*[in]*/ GfxRootSignature** ppRef);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxRootSignature() noexcept override;

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override;    //バインド処理

    //--------------------------------------------------------------------------
    /// ルートシグネチャ取得
    ///
    /// \return ルートシグネチャのポインタ
    //--------------------------------------------------------------------------
    ID3D12RootSignature* GetRootSignature() const noexcept;

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;
    GfxHeapMgr& m_heapMgr;
    int m_startIndexPS;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pRootSignature    ルートシグネチャのポインタ
    /// m_heapMgr           ヒープマネージャの参照先
    /// m_startIndexPS      ルートパラメータのディスクリプタ開始位置(PS)
    /// </summary>
};
