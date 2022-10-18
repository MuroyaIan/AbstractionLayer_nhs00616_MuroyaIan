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
    GfxMgr(WinWindow& Window);
    ~GfxMgr() noexcept;

    GfxMain* const GetGfx() const noexcept  //グラフィックAPI取得
    {
        return m_pGfx.get();
    }

private:

    //変数宣言
    std::unique_ptr<GfxMain> m_pGfx;
};
