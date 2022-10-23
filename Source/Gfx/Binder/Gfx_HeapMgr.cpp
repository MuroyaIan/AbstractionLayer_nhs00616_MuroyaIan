//==============================================================================
// Filename: Gfx_HeapMgr.cpp
// Description: ヒープマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_HeapMgr.h>

namespace wrl = Microsoft::WRL;

//===== クラス実装 =====
GfxHeapMgr::GfxHeapMgr(GfxMain& gfx, HeapInfo& heapInfo, GfxHeapMgr* pRef) :
    GfxBinder(), m_pBufferHeaps(), m_ahCPU(0), m_ahGPU(0)
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return;

    //ディスクリプ数取得
    UINT numView = static_cast<UINT>(heapInfo.slotCBV_VS.size() + heapInfo.slotSRV_VS.size());
    numView += static_cast<UINT>(heapInfo.slotCBV_PS.size() + heapInfo.slotSRV_PS.size());

    //エラーハンドル
    HRESULT hr{};

    //ディスクリプタヒープ作成
    D3D12_DESCRIPTOR_HEAP_DESC dhd{};
    dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    dhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //shaderから参照あり（SRVやCBVの場合はあり）
    dhd.NumDescriptors = numView;                           //バッファの数
    dhd.NodeMask = 0u;
    hr = GetDevice<DevDX12*>(&gfx)->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_pBufferHeaps));
    ERROR_DX(hr);

    //ハンドル取得
    UINT IncSize = GetDevice<DevDX12*>(&gfx)->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    for (size_t i = 0; i < numView; i++) {
        D3D12_CPU_DESCRIPTOR_HANDLE cdh = m_pBufferHeaps->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gdh = m_pBufferHeaps->GetGPUDescriptorHandleForHeapStart();
        cdh.ptr += IncSize * i;
        gdh.ptr += IncSize * i;
        m_ahCPU.push_back(cdh);
        m_ahGPU.push_back(gdh);
    }

    //CBV作成(VS)
    D3D12_CPU_DESCRIPTOR_HANDLE* cdhPtr = &m_ahCPU[0];
    HeapInfo::InfoCBV* icbvPtr = &heapInfo.slotCBV_VS[0];
    for (size_t i = 0, Cnt = heapInfo.slotCBV_VS.size(); i < Cnt; i++){
        GetDevice<DevDX12*>(&gfx)->CreateConstantBufferView(&icbvPtr->cbvd, *cdhPtr);
        icbvPtr++;
        cdhPtr++;
    }

    //SRV作成(VS)
    HeapInfo::InfoSRV* isrvPtr = &heapInfo.slotSRV_VS[0];
    for (size_t i = 0, Cnt = heapInfo.slotSRV_VS.size(); i < Cnt; i++) {
        GetDevice<DevDX12*>(&gfx)->CreateShaderResourceView(isrvPtr->pBuffer, &isrvPtr->srvd, *cdhPtr);
        isrvPtr++;
        cdhPtr++;
    }

    //CBV作成(PS)
    icbvPtr = &heapInfo.slotCBV_PS[0];
    for (size_t i = 0, Cnt = heapInfo.slotCBV_PS.size(); i < Cnt; i++) {
        GetDevice<DevDX12*>(&gfx)->CreateConstantBufferView(&icbvPtr->cbvd, *cdhPtr);
        icbvPtr++;
        cdhPtr++;
    }

    //SRV作成(PS)
    isrvPtr = &heapInfo.slotSRV_PS[0];
    for (size_t i = 0, Cnt = heapInfo.slotSRV_PS.size(); i < Cnt; i++) {
        GetDevice<DevDX12*>(&gfx)->CreateShaderResourceView(isrvPtr->pBuffer, &isrvPtr->srvd, *cdhPtr);
        isrvPtr++;
        cdhPtr++;
    }

    //自身のポインタを提供
    pRef = this;
}

GfxHeapMgr::~GfxHeapMgr() noexcept
{
}

//バインド処理
void GfxHeapMgr::Bind(GfxMain& gfx) noexcept
{
    (void)gfx;
}
