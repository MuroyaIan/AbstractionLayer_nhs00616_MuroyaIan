//==============================================================================
// Filename: Gfx_Texture.cpp
// Description: テクスチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Texture.h>

//===== クラス実装 =====
GfxTexture::GfxTexture(GfxMain& gfx, ToolTexLoader::TexData& data) :
    GfxBinder(), m_pTextureView()
{
    //エラーハンドル
    HRESULT hr{};

    //バッファ作成
    if (data.pImageData != nullptr) {

        //テクスチャリソース作成
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
        hr = GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture);
        ERROR_DX(hr);

        //リソースビュー作成
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;
        hr = GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(),
            &srvDesc, &m_pTextureView);
        ERROR_DX(hr);
    }
    else
        throw ERROR_EX2(S_OK, "テクスチャのポインタはNULLです。");
}

GfxTexture::~GfxTexture() noexcept
{
}

//バインド処理
void GfxTexture::Bind(GfxMain& gfx) noexcept
{
    GetContext(gfx)->PSSetShaderResources(0u, 1u, m_pTextureView.GetAddressOf());
}
