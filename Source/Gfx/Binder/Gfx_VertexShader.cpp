//==============================================================================
// Filename: Gfx_VertexShader.cpp
// Description: 頂点シェーダ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_VertexShader.h>

//===== クラス実装 =====

#if DX_11

GfxVertexShader::GfxVertexShader(GfxMain& gfx, const std::wstring& path) :
    GfxBinder(), m_pBytecodeBlob(), m_pVertexShader()
{
    //エラーハンドル
    HRESULT hr{};

    //シェーダ作成
    hr = D3DReadFileToBlob(path.c_str(), &m_pBytecodeBlob);
    ERROR_DX(hr);
    hr = GetDevice(gfx)->CreateVertexShader(
        m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(),
        nullptr, &m_pVertexShader);
    ERROR_DX(hr);
}

GfxVertexShader::~GfxVertexShader() noexcept
{
}

//バインド処理
void GfxVertexShader::Bind(GfxMain& gfx) noexcept
{
    GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

#elif DX_12


#endif // GFX
