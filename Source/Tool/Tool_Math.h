//==============================================================================
// Filename: Tool_Math.h
// Description: 計算処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <cmath>
#include <numbers>
#include <Gfx/Gfx_Main.h>

//===== 定数・マクロ定義 =====
constexpr float F_PI = std::numbers::pi_v<float>;
constexpr double D_PI = std::numbers::pi;           //円周率
constexpr float F_2PI = F_PI * 2.0f;
constexpr double D_2PI = D_PI * 2.0;                //360度ラジアン

//===== クラス宣言 =====

//***** ベクトル *****
class MathVector2
{
public:

    //演算子オーバーロード
    bool operator==(const MathVector2 data) const;          //比較
    MathVector2 operator+(const MathVector2 data) const;    //加算
    MathVector2& operator+=(const MathVector2 data);
    MathVector2 operator-(const MathVector2 data) const;    //減算
    MathVector2& operator-=(const MathVector2 data);
    MathVector2 operator*(const float data) const;          //乗算
    MathVector2& operator*=(const float data);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    MathVector2() noexcept;

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] xIn  二次元ベクトルのx軸の値
    /// \param[in] yIn  二次元ベクトルのy軸の値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    MathVector2(
        /*[in]*/ float xIn,
        /*[in]*/ float yIn) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~MathVector2() noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    float x;
    float y;
    //--------------------------------------------------------------------------

    /// <summary>
    /// x   二次元ベクトルのx軸の値
    /// y   二次元ベクトルのy軸の値
    /// </summary>
};

class MathVector3
{
public:

    //演算子オーバーロード
    bool operator==(const MathVector3 data) const;          //比較
    MathVector3 operator+(const MathVector3 data) const;    //加算
    MathVector3& operator+=(const MathVector3 data);
    MathVector3 operator-(const MathVector3 data) const;    //減算
    MathVector3& operator-=(const MathVector3 data);
    MathVector3 operator*(const float data) const;          //乗算
    MathVector3& operator*=(const float data);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    MathVector3() noexcept;

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] xIn  三次元ベクトルのx軸の値
    /// \param[in] yIn  三次元ベクトルのy軸の値
    /// \param[in] zIn  三次元ベクトルのz軸の値
    ///
    /// \return void
    //--------------------------------------------------------------------------
    MathVector3(
        /*[in]*/ float xIn,
        /*[in]*/ float yIn,
        /*[in]*/ float zIn) noexcept;

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~MathVector3() noexcept;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    float x;
    float y;
    float z;
    //--------------------------------------------------------------------------

    /// <summary>
    /// x   三次元ベクトルのx軸の値
    /// y   三次元ベクトルのy軸の値
    /// z   三次元ベクトルのz軸の値
    /// </summary>
};

//===== プロトタイプ宣言 =====
namespace gMath
{
    //--------------------------------------------------------------------------
    /// 二乗する計算
    ///
    /// \param[in] x    二乗させる値
    ///
    /// \return 入れた値の型で結果が返される
    //--------------------------------------------------------------------------
    template <typename T>
    inline constexpr T Square(
        /*[in]*/ const T x) noexcept
    {
        return x * x;
    }

    //--------------------------------------------------------------------------
    /// 角度変換(角度⇒ラジアン)
    ///
    /// \param[in] angle    角度（intまたはfloat）
    ///
    /// \return ラジアン
    //--------------------------------------------------------------------------
    inline float GetRad(
        /*[in]*/ int angle) noexcept
    {
        return (F_PI * angle / 180);
    }

    inline float GetRad(
        /*[in]*/ float angle) noexcept
    {
        return (F_PI * angle / 180);
    }

    //--------------------------------------------------------------------------
    /// 角度変換(ラジアン⇒角度)
    ///
    /// \param[in] radian   ラジアン
    ///
    /// \return 角度（int）
    //--------------------------------------------------------------------------
    inline int GetDegree(
        /*[in]*/ float radian) noexcept
    {
        return static_cast<int>(radian * 180 / F_PI);
    }

    //--------------------------------------------------------------------------
    /// 角度変換(ラジアン⇒角度)
    ///
    /// \param[in] radian   ラジアン
    ///
    /// \return 角度（float）
    //--------------------------------------------------------------------------
    inline float GetDegree_f(
        /*[in]*/ float radian) noexcept
    {
        return radian * 180 / F_PI;
    }

    //--------------------------------------------------------------------------
    /// ベクトル長さ取得
    ///
    /// \param[in] vector   ベクトル(二次元または三次元)
    ///
    /// \return ベクトル長さ
    //--------------------------------------------------------------------------
    inline float VecLength(
        /*[in]*/ MathVector2 vector) noexcept
    {
        return std::sqrt(Square(vector.x) + Square(vector.y));
    }

    inline float VecLength(
        /*[in]*/ MathVector3 vector) noexcept
    {
        return std::sqrt(Square(vector.x) + Square(vector.y) + Square(vector.z));
    }

    //--------------------------------------------------------------------------
    /// ベクトル正規化
    ///
    /// \param[in] vector   ベクトル(二次元または三次元)
    ///
    /// \return 正規化されたベクトル
    //--------------------------------------------------------------------------
    inline MathVector2 VecNormalize(
        /*[in]*/ MathVector2 vector)
    {
        //変数宣言
        MathVector2 vec = vector;
        float vecLen = VecLength(vector);

        //例外処理
        if (vecLen == 0.0f)
            throw ERROR_EX2(S_OK, "ベクトルの長さは0です!");

        //計算
        vec.x /= vecLen;
        vec.y /= vecLen;

        //戻り値
        return vec;
    }

    inline MathVector3 VecNormalize(
        /*[in]*/ MathVector3 vector)
    {
        //変数宣言
        MathVector3 vec = vector;
        float vecLen = VecLength(vector);

        //例外処理
        if (vecLen == 0.0f)
            throw ERROR_EX2(S_OK, "ベクトルの長さは0です!");

        //計算
        vec.x /= vecLen;
        vec.y /= vecLen;
        vec.z /= vecLen;

        //戻り値
        return vec;
    }

    //--------------------------------------------------------------------------
    /// 内積計算
    ///
    /// \param[in] vector1   一つ目のベクトル(二次元または三次元)
    /// \param[in] vector2   二つ目のベクトル(二次元または三次元)
    ///
    /// \return 内積計算の結果
    //--------------------------------------------------------------------------
    inline float VecDot(
        /*[in]*/ MathVector2 vector1,
        /*[in]*/ MathVector2 vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y;
    }

    inline float VecDot(
        /*[in]*/ MathVector3 vector1,
        /*[in]*/ MathVector3 vector2)
    {
        return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
    }

    //--------------------------------------------------------------------------
    /// 外積計算
    ///
    /// \param[in] vector1   一つ目のベクトル(二次元または三次元)
    /// \param[in] vector2   二つ目のベクトル(二次元または三次元)
    ///
    /// \return 外積計算の結果
    //--------------------------------------------------------------------------
    inline float VecCross(
        /*[in]*/ MathVector2 vector1,
        /*[in]*/ MathVector2 vector2)
    {
        return vector1.x * vector2.y - vector1.y * vector2.x;
    }

    inline MathVector3 VecCross(
        /*[in]*/ MathVector3 vector1,
        /*[in]*/ MathVector3 vector2)
    {
        //変数宣言
        MathVector3 vec{};

        //計算
        vec.x = vector1.y * vector2.z - vector1.z * vector2.y;
        vec.y = vector1.z * vector2.x - vector1.x * vector2.z;
        vec.z = vector1.x * vector2.y - vector1.y * vector2.x;

        //戻り値
        return vec;
    }

    //--------------------------------------------------------------------------
    /// 4x4行列の転置計算
    ///
    /// \param[in]  mtxA    転置させたい行列の先頭ポインタ
    /// \param[out] mtxB    転置させた行列の入れ先の先頭ポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    inline void MtxTranspose4x4_SSE(
        /*[in]*/ float* mtxA,
        /*[in]*/ float* mtxB = nullptr)
    {
        __m128 row1 = _mm_load_ps(mtxA + 0 * 4);
        __m128 row2 = _mm_load_ps(mtxA + 1 * 4);
        __m128 row3 = _mm_load_ps(mtxA + 2 * 4);
        __m128 row4 = _mm_load_ps(mtxA + 3 * 4);
        _MM_TRANSPOSE4_PS(row1, row2, row3, row4);

        //結果を返す
        if (mtxB != nullptr) {
            _mm_store_ps((mtxB + 0 * 4), row1);
            _mm_store_ps((mtxB + 1 * 4), row2);
            _mm_store_ps((mtxB + 2 * 4), row3);
            _mm_store_ps((mtxB + 3 * 4), row4);
        }
        else {
            _mm_store_ps((mtxA + 0 * 4), row1);
            _mm_store_ps((mtxA + 1 * 4), row2);
            _mm_store_ps((mtxA + 2 * 4), row3);
            _mm_store_ps((mtxA + 3 * 4), row4);
        }
    }

    //--------------------------------------------------------------------------
    /// ベクトルと4x4行列の掛け算
    ///
    /// \param[in, out] pVec    ベクトルの先頭ポインタ（三次元または四次元）
    /// \param[in]      pMtx    行列の先頭ポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    inline void Vec4Transform_AVX(
        /*[in]*/ float* pVec,
        /*[in]*/ float* pMtx)
    {
        //行列セット
        __m256 row01 = _mm256_load_ps(pMtx + 0 * 8);
        __m256 row23 = _mm256_load_ps(pMtx + 1 * 8);

        //ベクトルセット（セット関数は逆順で格納される）
        float* pVecTemp = pVec;
        __m256 multFactor01 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);
        pVecTemp += 2;
        __m256 multFactor23 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);

        //行列とベクトルの掛け算
        row01 = _mm256_mul_ps(row01, multFactor01);
        row23 = _mm256_mul_ps(row23, multFactor23);        //乗算
        __m256 result = _mm256_add_ps(row01, row23);    //加算

        //結果を返す
        pVecTemp = pVec;
        *pVecTemp = result.m256_f32[0] + result.m256_f32[4];
        pVecTemp++;
        *pVecTemp = result.m256_f32[1] + result.m256_f32[5];
        pVecTemp++;
        *pVecTemp = result.m256_f32[2] + result.m256_f32[6];
        pVecTemp++;
        *pVecTemp = result.m256_f32[3] + result.m256_f32[7];
    }

    inline void Vec3Transform_AVX(
        /*[in]*/ float* pVec,
        /*[in]*/ float* pMtx)
    {
        //行列セット
        __m256 row01 = _mm256_load_ps(pMtx + 0 * 8);
        __m256 row23 = _mm256_load_ps(pMtx + 1 * 8);

        //ベクトルセット（セット関数は逆順で格納される）
        float* pVecTemp = pVec;
        __m256 multFactor01 = _mm256_set_ps(*(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *(pVecTemp + 1), *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);
        pVecTemp += 2;
        __m256 multFactor23 = _mm256_set_ps(1.0f, 1.0f, 1.0f, 1.0f, *pVecTemp, *pVecTemp, *pVecTemp, *pVecTemp);

        //行列とベクトルの掛け算
        row01 = _mm256_mul_ps(row01, multFactor01);
        row23 = _mm256_mul_ps(row23, multFactor23);        //乗算
        __m256 result = _mm256_add_ps(row01, row23);    //加算

        //結果を返す
        pVecTemp = pVec;
        *pVecTemp = result.m256_f32[0] + result.m256_f32[4];
        pVecTemp++;
        *pVecTemp = result.m256_f32[1] + result.m256_f32[5];
        pVecTemp++;
        *pVecTemp = result.m256_f32[2] + result.m256_f32[6];
    }

    //--------------------------------------------------------------------------
    /// 4x4行列の掛け算
    ///
    /// \param[in, out] mtxA    行列1の先頭ポインタ
    /// \param[in]      mtxB    行列2の先頭ポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    inline void MtxMultiply4x4_AVX(
        /*[in]*/ float* mtxA,
        /*[in]*/ float* mtxB)
    {
        float* mtxATemp = mtxA;
        Vec4Transform_AVX(mtxATemp, mtxB);
        mtxATemp += 4;
        Vec4Transform_AVX(mtxATemp, mtxB);
        mtxATemp += 4;
        Vec4Transform_AVX(mtxATemp, mtxB);
        mtxATemp += 4;
        Vec4Transform_AVX(mtxATemp, mtxB);
    }

    //--------------------------------------------------------------------------
    /// 4x4行列の足し算
    ///
    /// \param[in, out] mtxA    行列1の先頭ポインタ
    /// \param[in]      mtxB    行列2の先頭ポインタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    inline void mtxAdd4x4_AVX(
        /*[in]*/ float* pMtx1,
        /*[in]*/ float* pMtx2)
    {
        //行列セット
        __m256 row01_1 = _mm256_load_ps(pMtx1 + 0 * 8);
        __m256 row23_1 = _mm256_load_ps(pMtx1 + 1 * 8);
        __m256 row01_2 = _mm256_load_ps(pMtx2 + 0 * 8);
        __m256 row23_2 = _mm256_load_ps(pMtx2 + 1 * 8);

        //行列同士の足し算
        __m256 result01 = _mm256_add_ps(row01_1, row01_2);
        __m256 result23 = _mm256_add_ps(row23_1, row23_2);

        //結果を返す
        float* pMtxTemp = pMtx1;
        float* pResult1 = &result01.m256_f32[0];
        float* pResult2 = &result23.m256_f32[0];
        *pMtxTemp = *pResult1;
        for (size_t i = 0; i < 7; i++) {
            pMtxTemp++;
            pResult1++;
            *pMtxTemp = *pResult1;
        }
        pMtxTemp++;
        *pMtxTemp = *pResult2;
        for (size_t i = 0; i < 7; i++) {
            pMtxTemp++;
            pResult2++;
            *pMtxTemp = *pResult2;
        }
    }
}
