//==============================================================================
// Filename: Gfx_VertexBuffer.h
// Description: 頂点バッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>

//===== クラス定義 =====

#if DX_11

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
        GfxBinder(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
    {
        //エラーハンドル
        HRESULT hr{};

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
        hr = GetDevice(gfx)->CreateBuffer(&bd, &sd,
            &m_pVertexBuffer[static_cast<int>(Type::VERTEX)]);
        ERROR_DX(hr);

        //その他初期化
        m_aStride[static_cast<int>(Type::VERTEX)] = static_cast<UINT>(sizeof(V));
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
        GfxBinder(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
    {
        //エラーハンドル
        HRESULT hr{};

        //バッファ作成（頂点情報）
        D3D11_BUFFER_DESC bd1{};
        bd1.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
        bd1.Usage = D3D11_USAGE_DEFAULT;
        bd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd1.CPUAccessFlags = 0u;
        bd1.MiscFlags = 0u;
        bd1.StructureByteStride = static_cast<UINT>(sizeof(V));
        D3D11_SUBRESOURCE_DATA sd1{};
        sd1.pSysMem = vertices.data();
        hr = GetDevice(gfx)->CreateBuffer(&bd1, &sd1,
            &m_pVertexBuffer[static_cast<int>(Type::VERTEX)]);
        ERROR_DX(hr);

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
        hr = GetDevice(gfx)->CreateBuffer(&bd2, nullptr,
            &m_pVertexBuffer[static_cast<int>(Type::INSTANCE)]);
        ERROR_DX(hr);

        //その他初期化
        m_aStride[static_cast<int>(Type::VERTEX)] = static_cast<UINT>(sizeof(V));
        m_aStride[static_cast<int>(Type::INSTANCE)] = static_cast<UINT>(sizeof(I));
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
        //例外処理
        if (m_pVertexBuffer[static_cast<int>(type)].Get() == nullptr)
            throw ERROR_EX2(S_OK, "バッファはnullです。");
        if (type == Type::VERTEX && !m_bDynamicBuffer)
            throw ERROR_EX2(S_OK, "頂点バッファは動的ではない!");

        //エラーハンドル
        HRESULT hr{};

        //更新処理
        D3D11_MAPPED_SUBRESOURCE msr{};
        hr = GetContext(gfx)->Map(
            m_pVertexBuffer[static_cast<int>(type)].Get(), 0u,
            D3D11_MAP_WRITE_DISCARD, 0u,
            &msr);                                          //GPUのアクセスをロック
        ERROR_DX(hr);
        memcpy(msr.pData, aData.data(),
            static_cast<UINT>(sizeof(D) * aData.size()));   //データ書込み
        GetContext(gfx)->Unmap(m_pVertexBuffer[static_cast<int>(type)].Get(), 0u);
                                                            //GPUのアクセスを解放
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
        //エラーハンドル
        HRESULT hr{};

        //インスタンスバッファ解放⇒新しく生成
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
        hr = GetDevice(gfx)->CreateBuffer(&bd2, nullptr,
            &m_pVertexBuffer[static_cast<int>(Type::INSTANCE)]);
        ERROR_DX(hr);

        //その他初期化
        m_aStride[static_cast<int>(Type::INSTANCE)] = static_cast<UINT>(sizeof(I));
    }

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
    UINT m_aStride[static_cast<int>(Type::MAX_TYPE)];
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer[static_cast<int>(Type::MAX_TYPE)];
    bool m_bDynamicBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_aStride           頂点バッファ配列の要素1個分のサイズ
    /// m_pVertexBuffer     バッファのポインタ
    /// m_bDynamicBuffer    頂点バッファはダイナミックかどうか
    /// </summary>
};

#elif DX_12


#endif // GFX
