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

//===== 構造体宣言 =====
struct GfxMaterialData      //マテリアル情報
{
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxMaterialData() noexcept :
        ambient(0.0f, 0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f, 0.0f),
        emissive(0.0f, 0.0f, 0.0f, 0.0f), transparent(0.0f, 0.0f, 0.0f, 0.0f),
        specular(0.0f, 0.0f, 0.0f, 0.0f), shininess(2.0f),
        pad1(0.0f), pad2(0.0f), pad3(0.0f)
    {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxMaterialData() noexcept {}

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 emissive;
    DirectX::XMFLOAT4 transparent;
    DirectX::XMFLOAT4 specular;
    float shininess;
    float pad1;
    float pad2;
    float pad3;
    //--------------------------------------------------------------------------

    /// <summary>
    /// ambient         環境光
    /// diffuse         拡散反射光
    /// emissive        発射光
    /// transparent     透過度
    /// specular        鏡面反射光
    /// shininess       光沢
    /// pad1            仮置き
    /// pad2            仮置き
    /// pad3            仮置き
    /// </summary>
};

struct GfxInstanceData  //インスタンス情報
{
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxInstanceData() noexcept : mtxWorld()
    {
        //ワールド行列初期化
        DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
    }

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxInstanceData() noexcept {}

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 mtxWorld;
    //--------------------------------------------------------------------------

    /// <summary>
    /// mtxWorld    ワールド行列
    /// </summary>
};

//===== クラス定義 =====

//***** 頂点データ作成 *****
class GfxVsdMaker
{
public:

    //形状タイプ
    enum class Shape
    {
        BOX,
        PYRAMID,
        CONE,
        PRISM,
        CYLINDER,
        SPHERE,
        PLANE,

        MAX_NUM
    };

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// データ作成
    ///
    /// \param[in] type     ジオメトリタイプ
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    static VsData<Vertex> MakeData_Default(
        /*[in]*/ Shape type)
    {
        switch (type)
        {
            case GfxVsdMaker::Shape::BOX:
                return VsdBox::MakeData<Vertex>();
            case GfxVsdMaker::Shape::PYRAMID:
                return VsdCone::MakeData<Vertex>(4);
            case GfxVsdMaker::Shape::CONE:
                return VsdCone::MakeData<Vertex>();
            case GfxVsdMaker::Shape::PRISM:
                return VsdPrism::MakeData<Vertex>(3);
            case GfxVsdMaker::Shape::CYLINDER:
                return VsdPrism::MakeData<Vertex>();
            case GfxVsdMaker::Shape::SPHERE:
                return VsdSphere::MakeData<Vertex>();
            case GfxVsdMaker::Shape::PLANE:
                return VsdPlane::MakeData<Vertex>();
            default:
                throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
        }
    }

    //--------------------------------------------------------------------------
    /// データ作成(UV情報あり)
    ///
    /// \param[in] type     ジオメトリタイプ
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    static VsData<VertexT> MakeData_Tex(
        /*[in]*/ Shape type)
    {
        switch (type)
        {
            case GfxVsdMaker::Shape::BOX:
                return VsdBox::MakeData_Tex<VertexT>();
            case GfxVsdMaker::Shape::PLANE:
                return VsdPlane::MakeData_Tex<VertexT>();
            default:
                throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
        }
    }

    //--------------------------------------------------------------------------
    /// データ作成(モデル用)
    ///
    /// \param[in] type     ジオメトリタイプ
    ///
    /// \return 頂点データ
    //--------------------------------------------------------------------------
    static VsData<VertexM> MakeData_Model(
        /*[in]*/ Shape type)
    {
        switch (type)
        {
			case GfxVsdMaker::Shape::BOX:
				return VsdBox::MakeData_Model<VertexM>();
			case GfxVsdMaker::Shape::PYRAMID:
				return VsdCone::MakeData_Model<VertexM>(4);
			case GfxVsdMaker::Shape::CONE:
				return VsdCone::MakeData_Model<VertexM>();
			case GfxVsdMaker::Shape::PRISM:
				return VsdPrism::MakeData_Model<VertexM>(3);
			case GfxVsdMaker::Shape::CYLINDER:
				return VsdPrism::MakeData_Model<VertexM>();
			case GfxVsdMaker::Shape::SPHERE:
				return VsdSphere::MakeData_Model<VertexM>();
			case GfxVsdMaker::Shape::PLANE:
				return VsdPlane::MakeData_Model<VertexM>();
            default:
                throw ERROR_EX2(S_OK, "頂点データのタイプエラー。");
        }
    }

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxVsdMaker() noexcept {}

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxVsdMaker() noexcept {}

    //--------------------------------------------------------------------------
};
