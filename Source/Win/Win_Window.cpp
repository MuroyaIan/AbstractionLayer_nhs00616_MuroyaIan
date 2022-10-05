//==============================================================================
// Filename: Win_Window.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_Window.h>
#include <Resource.h>

#ifdef IMGUI
#
#   include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#
#endif // IMGUI

//===== 追加ライブラリ =====
#pragma comment(lib, "imm32")

//===== 静的メンバ変数 =====
WinWindowClass WinWindowClass::m_wndClass;  //シングルトン定義

//===== クラス実装 =====

//***** Windowエラー出力 *****
WinErrorProc::WinErrorProc(int nLine, const char* chFile, HRESULT hr, const char* str) noexcept :
    WinErrorOutput(nLine, chFile), m_hr(hr), m_strError()
{
    if (str != nullptr)
        m_strError = str;
    else
        m_strError.resize(0);
}

WinErrorProc::~WinErrorProc() noexcept
{
}

//エラータイプ取得
const char* WinErrorProc::GetErrorType() const noexcept
{
    return "Windowエラー";
}

//エラーコード情報取得
std::string WinErrorProc::GetErrorCodeInfo(HRESULT hr) const noexcept
{
    //情報用メモリを確保
    LPSTR pMsgBuffer = nullptr;
    const DWORD dwMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);

    //例外処理
    if (dwMsgLen == 0)
        return "未定義のエラー";

    //カスタム例外
    if (hr == S_OK && (!m_strError.empty()))
        return m_strError;

    //情報取得
    std::string strError{ pMsgBuffer };
    LocalFree(pMsgBuffer);                  //メモリ解放
    return std::move(strError);
}

//エラー情報出力
const char* WinErrorProc::what() const noexcept
{
    std::ostringstream oss;
    oss << GetErrorType() << std::endl
        << "[Error Code] " << m_hr << std::endl
        << "[Description] " << GetErrorCodeInfo(m_hr) << std::endl
        << GetErrorInfo();
    m_infoBuffer = oss.str();
    return m_infoBuffer.c_str();
}

//***** Windowクラス *****

//コンストラクタ
WinWindowClass::WinWindowClass() : m_hAppInst(GetModuleHandle(nullptr))
{
    //クラス情報設定
    WNDCLASSEX windowClass{ 0 };
    windowClass.cbSize = static_cast<UINT>(sizeof(WNDCLASSEX));
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = WinWindow::WndProc_Init;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = m_hAppInst;
    windowClass.hIcon =LoadIcon(m_hAppInst,
        MAKEINTRESOURCE(IDI_ABSTRACTIONLAYERNHS00616MUROYAIAN));    //アイコン
    windowClass.hCursor = LoadCursor(m_hAppInst, IDC_ARROW);        //カーソル
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = CLASS_NAME;                 //クラス名
    windowClass.hIconSm =
        LoadIcon(m_hAppInst,MAKEINTRESOURCE(IDI_SMALL));    //小アイコン

    //クラス登録
    if (!RegisterClassEx(&windowClass))
        throw ERROR_DEFAULT();
}

//デストラクタ
WinWindowClass::~WinWindowClass() noexcept(false)
{
    //クラス登録を解除
    if (!UnregisterClass(CLASS_NAME, m_hAppInst))
        throw ERROR_DEFAULT();
}

//***** ゲームWindow *****

//コンストラクタ
WinWindow::WinWindow(LPCWSTR windowName, int nWndWidth, int nWndHeight, int nWndPosX, int nWndPosY) :
    m_posX(nWndPosX), m_posY(nWndPosY), m_width(nWndWidth), m_height(nWndHeight),
    m_hWindow(nullptr), m_bDrawCursor(true), m_rawBuffer(0), m_keyboard(), m_mouse()
{
    //サイズ算出
    DWORD dwExStyle = 0;
    DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    RECT rcWnd =
        { 0L, 0L,
        static_cast<LONG>(m_width), static_cast<LONG>(m_height) };  //Client領域サイズ設定
    if (!AdjustWindowRectEx(&rcWnd, dwStyle, false, dwExStyle))     //Windowサイズ算出
        throw ERROR_DEFAULT();
    int nWidth = rcWnd.right - rcWnd.left;
    int nHeight = rcWnd.bottom - rcWnd.top;                         //幅・高さを設定

    //Instance作成
    m_hWindow = CreateWindowEx(
        dwExStyle, WinWindowClass::GetName(), windowName, dwStyle,
        m_posX, m_posY, nWidth, nHeight,
        nullptr, nullptr, WinWindowClass::GetInstance(), this);
    if (!m_hWindow)
        throw ERROR_DEFAULT();

    //Window表示
    ShowWindow(m_hWindow, SW_SHOW);
    UpdateWindow(m_hWindow);

    //IME無効化
    ImmAssociateContext(m_hWindow, nullptr);

    //RawInputデバイス登録
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01;     //マウス識別用
    rid.usUsage = 0x02;         //マウス識別用
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
        throw ERROR_EX2(S_OK, "RawInput初期化失敗");

#ifdef IMGUI

    //IMGUI初期化
    if (!ImGui_ImplWin32_Init(m_hWindow))
        throw ERROR_EX2(S_OK, "IMGUI初期化失敗");

#endif // IMGUI

}

//デストラクタ
WinWindow::~WinWindow() noexcept(false)
{

#ifdef IMGUI

    //IMGUI終了
    ImGui_ImplWin32_Shutdown();

#endif // IMGUI

    //Instance破棄
    if (!DestroyWindow(m_hWindow))
        throw ERROR_DEFAULT();
}

//トランスフォーム
void WinWindow::Transform(int nWndPosX, int nWndPosY, int nWndWidth, int nWndHeight)
{
    //サイズ取得
    RECT rcClient;                              //クライアント領域
    if (!GetClientRect(m_hWindow, &rcClient))
        throw ERROR_DEFAULT();
    rcClient.right -= rcClient.left;
    rcClient.bottom -= rcClient.top;
    RECT rcWindow;                              //非クライアント領域
    if (!GetWindowRect(m_hWindow, &rcWindow))
        throw ERROR_DEFAULT();
    rcWindow.right -= rcWindow.left;
    rcWindow.bottom -= rcWindow.top;

    //サイズ更新
    int nWidth = 0;
    int nHeight = 0;
    if (nWndWidth <= 0)
        nWidth = rcWindow.right;
    else {
        nWidth = rcWindow.right - rcClient.right + nWndWidth;
        m_width = nWndWidth;
    }
    if (nWndHeight <= 0)
        nHeight = rcWindow.bottom;
    else {
        nHeight = rcWindow.bottom - rcClient.bottom + nWndHeight;
        m_height = nWndHeight;
    }

    //Window移動
    if (!SetWindowPos(m_hWindow, HWND_TOP, nWndPosX, nWndPosY, nWidth, nHeight, SWP_SHOWWINDOW))
        throw ERROR_DEFAULT();
}

//タイトル出力
void WinWindow::TitlePrint(const std::string& text) const
{
    if (!SetWindowTextA(m_hWindow, text.c_str()))
        throw ERROR_DEFAULT();
}

//タイトル出力（マウス座標）
void WinWindow::TitlePrint_MousePos() const
{
    //メッセージボックス表示バグあり
    auto [x, y] = m_mouse.GetPos();
    std::ostringstream oss;
    oss << "MousePos:(" << x << ", " << y << ")";
    TitlePrint(oss.str());
}

//タイトル出力（ホイール値）
void WinWindow::TitlePrint_WheelVal()
{
    while (!m_mouse.IsEmpty()) {

        //変数宣言
        const WinMouseEvents event = m_mouse.ReadBuffer();
        static int nCnt = 0;
        std::ostringstream oss;

        //更新処理
        switch (event.GetType()) {
            case WinMouseStatus::WheelUp:
                nCnt++;
                oss << "ホイール値：" << nCnt;
                TitlePrint(oss.str());
                break;
            case WinMouseStatus::WheelDown:
                nCnt--;
                oss << "ホイール値：" << nCnt;
                TitlePrint(oss.str());
                break;
            default:
                oss << "ホイール値：" << nCnt;
                TitlePrint(oss.str());
                break;
        }
    }
}

//マウス使用・不使用
void WinWindow::EnableCursor() noexcept
{
    m_bDrawCursor = true;
    ShowCursor();
    UnlockCursor();

#ifdef IMGUI

    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

void WinWindow::DisableCursor() noexcept
{
    m_bDrawCursor = false;
    HideCursor();
    LockCursor();

#ifdef IMGUI

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

//マウス使用状態確認
bool WinWindow::IsUsingCursor() const noexcept
{
    return m_bDrawCursor;
}

//WndProc初期化
LRESULT CALLBACK WinWindow::WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    //Instance作成時
    if (uMsg == WM_NCCREATE)
    {
        //Instanceポインタを取得
        const CREATESTRUCT* const pCreateInfo =
            reinterpret_cast<CREATESTRUCT*>(lParam);
        WinWindow* const pGameWnd =
            static_cast<WinWindow*>(pCreateInfo->lpCreateParams);

        //InstanceポインタをWinAPIのユーザデータとして登録（WinAPIはクラスのメンバ関数を認識できない為）
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGameWnd));

        //初期化以後のWndProc呼び出し関数を設定
        SetWindowLongPtr(hWnd, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(&WinWindow::WndProc_Call));

        //WndProc本処理
        return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
    }

    //WM_NCCREATE以前の処理先
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//WndProc呼び出し
LRESULT CALLBACK WinWindow::WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    //Instanceポインタを取得
    WinWindow* const pGameWnd =
        reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    //WndProc本処理
    return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

//WndProc本処理
LRESULT WinWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{

#ifdef IMGUI

    //IMGUI用
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    const ImGuiIO& io = ImGui::GetIO();

#endif // IMGUI

    switch (uMsg) {
        case WM_CLOSE:
            if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"終了確認",
                MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
                return 0;
            PostQuitMessage(0);             //WM_QUITを生成
            return 0;                       //デストラクタ呼び出し

        //ウィンドウアクティブ判定
        case WM_ACTIVATE:

            //マウス状態更新
            if (!m_bDrawCursor) {
                if (wParam & WA_ACTIVE) {
                    LockCursor();           //アクティブ⇒マウスロック
                    HideCursor();
                }
                else {
                    UnlockCursor();         //非アクティブ⇒マウスロック解除
                    ShowCursor();
                }
            }
            break;

        //キーボード処理
        case WM_KILLFOCUS:
            m_keyboard.ClearState();        //ウィンドウ非アクティブ⇒入力リセット
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:                 //「WM_SYSKEY」⇒「ALT」と「F10」を対応

#ifdef IMGUI

            if (io.WantCaptureKeyboard)                 //IMGUI入力切替
                break;

#endif // IMGUI

            switch (wParam) {
                case VK_ESCAPE:                         //「ESC」⇒ウィンドウ終了
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
            }
            if (!(lParam & 0x40000000))                 //キーを押した
                m_keyboard.KeyPressed(static_cast<unsigned char>(wParam));
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:

#ifdef IMGUI

            if (io.WantCaptureKeyboard)                 //IMGUI入力切替
                break;

#endif // IMGUI

            m_keyboard.KeyReleased(static_cast<unsigned char>(wParam));
            break;                                      //キーを離した
        case WM_CHAR:

#ifdef IMGUI

            if (io.WantCaptureKeyboard)                 //IMGUI入力切替
                break;

#endif // IMGUI

            m_keyboard.CharInput(static_cast<unsigned char>(wParam));
            break;                                      //テキストを入力した

        //マウス処理
        case WM_MOUSEMOVE:

            //マウス非表示の場合
            if (!m_bDrawCursor) {
                if (!m_mouse.IsInWindow()) {    //ウィンドウに入った初回ループ
                                                //⇒マウスがウィンドウ外判定の場合
                    SetCapture(hWnd);
                    m_mouse.Enterwindow();      //キャプチャーオン、マウスメッセージ更新
                    HideCursor();               //マウス非表示
                }
                break;                          //マウス座標処理をスキップ
            }

#ifdef IMGUI

            if (io.WantCaptureMouse)            //IMGUI入力切替
                break;

#endif // IMGUI

            {
                POINTS pt = MAKEPOINTS(lParam);
                if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height) {
                    m_mouse.MouseMove(pt.x, pt.y);  //ウィンドウ内の場合⇒座標記録
                    if (!m_mouse.IsInWindow()) {    //マウスキャプチャーオン
                        SetCapture(hWnd);
                        m_mouse.Enterwindow();
                    }
                }
                else {                                          //ウィンドウ外の場合
                    if (wParam & (MK_LBUTTON | MK_RBUTTON))     //クリックしてる場合
                        m_mouse.MouseMove(pt.x, pt.y);          //座標記録
                    else {                                      //クリックしてない場合
                        ReleaseCapture();                       //マウスキャプチャーオフ
                        m_mouse.LeaveWindow();
                    }
                }
            }
            break;
        case WM_LBUTTONDOWN:
            SetForegroundWindow(hWnd);                          //ウィンドウをアクティブ
            if (!m_bDrawCursor) {                               //マウス状態更新
                LockCursor();
                HideCursor();
            }

#ifdef IMGUI

            if (io.WantCaptureMouse)                            //IMGUI入力切替
                break;

#endif // IMGUI

            m_mouse.LeftPressed();
            break;
        case WM_LBUTTONUP:

#ifdef IMGUI

            if (io.WantCaptureMouse)                            //IMGUI入力切替
                break;

#endif // IMGUI

            {
                POINTS pt = MAKEPOINTS(lParam);
                if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height) {
                    ReleaseCapture();       //ウィンドウ外の場合⇒マウスキャプチャーオフ
                    m_mouse.LeaveWindow();
                }
            }
            m_mouse.LeftReleased();
            break;
        case WM_RBUTTONDOWN:

#ifdef IMGUI

            if (io.WantCaptureMouse)        //IMGUI入力切替
                break;

#endif // IMGUI

            m_mouse.RightPressed();
            break;
        case WM_RBUTTONUP:

#ifdef IMGUI

            if (io.WantCaptureMouse)        //IMGUI入力切替
                break;

#endif // IMGUI

            {
                POINTS pt = MAKEPOINTS(lParam);
                if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height) {
                    ReleaseCapture();       //ウィンドウ外の場合⇒マウスキャプチャーオフ
                    m_mouse.LeaveWindow();
                }
            }
            m_mouse.RightReleased();
            break;
        case WM_MOUSEWHEEL:

#ifdef IMGUI

            if (io.WantCaptureMouse)        //IMGUI入力切替
                break;

#endif // IMGUI

            {
                const int nDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                m_mouse.WheelProc(nDelta);  //ホイール操作量取得(メッセージ一回で+-120)
            }
            break;

        //RawInput処理
        case WM_INPUT:
        {
            if (!m_mouse.IsUsingRawInput())
                break;
            UINT size{};

            //入力情報のサイズを取得
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
                break;  //エラーメッセージ
            m_rawBuffer.resize(size);

            //入力情報読込
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                m_rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
                break;  //エラーメッセージ

            // process the raw input data
            auto& ri = reinterpret_cast<const RAWINPUT&>(*m_rawBuffer.data());
            if (ri.header.dwType == RIM_TYPEMOUSE &&
                (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
                m_mouse.GetRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);

            break;
        }

        default:
            break;
    }

    //基本動作をサポート
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//マウス非表示・表示
void WinWindow::HideCursor() noexcept
{
    while (::ShowCursor(FALSE) >= 0);
}

void WinWindow::ShowCursor() noexcept
{
    while (::ShowCursor(TRUE) < 0);
}

//マウスをウィンドウ内にロック・ロック解除
void WinWindow::LockCursor() noexcept
{
    RECT rect;
    GetClientRect(m_hWindow, &rect);
    MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);
                        //ウィンドウ内座標をスクリーン座標へ変換
    ClipCursor(&rect);  //マウスを指定範囲内にロックする
}

void WinWindow::UnlockCursor() noexcept
{
    ClipCursor(nullptr);

    //カーソルを画面中心へ戻す
    RECT rect;
    GetClientRect(m_hWindow, &rect);
    MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);
                                        //ウィンドウ内座標をスクリーン座標へ変換
    int x = rect.left + m_width / 2;
    int y = rect.top + m_height / 2;
    SetCursorPos(x, y);
}
