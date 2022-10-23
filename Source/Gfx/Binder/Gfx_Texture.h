//==============================================================================
// Filename: Gfx_Texture.h
// Description: テクスチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>
#include <Tool/Tool_TexLoader.h>
#include <Gfx/Binder/Gfx_HeapMgr.h>

//===== クラス定義 =====

//***** テクスチャ *****
class GfxTexture : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] data         テクスチャのデータ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotVS       入力スロット(VS)
    /// \param[in] slotPS       入力スロット(PS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxTexture(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ ToolTexLoader::TexData& data,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotVS = -1,
        /*[in]*/ int slotPS = -1);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxTexture() noexcept override;

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

protected:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// バッファ作成(DX11)
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    /// \param[in] data     テクスチャのデータ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ ToolTexLoader::TexData& data);

    //--------------------------------------------------------------------------
    /// バッファ作成(DX12)
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] data         テクスチャのデータ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX12(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ ToolTexLoader::TexData& data,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pTextureBuffer;
    D3D12_SHADER_RESOURCE_VIEW_DESC m_srvd;
    int m_slotVS;
    int m_slotPS;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pTextureView      SRVのポインタ
    /// m_pTextureBuffer    テクスチャバッファのポインタ
    /// m_srvd              SRV情報
    /// m_slotVS            入力スロット(VS)
    /// m_slotPS            入力スロット(PS)
    /// </summary>
};
