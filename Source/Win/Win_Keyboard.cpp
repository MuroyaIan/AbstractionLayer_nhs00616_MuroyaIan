//==============================================================================
// Filename: Win_Keyboard.cpp
// Description: キーボード処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_Keyboard.h>

//===== クラス実装 =====

//***** キーイベント *****
WinKeyEvents::WinKeyEvents() noexcept : m_type(WinKeyStatus::Invalid), m_code(0u)
{
}

WinKeyEvents::WinKeyEvents(WinKeyStatus type, unsigned char code) noexcept :
    m_type(type), m_code(code)
{
}

//***** キーボード処理 *****
WinKeyEvents::~WinKeyEvents() noexcept
{
}

WinKeyboard::WinKeyboard() noexcept :
    m_keyStates(0uL), m_keyBuffer(), m_charBuffer()
{
}

WinKeyboard::~WinKeyboard() noexcept
{
}

//キー押し確認
bool WinKeyboard::KeyIsPressed(unsigned char keyCode) const noexcept
{
    return m_keyStates[keyCode];
}

//キー読込み
WinKeyEvents WinKeyboard::ReadKey() noexcept
{
    if (m_keyBuffer.size() > 0u) {
        WinKeyEvents event = m_keyBuffer.front();   //次のイベントにアクセス
        m_keyBuffer.pop();                          //イベントを削除する
        return event;
    }
    else
        return WinKeyEvents();
}

//キーバッファの空き確認
bool WinKeyboard::KeyIsEmpty() const noexcept
{
    return m_keyBuffer.empty();
}

//文字読込み
unsigned char WinKeyboard::ReadChar() noexcept
{
    if (m_charBuffer.size() > 0u) {
        unsigned char charCode = m_charBuffer.front();  //次のイベントにアクセス
        m_charBuffer.pop();                             //イベントを削除する
        return charCode;
    }
    else
        return 255u;
}

//テキストバッファの空き確認
bool WinKeyboard::CharIsEmpty() const noexcept
{
    return m_charBuffer.empty();
}

//キーバッファのクリア
void WinKeyboard::ClearKeyBuffer() noexcept
{
    m_keyBuffer = std::queue<WinKeyEvents>();
}

//テキストバッファのクリア
void WinKeyboard::ClearCharBuffer() noexcept
{
    m_charBuffer = std::queue<unsigned char>();
}

//全バッファのクリア
void WinKeyboard::ClearBuffer() noexcept
{
    ClearKeyBuffer();
    ClearCharBuffer();
}

//キーを押した
void WinKeyboard::KeyPressed(unsigned char keyCode) noexcept
{
    m_keyStates.set(keyCode);                                       //状態格納
    m_keyBuffer.push(WinKeyEvents(WinKeyStatus::Press, keyCode));   //キュープッシュ
    TruncateBuffer(m_keyBuffer);                                    //バッファ上限管理
}

//キーを離した
void WinKeyboard::KeyReleased(unsigned char keyCode) noexcept
{
    m_keyStates.reset(keyCode);                                     //状態格納
    m_keyBuffer.push(WinKeyEvents(WinKeyStatus::Release, keyCode)); //キュープッシュ
    TruncateBuffer(m_keyBuffer);                                    //バッファ上限管理
}

//テキストが入力された
void WinKeyboard::CharInput(unsigned char character) noexcept
{
    m_charBuffer.push(character);   //キュープッシュ
    TruncateBuffer(m_charBuffer);   //バッファ上限管理
}

//キー状態リセット
void WinKeyboard::ClearState() noexcept
{
    m_keyStates.reset();
}
