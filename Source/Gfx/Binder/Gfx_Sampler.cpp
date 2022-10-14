//==============================================================================
// Filename: Gfx_Sampler.cpp
// Description: サンプラー処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Sampler.h>

//===== クラス実装 =====

#if DX_11

GfxSampler::GfxSampler(GfxMain& gfx) :
    GfxBinder(), m_pSampler()
{
    //エラーハンドル
    HRESULT hr{};

    //サンプラー作成
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = GetDevice(gfx)->CreateSamplerState(&samplerDesc, &m_pSampler);
    ERROR_DX(hr);
}

GfxSampler::~GfxSampler() noexcept
{
}

//バインド処理
void GfxSampler::Bind(GfxMain& gfx) noexcept
{
    GetContext(gfx)->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
}

#elif DX_12


#endif // GFX
