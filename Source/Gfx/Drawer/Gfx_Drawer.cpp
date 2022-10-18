////==============================================================================
//// Filename: Gfx_Drawer.cpp
//// Description: 描画処理（親クラス）
//// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
////==============================================================================
//
////===== インクルード部 =====
//#include <Gfx/Drawer/Gfx_Drawer.h>
//#include <Gfx/Binder/Gfx_IndexBuffer.h>
//#include <Gfx/Binder/Gfx_VertexBuffer.h>
//
////===== クラス実装 =====
//GfxDrawer::GfxDrawer() noexcept :
//    m_aBinder(), m_pIndexBuffer(nullptr), m_instanceNum(0), m_pVertexBuffer(nullptr)
//{
//}
//
//GfxDrawer::~GfxDrawer() noexcept
//{
//}
//
////インデックス数取得
//UINT GfxDrawer::GetIndexNum() const noexcept
//{
//    if (m_pIndexBuffer != nullptr)
//        return m_pIndexBuffer->GetIndexNum();
//    else
//        return GetStaticIndexBuffer().GetIndexNum();
//}
//
////書込み処理
//void GfxDrawer::Draw(GfxMain& gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
//{
//    for (auto& b : m_aBinder)                                   //バインド処理
//        b->Bind(gfx);
//    for (auto& sb : GetStaticBind())                            //バインド処理（静的）
//        sb->Bind(gfx);
//
//    if (m_pIndexBuffer != nullptr) {
//        if (!bDrawInstance)
//            gfx.DrawIndexed(m_pIndexBuffer->GetIndexNum());     //フレームバッファ書込み
//        else
//            gfx.DrawInstanced(m_pIndexBuffer->GetIndexNum(), m_instanceNum);
//    }
//    else {
//        if (!bDrawInstance)                                     //インデックスバッファ固定の場合
//            gfx.DrawIndexed(GetStaticIndexBuffer().GetIndexNum());
//        else
//            gfx.DrawInstanced(GetStaticIndexBuffer().GetIndexNum(), m_instanceNum);
//    }
//}
//
////バインダ登録
//void GfxDrawer::AddBind(std::unique_ptr<GfxBinder> pBinder)
//{
//    //例外処理
//    if (pBinder == nullptr)
//        throw ERROR_EX2(S_OK, "【警告】ポインタはNULLです！");
//
//    //複数インデックスのバインドを防止
//    if (typeid(*pBinder) == typeid(GfxIndexBuffer)) {
//        if (m_pIndexBuffer != nullptr)
//            throw ERROR_EX2(S_OK, "【警告】m_pIndexBufferは上書きされます！");
//        m_pIndexBuffer = dynamic_cast<GfxIndexBuffer*>(pBinder.get());
//    }
//
//    //頂点バッファのポインタを取得
//    if (typeid(*pBinder) == typeid(GfxVertexBuffer)) {
//        if (m_pVertexBuffer != nullptr)
//            throw ERROR_EX2(S_OK, "【警告】m_pVertexBufferは上書きされます！");
//        m_pVertexBuffer = dynamic_cast<GfxVertexBuffer*>(pBinder.get());
//    }
//
//    //所有権移行
//    m_aBinder.push_back(std::move(pBinder));
//}
