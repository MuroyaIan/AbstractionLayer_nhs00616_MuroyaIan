//==============================================================================
// Filename: Gfx_RootSignature.cpp
// Description: ルートシグネチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_RootSignature.h>

namespace wrl = Microsoft::WRL;

//===== クラス実装 =====
GfxRootSignature::GfxRootSignature(GfxMain& gfx) :
    GfxBinder(), m_pRootSignature()
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return;

    //エラーハンドル
    HRESULT hr{};

    //バージョン確認
    D3D12_FEATURE_DATA_ROOT_SIGNATURE fdrs{};
    fdrs.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
    hr = GetDevice<DevDX12*>(&gfx)->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &fdrs, sizeof(fdrs));
    if (hr != S_OK) {
        fdrs.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        hr = GetDevice<DevDX12*>(&gfx)->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &fdrs, sizeof(fdrs));
        ERROR_DX(hr);
    }
    D3D12_VERSIONED_ROOT_SIGNATURE_DESC vrsd{};
    vrsd.Version = fdrs.HighestVersion;

    //ルールシグネチャ作成
    wrl::ComPtr<ID3DBlob> pBlobRS;                                                              //バイナリコード作成
    wrl::ComPtr<ID3DBlob> pBlobErrorRS;
    if (vrsd.Version == D3D_ROOT_SIGNATURE_VERSION_1_1) {
        vrsd.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;     //IA許可
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    else {
        vrsd.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    if (hr != S_OK) {                                                                           //エラーチェック
        if (pBlobErrorRS.Get() != nullptr) {
            std::string strError;
            strError.resize(pBlobErrorRS->GetBufferSize());
            std::copy(static_cast<char*>(pBlobErrorRS->GetBufferPointer()), static_cast<char*>(pBlobErrorRS->GetBufferPointer()) + pBlobErrorRS->GetBufferSize(), strError.data());
            strError = "【VSシェーダ読込エラー】 " + strError;
            throw ERROR_EX2(S_OK, strError.c_str());
        }
        else
            ERROR_DX(hr);
    }
    hr = GetDevice<DevDX12*>(&gfx)->CreateRootSignature(0u, pBlobRS->GetBufferPointer(), pBlobRS->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
    ERROR_DX(hr);
}

GfxRootSignature::~GfxRootSignature() noexcept
{
}

//バインド処理
void GfxRootSignature::Bind(GfxMain& gfx) noexcept
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return;

    GetCommand<CmdDX12*>(&gfx)->SetGraphicsRootSignature(m_pRootSignature.Get());
}
