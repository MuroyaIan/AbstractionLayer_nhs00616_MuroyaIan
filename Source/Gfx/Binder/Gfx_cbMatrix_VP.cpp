//==============================================================================
// Filename: Gfx_cbMatrix_VP.cpp
// Description: カメラ情報用定数バッファ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_cbMatrix_VP.h>
#include <Tool/Tool_Math.h>

namespace dx = DirectX;
using vcbMtx = GfxVertexCBuffer<GfxCbMtxVP>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbMtx> GfxCBuffMtxVP::m_pVcBuff = nullptr;
int GfxCBuffMtxVP::m_refCount = 0;

//===== クラス実装 =====
GfxCBuffMtxVP::GfxCBuffMtxVP(GfxMain& gfx, GfxHeapMgr::HeapInfo* pheapInfo) : GfxBinder()
{
    //定数バッファ初期化
    m_refCount++;
    if (!m_pVcBuff)
        m_pVcBuff = std::make_unique<vcbMtx>(gfx, pheapInfo, static_cast<UINT>(CB_SLOT_VS::CAMERA));
}

GfxCBuffMtxVP::~GfxCBuffMtxVP() noexcept
{
    //定数バッファ終了
    m_refCount--;
    if (m_refCount == 0)
        m_pVcBuff.reset();
}

//バインド処理
void GfxCBuffMtxVP::Bind(GfxMain& gfx) noexcept
{
    //バッファ更新
    dx::XMFLOAT4X4 view = gfx.GetViewMtx();
    dx::XMFLOAT4X4 proj = gfx.GetProjectionMtx();
    gMath::MtxTranspose4x4_SSE(&view._11);
    gMath::MtxTranspose4x4_SSE(&proj._11);
    GfxCbMtxVP aMtx = { view, proj };
    m_pVcBuff->Update(gfx, aMtx);

    //バインド処理
    m_pVcBuff->Bind(gfx);
}
