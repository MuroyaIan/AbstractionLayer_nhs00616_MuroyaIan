//==============================================================================
// Filename: Tool_InputMgr.cpp
// Description: 入力処理マネージャ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Tool/Input/Tool_InputMgr.h>
#include <Win/Win_App.h>

//===== クラス実装 =====
ToolInputMgr::ToolInputMgr(App64& app) noexcept :
    m_kb(app), m_mouse(app, m_kb)
{
}

ToolInputMgr::~ToolInputMgr() noexcept
{
}

//更新処理
void ToolInputMgr::Update() noexcept
{
    //キーボード更新
    m_kb.Update();

    //マウス更新
    m_mouse.Update();
}
