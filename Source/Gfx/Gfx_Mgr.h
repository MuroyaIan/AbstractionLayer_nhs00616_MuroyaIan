//==============================================================================
// Filename: Gfx_Mgr.h
// Description: グラフィックマネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

//===== クラス定義 =====
class GfxMgr
{
public:

    //プロトタイプ宣言
    GfxMgr(WinWindow& window);
    ~GfxMgr() noexcept;
    void SetAPI(GfxMain::API_MODE mode);    //API切替

    GfxMain* const GetGfx() const noexcept  //グラフィックAPI取得
    {
        return m_pGfx.get();
    }

private:

    //変数宣言
    WinWindow& m_window;
    std::unique_ptr<GfxMain> m_pGfx;

    //プロトタイプ宣言
    void InitAPI();     //GfxApi初期化
};
