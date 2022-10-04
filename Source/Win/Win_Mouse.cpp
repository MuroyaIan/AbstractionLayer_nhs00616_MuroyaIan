//==============================================================================
// Filename: Win_Mouse.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_Mouse.h>
#include <Win/Win_Framework.h>

//===== クラス実装 =====

//***** マウスイベント ****
WinMouseEvents::WinMouseEvents() noexcept : m_type(WinMouseStatus::Invalid), m_info()
{
}

WinMouseEvents::WinMouseEvents(WinMouseStatus type, const WinMouseInfo& parent) noexcept :
    m_type(type), m_info(parent)
{
}

WinMouseEvents::~WinMouseEvents() noexcept
{
}

//***** マウス処理 *****
WinMouse::WinMouse() noexcept :
    m_buffer(), m_info(), m_wheelDelta(0), m_wheelVal(0), m_rawDeltaBuffer(), m_bUseRawInput(false)
{
}

WinMouse::~WinMouse() noexcept
{
}

//マウスXY座標取得
std::pair<int, int> WinMouse::GetPos() const noexcept
{
    return { m_info.nPosX, m_info.nPosY };
}

//マウスX座標取得
int WinMouse::GetPosX() const noexcept
{
    return m_info.nPosX;
}

//マウスY座標取得
int WinMouse::GetPosY() const noexcept
{
    return m_info.nPosY;
}

//ホイール値取得
int WinMouse::GetWheelVal() const noexcept
{
    return m_wheelVal;
}

//ウィンドウ内にいるか確認
bool WinMouse::IsInWindow() const noexcept
{
    return m_info.bIsInWindow;
}

//マウス左クリック確認
bool WinMouse::LeftIsPressed() const noexcept
{
    return m_info.bLeftIsPressed;
}

//マウス右クリック確認
bool WinMouse::RightIsPressed() const noexcept
{
    return m_info.bRightIsPressed;
}

//バッファ読込み
WinMouseEvents WinMouse::ReadBuffer() noexcept
{
    //例外処理
    if (m_buffer.empty())
        return WinMouseEvents();

    const WinMouseEvents event = m_buffer.front();  //次のイベントにアクセス
    m_buffer.pop();                                 //イベントを削除する
    return event;

}

//バッファの空き確認
bool WinMouse::IsEmpty() const noexcept
{
    return m_buffer.empty();
}

//バッファのクリア
void WinMouse::ClearBuffer() noexcept
{
    m_buffer = std::queue<WinMouseEvents>();
}

//RawInput バッファ読込み
WinMouse::RawDelta WinMouse::ReadRawDelta() noexcept
{
    //例外処理
    if (m_rawDeltaBuffer.empty())
        return RawDelta();

    const RawDelta data = m_rawDeltaBuffer.front();     //次のイベントにアクセス
    m_rawDeltaBuffer.pop();                             //イベントを削除する
    return data;
}

//RawInput 使用制御
void WinMouse::SetRawInput(bool bUse) noexcept
{
    m_bUseRawInput = bUse;
}

//RawInput 使用状態確認
bool WinMouse::IsUsingRawInput() const noexcept
{
    return m_bUseRawInput;
}

//バッファ切り捨て
void WinMouse::TruncateBuffer() noexcept
{
    while (m_buffer.size() > m_nBufferSize)     //上限サイズに収まるまで
        m_buffer.pop();                         //キューポップ
}

//マウスが移動した
void WinMouse::MouseMove(int posX, int posY) noexcept
{
    m_info.nPosX = posX;
    m_info.nPosY = posY;                                //座標格納
    m_buffer.push({ WinMouseStatus::Move, m_info });    //キュープッシュ
    TruncateBuffer();                                   //バッファ上限管理
}

//マウスがウィンドウ外に行った
void WinMouse::LeaveWindow() noexcept
{
    m_info.bIsInWindow = false;
    m_buffer.push({ WinMouseStatus::LeaveWindow, m_info });
    TruncateBuffer();
}

//マウスがウィンドウ内に入った
void WinMouse::Enterwindow() noexcept
{
    m_info.bIsInWindow = true;
    m_buffer.push({ WinMouseStatus::EnterWindow, m_info });
    TruncateBuffer();
}

//マウスが左クリックした
void WinMouse::LeftPressed() noexcept
{
    m_info.bLeftIsPressed = true;
    m_buffer.push({ WinMouseStatus::L_Press, m_info });
    TruncateBuffer();
}

//マウスが左クリックをやめた
void WinMouse::LeftReleased() noexcept
{
    m_info.bLeftIsPressed = false;
    m_buffer.push({ WinMouseStatus::L_Release, m_info });
    TruncateBuffer();
}

//マウスが右クリックした
void WinMouse::RightPressed() noexcept
{
    m_info.bRightIsPressed = true;
    m_buffer.push({ WinMouseStatus::R_Press, m_info });
    TruncateBuffer();
}

//マウスが右クリックをやめた
void WinMouse::RightReleased() noexcept
{
    m_info.bRightIsPressed = false;
    m_buffer.push({ WinMouseStatus::R_Release, m_info });
    TruncateBuffer();
}

//ホイールアップ
void WinMouse::WheelUp() noexcept
{
    m_buffer.push({ WinMouseStatus::WheelUp, m_info });
    TruncateBuffer();
    m_wheelVal++;
}

//ホイールダウン
void WinMouse::WheelDown() noexcept
{
    m_buffer.push({ WinMouseStatus::WheelDown, m_info });
    TruncateBuffer();
    m_wheelVal--;
}

//ホイール処理
void WinMouse::WheelProc(int nDelta) noexcept
{
    m_wheelDelta += nDelta;

    //ホイール応答精度制御(1回のメッセージで応答)
    static int nResponse = WHEEL_DELTA * 1;
    if (m_wheelDelta >= nResponse) {
        m_wheelDelta = 0;
        WheelUp();
    }
    if (m_wheelDelta <= -nResponse) {
        m_wheelDelta = 0;
        WheelDown();
    }
}

//RawInput バッファ切り捨て
void WinMouse::TruncateRawInputBuffer() noexcept
{
    while (m_rawDeltaBuffer.size() > m_nBufferSize)     //上限サイズに収まるまで
        m_rawDeltaBuffer.pop();                         //キューポップ
}

//RawInput 情報取得
void WinMouse::GetRawDelta(int dx, int dy) noexcept
{
    m_rawDeltaBuffer.push({ dx, dy });
    TruncateRawInputBuffer();
}
