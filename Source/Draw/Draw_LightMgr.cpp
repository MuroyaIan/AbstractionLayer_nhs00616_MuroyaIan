//==============================================================================
// Filename: Draw_LightMgr.cpp
// Description: ライトマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_LightMgr.h>
#include <Gfx/Binder/Gfx_BinderRef.h>

//===== クラス実装 =====

#if DX_11

DrawLightMgr::DrawLightMgr(App64& app) noexcept :
    m_gfx(app.GetGfxPack()), m_lightData(), m_pcBuff()
{
    //定数バッファ作成
    m_pcBuff = std::make_unique<GfxPixelCBuffer<LightPack>>(m_gfx.m_dx,
        static_cast<UINT>(CB_SLOT_PS::LIGHT));
}

DrawLightMgr::~DrawLightMgr() noexcept
{
}

//書込み処理
void DrawLightMgr::Draw() noexcept
{
    //定数バッファ更新
    m_pcBuff->Update(m_gfx.m_dx, m_lightData);
    m_pcBuff->Bind(m_gfx.m_dx);
}

#elif DX_12


#endif // GFX
