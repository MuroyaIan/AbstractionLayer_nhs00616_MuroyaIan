//==============================================================================
// Filename: Tool_Keyboard.cpp
// Description: キーボード受付処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Tool/Input/Tool_Keyboard.h>
#include <Win/Win_App.h>

//===== クラス実装 =====
ToolKeyboard::ToolKeyboard(App64& app) noexcept :
    m_app(app), m_state(0x0000),
    m_press(), m_trigger(), m_release(), m_repeat(), m_repeatCnt(), m_repeatInterval(),
    m_pPress(nullptr), m_pTrigger(nullptr), m_pRelease(nullptr),
    m_pRepeat(nullptr), m_pRepeatCnt(nullptr), m_pRepeatInterval(nullptr)
{
    //判定用情報初期化
    uShort repeatInterval =
        static_cast<uShort>(m_app.GetTimeProc().GetTargetFPS() * 0.5f);     //リピート間隔（デフォルト0.5秒）
    for (auto& ri : m_repeatInterval)
        ri = repeatInterval;
}

ToolKeyboard::~ToolKeyboard() noexcept
{
    //ポインタ解放
    m_pPress = nullptr;
    m_pTrigger = nullptr;
    m_pRelease = nullptr;
    m_pRepeat = nullptr;
    m_pRepeatCnt = nullptr;
    m_pRepeatInterval = nullptr;
}

//更新処理
void ToolKeyboard::Update() noexcept
{
    //ポインタリセット
    m_pPress = m_press;
    m_pTrigger = m_trigger;
    m_pRelease = m_release;
    m_pRepeat = m_repeat;
    m_pRepeatCnt = m_repeatCnt;
    m_pRepeatInterval = m_repeatInterval;

    //キーボード状態取得
    for (int cnt = 0; cnt < MAX_NUM_VK; cnt++) {
        m_state = GetAsyncKeyState(cnt);                    //キーの情報を取得

        //判定情報算出
        *m_pTrigger = (*m_pPress ^ m_state) & m_state;      //トリガー情報
        *m_pRelease = (*m_pPress ^ m_state) & ~m_state;     //リリース情報
        *m_pRepeat = *m_pTrigger;                           //リピート情報

        //リピート制御
        if (m_state & 0x8000) {                                         //押し続ける間
            *m_pRepeatCnt += 1;
            if (!(*m_pRepeatCnt % *m_pRepeatInterval))                  //指定時間後
                *m_pRepeat = m_state;                                   //0x8000をセット（リピート許可）
            if (*m_pRepeatCnt >= 0xFFFF - 0xFFFF % *m_pRepeatInterval)  //カウンタ上限設定
                *m_pRepeatCnt = 0;
        }
        else {
            *m_pRepeatCnt = 0;
            *m_pRepeat = 0x0000;    //リセット処理
        }

        //情報記録
        *m_pPress = m_state;        //押下情報取得　⇒　次回のビット演算用

        //ポインタ更新
        m_pPress++;
        m_pTrigger++;
        m_pRelease++;
        m_pRepeat++;
        m_pRepeatCnt++;
        m_pRepeatInterval++;
    }
}

//プレス判定
bool ToolKeyboard::GetPress(int nKey) const noexcept
{
    return (static_cast<int>(m_press[nKey]) & 0x8000) ? true : false;
}

//トリガー判定
bool ToolKeyboard::GetTrigger(int nKey) const noexcept
{
    return (static_cast<int>(m_trigger[nKey]) & 0x8000) ? true : false;
}

//リリース判定
bool ToolKeyboard::GetRelease(int nKey) const noexcept
{
    return (static_cast<int>(m_release[nKey]) & 0x8000) ? true : false;
}

//リピート判定
bool ToolKeyboard::GetRepeat(int nKey, float fInterval)
{
    //例外処理
    if (fInterval <= 0.0f)
        throw ERROR_EX2(S_OK, "リピート間隔は0以上にしてください。");

    m_repeatInterval[nKey] =
        static_cast<uShort>(m_app.GetTimeProc().GetTargetFPS() * fInterval);    //リピート間隔更新
    return (static_cast<int>(m_repeat[nKey]) & 0x8000) ? true : false;
}
