//==============================================================================
// Filename: Gfx_DrawerRef.h
// Description: 描画処理(中間クラス)
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_Drawer.h>
#include <Gfx/Binder/Gfx_IndexBuffer.h>

//===== クラス定義 =====

//***** ドロワーテンプレ *****
template<class T>
class GfxDrawerEx : public GfxDrawer
{
protected:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxDrawerEx() noexcept : GfxDrawer() {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxDrawerEx() noexcept override {}

    //--------------------------------------------------------------------------
    /// 初期化確認
    ///
    /// \return bool型変数(静的バインダが初期化したかどうか)
    //--------------------------------------------------------------------------
    bool StaticIsInit() const noexcept
    {
        return !ms_aBinder.empty();
    }

    //--------------------------------------------------------------------------
    /// バインダ登録（静的）
    ///
    /// \param[in] pBinder  登録先バインダのポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void AddStaticBind(
        /*[in]*/ std::unique_ptr<GfxBinder> pBinder)
    {
        //例外処理
        if (pBinder == nullptr)
            throw ERROR_EX2(S_OK, "【警告】ポインタはNULLです！");

        //複数インデックスのバインドを防止
        if (typeid(*pBinder) == typeid(GfxIndexBuffer)) {
            if (ms_pIndexBuffer != nullptr)
                throw ERROR_EX2(S_OK, "【警告】m_pIndexBufferは上書きされます！");
            ms_pIndexBuffer = dynamic_cast<GfxIndexBuffer*>(pBinder.get());
        }

        //所有権移行
        ms_aBinder.push_back(std::move(pBinder));
    }

    //--------------------------------------------------------------------------

private:

    //型省略
    using vec_upBinder = std::vector<std::unique_ptr<GfxBinder>>;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// 静的バインダ配列参照
    ///
    /// \return 静的バインダ配列の参照先
    //--------------------------------------------------------------------------
    const vec_upBinder& GetStaticBind() const noexcept override
    {
        return ms_aBinder;
    }

    //--------------------------------------------------------------------------
    /// 静的インデックスバッファ参照
    ///
    /// \return 静的インデックスバッファの参照先
    //--------------------------------------------------------------------------
    const GfxIndexBuffer& GetStaticIndexBuffer() const noexcept override
    {
        return *ms_pIndexBuffer;
    }

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static vec_upBinder ms_aBinder;
    static const GfxIndexBuffer* ms_pIndexBuffer;
    //--------------------------------------------------------------------------

    /// <summary>
    /// ms_aBinder          バインダのポインタ配列（静的）
    /// ms_pIndexBuffer     インデックスバッファのポインタ（所有権なし、重複利用）
    /// </summary>
};

//===== 静的メンバ変数 =====
template<class T>
std::vector<std::unique_ptr<GfxBinder>> GfxDrawerEx<T>::ms_aBinder{};

template<class T>
const GfxIndexBuffer* GfxDrawerEx<T>::ms_pIndexBuffer = nullptr;
