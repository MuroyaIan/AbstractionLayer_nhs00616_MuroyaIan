//==============================================================================
// Filename: Win_Window.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Debug/Debug_ImguiMgr.h>
#include <Win/Win_Framework.h>

//===== クラス実装 =====
DebugImguiMgr::DebugImguiMgr() noexcept
{
    //初期化処理
    IMGUI_CHECKVERSION();
    if (ImGui::CreateContext() == nullptr) {

#ifdef _DEBUG

        PrintD(L"Error : IMGUI初期化失敗!\n");

#endif // _DEBUG

    }

    //スタイル設定
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.5f, 0.5f, 0.5f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);

    //フォント読込
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = nullptr;
    font = io.Fonts->AddFontFromFileTTF("Asset/Font/UDEVGothicJPDOC-Regular.ttf",
        16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(font != NULL);
}

DebugImguiMgr::~DebugImguiMgr() noexcept
{
    ImGui::DestroyContext();
}

//DEMOウィンドウ描画
void DebugImguiMgr::DrawDemo() const noexcept
{
    static bool bShowDemo = true;
    if (bShowDemo)
        ImGui::ShowDemoWindow(&bShowDemo);
}
