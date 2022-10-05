//==============================================================================
// Filename: Gfx_vsdCone.h
// Description: コーン作成処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_VertexShaderData.h>
#include <Tool/Tool_Math.h>

//===== クラス定義 =====

//***** コーン形状 *****
class VsdCone
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// テッセレーション作成
    ///
    /// \param[in] divNum   分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeTessellation(
        /*[in]*/ int divNum)
    {
        //前処理
        namespace dx = DirectX;
        if (divNum < 3)
            throw ERROR_EX2(S_OK, "分割数は3以上にしてください。");

        //変数宣言
        const dx::XMVECTOR vBase =
            dx::XMVectorSet(0.5f, -0.5f, 0.0f, 0.0f);   //基準用頂点ベクトル
        const float fDivAngle = 2.0f * F_PI / divNum;   //分割用回転角(ラジアン)
        std::vector<V> aData;

        //底部円形上の頂点作成
        for (int i = 0; i < divNum; i++) {
            aData.emplace_back();
            dx::XMVECTOR vVertex =
                dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));    //Y軸回転
            dx::XMStoreFloat3(&aData.back().m_pos, vVertex);
        }

        //底部円形の中心頂点作成
        aData.emplace_back();
        aData.back().m_pos = { 0.0f, -0.5f, 0.0f };
        const unsigned short usCtrIdx =
            static_cast<unsigned short>(aData.size() - 1);  //インデックス参照

        //コーンの先端頂点作成
        aData.emplace_back();
        aData.back().m_pos = { 0.0f, 0.5f, 0.0f };
        const unsigned short usTopIdx = static_cast<unsigned short>(aData.size() - 1);

        //底部円形上のインデックスリスト作成
        std::vector<unsigned short> indices;
        const unsigned short mod = static_cast<unsigned short>(divNum);
        for (unsigned short i = 0; i < divNum; i++) {
            indices.push_back(usCtrIdx);
            indices.push_back((i + 1u) % mod);
            indices.push_back(i);
        }

        //その他のインデックスリスト作成
        for (unsigned short i = 0; i < divNum; i++) {
            indices.push_back(usTopIdx);
            indices.push_back(i);
            indices.push_back((i + 1u) % mod);
        }

        return VsData<V>(std::move(aData), std::move(indices));
    }

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \param[in] divNum   分割数
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    template<class V>
    static VsData<V> MakeData(
        /*[in]*/ int divNum = 24)
    {
        return MakeTessellation<V>(divNum);
    }

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    VsdCone() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~VsdCone() noexcept {}

    //--------------------------------------------------------------------------
};
