//==============================================================================
// Filename: Gfx_VertexBuffer.h
// Description: 頂点バッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

//***** 頂点バッファ *****
class GfxVertexBuffer : public GfxBinder
{
public:

    //バッファタイプ
    enum class Type
    {
        VERTEX,     //頂点情報
        INSTANCE,   //インスタンス情報

        MAX_TYPE
    };

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] vertices     頂点バッファ用データ
    /// \param[in] bDynamic     頂点バッファはダイナミックかどうか
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<class V>
    GfxVertexBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<V>& vertices,
        /*[in]*/ bool bDynamic = false) :
        GfxBinder(), m_aStride(), m_pVertexBuffer11(), m_pVertexBuffer12(), m_viewVB(), m_bDynamicBuffer(bDynamic)
    {
        //バッファ作成
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                CreateVertexBufferDX11(gfx, vertices);
                break;
            case GfxMain::API_MODE::DX_12:
                CreateVertexBufferDX12(gfx, vertices);
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// コンストラクタ（インスタンシングあり）
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] vertices     頂点バッファ用データ
    /// \param[in] instances    インスタンシングバッファ用データ
    /// \param[in] bDynamic     頂点バッファはダイナミックかどうか
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<class V, class I>
    GfxVertexBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<V>& vertices,
        /*[in]*/ const std::vector<I>& instances,
        /*[in]*/ bool bDynamic = false) :
        GfxBinder(), m_aStride(), m_pVertexBuffer11(), m_pVertexBuffer12(), m_viewVB(), m_bDynamicBuffer(bDynamic)
    {
        //バッファ作成
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                CreateVertexBufferDX11(gfx, vertices);
                CreateInstanceBufferDX11(gfx, instances);
                break;
            case GfxMain::API_MODE::DX_12:
                CreateVertexBufferDX12(gfx, vertices);
                CreateInstanceBufferDX12(gfx, instances);
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxVertexBuffer() noexcept override;

    //--------------------------------------------------------------------------
    /// バッファ更新
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] aData    バッファ用データ
    /// \param[in] type     バッファタイプ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<typename D>
    void UpdateBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<D>& aData,
        /*[in]*/ Type type) const
    {
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                UpdateBufferDX11(gfx, aData, type);
                break;
            case GfxMain::API_MODE::DX_12:
                UpdateBufferDX12(aData, type);
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// インスタンスバッファ再設定
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] instances    インスタンシングバッファ用データ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<typename I>
    void ResetInstanceBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<I>& instances)
    {
        //バッファ作成
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                CreateInstanceBufferDX11(gfx, instances);
                break;
            case GfxMain::API_MODE::DX_12:
                CreateInstanceBufferDX12(gfx, instances);
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx)  noexcept override;

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// 頂点バッファ作成
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] vertices     頂点バッファ用データ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<class V>
    void CreateVertexBufferDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<V>& vertices)
    {
        //エラーハンドル
        HRESULT hr{};
        int bufferIdx = static_cast<int>(Type::VERTEX);

        //バッファ作成
        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
        if (!m_bDynamicBuffer)
            bd.Usage = D3D11_USAGE_DEFAULT;
        else
            bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        if (!m_bDynamicBuffer)
            bd.CPUAccessFlags = 0u;
        else
            bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0u;
        bd.StructureByteStride = static_cast<UINT>(sizeof(V));
        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertices.data();
        hr = GetDevice<DevDX11*>(&gfx)->CreateBuffer(&bd, &sd,
            &m_pVertexBuffer11[bufferIdx]);
        ERROR_DX(hr);

        //その他初期化
        m_aStride[bufferIdx] = static_cast<UINT>(sizeof(V));
    }

    template<class V>
    void CreateVertexBufferDX12(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<V>& vertices)
    {
        //エラーハンドル
        HRESULT hr{};
        int bufferIdx = static_cast<int>(Type::VERTEX);

        //頂点バッファ設定
        D3D12_HEAP_PROPERTIES hp{};                             //頂点ヒープ
        hp.Type = D3D12_HEAP_TYPE_UPLOAD;                       //ヒープ種別
        hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;   //CPUのページング設定
        hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;    //メモリプール設定
        hp.CreationNodeMask = 0u;
        hp.VisibleNodeMask = 0u;                                //アダプター関連
        D3D12_RESOURCE_DESC rd{};                               //リソース設定
        rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;         //バッファかテクスチャか
        rd.Alignment = 0;                                       //アラインメント
        rd.Width = sizeof(V) * vertices.size();                 //データサイズ
        rd.Height = 1u;                                         //テクスチャじゃないので⇒1
        rd.DepthOrArraySize = 1;                                //配列要素数
        rd.MipLevels = 1;                                       //ミップマップ設定
        rd.Format = DXGI_FORMAT_UNKNOWN;                        //テクスチャフォーマット
        rd.SampleDesc.Count = 1u;
        rd.SampleDesc.Quality = 0u;                             //マルチサンプリング設定
        rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;             //テクスチャの場合⇒D3D12_TEXTURE_LAYOUT_UNKNOWN
        rd.Flags = D3D12_RESOURCE_FLAG_NONE;                    //(?)

        //頂点バッファ作成
        hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE,  //指定なし
            &rd, D3D12_RESOURCE_STATE_GENERIC_READ,                                         //読み取り専用
            nullptr, IID_PPV_ARGS(&m_pVertexBuffer12[bufferIdx]));
        ERROR_DX(hr);

        //データマップ
        UpdateBufferDX12(vertices, Type::VERTEX);

        //VBV作成
        m_viewVB[bufferIdx].BufferLocation = m_pVertexBuffer12[bufferIdx]->GetGPUVirtualAddress();
        m_viewVB[bufferIdx].SizeInBytes = static_cast<UINT>(rd.Width);
        m_viewVB[bufferIdx].StrideInBytes = sizeof(V);
    }

    //--------------------------------------------------------------------------
    /// インスタンスバッファ作成
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] instances    インスタンスバッファ用データ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<class I>
    void CreateInstanceBufferDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<I>& instances)
    {
        //エラーハンドル
        HRESULT hr{};
        int bufferIdx = static_cast<int>(Type::INSTANCE);

        //バッファ作成（インスタンス情報）
        int size{};
        if (instances.size() == 0)
            size = 1;
        else
            size = static_cast<int>(instances.size());
        D3D11_BUFFER_DESC bd2{};
        bd2.ByteWidth = static_cast<UINT>(sizeof(I) * size);
        bd2.Usage = D3D11_USAGE_DYNAMIC;
        bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd2.MiscFlags = 0u;
        bd2.StructureByteStride = static_cast<UINT>(sizeof(I));
        hr = GetDevice<DevDX11*>(&gfx)->CreateBuffer(&bd2, nullptr,
            &m_pVertexBuffer11[bufferIdx]);
        ERROR_DX(hr);

        //その他初期化
        m_aStride[bufferIdx] = static_cast<UINT>(sizeof(I));
    }

    template<class I>
    void CreateInstanceBufferDX12(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<I>& instances)
    {
        //エラーハンドル
        HRESULT hr{};
        int bufferIdx = static_cast<int>(Type::INSTANCE);

        //頂点バッファ設定
        D3D12_HEAP_PROPERTIES hp2{};                            //頂点ヒープ
        hp2.Type = D3D12_HEAP_TYPE_UPLOAD;                      //ヒープ種別
        hp2.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;  //CPUのページング設定
        hp2.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;   //メモリプール設定
        hp2.CreationNodeMask = 0u;
        hp2.VisibleNodeMask = 0u;                               //アダプター関連
        D3D12_RESOURCE_DESC rd2{};                              //リソース設定
        rd2.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;        //バッファかテクスチャか
        rd2.Alignment = 0;                                      //アラインメント
        rd2.Width = sizeof(I) * instances.size();                //データサイズ
        rd2.Height = 1u;                                        //テクスチャじゃないので⇒1
        rd2.DepthOrArraySize = 1;                               //配列要素数
        rd2.MipLevels = 1;                                      //ミップマップ設定
        rd2.Format = DXGI_FORMAT_UNKNOWN;                       //テクスチャフォーマット
        rd2.SampleDesc.Count = 1u;
        rd2.SampleDesc.Quality = 0u;                            //マルチサンプリング設定
        rd2.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;            //テクスチャの場合⇒D3D12_TEXTURE_LAYOUT_UNKNOWN
        rd2.Flags = D3D12_RESOURCE_FLAG_NONE;                   //(?)

        //頂点バッファ作成
        hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(&hp2, D3D12_HEAP_FLAG_NONE,     //指定なし
            &rd2, D3D12_RESOURCE_STATE_GENERIC_READ,                                            //読み取り専用
            nullptr, IID_PPV_ARGS(&m_pVertexBuffer12[bufferIdx]));
        ERROR_DX(hr);

        //データマップ
        UpdateBufferDX12(instances, Type::INSTANCE);

        //VBV作成
        m_viewVB[bufferIdx].BufferLocation = m_pVertexBuffer12[bufferIdx]->GetGPUVirtualAddress();
        m_viewVB[bufferIdx].SizeInBytes = static_cast<UINT>(rd2.Width);
        m_viewVB[bufferIdx].StrideInBytes = sizeof(I);
    }

    //--------------------------------------------------------------------------
    /// バッファ更新(dx11)
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] aData    バッファ用データ
    /// \param[in] type     バッファタイプ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<typename D>
    void UpdateBufferDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const std::vector<D>& aData,
        /*[in]*/ Type type) const
    {
        //例外処理
        if (m_pVertexBuffer11[static_cast<int>(type)].Get() == nullptr)
            throw ERROR_EX2(S_OK, "バッファはnullです。");
        if (type == Type::VERTEX && !m_bDynamicBuffer)
            throw ERROR_EX2(S_OK, "頂点バッファは動的ではない!");

        //データ更新
        GfxBinder::UpdateBuffer(gfx, aData, m_pVertexBuffer11[static_cast<int>(type)].Get());
    }

    //--------------------------------------------------------------------------
    /// バッファ更新(dx12)
    ///
    /// \param[in] aData    バッファ用データ
    /// \param[in] type     バッファタイプ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<typename D>
    void UpdateBufferDX12(
        /*[in]*/ const std::vector<D>& aData,
        /*[in]*/ Type type) const
    {
        //例外処理
        if (m_pVertexBuffer12[static_cast<int>(type)].Get() == nullptr)
            throw ERROR_EX2(S_OK, "バッファはnullです。");
        if (type == Type::VERTEX && !m_bDynamicBuffer)
            throw ERROR_EX2(S_OK, "頂点バッファは動的ではない!");

        //データ更新
        GfxBinder::UpdateBuffer(aData, m_pVertexBuffer12[static_cast<int>(type)].Get());
    }

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx  グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BindDX11(
        /*[in]*/ GfxMain& gfx) const noexcept;

    void BindDX12(
        /*[in]*/ GfxMain& gfx) const noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    UINT m_aStride[static_cast<int>(Type::MAX_TYPE)];
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer11[static_cast<int>(Type::MAX_TYPE)];
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBuffer12[static_cast<int>(Type::MAX_TYPE)];
    D3D12_VERTEX_BUFFER_VIEW m_viewVB[static_cast<int>(Type::MAX_TYPE)];
    bool m_bDynamicBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_aStride           頂点バッファ配列の要素1個分のサイズ
    /// m_pVertexBuffer11   バッファのポインタ(dx11)
    /// m_pVertexBuffer12   バッファのポインタ(dx12)
    /// m_viewVB            頂点バッファビュー
    /// m_bDynamicBuffer    頂点バッファはダイナミックかどうか
    /// </summary>
};
