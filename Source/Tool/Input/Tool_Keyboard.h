//==============================================================================
// Filename: Tool_Keyboard.h
// Description: キーボード受付処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== 定数・マクロ定義 =====
constexpr int VK_0 = static_cast<int>('0');
constexpr int VK_1 = static_cast<int>('1');
constexpr int VK_2 = static_cast<int>('2');
constexpr int VK_3 = static_cast<int>('3');
constexpr int VK_4 = static_cast<int>('4');
constexpr int VK_5 = static_cast<int>('5');
constexpr int VK_6 = static_cast<int>('6');
constexpr int VK_7 = static_cast<int>('7');
constexpr int VK_8 = static_cast<int>('8');
constexpr int VK_9 = static_cast<int>('9');
constexpr int VK_A = static_cast<int>('A');
constexpr int VK_B = static_cast<int>('B');
constexpr int VK_C = static_cast<int>('C');
constexpr int VK_D = static_cast<int>('D');
constexpr int VK_E = static_cast<int>('E');
constexpr int VK_F = static_cast<int>('F');
constexpr int VK_G = static_cast<int>('G');
constexpr int VK_H = static_cast<int>('H');
constexpr int VK_I = static_cast<int>('I');
constexpr int VK_J = static_cast<int>('J');
constexpr int VK_K = static_cast<int>('K');
constexpr int VK_L = static_cast<int>('L');
constexpr int VK_M = static_cast<int>('M');
constexpr int VK_N = static_cast<int>('N');
constexpr int VK_O = static_cast<int>('O');
constexpr int VK_P = static_cast<int>('P');
constexpr int VK_Q = static_cast<int>('Q');
constexpr int VK_R = static_cast<int>('R');
constexpr int VK_S = static_cast<int>('S');
constexpr int VK_T = static_cast<int>('T');
constexpr int VK_U = static_cast<int>('U');
constexpr int VK_V = static_cast<int>('V');
constexpr int VK_W = static_cast<int>('W');
constexpr int VK_X = static_cast<int>('X');
constexpr int VK_Y = static_cast<int>('Y');
constexpr int VK_Z = static_cast<int>('Z');     //仮想キーコード
constexpr int MAX_NUM_VK = 256;                 //仮想キーコードの最大数

//===== 前方宣言 =====
class App64;

//===== クラス定義 =====

//***** キーボード入力 *****
class ToolKeyboard
{
    //--------------------------------------------------------------------------
    friend class ToolInputMgr;
    //--------------------------------------------------------------------------

public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] app  App参照
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ToolKeyboard(
        /*[in]*/ App64& app) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~ToolKeyboard() noexcept;

    //--------------------------------------------------------------------------
    /// プレス判定
    ///
    /// \param[in] nKey  判定したいキーの値
    ///
    /// \return bool型変数(プレスしたかどうか)
    //--------------------------------------------------------------------------
    bool GetPress(
        /*[in]*/ int nKey) const noexcept;

    //--------------------------------------------------------------------------
    /// トリガー判定
    ///
    /// \param[in] nKey  判定したいキーの値
    ///
    /// \return bool型変数(トリガーしたかどうか)
    //--------------------------------------------------------------------------
    bool GetTrigger(
        /*[in]*/ int nKey) const noexcept;

    //--------------------------------------------------------------------------
    /// リリース判定
    ///
    /// \param[in] nKey  判定したいキーの値
    ///
    /// \return bool型変数(リリースしたかどうか)
    //--------------------------------------------------------------------------
    bool GetRelease(
        /*[in]*/ int nKey) const noexcept;

    //--------------------------------------------------------------------------
    /// リピート判定（デフォルト0.5秒）
    ///
    /// \param[in] nKey         判定したいキーの値
    /// \param[in] fInterval    判定する間隔
    ///
    /// \return bool型変数(リピートしたかどうか)
    //--------------------------------------------------------------------------
    bool GetRepeat(
        /*[in]*/ int nKey,
        /*[in]*/ float fInterval = 0.5f);

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// 更新処理
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Update() noexcept;

    //--------------------------------------------------------------------------

    using uShort = unsigned short;

    //--------------------------------------------------------------------------
    App64& m_app;
    uShort m_state;
    uShort m_press[MAX_NUM_VK];
    uShort m_trigger[MAX_NUM_VK];
    uShort m_release[MAX_NUM_VK];
    uShort m_repeat[MAX_NUM_VK];
    uShort m_repeatCnt[MAX_NUM_VK];
    uShort m_repeatInterval[MAX_NUM_VK];
    uShort* m_pPress;
    uShort* m_pTrigger;
    uShort* m_pRelease;
    uShort* m_pRepeat;
    uShort* m_pRepeatCnt;
    uShort* m_pRepeatInterval;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_app               App参照
    /// m_state             入力状態取得用
    /// m_press             押下状態判定用
    /// m_trigger           トリガ状態判定用
    /// m_release           リリース状態判定用
    /// m_repeat            リピート状態判定用
    /// m_repeatCnt         リピートカウンタ
    /// m_repeatInterval    リピート間隔
    /// m_pPress            以下ループ処理用ポインタ
    /// m_pTrigger
    /// m_pRelease
    /// m_pRepeat
    /// m_pRepeatCnt
    /// m_pRepeatInterval
    /// </summary>
};
