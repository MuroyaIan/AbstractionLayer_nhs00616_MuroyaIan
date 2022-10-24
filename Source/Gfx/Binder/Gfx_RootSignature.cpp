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

    //ディスクリプタレンジ設定
    std::vector<D3D12_DESCRIPTOR_RANGE1> aDR1(0);
    int vsCnt = 0;
    int psCnt = 0;
    if (heapInfo.slotCBV_VS.size() > 0) {
        D3D12_DESCRIPTOR_RANGE1 dr1{};
        dr1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        dr1.NumDescriptors = static_cast<UINT>(heapInfo.slotCBV_VS.size());
        dr1.BaseShaderRegister = 0u;                                                    //最初の入力スロット
        dr1.RegisterSpace = 0u;                                                         //合わせるためのスペース(?)
        dr1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        aDR1.push_back(dr1);
        vsCnt++;
    }
    if (heapInfo.slotSRV_VS.size() > 0) {
        D3D12_DESCRIPTOR_RANGE1 dr1{};
        dr1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        dr1.NumDescriptors = static_cast<UINT>(heapInfo.slotSRV_VS.size());
        dr1.BaseShaderRegister = 0u;
        dr1.RegisterSpace = 0u;
        dr1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        aDR1.push_back(dr1);
        vsCnt++;
    }
    if (heapInfo.slotCBV_PS.size() > 0) {
        D3D12_DESCRIPTOR_RANGE1 dr1{};
        dr1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        dr1.NumDescriptors = static_cast<UINT>(heapInfo.slotCBV_PS.size());
        dr1.BaseShaderRegister = 0u;
        dr1.RegisterSpace = 0u;
        dr1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        aDR1.push_back(dr1);
        psCnt++;
    }
    if (heapInfo.slotSRV_PS.size() > 0) {
        D3D12_DESCRIPTOR_RANGE1 dr1{};
        dr1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        dr1.NumDescriptors = static_cast<UINT>(heapInfo.slotSRV_PS.size());
        dr1.BaseShaderRegister = 0u;
        dr1.RegisterSpace = 0u;
        dr1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        aDR1.push_back(dr1);
        psCnt++;
    }

    //ルートパラメータ設定
    std::vector<D3D12_ROOT_PARAMETER1> aRP1(0);
    if (vsCnt > 0) {
        D3D12_ROOT_PARAMETER1 rp1{};
        rp1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;     //タイプ
        rp1.DescriptorTable.pDescriptorRanges = &aDR1[0];                   //ディスクリプタレンジのポインタ
        rp1.DescriptorTable.NumDescriptorRanges = vsCnt;                    //ディスクリプタレンジ数
        rp1.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;              //VSシェーダ利用許可
        aRP1.push_back(rp1);
    }
    if (psCnt > 0) {
        D3D12_ROOT_PARAMETER1 rp1{};
        rp1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rp1.DescriptorTable.pDescriptorRanges = &aDR1[vsCnt];
        rp1.DescriptorTable.NumDescriptorRanges = psCnt;
        rp1.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;               //PSシェーダ利用許可
        aRP1.push_back(rp1);
    }

    //別バージョン(1.0)の場合の設定
    std::vector<D3D12_DESCRIPTOR_RANGE> aDR(aDR1.size());
    for (size_t i = 0, Cnt = aDR.size(); i < Cnt; i++) {
        aDR[i].RangeType = aDR1[i].RangeType;
        aDR[i].NumDescriptors = aDR1[i].NumDescriptors;
        aDR[i].BaseShaderRegister = aDR1[i].BaseShaderRegister;
        aDR[i].RegisterSpace = aDR1[i].RegisterSpace;
        aDR[i].OffsetInDescriptorsFromTableStart = aDR1[i].OffsetInDescriptorsFromTableStart;
    }
    std::vector<D3D12_ROOT_PARAMETER> aRP(aRP1.size());
    for (size_t i = 0, Cnt = aRP.size(); i < Cnt; i++) {
        aRP[i].ParameterType = aRP1[i].ParameterType;
        aRP[i].DescriptorTable.NumDescriptorRanges = aRP1[i].DescriptorTable.NumDescriptorRanges;
        aRP[i].ShaderVisibility = aRP1[i].ShaderVisibility;

        if (aRP[i].ShaderVisibility == D3D12_SHADER_VISIBILITY_VERTEX)
            aRP[i].DescriptorTable.pDescriptorRanges = &aDR[0];
        else if (aRP[i].ShaderVisibility == D3D12_SHADER_VISIBILITY_PIXEL)
            aRP[i].DescriptorTable.pDescriptorRanges = &aDR[vsCnt];
    }

    //ルールシグネチャ作成
    wrl::ComPtr<ID3DBlob> pBlobRS;                                                              //バイナリコード作成
    wrl::ComPtr<ID3DBlob> pBlobErrorRS;
    if (vrsd.Version == D3D_ROOT_SIGNATURE_VERSION_1_1) {
        if (aRP1.size() > 0) {
            vrsd.Desc_1_1.NumParameters = static_cast<UINT>(aRP1.size());                       //ルートパラメータ
            vrsd.Desc_1_1.pParameters = &aRP1[0];
        }
        vrsd.Desc_1_1.NumStaticSamplers = 1u;                                                   //サンプラー
        vrsd.Desc_1_1.pStaticSamplers = sampler.GetSampler();
        vrsd.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;     //IA許可
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    else {
        if (aRP.size() > 0) {
            vrsd.Desc_1_0.NumParameters = static_cast<UINT>(aRP.size());
            vrsd.Desc_1_0.pParameters = &aRP[0];
        }
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
