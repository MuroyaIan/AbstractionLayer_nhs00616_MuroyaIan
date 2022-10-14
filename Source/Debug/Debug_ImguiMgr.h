﻿//==============================================================================
// Filename: Win_Window.h
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <3rd_Party/ImGui/imgui.h>

//===== 定数・マクロ定義 =====
#define U8(str) reinterpret_cast<const char*>(str)

//===== クラス宣言 =====

//***** ImGuiマネージャ *****
class DebugImguiMgr
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    DebugImguiMgr() noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~DebugImguiMgr() noexcept;

    //--------------------------------------------------------------------------
    /// DEMOウィンドウ描画
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawDemo() const noexcept;

    //--------------------------------------------------------------------------
};
