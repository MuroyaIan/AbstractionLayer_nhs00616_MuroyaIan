﻿//==============================================================================
// Filename: Win_Keyboard.h
// Description: キーボード処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <bitset>               //ビット集合クラス
#include <queue>                //FIFOコンテナ

//===== クラス定義 =====

//***** キー状態 *****
enum class WinKeyStatus
{
    Press,
    Release,
    Invalid
};

//***** キーイベント *****
class WinKeyEvents
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinKeyEvents() noexcept;

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] type     キーの状態
    /// \param[in] code     仮想キーコードの値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinKeyEvents(
        /*[in]*/ WinKeyStatus type,
        /*[in]*/ unsigned char code) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~WinKeyEvents() noexcept;

    //--------------------------------------------------------------------------
    /// キー押し確認
    ///
    /// \return bool型変数(キー押し判定)
    //--------------------------------------------------------------------------
    bool IsPress() const noexcept
    {
        return (m_type == WinKeyStatus::Press) ? true : false;
    }

    //--------------------------------------------------------------------------
    /// キー離し確認
    ///
    /// \return bool型変数(キー離し判定)
    //--------------------------------------------------------------------------
    bool IsRelease() const noexcept
    {
        return (m_type == WinKeyStatus::Release) ? true : false;
    }

    //--------------------------------------------------------------------------
    /// キーイベント有効確認
    ///
    /// \return bool型変数(キーイベント有効判定)
    //--------------------------------------------------------------------------
    bool IsValid() const noexcept
    {
        return (m_type == WinKeyStatus::Invalid) ? true : false;
    }

    //--------------------------------------------------------------------------
    /// キーコード取得
    ///
    /// \return キーコード変数(unsigned char)
    //--------------------------------------------------------------------------
    unsigned char GetCode() const noexcept
    {
        return m_code;
    }
    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    WinKeyStatus m_type;
    unsigned char m_code;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_type  キーの状態
    /// m_code  仮想キーコードの値
    /// </summary>
};

//***** キーボード処理 *****
class WinKeyboard
{
    //--------------------------------------------------------------------------
    friend class WinDirectX;
    //--------------------------------------------------------------------------

public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinKeyboard() noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~WinKeyboard() noexcept;

    //--------------------------------------------------------------------------
    /// キー押し確認
    ///
    /// \param[in] keyCode  確認したいキーの値
    ///
    /// \return エラー情報の文字列
    //--------------------------------------------------------------------------
    bool KeyIsPressed(
        /*[in]*/ unsigned char keyCode) const noexcept;

    //--------------------------------------------------------------------------
    /// キー読込み
    ///
    /// \return 格納されたキー情報
    //--------------------------------------------------------------------------
    WinKeyEvents ReadKey() noexcept;

    //--------------------------------------------------------------------------
    /// キーバッファの空き確認
    ///
    /// \return bool型変数(キーバッファが空いてるかどうか)
    //--------------------------------------------------------------------------
    bool KeyIsEmpty() const noexcept;

    //--------------------------------------------------------------------------
    /// 文字読込み
    ///
    /// \return 格納された文字情報
    //--------------------------------------------------------------------------
    unsigned char ReadChar() noexcept;

    //--------------------------------------------------------------------------
    /// テキストバッファの空き確認
    ///
    /// \return bool型変数(テキストバッファが空いてるかどうか)
    //--------------------------------------------------------------------------
    bool CharIsEmpty() const noexcept;

    //--------------------------------------------------------------------------
    /// キーバッファのクリア
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ClearKeyBuffer() noexcept;

    //--------------------------------------------------------------------------
    /// テキストバッファのクリア
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ClearCharBuffer() noexcept;

    //--------------------------------------------------------------------------
    /// 全バッファのクリア
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ClearBuffer() noexcept;

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// バッファ切り捨て
    ///
    /// \param[in] buffer    入力データ保持用バッファ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    template<typename T>
    static void TruncateBuffer(
        /*[in]*/ std::queue<T>& buffer) noexcept
    {
        while (buffer.size() > m_nBufferSize)   //上限サイズに収まるまで
            buffer.pop();                       //キューポップ
    }

    //--------------------------------------------------------------------------
    /// キーを押した
    ///
    /// \param[in] keyCode  押したキーの値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void KeyPressed(
        /*[in]*/ unsigned char keyCode) noexcept;

    //--------------------------------------------------------------------------
    /// キーを離した
    ///
    /// \param[in] keyCode  離したキーの値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void KeyReleased(
        /*[in]*/ unsigned char keyCode) noexcept;

    //--------------------------------------------------------------------------
    /// テキストが入力された
    ///
    /// \param[in] character  入力された文字の値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void CharInput(
        /*[in]*/ unsigned char character) noexcept;

    //--------------------------------------------------------------------------
    /// キー状態リセット
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ClearState() noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static constexpr unsigned int m_nKeys = 256u;
    static constexpr unsigned int m_nBufferSize = 16u;
    std::bitset<m_nKeys> m_keyStates;
    std::queue<WinKeyEvents> m_keyBuffer;
    std::queue<unsigned char> m_charBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_nKeys         キー数
    /// m_nBufferSize   キューのサイズ（フレームごとの最大受付数）
    /// m_keyStates     256個のキーに対応するビット集合
    /// m_keyBuffer     キーイベント用キュー
    /// m_charBuffer    テキスト入力用キュー
    /// </summary>
};
