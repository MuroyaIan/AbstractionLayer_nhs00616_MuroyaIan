﻿//==============================================================================
// Filename: Gfx_HeapMgr.h
// Description: ヒープマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>
#include <Tool/Tool_TexLoader.h>

//===== クラス定義 =====

//***** テクスチャ *****
class GfxHeapMgr : public GfxBinder
{
    //--------------------------------------------------------------------------
    friend class GfxTexture;

    template<typename C>
    friend class GfxConstantBuffer;
    //--------------------------------------------------------------------------

public:

    //ヒープ登録用情報
    struct HeapInfo
    {
        //CBV情報
        struct InfoCBV
        {
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            /// コンストラクタ
            ///
            /// \return void
            //------------------------------------------------------------------
            InfoCBV() noexcept : cbvd()
            {}

            //------------------------------------------------------------------
            /// デストラクタ
            ///
            /// \return void
            //------------------------------------------------------------------
            ~InfoCBV() noexcept {}

            //------------------------------------------------------------------

            //------------------------------------------------------------------
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd;
            //------------------------------------------------------------------

            /// <summary>
            /// cbvd    CBV用登録情報
            /// </summary>
        };

        //SRV情報
        struct InfoSRV
        {
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            /// コンストラクタ
            ///
            /// \return void
            //------------------------------------------------------------------
            InfoSRV() noexcept : pBuffer(nullptr), srvd()
            {}

            //------------------------------------------------------------------
            /// デストラクタ
            ///
            /// \return void
            //------------------------------------------------------------------
            ~InfoSRV() noexcept {}

            //------------------------------------------------------------------

            //------------------------------------------------------------------
            ID3D12Resource* pBuffer;
            D3D12_SHADER_RESOURCE_VIEW_DESC srvd;
            //------------------------------------------------------------------

            /// <summary>
            /// pBuffer     SRV用バッファポインタ
            /// srvd        SRV用登録情報
            /// </summary>
        };

        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        /// コンストラクタ
        ///
        /// \param[in] numCbvVS     VS用CBVの数
        /// \param[in] numSrvVS     VS用SRVの数
        /// \param[in] numCbvPS     PS用CBVの数
        /// \param[in] numSrvPS     PS用SRVの数
        ///
        /// \return void
        //----------------------------------------------------------------------
        HeapInfo(
            /*[in]*/ int numCbvVS,
            /*[in]*/ int numSrvVS,
            /*[in]*/ int numCbvPS,
            /*[in]*/ int numSrvPS) noexcept :
            slotCBV_VS(numCbvVS), slotSRV_VS(numSrvVS), slotCBV_PS(numCbvPS), slotSRV_PS(numSrvPS)
        {}

        //----------------------------------------------------------------------
        /// デストラクタ
        ///
        /// \return void
        //----------------------------------------------------------------------
        ~HeapInfo() noexcept {}

        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        std::vector<InfoCBV> slotCBV_VS;
        std::vector<InfoSRV> slotSRV_VS;
        std::vector<InfoCBV> slotCBV_PS;
        std::vector<InfoSRV> slotSRV_PS;
        //----------------------------------------------------------------------

        /// <summary>
        /// NumCBV_VS   VS用CBVのスロット情報
        /// NumSRV_VS   VS用SRVのスロット情報
        /// NumCBV_PS   PS用CBVのスロット情報
        /// NumSRV_PS   PS用SRVのスロット情報
        /// </summary>
    };

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] heapInfo     ヒープ登録用情報の参照先
    /// \param[in] pRef         自身のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxHeapMgr(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ HeapInfo& heapInfo,
        /*[in]*/ GfxHeapMgr* pRef);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxHeapMgr() noexcept override;

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

protected:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pBufferHeaps;
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_ahCPU;
    std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_ahGPU;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pBufferHeaps  バッファ用ヒープ
    /// m_ahCPU         ヒープハンドル(CPU)
    /// m_ahGPU         ヒープハンドル(GPU)
    /// </summary>
};
