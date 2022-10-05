//==============================================================================
// Filename: Tool_Math.cpp
// Description: 計算処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Tool/Tool_Math.h>

//===== クラス実装 =====

//***** ベクトル *****
MathVector2::MathVector2() noexcept : x(0.0f), y(0.0f)
{
}

MathVector2::MathVector2(float xIn, float yIn) noexcept : x(xIn), y(yIn)
{
}

MathVector2::~MathVector2() noexcept
{
}

//演算子オーバーロード
bool MathVector2::operator==(const MathVector2 data) const          //比較
{
    if (this->x == data.x && this->y == data.y)
        return true;
    return false;
}

MathVector2 MathVector2::operator+(const MathVector2 data) const    //加算
{
    //変数宣言
    MathVector2 f2{};

    //計算
    f2.x = this->x + data.x;
    f2.y = this->y + data.y;

    //戻り値
    return f2;
}

MathVector2& MathVector2::operator+=(const MathVector2 data)
{
    //計算
    this->x += data.x;
    this->y += data.y;

    //戻り値
    return *this;
}

MathVector2 MathVector2::operator-(const MathVector2 data) const    //減算
{
    //変数宣言
    MathVector2 f2{};

    //計算
    f2.x = this->x - data.x;
    f2.y = this->y - data.y;

    //戻り値
    return f2;
}

MathVector2& MathVector2::operator-=(const MathVector2 data)
{
    //計算
    this->x -= data.x;
    this->y -= data.y;

    //戻り値
    return *this;
}

MathVector2 MathVector2::operator*(const float data) const          //乗算
{
    //変数宣言
    MathVector2 f2{};

    //計算
    f2.x = this->x * data;
    f2.y = this->y * data;

    //戻り値
    return f2;
}

MathVector2& MathVector2::operator*=(const float data)
{
    //計算
    this->x *= data;
    this->y *= data;

    //戻り値
    return *this;
}

MathVector3::MathVector3() noexcept : x(0.0f), y(0.0f), z(0.0f)
{
}

MathVector3::MathVector3(float xIn, float yIn, float zIn) noexcept :
    x(xIn), y(yIn), z(zIn)
{
}

MathVector3::~MathVector3() noexcept
{
}

//演算子オーバーロード
bool MathVector3::operator==(const MathVector3 data) const          //比較
{
    if (this->x == data.x && this->y == data.y && this->z == data.z)
        return true;
    return false;
}

MathVector3 MathVector3::operator+(const MathVector3 data) const    //加算
{
    //変数宣言
    MathVector3 f3{};

    //計算
    f3.x = this->x + data.x;
    f3.y = this->y + data.y;
    f3.z = this->z + data.z;

    //戻り値
    return f3;
}

MathVector3& MathVector3::operator+=(const MathVector3 data)
{
    //計算
    this->x += data.x;
    this->y += data.y;
    this->z += data.z;

    //戻り値
    return *this;
}

MathVector3 MathVector3::operator-(const MathVector3 data) const    //減算
{
    //変数宣言
    MathVector3 f3{};

    //計算
    f3.x = this->x - data.x;
    f3.y = this->y - data.y;
    f3.z = this->z - data.z;

    //戻り値
    return f3;
}

MathVector3& MathVector3::operator-=(const MathVector3 data)
{
    //計算
    this->x -= data.x;
    this->y -= data.y;
    this->z -= data.z;

    //戻り値
    return *this;
}

MathVector3 MathVector3::operator*(const float data) const          //乗算
{
    //変数宣言
    MathVector3 f3{};

    //計算
    f3.x = this->x * data;
    f3.y = this->y * data;
    f3.z = this->z * data;

    //戻り値
    return f3;
}

MathVector3& MathVector3::operator*=(const float data)
{
    //計算
    this->x *= data;
    this->y *= data;
    this->z *= data;

    //戻り値
    return *this;
}
