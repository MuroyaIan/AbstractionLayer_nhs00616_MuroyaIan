//==============================================================================
// Filename: Gfx_cbMaterial.cpp
// Description: マテリアル情報用定数バッファ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

////===== インクルード部 =====
//#include <Gfx/Binder/Gfx_cbMaterial.h>
////#include <GraphicApp/Drawer/vsdRef.h>
//
//using pcbMta = GfxPixelCBuffer<MATERIAL_DATA>;
//
////===== 静的メンバ変数 =====
//std::unique_ptr<pcbMta> CB_MATERIAL::m_pPcBuff = nullptr;
//
////===== クラス実装 =====
//CB_MATERIAL::CB_MATERIAL(GfxMain& Gfx, const MATERIAL_DATA& Material) :
//    GfxBinder(), m_Material(Material)
//{
//    //定数バッファ初期化
//    if (!m_pPcBuff)
//        m_pPcBuff = std::make_unique<pcbMta>(Gfx, static_cast<UINT>(CB_SLOT_PS::MATERIAL));
//}
//
//CB_MATERIAL::~CB_MATERIAL() noexcept
//{
//}
//
////バインド処理
//void CB_MATERIAL::Bind(GfxMain& Gfx) noexcept
//{
//    //バッファ更新
//    m_pPcBuff->Update(Gfx, m_Material);
//
//    //バインド処理
//    m_pPcBuff->Bind(Gfx);
//}
