////==============================================================================
//// Filename: Gfx_ConstantBuffer.h
//// Description: 定数バッファ処理（親クラス）
//// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
////==============================================================================
//
//#pragma once
//
////===== インクルード部 =====
//#include <Gfx/Binder/Gfx_Binder.h>
//
////===== 列挙型宣言 =====
//enum class CB_SLOT_VS       //頂点シェーダ用定数バッファID
//{
//    DEFAULT,
//    CAMERA,
//};
//
//enum class CB_SLOT_PS       //ピクセルシェーダ用定数バッファID
//{
//    DEFAULT,
//    LIGHT,
//    MATERIAL
//};
//
////===== 構造体宣言 =====
//struct GfxCbColor              //カラー用定数バッファ
//{
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxCbColor() noexcept
//    {
//        faceColor[0]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
//        faceColor[1]  = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
//        faceColor[2]  = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
//        faceColor[3]  = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
//        faceColor[4]  = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
//        faceColor[5]  = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
//        faceColor[6]  = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
//        faceColor[7]  = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
//        faceColor[8]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
//        faceColor[9]  = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
//        faceColor[10] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
//        faceColor[11] = DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
//        faceColor[12] = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
//        faceColor[13] = DirectX::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
//        faceColor[14] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
//        faceColor[15] = DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
//        faceColor[16] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
//        faceColor[17] = DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
//        faceColor[18] = DirectX::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
//        faceColor[19] = DirectX::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
//        faceColor[20] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
//        faceColor[21] = DirectX::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
//        faceColor[22] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
//        faceColor[23] = DirectX::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
//    }
//
//    //--------------------------------------------------------------------------
//    /// デストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    ~GfxCbColor() noexcept
//    {}
//
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    DirectX::XMFLOAT4 faceColor[24];
//    //--------------------------------------------------------------------------
//
//    /// <summary>
//    /// faceColor[24]   ポリゴン色（面情報）
//    /// </summary>
//};
//
//struct GfxCbMtxVP  //変換行列用定数バッファ
//{
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxCbMtxVP() noexcept : mtxView(), mtxProj()
//    {
//        DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
//        DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
//    }
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \param[in] mtxV  ビュー行列
//    /// \param[in] mtxP  投影行列
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxCbMtxVP(
//        /*[in]*/ DirectX::XMFLOAT4X4 mtxV,
//        /*[in]*/ DirectX::XMFLOAT4X4 mtxP) noexcept :
//        mtxView(mtxV), mtxProj(mtxP)
//    {}
//
//    //--------------------------------------------------------------------------
//    /// デストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    ~GfxCbMtxVP() noexcept
//    {}
//
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    DirectX::XMFLOAT4X4 mtxView;
//    DirectX::XMFLOAT4X4 mtxProj;
//    //--------------------------------------------------------------------------
//
//    /// <summary>
//    /// mtxView     ビュー行列
//    /// mtxProj     投影行列
//    /// </summary>
//};
//
////===== クラス定義 =====
//
////***** 定数バッファ *****
//template<typename C>
//class GfxConstantBuffer : public GfxBinder
//{
//public:
//
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] consts   定数バッファ
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxConstantBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ const C& consts,
//        /*[in]*/ UINT slot = 0u) :
//        GfxBinder(), m_pConstantBuffer(), m_slot(slot)
//    {
//        //エラーハンドル
//        HRESULT hr{};
//
//        //バッファ作成
//        D3D11_BUFFER_DESC cbd{};
//        cbd.ByteWidth = static_cast<UINT>(sizeof(consts));
//        cbd.Usage = D3D11_USAGE_DYNAMIC;
//        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//        cbd.MiscFlags = 0u;
//        cbd.StructureByteStride = 0u;
//        D3D11_SUBRESOURCE_DATA csd{};
//        csd.pSysMem = &consts;
//        hr = GetDevice(gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);
//        ERROR_DX(hr);
//    }
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ（バッファ初期化なし）
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxConstantBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ UINT slot = 0u) :
//        GfxBinder(), m_pConstantBuffer(), m_slot(slot)
//    {
//        //エラーハンドル
//        HRESULT hr{};
//
//        //バッファ作成
//        D3D11_BUFFER_DESC cbd{};
//        cbd.ByteWidth = static_cast<UINT>(sizeof(C));
//        cbd.Usage = D3D11_USAGE_DYNAMIC;
//        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//        cbd.MiscFlags = 0u;
//        cbd.StructureByteStride = 0u;
//        hr = GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
//        ERROR_DX(hr);
//    }
//
//    //--------------------------------------------------------------------------
//    /// デストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    ~GfxConstantBuffer() noexcept override {}
//
//    //--------------------------------------------------------------------------
//    /// バッファ更新
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] consts   定数バッファ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    void Update(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ const C& consts) const
//    {
//        //エラーハンドル
//        HRESULT hr{};
//
//        //更新処理
//        D3D11_MAPPED_SUBRESOURCE msr{};
//        hr = GetContext(gfx)->Map(
//            m_pConstantBuffer.Get(), 0u,
//            D3D11_MAP_WRITE_DISCARD, 0u,
//            &msr);                                              //GPUのアクセスをロック
//        ERROR_DX(hr);
//        memcpy(msr.pData, &consts, sizeof(consts));             //データ書込み
//        GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);    //GPUのアクセスを解放
//    }
//
//    //--------------------------------------------------------------------------
//
//protected:
//
//    //--------------------------------------------------------------------------
//    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
//    UINT m_slot;
//    //--------------------------------------------------------------------------
//
//    /// <summary>
//    /// m_pConstantBuffer   ポインタ
//    /// m_slot              入力スロット
//    /// </summary>
//};
//
////***** 定数バッファ（頂点シェーダ用） *****
//template<typename C>
//class GfxVertexCBuffer : public GfxConstantBuffer<C>
//{
//public:
//
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] consts   定数バッファ
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxVertexCBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ const C& consts,
//        /*[in]*/ UINT slot = 0u) : CBuff::GfxConstantBuffer(gfx, consts, slot) {}
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ（バッファ初期化なし）
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxVertexCBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ UINT slot = 0u) : CBuff::GfxConstantBuffer(gfx, slot) {}
//
//    //--------------------------------------------------------------------------
//    /// デストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    ~GfxVertexCBuffer() noexcept override {}
//
//    //--------------------------------------------------------------------------
//    /// バインド処理
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    void Bind(
//        /*[in]*/ GfxMain& gfx) noexcept override
//    {
//        GfxBinder::GetContext(gfx)->VSSetConstantBuffers(CBuff::m_slot, 1u, CBuff::m_pConstantBuffer.GetAddressOf());
//    }
//
//    //--------------------------------------------------------------------------
//
//private:
//
//    //テンプレート対策
//    using CBuff = GfxConstantBuffer<C>;
//};
//
////***** 定数バッファ（ピクセルシェーダ用） *****
//template<typename C>
//class GfxPixelCBuffer : public GfxConstantBuffer<C>
//{
//public:
//
//    //--------------------------------------------------------------------------
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] consts   定数バッファ
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxPixelCBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ const C& consts,
//        /*[in]*/ UINT slot = 0u) : CBuff::GfxConstantBuffer(gfx, consts, slot) {}
//
//    //--------------------------------------------------------------------------
//    /// コンストラクタ（バッファ初期化なし）
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    /// \param[in] slot     入力スロット
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    GfxPixelCBuffer(
//        /*[in]*/ GfxMain& gfx,
//        /*[in]*/ UINT slot = 0u) : CBuff::GfxConstantBuffer(gfx, slot) {}
//
//    //--------------------------------------------------------------------------
//    /// デストラクタ
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    ~GfxPixelCBuffer() noexcept override {}
//
//    //--------------------------------------------------------------------------
//    /// バインド処理
//    ///
//    /// \param[in] gfx      グラフィック処理の参照先
//    ///
//    /// \return void
//    //--------------------------------------------------------------------------
//    void Bind(
//        /*[in]*/ GfxMain& gfx) noexcept override
//    {
//        GfxBinder::GetContext(gfx)->PSSetConstantBuffers(CBuff::m_slot, 1u, CBuff::m_pConstantBuffer.GetAddressOf());
//    }
//
//    //--------------------------------------------------------------------------
//
//private:
//
//    //テンプレート対策
//    using CBuff = GfxConstantBuffer<C>;
//};
