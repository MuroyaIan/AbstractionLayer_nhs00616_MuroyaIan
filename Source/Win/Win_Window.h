﻿//==============================================================================
// Filename: Win_Window.h
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Framework.h>
#include <crtdbg.h>
#include <Win/Win_ErrorOutput.h>
#include <Win/Win_Keyboard.h>
#include <Win/Win_Mouse.h>

//===== クラス定義 =====

//***** Windowエラー出力 *****
class WinErrorProc : public WinErrorOutput
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] nLine    エラーの発生位置（何行目）
    /// \param[in] chFile   エラーが発生したファイル名
    /// \param[in] hr       メッセージ識別子
    /// \param[in] str      カスタムエラー情報
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinErrorProc(
        /*[in]*/ int nLine,
        /*[in]*/ const char* chFile,
        /*[in]*/ HRESULT hr,
        /*[in]*/ const char* str = nullptr) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~WinErrorProc() noexcept override;

    //--------------------------------------------------------------------------
    /// エラータイプ取得
    ///
    /// \return void
    //--------------------------------------------------------------------------
    const char* GetErrorType() const noexcept override;

    //--------------------------------------------------------------------------
    /// エラーコード情報取得
    ///
    /// \param[in] hr   メッセージ識別子
    ///
    /// \return void
    //--------------------------------------------------------------------------
    std::string GetErrorCodeInfo(
        /*[in]*/ HRESULT hr) const noexcept;

    //--------------------------------------------------------------------------
    /// エラー情報出力
    ///
    /// \return void
    //--------------------------------------------------------------------------
    const char* what() const noexcept override;

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    HRESULT m_hr;
    std::string m_strError;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_hr        メッセージ識別子
    /// m_strError  カスタムエラー情報
    /// </summary>
};

// 追加マクロ定義：Windowエラー出力用
#define ERROR_DEFAULT() WinErrorProc{__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError())}
#define ERROR_EX(hr) WinErrorProc{__LINE__, __FILE__, (hr)}
#define ERROR_EX2(hr, str) WinErrorProc{__LINE__, __FILE__, (hr), (str)}

//***** Windowクラス *****
class WinWindowClass
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// クラス名取得
    ///
    /// \return クラス名
    //--------------------------------------------------------------------------
    static const LPCWSTR GetName() noexcept
    {
        return CLASS_NAME;
    }

    //--------------------------------------------------------------------------
    /// Instanceハンドル取得
    ///
    /// \return Instanceハンドル
    //--------------------------------------------------------------------------
    static const HINSTANCE GetInstance() noexcept
    {
        return m_wndClass.m_hAppInst;
    }
    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinWindowClass();

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~WinWindowClass() noexcept(false);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static WinWindowClass m_wndClass;
    static constexpr const LPCWSTR CLASS_NAME = L"WindowClass_Game";
    HINSTANCE m_hAppInst;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_wndClass  シングルトン
    /// CLASS_NAME  クラス名
    /// m_strError  Instanceハンドル
    /// </summary>
};

//***** ゲームWindow *****
class WinWindow
{
    //--------------------------------------------------------------------------
    friend WinWindowClass;
    //--------------------------------------------------------------------------

public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] windowName   ウィンドウ名
    /// \param[in] nWndWidth    ウィンドウ幅（クライアント領域）
    /// \param[in] nWndHeight   ウィンドウ高さ（クライアント領域）
    /// \param[in] nWndPosX     ウィンドウの左上位置（x軸）
    /// \param[in] nWndPosY     ウィンドウの左上位置（y軸）
    ///
    /// \return void
    //--------------------------------------------------------------------------
    WinWindow(
        /*[in]*/ LPCWSTR windowName,
        /*[in]*/ int nWndWidth,
        /*[in]*/ int nWndHeight,
        /*[in]*/ int nWndPosX = CW_USEDEFAULT,
        /*[in]*/ int nWndPosY = CW_USEDEFAULT);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~WinWindow() noexcept(false);

    //--------------------------------------------------------------------------
    /// トランスフォーム
    ///
    /// \param[in] nWndPosX     ウィンドウの左上位置（x軸）
    /// \param[in] nWndPosY     ウィンドウの左上位置（y軸）
    /// \param[in] nWndWidth    ウィンドウ幅（クライアント領域）
    /// \param[in] nWndHeight   ウィンドウ高さ（クライアント領域）
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Transform(
        /*[in]*/ int nWndPosX,
        /*[in]*/ int nWndPosY,
        /*[in]*/ int nWndWidth = 0,
        /*[in]*/ int nWndHeight = 0);

    //--------------------------------------------------------------------------
    /// タイトル出力
    ///
    /// \param[in] text     出力文字列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void TitlePrint(
        /*[in]*/ const std::string& text) const;

    //--------------------------------------------------------------------------
    /// タイトル出力（マウス座標）
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void TitlePrint_MousePos() const;

    //--------------------------------------------------------------------------
    /// タイトル出力（ホイール値）
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void TitlePrint_WheelVal();

    //--------------------------------------------------------------------------
    /// マウス使用
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void EnableCursor() noexcept;

    //--------------------------------------------------------------------------
    /// マウス不使用
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DisableCursor() noexcept;

    //--------------------------------------------------------------------------
    /// マウス使用状態確認
    ///
    /// \return bool型変数(マウスが使用中かどうか)
    //--------------------------------------------------------------------------
    bool IsUsingCursor() const noexcept;

    //--------------------------------------------------------------------------
    /// ウィンドウ左上座標取得
    ///
    /// \return ウィンドウ左上座標(int, int)
    //--------------------------------------------------------------------------
    std::pair<int, int> GetPos() const noexcept
    {
        return { m_posX, m_posY };
    }

    //--------------------------------------------------------------------------
    /// ウィンドウサイズ取得
    ///
    /// \return ウィンドウサイズ
    //--------------------------------------------------------------------------
    std::pair<int, int> GetSize() const noexcept
    {
        return { m_width, m_height };
    }

    //--------------------------------------------------------------------------
    /// ウィンドウハンドル取得
    ///
    /// \return ウィンドウハンドル
    //--------------------------------------------------------------------------
    const HWND GetHandle() const noexcept
    {
        return m_hWindow;
    }
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    WinKeyboard m_keyboard;
    WinMouse m_mouse;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_keyboard  キーボード処理用インスタンス
    /// m_mouse     マウス処理用インスタンス
    /// </summary>

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// ウィンドウプロシージャ（初期化処理用）
    ///
    /// \param[in] hWnd     ウィンドウハンドル
    /// \param[in] uMsg     メッセージ情報
    /// \param[in] wParam   追加メッセージ情報
    /// \param[in] lParam   追加メッセージ情報
    ///
    /// \return メッセージ処理の結果
    //--------------------------------------------------------------------------
    static LRESULT CALLBACK WndProc_Init(
        /*[in]*/ HWND hWnd,
        /*[in]*/ UINT uMsg,
        /*[in]*/ WPARAM wParam,
        /*[in]*/ LPARAM lParam) noexcept;    //WndProc初期化

    //--------------------------------------------------------------------------
    /// ウィンドウプロシージャ（更新処理の呼び出し用）
    ///
    /// \param[in] hWnd     ウィンドウハンドル
    /// \param[in] uMsg     メッセージ情報
    /// \param[in] wParam   追加メッセージ情報
    /// \param[in] lParam   追加メッセージ情報
    ///
    /// \return メッセージ処理の結果
    //--------------------------------------------------------------------------
    static LRESULT CALLBACK WndProc_Call(
        /*[in]*/ HWND hWnd,
        /*[in]*/ UINT uMsg,
        /*[in]*/ WPARAM wParam,
        /*[in]*/ LPARAM lParam) noexcept;    //WndProc呼び出し

    //--------------------------------------------------------------------------
    /// ウィンドウプロシージャ（更新処理用）
    ///
    /// \param[in] hWnd     ウィンドウハンドル
    /// \param[in] uMsg     メッセージ情報
    /// \param[in] wParam   追加メッセージ情報
    /// \param[in] lParam   追加メッセージ情報
    ///
    /// \return メッセージ処理の結果
    //--------------------------------------------------------------------------
    LRESULT WndProc(
        /*[in]*/ HWND hWnd,
        /*[in]*/ UINT uMsg,
        /*[in]*/ WPARAM wParam,
        /*[in]*/ LPARAM lParam) noexcept;                        //WndProc本処理

    //--------------------------------------------------------------------------
    /// マウス表示
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void HideCursor() noexcept;

    //--------------------------------------------------------------------------
    /// マウス非表示
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void ShowCursor() noexcept;

    //--------------------------------------------------------------------------
    /// マウスをウィンドウ内にロック
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void LockCursor() noexcept;

    //--------------------------------------------------------------------------
    /// マウスロック解除
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void UnlockCursor() noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    int m_posX;
    int m_posY;
    int m_width;
    int m_height;
    HWND m_hWindow;
    bool m_bDrawCursor;
    std::vector<BYTE> m_rawBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_posX          ウィンドウの左上位置（x軸）
    /// m_posY          ウィンドウの左上位置（y軸）
    /// m_width         ウィンドウ幅（クライアント領域）
    /// m_height        ウィンドウ高さ（クライアント領域）
    /// m_hWindow       ウィンドウハンドル
    /// m_bDrawCursor   マウス描画フラグ
    /// m_rawBuffer     RawInput用バッファ
    /// </summary>
};
