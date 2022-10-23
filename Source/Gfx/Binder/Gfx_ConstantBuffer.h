//==============================================================================
// Filename: Gfx_ConstantBuffer.h
// Description: 定数バッファ処理（親クラス）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Binder.h>
#include <Gfx/Binder/Gfx_HeapMgr.h>

//===== 列挙型宣言 =====
enum class CB_SLOT_VS       //頂点シェーダ用定数バッファID
{
    CAMERA
};

enum class CB_SLOT_PS       //ピクセルシェーダ用定数バッファID
{
    LIGHT,
    MATERIAL
};

//===== 構造体宣言 =====
struct GfxCbColor           //カラー用定数バッファ
{
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxCbColor() noexcept
    {
        faceColor[0]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        faceColor[1]  = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
        faceColor[2]  = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
        faceColor[3]  = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
        faceColor[4]  = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
        faceColor[5]  = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
        faceColor[6]  = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
        faceColor[7]  = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
        faceColor[8]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
        faceColor[9]  = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
        faceColor[10] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
        faceColor[11] = DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
        faceColor[12] = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
        faceColor[13] = DirectX::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
        faceColor[14] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
        faceColor[15] = DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
        faceColor[16] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
        faceColor[17] = DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
        faceColor[18] = DirectX::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
        faceColor[19] = DirectX::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
        faceColor[20] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
        faceColor[21] = DirectX::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
        faceColor[22] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
        faceColor[23] = DirectX::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
    }

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxCbColor() noexcept
    {}

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4 faceColor[24];
    //--------------------------------------------------------------------------

    /// <summary>
    /// faceColor[24]   ポリゴン色（面情報）
    /// </summary>
};

struct GfxCbMtxVP  //変換行列用定数バッファ
{
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxCbMtxVP() noexcept : mtxView(), mtxProj()
    {
        DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
    }

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] mtxV  ビュー行列
    /// \param[in] mtxP  投影行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxCbMtxVP(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxV,
        /*[in]*/ DirectX::XMFLOAT4X4 mtxP) noexcept :
        mtxView(mtxV), mtxProj(mtxP)
    {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxCbMtxVP() noexcept
    {}

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 mtxView;
    DirectX::XMFLOAT4X4 mtxProj;
    //--------------------------------------------------------------------------

    /// <summary>
    /// mtxView     ビュー行列
    /// mtxProj     投影行列
    /// </summary>
};

//===== クラス定義 =====

//***** 定数バッファ *****
template<typename C>
class GfxConstantBuffer : public GfxBinder
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] consts       定数バッファ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotVS       入力スロット(VS)
    /// \param[in] slotPS       入力スロット(PS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxConstantBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotVS = -1,
        /*[in]*/ int slotPS = -1) :
    GfxBinder(), m_pConstantBuffer11(), m_pConstantBuffer12(), m_cbvd(),
    m_slotVS(slotVS), m_slotPS(slotPS)
    {
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                CreateBufferDX11(gfx, consts);
                break;
            case GfxMain::API_MODE::DX_12:
                CreateBufferDX12(gfx, consts, pheapInfo);
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// コンストラクタ（バッファ初期化なし）
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotVS       入力スロット(VS)
    /// \param[in] slotPS       入力スロット(PS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxConstantBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotVS = -1,
        /*[in]*/ int slotPS = -1) :
    GfxBinder(), m_pConstantBuffer11(), m_pConstantBuffer12(), m_cbvd(),
    m_slotVS(slotVS), m_slotPS(slotPS)
    {
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                CreateBufferDX11(gfx);
                break;
            case GfxMain::API_MODE::DX_12:
                CreateBufferDX12(gfx, pheapInfo);
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
    ~GfxConstantBuffer() noexcept override {}

    //--------------------------------------------------------------------------
    /// バッファ更新
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] consts   定数バッファ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Update(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts) const
    {
        switch (GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                UpdateBuffer(gfx, consts, m_pConstantBuffer11.Get());
                break;
            case GfxMain::API_MODE::DX_12:
                UpdateBuffer(consts, m_pConstantBuffer12.Get());
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// ビュー情報登録
    ///
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void AddViewInfo(
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo) const noexcept
    {
        //例外処理
        if (GetApiMode() != GfxMain::API_MODE::DX_12)
            return;
        if (pheapInfo == nullptr)
            return;

        //登録処理
        if (m_slotVS >= 0)
            pheapInfo->slotCBV_VS[m_slotVS].cbvd = m_cbvd;
        if (m_slotPS >= 0)
            pheapInfo->slotCBV_PS[m_slotPS].cbvd = m_cbvd;
    }

    //--------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// バッファ作成(DX11)
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] consts   定数バッファ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX11(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts)
    {
        //エラーハンドル
        HRESULT hr{};

        //バッファ作成
        D3D11_BUFFER_DESC cbd{};
        cbd.ByteWidth = static_cast<UINT>(sizeof(consts));
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.StructureByteStride = 0u;
        D3D11_SUBRESOURCE_DATA csd{};
        csd.pSysMem = &consts;
        hr = GetDevice<DevDX11*>(&gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer11);
        ERROR_DX(hr);
    }

    //--------------------------------------------------------------------------
    /// 初期化なしバッファ作成(DX11)
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX11(
        /*[in]*/ GfxMain& gfx)
    {
        //エラーハンドル
        HRESULT hr{};

        //バッファ作成
        D3D11_BUFFER_DESC cbd{};
        cbd.ByteWidth = static_cast<UINT>(sizeof(C));
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.StructureByteStride = 0u;
        hr = GetDevice<DevDX11*>(&gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer11);
        ERROR_DX(hr);
    }

    //--------------------------------------------------------------------------
    /// バッファ作成(DX12)
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] consts       定数バッファ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX12(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo)
    {
        //エラーハンドル
        HRESULT hr{};

        //定数バッファ設定
        D3D12_HEAP_PROPERTIES hp{};                                 //頂点ヒープ
        hp.Type = D3D12_HEAP_TYPE_UPLOAD;                           //ヒープ種別
        hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;       //CPUのページング設定
        hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;        //メモリプール設定(CPU)
        hp.CreationNodeMask = 0u;
        hp.VisibleNodeMask = 0u;                                    //アダプター関連
        D3D12_RESOURCE_DESC rd{};                                   //リソース設定
        rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;             //バッファかテクスチャか
        rd.Alignment = 0;                                           //アラインメント
        rd.Width = ALIGN256(sizeof(consts));                        //データサイズ
        rd.Height = 1u;
        rd.DepthOrArraySize = 1;                                    //配列要素数
        rd.MipLevels = 1;                                           //ミップマップ設定
        rd.Format = DXGI_FORMAT_UNKNOWN;                            //テクスチャフォーマット
        rd.SampleDesc.Count = 1u;
        rd.SampleDesc.Quality = 0u;                                 //マルチサンプリング設定
        rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rd.Flags = D3D12_RESOURCE_FLAG_NONE;                        //(?)

        //定数バッファ作成
        hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(
            &hp, D3D12_HEAP_FLAG_NONE,                              //指定なし
            &rd, D3D12_RESOURCE_STATE_GENERIC_READ,                 //読み取り専用
            nullptr, IID_PPV_ARGS(&m_pConstantBuffer12));
        ERROR_DX(hr);

        //データマップ
        UpdateBuffer(consts, m_pConstantBuffer12.Get());

        //CBV情報作成
        m_cbvd.BufferLocation = m_pConstantBuffer12->GetGPUVirtualAddress();  //バッファアドレス
        m_cbvd.SizeInBytes = static_cast<UINT>(rd.Width);                     //バッファサイズ

        //CBV情報登録
        AddViewInfo(pheapInfo);
    }

    //--------------------------------------------------------------------------
    /// 初期化なしバッファ作成(DX12)
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CreateBufferDX12(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo)
    {
        //エラーハンドル
        HRESULT hr{};

        //定数バッファ設定
        D3D12_HEAP_PROPERTIES hp{};                                 //頂点ヒープ
        hp.Type = D3D12_HEAP_TYPE_UPLOAD;                           //ヒープ種別
        hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;       //CPUのページング設定
        hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;        //メモリプール設定(CPU)
        hp.CreationNodeMask = 0u;
        hp.VisibleNodeMask = 0u;                                    //アダプター関連
        D3D12_RESOURCE_DESC rd{};                                   //リソース設定
        rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;             //バッファかテクスチャか
        rd.Alignment = 0;                                           //アラインメント
        rd.Width = ALIGN256(sizeof(C));                             //データサイズ
        rd.Height = 1u;
        rd.DepthOrArraySize = 1;                                    //配列要素数
        rd.MipLevels = 1;                                           //ミップマップ設定
        rd.Format = DXGI_FORMAT_UNKNOWN;                            //テクスチャフォーマット
        rd.SampleDesc.Count = 1u;
        rd.SampleDesc.Quality = 0u;                                 //マルチサンプリング設定
        rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        rd.Flags = D3D12_RESOURCE_FLAG_NONE;                        //(?)

        //定数バッファ作成
        hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(
            &hp, D3D12_HEAP_FLAG_NONE,                              //指定なし
            &rd, D3D12_RESOURCE_STATE_GENERIC_READ,                 //読み取り専用
            nullptr, IID_PPV_ARGS(&m_pConstantBuffer12));
        ERROR_DX(hr);

        //CBV情報作成
        m_cbvd.BufferLocation = m_pConstantBuffer12->GetGPUVirtualAddress();  //バッファアドレス
        m_cbvd.SizeInBytes = static_cast<UINT>(rd.Width);                     //バッファサイズ

        //CBV情報登録
        AddViewInfo(pheapInfo);
    }

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer11;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pConstantBuffer12;
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_cbvd;
    int m_slotVS;
    int m_slotPS;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pConstantBuffer11     バッファのポインタ
    /// m_pConstantBuffer12     バッファのポインタ
    /// m_cbvd                  CBV情報
    /// m_slotVS                入力スロット(VS)
    /// m_slotPS                入力スロット(PS)
    /// </summary>
};

//***** 定数バッファ（頂点シェーダ用） *****
template<typename C>
class GfxVertexCBuffer : public GfxConstantBuffer<C>
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] consts       定数バッファ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotVS       入力スロット(VS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxVertexCBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotVS = -1) :
        CBuff::GfxConstantBuffer(gfx, consts, pheapInfo, slotVS) {}

    //--------------------------------------------------------------------------
    /// コンストラクタ（バッファ初期化なし）
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotVS       入力スロット(VS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxVertexCBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotVS = -1) :
        CBuff::GfxConstantBuffer(gfx, pheapInfo, slotVS) {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxVertexCBuffer() noexcept override {}

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override
    {
        switch (GfxBinder::GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                if (CBuff::m_slotVS >= 0)
                    GfxBinder::GetCommand<GfxBinder::CmdDX11*>(&gfx)->VSSetConstantBuffers(CBuff::m_slotVS, 1u, CBuff::m_pConstantBuffer11.GetAddressOf());
                break;
            case GfxMain::API_MODE::DX_12:
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// ビュー情報登録
    ///
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void AddViewInfo(
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo) const noexcept
    {
        CBuff::AddViewInfo(pheapInfo);
    }

    //--------------------------------------------------------------------------

private:

    //テンプレート対策
    using CBuff = GfxConstantBuffer<C>;
};

//***** 定数バッファ（ピクセルシェーダ用） *****
template<typename C>
class GfxPixelCBuffer : public GfxConstantBuffer<C>
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] consts       定数バッファ
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotPS       入力スロット(PS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxPixelCBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ const C& consts,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotPS = -1) :
        CBuff::GfxConstantBuffer(gfx, consts, pheapInfo, -1, slotPS) {}

    //--------------------------------------------------------------------------
    /// コンストラクタ（バッファ初期化なし）
    ///
    /// \param[in] gfx          グラフィック処理の参照先
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    /// \param[in] slotPS       入力スロット(PS)
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxPixelCBuffer(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo,
        /*[in]*/ int slotPS = -1) :
        CBuff::GfxConstantBuffer(gfx, pheapInfo, -1, slotPS) {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxPixelCBuffer() noexcept override {}

    //--------------------------------------------------------------------------
    /// バインド処理
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Bind(
        /*[in]*/ GfxMain& gfx) noexcept override
    {
        switch (GfxBinder::GetApiMode()) {
            case GfxMain::API_MODE::DX_11:
                if (CBuff::m_slotPS >= 0)
                    GfxBinder::GetCommand<GfxBinder::CmdDX11*>(&gfx)->PSSetConstantBuffers(CBuff::m_slotPS, 1u, CBuff::m_pConstantBuffer11.GetAddressOf());
                break;
            case GfxMain::API_MODE::DX_12:
                break;
            default:
                break;
        }
    }

    //--------------------------------------------------------------------------
    /// ビュー情報登録
    ///
    /// \param[in] pheapInfo    ヒープ登録用情報のポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void AddViewInfo(
        /*[in]*/ GfxHeapMgr::HeapInfo* pheapInfo) const noexcept
    {
        CBuff::AddViewInfo(pheapInfo);
    }

    //--------------------------------------------------------------------------

private:

    //テンプレート対策
    using CBuff = GfxConstantBuffer<C>;
};
