//==============================================================================
// Filename: Win_App.cpp
// Description: アプリケーション処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_App.h>
//#include <Tool/Rand.h>
//#include <EditorApp/Editor.h>
//
//#ifdef IMGUI
//#
//#    include <Geometry/Debug/DebugMgr.h>
//#
//#endif // IMGUI
//
//#include <Camera/CameraMgr.h>
//#include <Light/LightMgr.h>
//
//
//
//#include <Geometry/Shape_Default.h>            //【描画テスト】
//#include <Geometry/Shape_Tex.h>
//#include <Geometry/Shape_Model.h>
//#include <Geometry/Model/Model.h>
//#include <Light/DirectionalLight.h>
//#include <Light/PointLight.h>
//#include <Geometry/Model/TestPlayer.h>
//
//namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr LPCWSTR WINDOW_NAME = L"Game_Base";    //Window名
constexpr float SCREEN_WIDTH = 1600.0f;
constexpr float SCREEN_HEIGHT = 900.0f;            //画面解像度
constexpr int WND_POS_X = 150;
constexpr int WND_POS_Y = 50;                    //Window左上座標

//===== クラス実装 =====
APP_64::APP_64() :
    m_Window(WINDOW_NAME, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), WND_POS_X, WND_POS_Y), m_Message(), m_Time(),
    m_pDX()//, m_pShaderMgr(), m_pTextureMgr(), m_pModelMgr(), m_pGfx(), m_pInputMgr(), m_pEditor(), m_pCameraMgr(), m_pLightMgr(),
    //m_aDrawer(0), m_aModel(0), m_pSunLight(), m_aLight(4), m_pPlayer()
{
    //DirectX初期化
    m_pDX = std::make_unique<GRAPHIC>(m_Window.GetHandle(), SCREEN_WIDTH, SCREEN_HEIGHT);

//    //シェーダMgr初期化
//    m_pShaderMgr = std::make_unique<SHADER_MGR>(*m_pDX);
//
//    //テクスチャMgr初期化
//    m_pTextureMgr = std::make_unique<TEXTURE_MGR>(*m_pDX);
//
//    //モデルMgr初期化
//    m_pModelMgr = std::make_unique<MODEL_MGR>();
//
//    //描画データ初期化
//    m_pGfx = std::make_unique<GFX_PACK>(*m_pDX, *m_pShaderMgr, *m_pTextureMgr, *m_pModelMgr);
//
//    //入力マネージャ初期化
//    m_pInputMgr = std::make_unique<INPUT_MGR>(*this);
//
//    //乱数初期化
//    RAND_MAKER::InitMaker();
//
//    //エディタ初期化
//    m_pEditor = std::make_unique<EDITOR>(*this);
//
//#ifdef IMGUI
//
//    //デバッグマネージャ初期化
//    m_pDebugMgr = std::make_unique<DEBUG_MGR>(*this);
//
//#endif // IMGUI
//
//    //カメラマネージャ初期化
//    m_pCameraMgr = std::make_unique<CAMERA_MGR>(*this);
//
//    //ライトマネージャ初期化
//    m_pLightMgr = std::make_unique<LIGHT_MGR>(*this);
//
//
//
//    //【描画テスト】
//    m_aDrawer.reserve(10);
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::BOX));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::PYRAMID));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::CONE));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::PRISM));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::CYLINDER));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::SPHERE));
//    m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::PLANE));
//    m_aDrawer.push_back(std::make_unique<SHAPE_TEX>(*m_pGfx, VSD_MAKER::SHAPE::PLANE, TEXTURE_MGR::TEX_ID::TEX_TestPlane));
//    m_aDrawer.push_back(std::make_unique<SHAPE_TEX>(*m_pGfx, VSD_MAKER::SHAPE::BOX, TEXTURE_MGR::TEX_ID::TEX_TestBox));
//    m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::SPHERE));
//
//    //形状生成用ラムダ式
//    std::vector<std::unique_ptr<DRAWER>>& aDrawer = m_aDrawer;
//    auto MakeGeom = [&aDrawer]()
//    {
//        enum class SHAPE
//        {
//            BOX,
//            PYRAMID,
//            CONE,
//            PRISM,
//            CYLINDER,
//            SPHERE,
//            PLANE,
//            PLANE_TEX,
//            BOX_TEX,
//            MODEL,
//
//            MAX_NUM
//        };
//
//        SHAPE Shape = static_cast<SHAPE>(RAND_MAKER::MakeRand_Int(9, 9));
//        //SHAPE shape = static_cast<SHAPE>(RAND_MAKER::MakeRand_Int(0, static_cast<int>(SHAPE::MAX_NUM) - 1));
//        aDrawer[static_cast<int>(Shape)]->AddInstance();
//        return;
//    };
//
//    //生成処理
//    //for (int i = 0; i < nDrawNum; i++)
//    //    MakeGeom();
//
//    //【モデルテスト】
//    //m_aModel.reserve(1);
//    //m_aModel.push_back(std::make_unique<MODEL>(*m_pGfx, MODEL_MGR::MODEL_ID::Tarantella));
//    //for (size_t i = 0; i < 1600; i++)
//    //    m_aModel[0]->AddInstance();
//
//
//
//    //カメラ初期化
//    m_pCameraMgr->SetCamera(CAMERA_MGR::CAMERA_ID::FP);
//
//    //太陽光初期化
//    m_pSunLight = std::make_unique<DIRECTIONAL_LIGHT>(*this);
//
//    //点光源初期化
//    //for(auto& l : m_aLight)
//    //    l = std::make_unique<POINT_LIGHT>(*this);
//    //m_aLight[0]->GetData().Color_D = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
//    //m_aLight[1]->GetData().Color_D = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
//    //m_aLight[2]->GetData().Color_D = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
//    //m_aLight[3]->GetData().Color_D = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
//    //m_aLight[0]->GetData().Pos = DirectX::XMFLOAT4( 8.0f, 2.0f,  8.0f, 1.0f);
//    //m_aLight[1]->GetData().Pos = DirectX::XMFLOAT4(-8.0f, 2.0f,  8.0f, 1.0f);
//    //m_aLight[2]->GetData().Pos = DirectX::XMFLOAT4( 8.0f, 2.0f, -8.0f, 1.0f);
//    //m_aLight[3]->GetData().Pos = DirectX::XMFLOAT4(-8.0f, 2.0f, -8.0f, 1.0f);
//
//
//
//    m_pPlayer = std::make_unique<PLAYER>(*this);
}

APP_64::~APP_64()
{
}

//アプリケーション実行
int APP_64::Run()
{
    //ゲームループ
    while (true)
    {
        //メッセージ処理
        if (PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE)) {

            //終了処理
            if (m_Message.message == WM_QUIT)
                return static_cast<int>(m_Message.wParam);

            //その他
            TranslateMessage(&m_Message);
            DispatchMessage(&m_Message);
        }

        //FPS判定
        if (!m_Time.Update())
            continue;

        //ゲーム処理
        Update();
        Draw();
    }
}

//更新処理
void APP_64::Update()
{
//    //入力処理更新
//    m_pInputMgr->Update();
//
//    //エディタ更新
//    if (m_pEditor->IsEnabled())        //モード確認
//        m_pEditor->Update();
//    else {
//
//        //カメラマネージャ更新
//        m_pCameraMgr->Update();
//
//        //太陽光更新
//        m_pSunLight->Update();
//
//        //点光源更新
//        //for (auto& l : m_aLight)
//        //    l->Update();
//
//        //3D更新
//        for (auto& d : m_aDrawer)
//            d->Update();
//        for (auto& m : m_aModel)
//            m->Update();
//        m_pPlayer->Update();
//    }
//
//#ifdef IMGUI
//
//    //デバッグマネージャ更新
//    m_pDebugMgr->Update();
//
//    //ImGui描画制御
//    if (m_pInputMgr->m_KB.GetPress(VK_TAB))
//        m_pDX->SetImGuiMode(false);
//    else
//        m_pDX->SetImGuiMode(true);
//
//    //マウス表示・非表示
//    if (m_pInputMgr->m_KB.GetTrigger(VK_Q)) {
//        if (m_Window.IsUsingCursor()) {
//            m_Window.DisableCursor();
//            m_Window.m_Mouse.SetRawInput(true);
//        }
//        else {
//            m_Window.EnableCursor();
//            m_Window.m_Mouse.SetRawInput(false);
//        }
//    }
//
//#endif // IMGUI

}

//描画処理
void APP_64::Draw()
{
    //描画開始
    m_pDX->BeginFrame(0.0f, 0.0f, 0.0f);



//    //モード切替
//    if (m_pEditor->IsEnabled())
//        m_pEditor->Draw();            //エディタモード
//    else {
//
//        //カメラマネージャ描画
//        m_pCameraMgr->Draw();
//
//        //ライトマネージャ描画
//        m_pLightMgr->Draw();
//
//        //3D描画
//        for (auto& d : m_aDrawer)
//            d->Draw(*m_pDX);
//        for (auto& m : m_aModel)
//            m->Draw();
//        m_pPlayer->Draw();
//
//
//
//#ifdef IMGUI
//
//        //デバッグマネージャ描画
//        m_pDebugMgr->Draw(*m_pDX);
//
//        //ImGui描画
//        if (m_pDX->IsImGuiEnabled()) {
//
//            //テストメニュー
//            if (ImGui::Begin(U8(u8"コントロールパネル")))
//            {
//                //性能計測
//                static float FpsHist[10]{ 0.0f };                                                        //計測履歴
//                static unsigned short HistSlot = 0;                                                        //計測スロット
//                static unsigned short TgtFPS = static_cast<unsigned short>(m_Time.GetTargetFPS() / 2);    //計測時刻（フレーム単位)
//                HistSlot++;                                                                                //スロット更新
//                if (HistSlot >= _countof(FpsHist) * TgtFPS)
//                    HistSlot = 0;
//                float fFps = static_cast<float>(m_Time.GetFPS());
//                if (HistSlot % TgtFPS == 0) {
//                    for (int i = 1; i < _countof(FpsHist); i++)                //データシフト
//                        FpsHist[i - 1] = FpsHist[i];
//                    FpsHist[_countof(FpsHist) - 1] = fFps;                    //履歴追加
//                }
//                ImGui::Text(U8(u8"FPS計測"));
//                ImGui::SameLine();
//                ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
//                ImGui::PlotLines(U8(u8"RT図"), FpsHist, IM_ARRAYSIZE(FpsHist), 0, 0, 0.0f, 60.0f);
//
//                //ポリゴン数表示
//                UINT PolyNum = 0;
//                for (auto& d : m_aDrawer)
//                    PolyNum += d->GetPolygonNum();
//                for (auto& m : m_aModel)
//                    PolyNum += m->GetPolygonNum();
//                PolyNum += m_pDebugMgr->GetPolygonNum();
//                ImGui::NewLine();
//                ImGui::Text(U8(u8"ポリゴン数（三角形）")); ImGui::SameLine(); ImGui::Text(": %d", PolyNum);
//
//                //入力情報表示
//                ImGui::NewLine();
//                if (ImGui::TreeNode(U8(u8"入力デバイス"))) {
//
//                    //マウス情報
//                    ImGui::Text(U8(u8"　マウス"));
//
//                    dx::XMFLOAT2 MousePos = m_pInputMgr->m_Mouse.GetPos();
//                    ImGui::Text(U8(u8"座標")); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.x)); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.y));
//
//                    ImGui::Text(U8(u8"ホイール値")); ImGui::SameLine(); ImGui::Text(": %d", m_pInputMgr->m_Mouse.GetWheelVal());
//
//                    ImGui::Text(U8(u8"マウス表示")); ImGui::SameLine(); ImGui::Text(":"); ImGui::SameLine();
//                    if (m_Window.IsUsingCursor())
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "True");
//                    else {
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "False");
//
//                        dx::XMINT2 MouseMove = m_pInputMgr->m_Mouse.GetMoveVal();
//                        ImGui::Text(U8(u8"マウス移動量"));
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.x); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.y);
//                    }
//
//                    //コントローラ情報
//                    ImGui::Text(U8(u8"　パッド"));
//                    ImGui::Text(U8(u8"接続番号")); ImGui::SameLine(); ImGui::Text(":");
//                    if (m_pInputMgr->m_GamePad.CheckPad(PAD_1)) {
//                        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_1 + 1);
//                    }
//                    else if (m_pInputMgr->m_GamePad.CheckPad(PAD_2)) {
//                        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_2 + 1);
//                    }
//                    else if (m_pInputMgr->m_GamePad.CheckPad(PAD_3)) {
//                        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_3 + 1);
//                    }
//                    else if (m_pInputMgr->m_GamePad.CheckPad(PAD_4)) {
//                        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_4 + 1);
//                    }
//                    else {
//                        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "None");
//                    }
//
//                    if (m_pInputMgr->m_GamePad.CheckPad(PAD_1)) {
//                        dx::XMFLOAT2 StickL3 = m_pInputMgr->m_GamePad.GetStickL3(PAD_1);
//                        dx::XMFLOAT2 StickR3 = m_pInputMgr->m_GamePad.GetStickR3(PAD_1);
//                        ImGui::Text(U8(u8"スティック"));
//                        ImGui::Text("(L3)"); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", StickL3.x); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f", StickL3.y);
//                        ImGui::Text("(R3)"); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", StickR3.x); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f", StickR3.y);
//
//                        ImGui::Text(U8(u8"トリガー"));
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "L2"); ImGui::SameLine(); ImGui::Text(": %.1f ", m_pInputMgr->m_GamePad.GetValL2(PAD_1)); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "R2"); ImGui::SameLine(); ImGui::Text(": %.1f", m_pInputMgr->m_GamePad.GetValR2(PAD_1));
//
//                        ImGui::Text(U8(u8"振動")); ImGui::SameLine(); ImGui::Text(":");
//                        if (m_pInputMgr->m_GamePad.GetVibeL(PAD_1) == 0.0f && m_pInputMgr->m_GamePad.GetVibeR(PAD_1) == 0.0f) {
//                            ImGui::SameLine(); ImGui::Text("Off");
//                        }
//                        else {
//                            ImGui::SameLine(); ImGui::Text("On");
//                        }
//                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "L"); ImGui::SameLine(); ImGui::Text(": %.1f ", m_pInputMgr->m_GamePad.GetVibeL(PAD_1)); ImGui::SameLine();
//                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "R"); ImGui::SameLine(); ImGui::Text(": %.1f", m_pInputMgr->m_GamePad.GetVibeR(PAD_1));
//                    }
//
//
//
//                    ImGui::TreePop();
//                }
//
//                //カメラ情報
//                if (ImGui::TreeNode(U8(u8"カメラ情報"))) {
//
//                    //出力処理
//                    DirectX::XMFLOAT4X4 mtxW = m_pCameraMgr->GetWorldMtx();
//                    ImGui::Text(U8(u8"　位置")); ImGui::SameLine(); ImGui::Text("(cm)");
//                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._41); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._42); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", mtxW._43);
//
//                    DirectX::XMFLOAT3 Rot = m_pCameraMgr->GetRotation();
//                    ImGui::Text(U8(u8"　回転")); ImGui::SameLine(); ImGui::Text("(deg.)");
//                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.x)); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.y)); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %d", gMath::GetDegree(Rot.z));
//
//                    ImGui::TreePop();
//                }
//
//                //ライト情報
//                if (ImGui::TreeNode(U8(u8"ライト情報"))) {
//                    auto& LightData = m_pLightMgr->GetData();
//
//                    //平行光源
//                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"平行光源"));
//
//                    ImGui::Text(U8(u8"光の向き"));
//                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightData.DirectionalLight.Pos.x); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightData.DirectionalLight.Pos.y); ImGui::SameLine();
//                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", LightData.DirectionalLight.Pos.z);
//
//                    ImGui::Text(U8(u8"色情報"));
//                    ImGui::ColorEdit3(U8(u8"拡散色"), &LightData.DirectionalLight.Color_D.x);
//                    ImGui::Text(U8(u8"拡散光強度")); ImGui::SameLine(); ImGui::Text(": %.2f", LightData.DirectionalLight.Color_D.w);
//
//                    //環境光
//                    ImGui::NewLine();
//                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"環境光"));
//                    ImGui::ColorEdit3(U8(u8"環境色"), &LightData.AmbientLight.x);
//                    ImGui::Text(U8(u8"環境光強度")); ImGui::SameLine(); ImGui::Text(": %.2f", LightData.AmbientLight.w);
//
//                    //点光源
//                    ImGui::NewLine();
//                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"点光源"));
//                    ImGui::Text(U8(u8"有効光源数")); ImGui::SameLine(); ImGui::Text(": %d", m_pLightMgr->GetLightNum());
//
//                    ImGui::TreePop();
//                }
//            }
//            ImGui::End();
//        }
//
//#endif // IMGUI
//
//    }

    //描画終了
    m_pDX->EndFrame();
}
