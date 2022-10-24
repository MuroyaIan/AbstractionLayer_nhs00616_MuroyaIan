//==============================================================================
// Filename: Gfx_Inputlayout.cpp
// Description: 入力レイアウト処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_InputLayout.h>

//===== クラス実装 =====
GfxInputLayout::GfxInputLayout(GfxMain& gfx, const std::vector<LayoutInfo>& layout,
    ID3DBlob* pVertexShaderBytecode) :
    GfxBinder(), m_pInputLayout(), m_aLayoutDesc(0)
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            CreateLayoutDX11(gfx, layout, pVertexShaderBytecode);
            break;
        case GfxMain::API_MODE::DX_12:
            CreateLayoutDX12(layout);
            break;
        default:
            break;
    }
}

GfxInputLayout::~GfxInputLayout() noexcept
{
}

//バインド処理
void GfxInputLayout::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->IASetInputLayout(m_pInputLayout.Get());
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}

//レイアウト作成
void GfxInputLayout::CreateLayoutDX11(GfxMain& gfx, const std::vector<LayoutInfo>& layout, ID3DBlob* pVertexShaderBytecode)
{
    //DX11用の型にコピー
    std::vector<D3D11_INPUT_ELEMENT_DESC> aLayout(layout.size());
    for (size_t i = 0, Cnt = aLayout.size(); i < Cnt; i++) {
        aLayout[i].SemanticName = layout[i].semanticName;
        aLayout[i].SemanticIndex = layout[i].semanticIndex;
        aLayout[i].Format = layout[i].format;
        aLayout[i].InputSlot = layout[i].inputSlot;
        aLayout[i].AlignedByteOffset = layout[i].alignedByteOffset;
        aLayout[i].InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(layout[i].isPerInstanceData);
        aLayout[i].InstanceDataStepRate = layout[i].instanceDataStepRate;
    }

    //エラーハンドル
    HRESULT hr{};

    //レイアウト作成
    hr = GetDevice<DevDX11*>(&gfx)->CreateInputLayout(
        aLayout.data(), static_cast<UINT>(aLayout.size()),
        pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(),
        &m_pInputLayout);
    ERROR_DX(hr);
}

void GfxInputLayout::CreateLayoutDX12(const std::vector<LayoutInfo>& layout)
{
    //DX12用の型にコピー
    m_aLayoutDesc.resize(layout.size());
    for (size_t i = 0, Cnt = m_aLayoutDesc.size(); i < Cnt; i++) {
        m_aLayoutDesc[i].SemanticName = layout[i].semanticName;
        m_aLayoutDesc[i].SemanticIndex = layout[i].semanticIndex;
        m_aLayoutDesc[i].Format = layout[i].format;
        m_aLayoutDesc[i].InputSlot = layout[i].inputSlot;
        m_aLayoutDesc[i].AlignedByteOffset = layout[i].alignedByteOffset;
        m_aLayoutDesc[i].InputSlotClass = static_cast<D3D12_INPUT_CLASSIFICATION>(layout[i].isPerInstanceData);
        m_aLayoutDesc[i].InstanceDataStepRate = layout[i].instanceDataStepRate;
    }
}
