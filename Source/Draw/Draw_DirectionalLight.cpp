//==============================================================================
// Filename: Draw_DirectionalLight.cpp
// Description: 平行光源
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_DirectionalLight.h>

namespace dx = DirectX;

//===== クラス実装 =====
DrawDirectionalLight::DrawDirectionalLight(App64& app) noexcept : DrawLight(app),
    m_lightMgr(app.GetLightMgr()), m_lightData()
{
    m_lightData.colorD = { 1.0f, 1.0f, 1.0f, 1.0f };
}

DrawDirectionalLight::~DrawDirectionalLight() noexcept
{
}

//更新処理
void DrawDirectionalLight::Update() const noexcept
{
    //データ更新
    m_lightMgr.GetData().directionalLight = m_lightData;
}
