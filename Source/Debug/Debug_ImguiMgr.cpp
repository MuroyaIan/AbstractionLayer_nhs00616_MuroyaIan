//==============================================================================
// Filename: Win_Window.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Debug/Debug_ImguiMgr.h>

//===== クラス実装 =====
IMGUI_MGR::IMGUI_MGR() noexcept
{
    //初期化処理
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //スタイル設定
    ImGui::StyleColorsDark();
    ImGuiStyle& Style = ImGui::GetStyle();
    Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.5f, 0.5f, 0.5f);
    Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
    Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);

    //フォント読込
    ImGuiIO& io = ImGui::GetIO();
    ImFont* Font = nullptr;
    Font = io.Fonts->AddFontFromFileTTF("Asset/Font/UDEVGothicJPDOC-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(Font != NULL);
}

IMGUI_MGR::~IMGUI_MGR() noexcept
{
    ImGui::DestroyContext();
}

//DEMOウィンドウ描画
void IMGUI_MGR::DrawDemo() const noexcept
{
    static bool bShowDemo = true;
    if (bShowDemo)
        ImGui::ShowDemoWindow(&bShowDemo);
}
