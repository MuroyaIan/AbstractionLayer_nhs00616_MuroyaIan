//==============================================================================
// Filename: Gfx_Inputlayout.cpp
// Description: 入力レイアウト処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_InputLayout.h>

//===== クラス実装 =====
GfxInputLayout::GfxInputLayout(GfxMain& gfx,
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode) :
    GfxBinder(), m_pInputLayout()
{
    //エラーハンドル
    HRESULT hr{};

    //レイアウト作成
    hr = GetDevice(gfx)->CreateInputLayout(
        layout.data(), static_cast<UINT>(layout.size()),
        pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(),
        &m_pInputLayout);
    ERROR_DX(hr);
}

GfxInputLayout::~GfxInputLayout() noexcept
{
}

//バインド処理
void GfxInputLayout::Bind(GfxMain& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}
