//==============================================================================
// Filename: Gfx_RootSignature.cpp
// Description: ルートシグネチャ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_RootSignature.h>

namespace wrl = Microsoft::WRL;

//===== クラス実装 =====
GfxRootSignature::GfxRootSignature(GfxMain& gfx, GfxHeapMgr::HeapInfo& heapInfo, GfxSampler& sampler, GfxHeapMgr& heapMgr, GfxRootSignature** ppRef) :
    GfxBinder(), m_pRootSignature(), m_heapMgr(heapMgr), m_startIndexPS(static_cast<int>(heapInfo.slotCBV_VS.size() + heapInfo.slotSRV_VS.size()))
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

    //ルートパラメータ設定
    D3D12_DESCRIPTOR_RANGE1 dr1[4]{};
    D3D12_ROOT_PARAMETER1 rp1[2]{};
    dr1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    dr1[0].NumDescriptors = static_cast<UINT>(heapInfo.slotCBV_VS.size());
    if (dr1[0].NumDescriptors == 0)
        dr1[0].NumDescriptors = 1;
    dr1[0].BaseShaderRegister = 0u;                                                     //最初の入力スロット
    dr1[0].RegisterSpace = 0u;                                                          //合わせるためのスペース(?)
    dr1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    dr1[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    dr1[1].NumDescriptors = static_cast<UINT>(heapInfo.slotSRV_VS.size());
    if (dr1[1].NumDescriptors == 0)
        dr1[1].NumDescriptors = 1;
    dr1[1].BaseShaderRegister = 0u;
    dr1[1].RegisterSpace = 0u;
    dr1[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    dr1[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    dr1[2].NumDescriptors = static_cast<UINT>(heapInfo.slotCBV_PS.size());
    if (dr1[2].NumDescriptors == 0)
        dr1[2].NumDescriptors = 1;
    dr1[2].BaseShaderRegister = 0u;
    dr1[2].RegisterSpace = 0u;
    dr1[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    dr1[3].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    dr1[3].NumDescriptors = static_cast<UINT>(heapInfo.slotSRV_PS.size());
    if (dr1[3].NumDescriptors == 0)
        dr1[3].NumDescriptors = 1;
    dr1[3].BaseShaderRegister = 0u;
    dr1[3].RegisterSpace = 0u;
    dr1[3].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    rp1[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;  //タイプ
    rp1[0].DescriptorTable.pDescriptorRanges = &dr1[0];                 //ディスクリプタレンジのポインタ
    rp1[0].DescriptorTable.NumDescriptorRanges = 2u;                    //ディスクリプタレンジ数
    rp1[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;           //VSシェーダ利用許可
    rp1[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rp1[1].DescriptorTable.pDescriptorRanges = &dr1[2];
    rp1[1].DescriptorTable.NumDescriptorRanges = 2u;
    rp1[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;            //PSシェーダ利用許可

    D3D12_DESCRIPTOR_RANGE dr[4]{};
    D3D12_ROOT_PARAMETER rp[2]{};
    dr[0].RangeType = dr1[0].RangeType;
    dr[0].NumDescriptors = dr1[0].NumDescriptors;
    dr[0].BaseShaderRegister = dr1[0].BaseShaderRegister;
    dr[0].RegisterSpace = dr1[0].RegisterSpace;
    dr[0].OffsetInDescriptorsFromTableStart = dr1[0].OffsetInDescriptorsFromTableStart;
    dr[1].RangeType = dr1[1].RangeType;
    dr[1].NumDescriptors = dr1[1].NumDescriptors;
    dr[1].BaseShaderRegister = dr1[1].BaseShaderRegister;
    dr[1].RegisterSpace = dr1[1].RegisterSpace;
    dr[1].OffsetInDescriptorsFromTableStart = dr1[1].OffsetInDescriptorsFromTableStart;
    dr[2].RangeType = dr1[2].RangeType;
    dr[2].NumDescriptors = dr1[2].NumDescriptors;
    dr[2].BaseShaderRegister = dr1[2].BaseShaderRegister;
    dr[2].RegisterSpace = dr1[2].RegisterSpace;
    dr[2].OffsetInDescriptorsFromTableStart = dr1[2].OffsetInDescriptorsFromTableStart;
    dr[3].RangeType = dr1[3].RangeType;
    dr[3].NumDescriptors = dr1[3].NumDescriptors;
    dr[3].BaseShaderRegister = dr1[3].BaseShaderRegister;
    dr[3].RegisterSpace = dr1[3].RegisterSpace;
    dr[3].OffsetInDescriptorsFromTableStart = dr1[3].OffsetInDescriptorsFromTableStart;

    rp[0].ParameterType = rp1[0].ParameterType;
    rp[0].DescriptorTable.pDescriptorRanges = &dr[0];
    rp[0].DescriptorTable.NumDescriptorRanges = rp1[0].DescriptorTable.NumDescriptorRanges;
    rp[0].ShaderVisibility = rp1[0].ShaderVisibility;
    rp[1].ParameterType = rp1[1].ParameterType;
    rp[1].DescriptorTable.pDescriptorRanges = &dr[2];
    rp[1].DescriptorTable.NumDescriptorRanges = rp1[1].DescriptorTable.NumDescriptorRanges;
    rp[1].ShaderVisibility = rp1[1].ShaderVisibility;

    //ルールシグネチャ作成
    wrl::ComPtr<ID3DBlob> pBlobRS;                                                              //バイナリコード作成
    wrl::ComPtr<ID3DBlob> pBlobErrorRS;
    if (vrsd.Version == D3D_ROOT_SIGNATURE_VERSION_1_1) {
        vrsd.Desc_1_1.NumParameters = 2u;                                                       //ルートパラメータ
        vrsd.Desc_1_1.pParameters = rp1;
        vrsd.Desc_1_1.NumStaticSamplers = 1u;                                                   //サンプラー
        vrsd.Desc_1_1.pStaticSamplers = sampler.GetSampler();
        vrsd.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;     //IA許可
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    else {
        vrsd.Desc_1_0.NumParameters = 2u;
        vrsd.Desc_1_0.pParameters = rp;
        vrsd.Desc_1_0.NumStaticSamplers = 1u;
        vrsd.Desc_1_0.pStaticSamplers = sampler.GetSampler();
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

    //自身のポインタを提供
    *ppRef = this;
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

    GetCommand<CmdDX12*>(&gfx)->SetDescriptorHeaps(1u, m_heapMgr.m_pBufferHeaps.GetAddressOf());    //ディスクリプタヒープ指定
    GetCommand<CmdDX12*>(&gfx)->SetGraphicsRootSignature(m_pRootSignature.Get());
    GetCommand<CmdDX12*>(&gfx)->SetGraphicsRootDescriptorTable(0u, m_heapMgr.m_ahGPU[0]);
    GetCommand<CmdDX12*>(&gfx)->SetGraphicsRootDescriptorTable(1u, m_heapMgr.m_ahGPU[m_startIndexPS]);
}

//ルートシグネチャ取得
ID3D12RootSignature* GfxRootSignature::GetRootSignature() const noexcept
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return nullptr;

    return m_pRootSignature.Get();
}
