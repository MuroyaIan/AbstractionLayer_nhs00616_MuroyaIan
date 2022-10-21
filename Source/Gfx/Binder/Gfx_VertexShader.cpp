//==============================================================================
// Filename: Gfx_VertexShader.cpp
// Description: 頂点シェーダ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_VertexShader.h>

//===== クラス実装 =====
GfxVertexShader::GfxVertexShader(GfxMain& gfx, const std::wstring& path) :
    GfxBinder(), m_pBytecodeVS(), m_pVertexShader()
{
    //エラーハンドル
    HRESULT hr{};

    //バイナリ読込
    hr = D3DReadFileToBlob(path.c_str(), &m_pBytecodeVS);
    ERROR_DX(hr);

    //必要な場合シェーダ作成
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            hr = GetDevice<DevDX11*>(&gfx)->CreateVertexShader(
                m_pBytecodeVS->GetBufferPointer(), m_pBytecodeVS->GetBufferSize(),
                nullptr, &m_pVertexShader);
            ERROR_DX(hr);
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}

GfxVertexShader::~GfxVertexShader() noexcept
{
}

//バインド処理
void GfxVertexShader::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}
