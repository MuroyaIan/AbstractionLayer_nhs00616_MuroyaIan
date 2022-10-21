//==============================================================================
// Filename: Gfx_Sampler.cpp
// Description: サンプラー処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Sampler.h>

//===== クラス実装 =====
GfxSampler::GfxSampler(GfxMain& gfx, UINT slot) :
    GfxBinder(), m_pSampler(), m_samplerDesc(), m_slot(slot)
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            CreateSamplerDX11(gfx);
            break;
        case GfxMain::API_MODE::DX_12:
            CreateSamplerDX12();
            break;
        default:
            break;
    }
}

GfxSampler::~GfxSampler() noexcept
{
}

//バインド処理
void GfxSampler::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->PSSetSamplers(m_slot, 1u, m_pSampler.GetAddressOf());
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}

//サンプラー作成
void GfxSampler::CreateSamplerDX11(GfxMain& gfx)
{
    //エラーハンドル
    HRESULT hr{};

    //サンプラー作成
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = GetDevice<DevDX11*>(&gfx)->CreateSamplerState(&samplerDesc, &m_pSampler);
    ERROR_DX(hr);
}

void GfxSampler::CreateSamplerDX12() noexcept
{
    m_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                //補間方法
    m_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;             //アドレッシングモード（繰り返し）
    m_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    m_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    //m_samplerDesc.MipLODBias;                                             //計算結果ミップマップレベルからのオフセット(?)
    //m_samplerDesc.MaxAnisotropy;                                          //異方性フィルタの最大値
    m_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;           //比較関数（サンプリングなし）
    m_samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;   //端の色
    m_samplerDesc.MinLOD = 0.0f;
    m_samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                             //ミップマップの最大値・最小値
    m_samplerDesc.ShaderRegister = 0u;                                    //入力スロット
    m_samplerDesc.RegisterSpace = 0u;
    m_samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;       //PSシェーダ利用許可
}
