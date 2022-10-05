//==============================================================================
// Filename: Gfx_vsdRef.h
// Description: 描画処理用作成クラスのインクルードヘッダ
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_vsdBox.h>
#include <Gfx/Drawer/Gfx_vsdCone.h>
#include <Gfx/Drawer/Gfx_vsdPrism.h>
#include <Gfx/Drawer/Gfx_vsdSphere.h>
#include <Gfx/Drawer/Gfx_vsdPlane.h>

////===== 構造体宣言 =====
//struct MATERIAL_DATA                    //マテリアル情報
//{
//    DirectX::XMFLOAT4 Ambient;            //環境光
//    DirectX::XMFLOAT4 Diffuse;            //拡散反射光
//    DirectX::XMFLOAT4 Emissive;            //発射光
//    DirectX::XMFLOAT4 Transparent;        //透過度
//    DirectX::XMFLOAT4 Specular;            //鏡面反射光
//    float Shininess;                    //光沢
//    float Pad1;
//    float Pad2;
//    float Pad3;                            //仮置き
//
//    MATERIAL_DATA() noexcept :
//        Ambient(0.0f, 0.0f, 0.0f, 0.0f), Diffuse(0.0f, 0.0f, 0.0f, 0.0f), Emissive(0.0f, 0.0f, 0.0f, 0.0f), Transparent(0.0f, 0.0f, 0.0f, 0.0f),
//        Specular(0.0f, 0.0f, 0.0f, 0.0f), Shininess(2.0f), Pad1(0.0f), Pad2(0.0f), Pad3(0.0f)
//    {}
//    ~MATERIAL_DATA() noexcept {}
//};
//
//struct INSTANCE_DATA                    //インスタンス情報
//{
//    DirectX::XMFLOAT4X4 MtxWorld;        //ワールド行列
//
//    INSTANCE_DATA() noexcept : MtxWorld()
//    {
//        //ワールド行列初期化
//        DirectX::XMStoreFloat4x4(&MtxWorld, DirectX::XMMatrixIdentity());
//    }
//    ~INSTANCE_DATA() noexcept {}
//};
//
////===== クラス定義 =====
//
////***** 頂点データ作成 *****
//class VSD_MAKER
//{
//public:
//
//    //形状タイプ
//    enum class SHAPE
//    {
//        BOX,
//        PYRAMID,
//        CONE,
//        PRISM,
//        CYLINDER,
//        SPHERE,
//        PLANE,
//
//        MAX_NUM
//    };
//
//    //プロトタイプ宣言
//    static VS_DATA<VERTEX> MakeData_Default(SHAPE Type)        //デフォルトデータ
//    {
//        switch (Type)
//        {
//        case VSD_MAKER::SHAPE::BOX:
//            return VSD_BOX::MakeData<VERTEX>();
//        case VSD_MAKER::SHAPE::PYRAMID:
//            return VSD_CONE::MakeData<VERTEX>(4);
//        case VSD_MAKER::SHAPE::CONE:
//            return VSD_CONE::MakeData<VERTEX>();
//        case VSD_MAKER::SHAPE::PRISM:
//            return VSD_PRISM::MakeData<VERTEX>(3);
//        case VSD_MAKER::SHAPE::CYLINDER:
//            return VSD_PRISM::MakeData<VERTEX>();
//        case VSD_MAKER::SHAPE::SPHERE:
//            return VSD_SPHERE::MakeData<VERTEX>();
//        case VSD_MAKER::SHAPE::PLANE:
//            return VSD_PLANE::MakeData<VERTEX>();
//        default:
//            throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
//        }
//    }
//
//    static VS_DATA<VERTEX_T> MakeData_Tex(SHAPE Type)        //UV情報あり
//    {
//        switch (Type)
//        {
//        case VSD_MAKER::SHAPE::BOX:
//            return VSD_BOX::MakeData_Tex<VERTEX_T>();
//        case VSD_MAKER::SHAPE::PLANE:
//            return VSD_PLANE::MakeData_Tex<VERTEX_T>();
//        default:
//            throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
//        }
//    }
//
//    static VS_DATA<VERTEX_M> MakeData_Model(SHAPE Type)        //モデル用(データ作成)
//    {
//        switch (Type)
//        {
//        case VSD_MAKER::SHAPE::BOX:
//            return VSD_BOX::MakeData_Model<VERTEX_M>();
//        case VSD_MAKER::SHAPE::SPHERE:
//            return VSD_SPHERE::MakeData_Model<VERTEX_M>();
//        default:
//            throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
//        }
//    }
//
//private:
//
//    //プロトタイプ宣言
//    VSD_MAKER() noexcept {}
//    ~VSD_MAKER() noexcept {}
//};
