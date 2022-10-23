//==============================================================================
// Filename: Draw_LightMgr.cpp
// Description: ライトマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_LightMgr.h>
#include <Gfx/Binder/Gfx_BinderRef.h>

//===== クラス実装 =====
DrawLightMgr::DrawLightMgr(App64& app) noexcept :
    m_gfx(app.GetGfxPack()), m_lightData(), m_pcBuff(nullptr)
{
    //定数バッファ作成
    m_pcBuff = dynamic_cast<GfxPixelCBuffer<LightPack>*>(app.GetGfxPack().m_shaderMgr.GetBind(DrawShaderMgr::BinderID::CB_PS_LIGHT).get());
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
