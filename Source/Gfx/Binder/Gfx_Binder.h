//==============================================================================
// Filename: Gfx_Binder.h
// Description: パイプライン用バインド処理（親クラス）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_dx11.h>
#include <Gfx/Gfx_dx12.h>

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
    /// Apiモード取得
    ///
    /// \return GfxApiモードを取得
    //--------------------------------------------------------------------------
    static const GfxMain::API_MODE& GetApiMode() noexcept
    {
        return GfxMain::m_api;
    }

    //--------------------------------------------------------------------------
    /// デバイス取得
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return GFXデバイスのポインタ
    //--------------------------------------------------------------------------
    template <typename T>
    T GetDevice(
        /*[in]*/ GfxMain* gfx) const noexcept {}

    template <>
    ID3D11Device* GetDevice<ID3D11Device*>(
        /*[in]*/ GfxMain* gfx) const noexcept
    {
        return dynamic_cast<GfxDX11*>(gfx)->m_pDevice.Get();
    }

    template <>
    ID3D12Device* GetDevice<ID3D12Device*>(
        /*[in]*/ GfxMain* gfx) const noexcept
    {
        return dynamic_cast<GfxDX12*>(gfx)->m_pDevice.Get();
    }

    //--------------------------------------------------------------------------
    /// コマンドオブジェクト取得
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return GFXコマンドオブジェクトのポインタ
    //--------------------------------------------------------------------------
    template <typename T>
    T GetCommand(
        /*[in]*/ GfxMain* gfx) const noexcept {}

    template <>
    ID3D11DeviceContext* GetCommand<ID3D11DeviceContext*>(
        /*[in]*/ GfxMain* gfx) const noexcept
    {
        return dynamic_cast<GfxDX11*>(gfx)->m_pContext.Get();
    }

    template <>
    ID3D12GraphicsCommandList* GetCommand<ID3D12GraphicsCommandList*>(
        /*[in]*/ GfxMain* gfx) const noexcept
    {
        return dynamic_cast<GfxDX12*>(gfx)->m_pCmdList.Get();
    }

    //--------------------------------------------------------------------------
    /// バッファ配列更新(dx11)
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] aData    バッファ用データ配列
    /// \param[in] pBuffer  バッファのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template <typename D>
    void UpdateBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<D>& aData,
        /*[in]*/ ID3D11Resource* pBuffer) const
    {
        //エラーハンドル
        HRESULT hr{};

        //更新処理
        D3D11_MAPPED_SUBRESOURCE msr{};
        hr = GetCommand<CmdDX11*>(&gfx)->Map(
            pBuffer, 0u,
            D3D11_MAP_WRITE_DISCARD, 0u,
            &msr);                                          //GPUのアクセスをロック
        ERROR_DX(hr);
        memcpy(msr.pData, aData.data(),
            static_cast<UINT>(sizeof(D) * aData.size()));   //データ書込み
        GetCommand<CmdDX11*>(&gfx)->Unmap(pBuffer, 0u);     //GPUのアクセスを解放
    }

    //--------------------------------------------------------------------------
    /// バッファ更新(dx11)
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] data     バッファ用データ
    /// \param[in] pBuffer  バッファのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template <typename D>
    void UpdateBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const D& data,
        /*[in]*/ ID3D11Resource* pBuffer) const
    {
        //エラーハンドル
        HRESULT hr{};

        //更新処理
        D3D11_MAPPED_SUBRESOURCE msr{};
        hr = GetCommand<CmdDX11*>(&gfx)->Map(
            pBuffer, 0u,
            D3D11_MAP_WRITE_DISCARD, 0u,
            &msr);                                          //GPUのアクセスをロック
        ERROR_DX(hr);
        memcpy(msr.pData, &data, sizeof(data));             //データ書込み
        GetCommand<CmdDX11*>(&gfx)->Unmap(pBuffer, 0u);     //GPUのアクセスを解放
    }

    //--------------------------------------------------------------------------
    /// バッファ配列更新(dx12)
    ///
    /// \param[in] aData    バッファ用データ配列
    /// \param[in] pBuffer  バッファのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template <typename D>
    void UpdateBuffer(
        /*[in]*/ const std::vector<D>& aData,
        /*[in]*/ ID3D12Resource* pBuffer) const
    {
        //エラーハンドル
        HRESULT hr{};

        //データマップ
        D* pMap = nullptr;
        hr = pBuffer->Map(0u,   //ミップマップ設定
            nullptr,            //範囲指定⇒nullptr(全範囲)
            (void**)&pMap);
        ERROR_DX(hr);
        std::copy(std::begin(aData), std::end(aData), pMap);
        pBuffer->Unmap(0u, nullptr);
    }

    //--------------------------------------------------------------------------
    /// バッファ更新(dx12)
    ///
    /// \param[in] data     バッファ用データ
    /// \param[in] pBuffer  バッファのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template <typename D>
    void UpdateBuffer(
        /*[in]*/ const D& data,
        /*[in]*/ ID3D12Resource* pBuffer) const
    {
        //エラーハンドル
        HRESULT hr{};

        //データマップ
        D* pMap = nullptr;
        hr = pBuffer->Map(0u,   //ミップマップ設定
            nullptr,            //範囲指定⇒nullptr(全範囲)
            (void**)&pMap);
        ERROR_DX(hr);
        *pMap = data;
        pBuffer->Unmap(0u, nullptr);
    }

    //--------------------------------------------------------------------------

    //型省略
    using DevDX11 = ID3D11Device;
    using DevDX12 = ID3D12Device;
    using CmdDX11 = ID3D11DeviceContext;
    using CmdDX12 = ID3D12GraphicsCommandList;
};
