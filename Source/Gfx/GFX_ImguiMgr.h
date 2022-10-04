﻿//==============================================================================
// Filename: Win_Window.h
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== 定数・マクロ定義 =====
#include <3rd_Party/ImGui/imgui.h>
#include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#include <3rd_Party/ImGui/backends/imgui_impl_dx11.h>

#define U8(str) reinterpret_cast<const char*>(str)

//===== クラス宣言 =====

//***** ImGuiマネージャ *****
class IMGUI_MGR
{
public:

    //プロトタイプ宣言
    IMGUI_MGR() noexcept;
    ~IMGUI_MGR() noexcept;
    void DrawDemo() const noexcept;     //DEMOウィンドウ描画
};
