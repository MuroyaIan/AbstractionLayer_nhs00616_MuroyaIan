////==============================================================================
//// Filename: Gfx_cbMaterial.cpp
//// Description: マテリアル情報用定数バッファ
//// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
////==============================================================================
//
////===== インクルード部 =====
//#include <Gfx/Binder/Gfx_cbMaterial.h>
//#include <Gfx/Drawer/Gfx_vsdRef.h>
//
//using pcbMta = GfxPixelCBuffer<GfxMaterialData>;
//
////===== 静的メンバ変数 =====
//std::unique_ptr<pcbMta> GfxCBuffMaterial::m_pPcBuff = nullptr;
//
////===== クラス実装 =====
//GfxCBuffMaterial::GfxCBuffMaterial(GfxMain& gfx, const GfxMaterialData& material) :
//    GfxBinder(), m_material(material)
//{
//    //定数バッファ初期化
//    if (!m_pPcBuff)
//        m_pPcBuff = std::make_unique<pcbMta>(gfx, static_cast<UINT>(CB_SLOT_PS::MATERIAL));
//}
//
//GfxCBuffMaterial::~GfxCBuffMaterial() noexcept
//{
//}
//
////バインド処理
//void GfxCBuffMaterial::Bind(GfxMain& gfx) noexcept
//{
//    //バッファ更新
//    m_pPcBuff->Update(gfx, m_material);
//
//    //バインド処理
//    m_pPcBuff->Bind(gfx);
//}
