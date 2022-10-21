//==============================================================================
// Filename: Gfx_PixelShader.cpp
// Description: ピクセルシェーダ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_PixelShader.h>

//===== クラス実装 =====
GfxPixelShader::GfxPixelShader(GfxMain& gfx, const std::wstring& path) :
    GfxBinder(), m_pPixelShader()
{
    //エラーハンドル
    HRESULT hr{};

    //バイナリ読込
    hr = D3DReadFileToBlob(path.c_str(), &m_pBytecodePS);
    ERROR_DX(hr);

    //必要な場合シェーダ作成
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            hr = GetDevice<DevDX11*>(&gfx)->CreatePixelShader(
                m_pBytecodePS->GetBufferPointer(), m_pBytecodePS->GetBufferSize(),
                nullptr, &m_pPixelShader);
            ERROR_DX(hr);
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}

GfxPixelShader::~GfxPixelShader() noexcept
{
}

//バインド処理
void GfxPixelShader::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
            break;
        case GfxMain::API_MODE::DX_12:
            break;
        default:
            break;
    }
}
