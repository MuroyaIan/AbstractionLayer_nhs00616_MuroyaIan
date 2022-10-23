//==============================================================================
// Filename: Draw_ShaderMgr.cpp
// Description: シェーダマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_ShaderMgr.h>
#include <Gfx/Binder/Gfx_BinderRef.h>
#include <Draw/Draw_LightMgr.h>

//===== クラス実装 =====
DrawShaderMgr::DrawShaderMgr(GfxMain& gfx) :
    m_dx(gfx), m_aBinder(static_cast<int>(BinderID::ID_MAX))
{
    //INSTANCE_PHONG
    {
        //頂点シェーダ作成
        std::unique_ptr<GfxVertexShader> pVS = std::make_unique<GfxVertexShader>(m_dx,
            L"Asset/Shader/VS_Instance_Phong.cso");
        auto pvsBC = pVS->GetBytecode();
        m_aBinder[static_cast<int>(BinderID::VS_INSTANCE_PHONG)] = std::move(pVS);

        //入力レイアウト作成
        if (gfx.CheckApiVer(GfxMain::API_MODE::DX_11)) {
            const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
            {
                { "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,    0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0u },
                { "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,       0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0u },
                { "NORMAL",    0u, DXGI_FORMAT_R32G32B32_FLOAT,    0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0u },
                { "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1u }
            };
            m_aBinder[static_cast<int>(BinderID::IL_INSTANCE_PHONG)] =
                std::make_unique<GfxInputLayout>(m_dx, ied, pvsBC);
        }
        else if (gfx.CheckApiVer(GfxMain::API_MODE::DX_12)) {
            const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
            {
                { "POSITION",  0u, DXGI_FORMAT_R32G32B32_FLOAT,    0u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,   0u },
                { "TEXCOORD",  0u, DXGI_FORMAT_R32G32_FLOAT,       0u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,   0u },
                { "NORMAL",    0u, DXGI_FORMAT_R32G32B32_FLOAT,    0u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,   0u },
                { "WORLD_MTX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 1u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 2u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1u },
                { "WORLD_MTX", 3u, DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1u }
            };
            m_aBinder[static_cast<int>(BinderID::IL_INSTANCE_PHONG)] =
                std::make_unique<GfxInputLayout>(ied);
        }
    }

    //プリミティブトポロジー作成
    m_aBinder[static_cast<int>(BinderID::PT_LINE)] =
        std::make_unique<GfxTopology>(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    m_aBinder[static_cast<int>(BinderID::PT_TRI)] =
        std::make_unique<GfxTopology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //サンプラー作成
    m_aBinder[static_cast<int>(BinderID::SAMPLER)] =
        std::make_unique<GfxSampler>(m_dx);

    //ピクセルシェーダ作成
    m_aBinder[static_cast<int>(BinderID::PS_PHONG)] =
        std::make_unique<GfxPixelShader>(m_dx, L"Asset/Shader/PS_Phong.cso");
    m_aBinder[static_cast<int>(BinderID::PS_PHONG_NO_TEX)] =
        std::make_unique<GfxPixelShader>(m_dx, L"Asset/Shader/PS_Phong_NoTex.cso");

    //定数バッファ作成（VP行列）
    m_aBinder[static_cast<int>(BinderID::CB_VS_MTX_VP)] =
        std::make_unique<GfxCBuffMtxVP>(m_dx, nullptr);

    //定数バッファ作成（ライト情報）
    m_aBinder[static_cast<int>(BinderID::CB_PS_LIGHT)] =
        std::make_unique<GfxPixelCBuffer<DrawLightMgr::LightPack>>(m_dx, nullptr, static_cast<UINT>(CB_SLOT_PS::LIGHT));
}

DrawShaderMgr::~DrawShaderMgr() noexcept
{
}

//バインド処理
void DrawShaderMgr::Bind(BinderID id) const noexcept
{
    m_aBinder[static_cast<int>(id)]->Bind(m_dx);
}

//バインド処理（インスタンシング_Phongモデル）
void DrawShaderMgr::Bind_Instance_Phong() const noexcept
{
    m_aBinder[static_cast<int>(BinderID::VS_INSTANCE_PHONG)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::IL_INSTANCE_PHONG)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::PT_TRI)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::SAMPLER)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::PS_PHONG)]->Bind(m_dx);
}

// バインド処理（インスタンシング_Phongモデル_テクスチャなし）
void DrawShaderMgr::Bind_Instance_Phong_NoTex() const noexcept
{
    m_aBinder[static_cast<int>(BinderID::VS_INSTANCE_PHONG)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::IL_INSTANCE_PHONG)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::PT_TRI)]->Bind(m_dx);
    m_aBinder[static_cast<int>(BinderID::PS_PHONG_NO_TEX)]->Bind(m_dx);
}
