//==============================================================================
// Filename: Win_App.cpp
// Description: アプリケーション処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_App.h>
#include <Draw/Draw_CameraMgr.h>
#include <Draw/Draw_LightMgr.h>

//#include <Geometry/Shape_Default.h>            //【描画テスト】
//#include <Geometry/Shape_Tex.h>
//#include <Geometry/Shape_Model.h>
#include <Draw/Draw_DirectionalLight.h>
#include <Tool/Tool_Math.h>

namespace dx = DirectX;

//===== グローバル定数宣言 =====
constexpr LPCWSTR WINDOW_NAME = L"AbstractionLayer";    //Window名
constexpr float SCREEN_WIDTH = 1600.0f;
constexpr float SCREEN_HEIGHT = 900.0f;                 //画面解像度
constexpr int WND_POS_X = 150;
constexpr int WND_POS_Y = 50;                           //Window左上座標

//===== クラス実装 =====
App64::App64() :
    m_window(WINDOW_NAME, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), WND_POS_X, WND_POS_Y), m_message(), m_time(),
    m_pDX(), m_pShaderMgr()/*, m_pTextureMgr()*/, m_pGfx()/*, m_pInputMgr()*/, m_pCameraMgr()//, m_pLightMgr(),
    /*m_aDrawer(0)*/, m_pSunLight()
{
    //DirectX初期化
    m_pDX = std::make_unique<GfxMain>(m_window.GetHandle(), SCREEN_WIDTH, SCREEN_HEIGHT);

    //シェーダMgr初期化
    m_pShaderMgr = std::make_unique<DrawShaderMgr>(*m_pDX);

//    //テクスチャMgr初期化
//    m_pTextureMgr = std::make_unique<TEXTURE_MGR>(*m_pDX);

    //描画データ初期化
    m_pGfx = std::make_unique<GfxPack>(*m_pDX, *m_pShaderMgr/*, *m_pTextureMgr*/);

//    //入力マネージャ初期化
//    m_pInputMgr = std::make_unique<INPUT_MGR>(*this);

    //カメラマネージャ初期化
    m_pCameraMgr = std::make_unique<DrawCameraMgr>(*this);

    //ライトマネージャ初期化
    m_pLightMgr = std::make_unique<DrawLightMgr>(*this);



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



    //カメラ初期化
    m_pCameraMgr->SetCamera(DrawCameraMgr::CameraID::TEST);

    //太陽光初期化
    m_pSunLight = std::make_unique<DrawDirectionalLight>(*this);
}

App64::~App64()
{
}

//アプリケーション実行
int App64::Run()
{
    //ゲームループ
    while (true)
    {
        //メッセージ処理
        if (PeekMessage(&m_message, nullptr, 0, 0, PM_REMOVE)) {

            //終了処理
            if (m_message.message == WM_QUIT)
                return static_cast<int>(m_message.wParam);

            //その他
            TranslateMessage(&m_message);
            DispatchMessage(&m_message);
        }

        //FPS判定
        if (!m_time.Update())
            continue;

        //ゲーム処理
        Update();
        Draw();
    }
}

//更新処理
void App64::Update()
{
    ////入力処理更新
    //m_pInputMgr->Update();

    //カメラマネージャ更新
    m_pCameraMgr->Update();

    //太陽光更新
    m_pSunLight->Update();

    ////3D更新
    //for (auto& d : m_aDrawer)
    //    d->Update();

//#ifdef IMGUI
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
void App64::Draw()
{
    //描画開始
    m_pDX->BeginFrame(0.0f, 0.0f, 0.0f);



    //カメラマネージャ描画
    m_pCameraMgr->Draw();

    //ライトマネージャ描画
    m_pLightMgr->Draw();

    ////3D描画
    //for (auto& d : m_aDrawer)
    //    d->Draw(*m_pDX);



#ifdef IMGUI

    //ImGui描画
    if (m_pDX->IsImGuiEnabled()) {

        //テストメニュー
        if (ImGui::Begin(U8(u8"デバッグUI")))
        {
            //性能計測
            static float fpsHist[10]{ 0.0f };                                                        //計測履歴
            static unsigned short histSlot = 0;                                                        //計測スロット
            static unsigned short tgtFPS = static_cast<unsigned short>(m_time.GetTargetFPS() / 2);    //計測時刻（フレーム単位)
            histSlot++;                                                                                //スロット更新
            if (histSlot >= _countof(fpsHist) * tgtFPS)
                histSlot = 0;
            float fFps = static_cast<float>(m_time.GetFPS());
            if (histSlot % tgtFPS == 0) {
                for (int i = 1; i < _countof(fpsHist); i++)                //データシフト
                    fpsHist[i - 1] = fpsHist[i];
                fpsHist[_countof(fpsHist) - 1] = fFps;                    //履歴追加
            }
            ImGui::Text(U8(u8"FPS計測"));
            ImGui::SameLine();
            ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
            ImGui::PlotLines(U8(u8"RT図"), fpsHist, IM_ARRAYSIZE(fpsHist), 0, 0, 0.0f, 60.0f);

            ////ポリゴン数表示
            //UINT PolyNum = 0;
            //for (auto& d : m_aDrawer)
            //    PolyNum += d->GetPolygonNum();
            //for (auto& m : m_aModel)
            //    PolyNum += m->GetPolygonNum();
            //PolyNum += m_pDebugMgr->GetPolygonNum();
            //ImGui::NewLine();
            //ImGui::Text(U8(u8"ポリゴン数（三角形）")); ImGui::SameLine(); ImGui::Text(": %d", PolyNum);

            ////入力情報表示
            //ImGui::NewLine();
            //if (ImGui::TreeNode(U8(u8"入力デバイス"))) {

            //    //マウス情報
            //    ImGui::Text(U8(u8"　マウス"));

            //    dx::XMFLOAT2 MousePos = m_pInputMgr->m_Mouse.GetPos();
            //    ImGui::Text(U8(u8"座標")); ImGui::SameLine();
            //    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.x)); ImGui::SameLine();
            //    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.y));

            //    ImGui::Text(U8(u8"ホイール値")); ImGui::SameLine(); ImGui::Text(": %d", m_pInputMgr->m_Mouse.GetWheelVal());

            //    ImGui::Text(U8(u8"マウス表示")); ImGui::SameLine(); ImGui::Text(":"); ImGui::SameLine();
            //    if (m_Window.IsUsingCursor())
            //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "True");
            //    else {
            //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "False");

            //        dx::XMINT2 MouseMove = m_pInputMgr->m_Mouse.GetMoveVal();
            //        ImGui::Text(U8(u8"マウス移動量"));
            //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.x); ImGui::SameLine();
            //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.y);
            //    }

            //    ImGui::TreePop();
            //}

            //カメラ情報
            if (ImGui::TreeNode(U8(u8"カメラ情報"))) {

                //出力処理
                DirectX::XMFLOAT4X4 mtxW = m_pCameraMgr->GetWorldMtx();
                ImGui::Text(U8(u8"　位置")); ImGui::SameLine(); ImGui::Text("(cm)");
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._41); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._42); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", mtxW._43);

                DirectX::XMFLOAT3 Rot = m_pCameraMgr->GetRotation();
                ImGui::Text(U8(u8"　回転")); ImGui::SameLine(); ImGui::Text("(deg.)");
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.x)); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.y)); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %d", gMath::GetDegree(Rot.z));

                ImGui::TreePop();
            }

            //ライト情報
            if (ImGui::TreeNode(U8(u8"ライト情報"))) {
                auto& LightData = m_pLightMgr->GetData();

                //平行光源
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"平行光源"));

                ImGui::Text(U8(u8"光の向き"));
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightData.directionalLight.pos.x); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightData.directionalLight.pos.y); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", LightData.directionalLight.pos.z);

                ImGui::Text(U8(u8"色情報"));
                ImGui::ColorEdit3(U8(u8"拡散色"), &LightData.directionalLight.colorD.x);
                ImGui::Text(U8(u8"拡散光強度")); ImGui::SameLine(); ImGui::Text(": %.2f", LightData.directionalLight.colorD.w);

                //環境光
                ImGui::NewLine();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"環境光"));
                ImGui::ColorEdit3(U8(u8"環境色"), &LightData.ambientLight.x);
                ImGui::Text(U8(u8"環境光強度")); ImGui::SameLine(); ImGui::Text(": %.2f", LightData.ambientLight.w);

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

#endif // IMGUI

    //描画終了
    m_pDX->EndFrame();
}
