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
GfxMgr::GfxMgr(WinWindow& window) : m_window(window), m_pGfx()
{
    //GfxAPI初期化
    InitAPI();
}

GfxMgr::~GfxMgr() noexcept
{
}

//API切替
void GfxMgr::SetAPI(GfxMain::API_MODE mode)
{
    if (GfxMain::m_api != mode)
    {
        //API再初期化
        m_pGfx.reset();
        GfxMain::m_api = mode;
        InitAPI();
    }
}

//GfxApi初期化
void GfxMgr::InitAPI()
{
    switch (GfxMain::m_api) {
        case GfxMain::API_MODE::DX_11:
            m_pGfx = std::make_unique<GfxDX11>(m_window);
            break;
        case GfxMain::API_MODE::DX_12:
            m_pGfx = std::make_unique<GfxDX12>(m_window);
            break;
        default:
            break;
    }
}
