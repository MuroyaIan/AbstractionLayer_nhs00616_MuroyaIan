//==============================================================================
// Filename: Gfx_Texture.cpp
// Description: テクスチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Texture.h>

//===== クラス実装 =====
GfxTexture::GfxTexture(GfxMain& gfx, ToolTexLoader::TexData& data, GfxHeapMgr& heapMgr, UINT slot) :
    GfxBinder(), m_pTextureView(), m_pTextureBuffer(), m_heapMgr(heapMgr), m_slot(slot)
{
    //例外処理
    if (data.pImageData == nullptr)
        throw ERROR_EX2(S_OK, "テクスチャのポインタはNULLです。");

    //バッファ作成
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            CreateBufferDX11(gfx, data);
            break;
        case GfxMain::API_MODE::DX_12:
            CreateBufferDX12(gfx, data);
            break;
        default:
            break;
    }
}

GfxTexture::~GfxTexture() noexcept
{
}

//バインド処理
void GfxTexture::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->PSSetShaderResources(m_slot, 1u, m_pTextureView.GetAddressOf());
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}

//バッファ作成
void GfxTexture::CreateBufferDX11(GfxMain& gfx, ToolTexLoader::TexData& data)
{
    //エラーハンドル
    HRESULT hr{};

    //バッファ作成
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = static_cast<UINT>(data.nWidth);
    textureDesc.Height = static_cast<UINT>(data.nHeight);
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = data.pImageData;
    sd.SysMemPitch = data.nWidth * sizeof(uint32_t);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
    hr = GetDevice<DevDX11*>(&gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture);
    ERROR_DX(hr);

    //リソースビュー作成
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    hr = GetDevice<DevDX11*>(&gfx)->CreateShaderResourceView(pTexture.Get(),
        &srvDesc, &m_pTextureView);
    ERROR_DX(hr);
}

void GfxTexture::CreateBufferDX12(GfxMain& gfx, ToolTexLoader::TexData& data)
{
    //エラーハンドル
    HRESULT hr{};

    //テクスチャバッファ設定
    D3D12_HEAP_PROPERTIES hp{};                                 //頂点ヒープ
    hp.Type = D3D12_HEAP_TYPE_CUSTOM;                           //ヒープ種別
    hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;    //CPUのページング設定
    hp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;             //メモリプール設定(CPU)
    hp.CreationNodeMask = 0u;
    hp.VisibleNodeMask = 0u;                                    //アダプター関連
    D3D12_RESOURCE_DESC rd{};                                   //リソース設定
    rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;          //バッファかテクスチャか
    rd.Alignment = 0;                                           //アラインメント
    rd.Width = static_cast<UINT>(data.nWidth);                  //データサイズ
    rd.Height = static_cast<UINT>(data.nHeight);
    rd.DepthOrArraySize = 1;                                    //配列要素数
    rd.MipLevels = 1;                                           //ミップマップ設定
    rd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                     //テクスチャフォーマット
    rd.SampleDesc.Count = 1u;
    rd.SampleDesc.Quality = 0u;                                 //マルチサンプリング設定
    rd.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    rd.Flags = D3D12_RESOURCE_FLAG_NONE;                        //(?)

    //テクスチャバッファ作成
    hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(
        &hp, D3D12_HEAP_FLAG_NONE,                              //指定なし
        &rd, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,        //テクスチャ用
        nullptr, IID_PPV_ARGS(&m_pTextureBuffer));
    ERROR_DX(hr);

    //データマップ
    UINT rowPitch = static_cast<UINT>(sizeof(uint32_t) * rd.Width);     //1行当たりのサイズ
    UINT depthPitch = rowPitch * rd.Height;                             //スライス当たりのサイズ
    hr = m_pTextureBuffer->WriteToSubresource(0u, nullptr,              //書込み領域設定（全部）
        data.pImageData, rowPitch, depthPitch);
    ERROR_DX(hr);

    //SRV作成
    D3D12_SHADER_RESOURCE_VIEW_DESC srvd{};
    srvd.Format = rd.Format;
    srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;    //データの順序通りに割当て
    srvd.Texture2D.MipLevels = 1u;                                              //ミップマップなし
    GetDevice<DevDX12*>(&gfx)->CreateShaderResourceView(m_pTextureBuffer.Get(), &srvd, m_heapMgr.m_ahCPU[m_slot]);
}
