//==============================================================================
// Filename: Win_App.cpp
// Description: アプリケーション処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_App.h>
#include <Draw/Draw_CameraMgr.h>
#include <Draw/Draw_LightMgr.h>
#include <Draw/Draw_Shape.h>
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
    m_window(WINDOW_NAME, static_cast<int>(SCREEN_WIDTH),
    static_cast<int>(SCREEN_HEIGHT), WND_POS_X, WND_POS_Y), m_message(), m_time(),
    m_pGfxMgr(), m_pShaderMgr(), m_pGfx(), m_pInputMgr(), m_pCameraMgr(), m_pLightMgr(),
    m_aDrawer(0), m_shapeID(0), m_pSunLight(), m_rot()
{
    //DirectX初期化
    m_pGfxMgr = std::make_unique<GfxMgr>(m_window);

    //シェーダMgr初期化
    m_pShaderMgr = std::make_unique<DrawShaderMgr>(*m_pGfxMgr->GetGfx());

    ////描画データ初期化
    m_pGfx = std::make_unique<GfxPack>(*m_pGfxMgr->GetGfx(), *m_pShaderMgr);

    //入力マネージャ初期化
    m_pInputMgr = std::make_unique<ToolInputMgr>(*this);

    //カメラマネージャ初期化
    m_pCameraMgr = std::make_unique<DrawCameraMgr>(*this);

    //ライトマネージャ初期化
    m_pLightMgr = std::make_unique<DrawLightMgr>(*this);

    //【描画テスト】
    m_aDrawer.reserve(7);
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::BOX));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::PYRAMID));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::CONE));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::PRISM));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::CYLINDER));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::SPHERE));
    m_aDrawer.push_back(std::make_unique<DrawShapeModel>(*m_pGfx, GfxVsdMaker::Shape::PLANE));

    //生成処理
    for (auto& d : m_aDrawer)
        d->AddInstance();

    //カメラ初期化
    m_pCameraMgr->SetCamera(DrawCameraMgr::CameraID::TEST);

    //太陽光初期化
    m_pSunLight = std::make_unique<DrawDirectionalLight>(*this);

    m_window.m_mouse.SetRawInput(true);
}

App64::~App64()
{
}

//アプリケーション実行
int App64::Run()
{
    //実行ループ
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

        //API判定（終了待ち）
        if (GfxMain::CheckApiVer(GfxMain::API_MODE::NONE))
            continue;

        //FPS判定
        if (!m_time.Update())
            continue;

        //ゲーム処理
        Update();
        Draw();

        //GfxAPI切替（ウィンドウ再起動）
        if (m_pInputMgr->m_kb.GetTrigger(VK_Q))
            m_pGfxMgr->SetAPI(GfxMain::API_MODE::DX_11);
        else if (m_pInputMgr->m_kb.GetTrigger(VK_E))
            m_pGfxMgr->SetAPI(GfxMain::API_MODE::DX_12);
    }
}

//更新処理
void App64::Update()
{
    //入力処理更新
    m_pInputMgr->Update();

    //カメラマネージャ更新
    m_pCameraMgr->Update();

    //太陽光更新
    m_pSunLight->Update();

    //3D更新
    for (auto& d : m_aDrawer)
        d->Update();

    //モデル回転制御
    if (m_pInputMgr->m_mouse.GetPress(VK_LBUTTON) && !m_window.IsUsingImgui()) {
        dx::XMINT2 mouseMove = m_pInputMgr->m_mouse.GetMoveVal();
        m_rot.y -= static_cast<int>(mouseMove.x * 0.5f);
        m_rot.x -= static_cast<int>(mouseMove.y * 0.5f);
        if (m_rot.x > 180)
            m_rot.x -= 360;
        else if (m_rot.x < -180)
            m_rot.x += 360;
        if (m_rot.y > 180)
            m_rot.y -= 360;
        else if (m_rot.y < -180)
            m_rot.y += 360;
        DrawShapeModel::m_rotX = m_rot.x;
        DrawShapeModel::m_rotY = m_rot.y;
    }
}

//描画処理
void App64::Draw()
{
    //描画開始
    m_pGfxMgr->GetGfx()->BeginFrame(0.0f, 0.0f, 0.0f);

    //カメラマネージャ描画
    m_pCameraMgr->Draw();

    //ライトマネージャ描画
    m_pLightMgr->Draw();

    //3D描画
    m_aDrawer[m_shapeID]->Draw(*m_pGfxMgr->GetGfx());

#ifdef IMGUI

    //ImGui描画
    if (m_pDX->IsImGuiEnabled()) {

        //テストメニュー
        if (ImGui::Begin(U8(u8"デバッグUI")))
        {
            //性能計測
            static float fpsHist[10]{ 0.0f };                               //計測履歴
            static unsigned short histSlot = 0;                             //計測スロット
            static unsigned short tgtFPS =
                static_cast<unsigned short>(m_time.GetTargetFPS() / 2);     //計測時刻（フレーム単位)
            histSlot++;                                                     //スロット更新
            if (histSlot >= _countof(fpsHist) * tgtFPS)
                histSlot = 0;
            float fFps = static_cast<float>(m_time.GetFPS());
            if (histSlot % tgtFPS == 0) {
                for (int i = 1; i < _countof(fpsHist); i++)                 //データシフト
                    fpsHist[i - 1] = fpsHist[i];
                fpsHist[_countof(fpsHist) - 1] = fFps;                      //履歴追加
            }
            ImGui::Text(U8(u8"FPS計測"));
            ImGui::SameLine();
            ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
            ImGui::PlotLines(U8(u8"RT図"), fpsHist, IM_ARRAYSIZE(fpsHist), 0, 0, 0.0f, 60.0f);

        //    //ポリゴン数表示
        //    UINT polyNum = 0;
        //    for (auto& d : m_aDrawer)
        //        polyNum += d->GetPolygonNum();
        //    ImGui::NewLine();
        //    ImGui::Text(U8(u8"ポリゴン数（三角形）")); ImGui::SameLine();
        //    ImGui::Text(": %d", polyNum);

            //入力情報表示
            ImGui::NewLine();
            if (ImGui::TreeNode(U8(u8"入力デバイス"))) {

                //マウス情報
                ImGui::Text(U8(u8"　マウス"));

                dx::XMFLOAT2 mousePos = m_pInputMgr->m_mouse.GetPos();
                ImGui::Text(U8(u8"座標")); ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine();
                ImGui::Text(": %d", static_cast<int>(mousePos.x)); ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine();
                ImGui::Text(": %d", static_cast<int>(mousePos.y));

                ImGui::Text(U8(u8"ホイール値")); ImGui::SameLine();
                ImGui::Text(": %d", m_pInputMgr->m_mouse.GetWheelVal());

                ImGui::Text(U8(u8"マウス表示")); ImGui::SameLine(); ImGui::Text(":"); ImGui::SameLine();
                if (m_window.IsUsingCursor())
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "True");
                else {
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "False");

                    dx::XMINT2 mouseMove = m_pInputMgr->m_mouse.GetMoveVal();
                    ImGui::Text(U8(u8"マウス移動量"));
                    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine();
                    ImGui::Text(": %d", mouseMove.x); ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine();
                    ImGui::Text(": %d", mouseMove.y);
                }

                ImGui::TreePop();
            }

        //    //モデル情報
        //    if (ImGui::TreeNode(U8(u8"モデル情報"))) {

        //        //モデル切替
        //        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"モデルID")); ImGui::SameLine();
        //        ImGui::Text(": %d", m_shapeID);
        //        ImGui::SameLine();
        //        if (ImGui::Button("<")) {
        //            if (m_shapeID > 0)
        //                m_shapeID--;
        //        }
        //        ImGui::SameLine();
        //        if (ImGui::Button(">")) {
        //            if (m_shapeID < m_aDrawer.size() - 1)
        //                m_shapeID++;
        //        }

        //        //出力処理
        //        DirectX::XMFLOAT4X4 mtxW = m_aDrawer[0]->GetTransformMtx();
        //        ImGui::Text(U8(u8"　位置")); ImGui::SameLine(); ImGui::Text("(cm)");
        //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", mtxW._41); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", mtxW._42); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine();
        //        ImGui::Text(": %.1f", mtxW._43);

        //        ImGui::Text(U8(u8"　回転")); ImGui::SameLine(); ImGui::Text("(deg.)");
        //        ImGui::SliderInt(U8(u8"X軸"), &m_rot.x, -180, 180);
        //        ImGui::SliderInt(U8(u8"Y軸"), &m_rot.y, -180, 180);
        //        ImGui::SliderInt(U8(u8"Z軸"), &DrawShapeModel::m_rotZ, -180, 180);
        //        if (ImGui::Button(U8(u8"回転リセット"))) {
        //            m_rot.x = 0;
        //            m_rot.y = 0;
        //            DrawShapeModel::m_rotX = m_rot.x;
        //            DrawShapeModel::m_rotY = m_rot.y;
        //            DrawShapeModel::m_rotZ = 0;
        //        }

        //        ImGui::TreePop();
        //    }

        //    //カメラ情報
        //    if (ImGui::TreeNode(U8(u8"カメラ情報"))) {

        //        //出力処理
        //        DirectX::XMFLOAT4X4 mtxW = m_pCameraMgr->GetWorldMtx();
        //        ImGui::Text(U8(u8"　位置")); ImGui::SameLine(); ImGui::Text("(cm)");
        //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", mtxW._41); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", mtxW._42); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine();
        //        ImGui::Text(": %.1f", mtxW._43);

        //        DirectX::XMFLOAT3 rot = m_pCameraMgr->GetRotation();
        //        ImGui::Text(U8(u8"　回転")); ImGui::SameLine(); ImGui::Text("(deg.)");
        //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine();
        //        ImGui::Text(": %d ", gMath::GetDegree(rot.x)); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine();
        //        ImGui::Text(": %d ", gMath::GetDegree(rot.y)); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine();
        //        ImGui::Text(": %d", gMath::GetDegree(rot.z));

        //        ImGui::TreePop();
        //    }

        //    //ライト情報
        //    if (ImGui::TreeNode(U8(u8"ライト情報"))) {
        //        auto& lightDataD = m_pSunLight->GetData();
        //        auto& lightDataA = m_pLightMgr->GetData();

        //        //平行光源
        //        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"平行光源"));

        //        ImGui::Text(U8(u8"光の向き"));
        //        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", lightDataD.pos.x); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine();
        //        ImGui::Text(": %.1f ", lightDataD.pos.y); ImGui::SameLine();
        //        ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine();
        //        ImGui::Text(": %.1f", lightDataD.pos.z);

        //        ImGui::Text(U8(u8"色情報"));
        //        ImGui::ColorEdit3(U8(u8"拡散色"), &lightDataD.colorD.x);
        //        ImGui::Text(U8(u8"拡散光強度")); ImGui::SameLine();
        //        ImGui::Text(": %.2f", lightDataD.colorD.w);

        //        //環境光
        //        ImGui::NewLine();
        //        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"環境光"));
        //        ImGui::ColorEdit3(U8(u8"環境色"), &lightDataA.ambientLight.x);
        //        ImGui::Text(U8(u8"環境光強度")); ImGui::SameLine();
        //        ImGui::Text(": %.2f", lightDataA.ambientLight.w);

        //        ImGui::TreePop();
        //    }
        }
        ImGui::End();
    }

#endif // IMGUI

    //描画終了
    m_pGfxMgr->GetGfx()->EndFrame();
}
