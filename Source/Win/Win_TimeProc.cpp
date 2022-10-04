//==============================================================================
// Filename: Win_TimeProc.cpp
// Description: ウィンドウ内の時間処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_TimeProc.h>

using namespace TimeProc;

//===== 定数・マクロ定義 =====
constexpr int SECOND = 1000;        //1秒（ミリ秒）
constexpr int HALF_SECOND = 500;    //0.5秒（ミリ秒）
constexpr int DEFAULT_FPS = 60;     //デフォルトFPS

//===== クラス実装 =====

//***** 時間計測クラス *****
WinTime::WinTime() noexcept
{
    //時間計測用
    m_currentTime = GetTime::now();     //初期化時刻取得
    m_prevExecTime = m_currentTime;     //初期化時刻のバックアップ
    m_execInterval = 0;

    //FPS計算用
    m_prevMeasTime = m_currentTime;     //初期化時刻のバックアップ
    m_measInterval = 0;
    m_measCnt = 0;
    m_measFPS = 0;

    //FPS設定
    m_FPS = DEFAULT_FPS;
    m_limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

WinTime::WinTime(int FPS) noexcept
{
    //時間計測用
    m_currentTime = GetTime::now();     //初期化時刻取得
    m_prevExecTime = m_currentTime;     //初期化時刻のバックアップ
    m_execInterval = 0;

    //FPS計算用
    m_prevMeasTime = m_currentTime;     //初期化時刻のバックアップ
    m_measInterval = 0;
    m_measCnt = 0;
    m_measFPS = 0;

    //FPS設定
    m_FPS = FPS;
    m_limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

WinTime::~WinTime() noexcept
{
}

//更新処理
bool WinTime::Update() noexcept
{
    //計測処理
    m_currentTime = GetTime::now();                                                                     //現在時刻取得
    m_execInterval = Time::duration_cast<Time::microseconds>(m_currentTime - m_prevExecTime).count();
    m_measInterval = Time::duration_cast<Time::milliseconds>(m_currentTime - m_prevMeasTime).count();   //経過時間算出

    //FPS計算（精度ミリ秒）
    if (static_cast<int>(m_measInterval) >= HALF_SECOND) {                                      //計算頻度　⇒　0.5秒/回
        m_measFPS = static_cast<int>(m_measCnt * SECOND / static_cast<float>(m_measInterval));  //FPS計算
        m_prevMeasTime = m_currentTime;                                                         //バックアップ更新
        m_measCnt = 0;                                                                          //カウンタリセット
    }

    //FPSチェック（精度マイクロ秒）
    if (static_cast<int>(m_execInterval) >= m_limiter) {
        m_prevExecTime = m_currentTime;                     //バックアップ更新
        m_measCnt++;                                        //カウンタ加算
        return true;
    }
    else
        return false;
}

//***** タイマクラス *****
WinTime::Timer::Timer() noexcept : m_timerCnt(0), m_time(0)
{
}
WinTime::Timer::~Timer() noexcept
{
}

//更新処理
void WinTime::Timer::Update(int nFPS) noexcept
{
    m_timerCnt++;
    if (m_timerCnt >= nFPS) {
        m_timerCnt = 0;
        m_time++;
    }
}

//タイマリセット
void WinTime::Timer::Reset() noexcept
{
    m_timerCnt = 0;
    m_time = 0;
}
