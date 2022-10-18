////==============================================================================
//// Filename: Gfx_PixelShader.cpp
//// Description: ピクセルシェーダ処理
//// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
////==============================================================================
//
////===== インクルード部 =====
//#include <Gfx/Binder/Gfx_PixelShader.h>
//
////===== クラス実装 =====
//GfxPixelShader::GfxPixelShader(GfxMain& gfx, const std::wstring& path) :
//    GfxBinder(), m_pPixelShader()
//{
//    //エラーハンドル
//    HRESULT hr{};
//
//    //シェーダ作成
//    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
//    hr = D3DReadFileToBlob(path.c_str(), &pBlob);
//    ERROR_DX(hr);
//    hr = GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(),
//        pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
//    ERROR_DX(hr);
//}
//
//GfxPixelShader::~GfxPixelShader() noexcept
//{
//}
//
////バインド処理
//void GfxPixelShader::Bind(GfxMain& gfx) noexcept
//{
//    GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
//}
