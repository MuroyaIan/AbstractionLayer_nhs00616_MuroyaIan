//==============================================================================
// Filename: Tool_Mouse.cpp
// Description: マウス受付処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Tool/Input/Tool_Mouse.h>
#include <Win/Win_App.h>
#include <Tool/Input/Tool_Keyboard.h>

//===== クラス実装 =====
ToolMouse::ToolMouse(App64& app, ToolKeyboard& kb) noexcept :
    m_mouse(app.GetWindowProc()->m_mouse), m_kb(kb), moveVal()
{
}

ToolMouse::~ToolMouse() noexcept
{
}

//プレス判定
bool ToolMouse::GetPress(int nBtn) const noexcept
{
    return m_kb.GetPress(nBtn);
}

//トリガー判定
bool ToolMouse::GetTrigger(int nBtn) const noexcept
{
    return m_kb.GetTrigger(nBtn);
}

//リリース判定
bool ToolMouse::GetRelease(int nBtn) const noexcept
{
    return m_kb.GetRelease(nBtn);
}

//リピート判定
bool ToolMouse::GetRepeat(int nBtn, float fInterval)
{
    return m_kb.GetRepeat(nBtn, fInterval);
}

//座標取得
DirectX::XMFLOAT2 ToolMouse::GetPos() const noexcept
{
    auto [x, y] = m_mouse.GetPos();
    return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
}

//ホイール値取得
int ToolMouse::GetWheelVal() const noexcept
{
    return m_mouse.GetWheelVal();
}

//移動量取得
DirectX::XMINT2 ToolMouse::GetMoveVal() const noexcept
{
    return moveVal;
}

//更新処理
void ToolMouse::Update() noexcept
{
    //マウス移動量取得
    if (!m_mouse.IsUsingRawInput())
        return;
    moveVal.x = 0;
    moveVal.y = 0;
    while (true) {
        const auto d = m_mouse.ReadRawDelta();
        moveVal.x += d.x;
        moveVal.y += d.y;

        //脱出処理
        if (d.x == 0 && d.y == 0)
            break;
    }
}
