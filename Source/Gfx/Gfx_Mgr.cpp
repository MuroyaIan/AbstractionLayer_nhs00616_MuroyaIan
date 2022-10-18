//==============================================================================
// Filename: Gfx_Mgr.cpp
// Description: グラフィックマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Gfx_Mgr.h>
#include <Gfx/Gfx_dx11.h>
#include <Gfx/Gfx_dx12.h>

//===== クラス実装 =====
GfxMgr::GfxMgr(WinWindow& Window) : m_pGfx()
{
    //GfxAPI初期化
    switch (GfxMain::m_Api) {
    case GfxMain::API_MODE::aDX_11:
        m_pGfx = std::make_unique<GfxDX11>(Window);
        break;
    case GfxMain::API_MODE::aDX_12:
        m_pGfx = std::make_unique<GfxDX12>(Window);
        break;
    default:
        break;
    }
}

GfxMgr::~GfxMgr() noexcept
{
}
