#pragma once
#include "pch.h"

#include <cmath>
#include <algorithm>
#include <limits>

#define NOMINMAX
#include <Windows.h>

//https://github.com/Microsoft/DirectXTK/wiki/SimpleMath
#if __has_include("DirectXMath.h")
// In this case, DirectXMath is coming from Windows SDK.
//	It is better to use this on Windows as some Windows libraries could depend on the same 
//	DirectXMath headers
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#endif

//#define XM_CONSTEXPR

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace math
{
    struct Vector2;
    struct Vector4;
    struct Matrix;
    struct Quaternion;
    struct Plane;

    // radian <-> degree
    inline float Radian(const float degree)
    {
        return degree * XM_PI / 180.0f;
    }

    inline float Degree(const float radian)
    {
        return radian * 180.0f / XM_PI;
    }

    //template<class T> inline T XMMin(T a, T b) noexcept { return (a < b) ? a : b; }
    //template<class T> inline T XMMax(T a, T b) noexcept { return (a > b) ? a : b; }

    //------------------------------------------------------------------------------
    // 2D rectangle
    struct Rectangle
    {
        long x;
        long y;
        long width;
        long height;

        // Creators
        Rectangle() noexcept : x(0), y(0), width(0), height(0) {}
        constexpr Rectangle(long ix, long iy, long iw, long ih) noexcept : x(ix), y(iy), width(iw), height(ih) {}
        explicit Rectangle(const RECT& rct) noexcept : x(rct.left), y(rct.top), width(rct.right - rct.left), height(rct.bottom - rct.top) {}

        Rectangle(const Rectangle&) = default;
        Rectangle& operator=(const Rectangle&) = default;

        Rectangle(Rectangle&&) = default;
        Rectangle& operator=(Rectangle&&) = default;

        operator RECT() noexcept { RECT rct; rct.left = x; rct.top = y; rct.right = (x + width); rct.bottom = (y + height); return rct; }
#ifdef __cplusplus_winrt
        operator Windows::Foundation::Rect() noexcept { return Windows::Foundation::Rect(float(x), float(y), float(width), float(height)); }
#endif

        // Comparison operators
#if (__cplusplus >= 202002L)
        bool operator == (const Rectangle&) const = default;
        auto operator <=> (const Rectangle&) const = default;
#else
        bool operator == (const Rectangle& r) const noexcept { return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height); }
        bool operator != (const Rectangle& r) const noexcept { return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height); }
#endif
        bool operator == (const RECT& rct) const noexcept { return (x == rct.left) && (y == rct.top) && (width == (rct.right - rct.left)) && (height == (rct.bottom - rct.top)); }
        bool operator != (const RECT& rct) const noexcept { return (x != rct.left) || (y != rct.top) || (width != (rct.right - rct.left)) || (height != (rct.bottom - rct.top)); }

        // Assignment operators
        Rectangle& operator=(_In_ const RECT& rct) noexcept { x = rct.left; y = rct.top; width = (rct.right - rct.left); height = (rct.bottom - rct.top); return *this; }

        // Rectangle operations
        Vector2 Location() const noexcept;
        Vector2 Center() const noexcept;

        bool IsEmpty() const noexcept { return (width == 0 && height == 0 && x == 0 && y == 0); }

        bool Contains(long ix, long iy) const noexcept { return (x <= ix) && (ix < (x + width)) && (y <= iy) && (iy < (y + height)); }
        bool Contains(const Vector2& point) const noexcept;
        bool Contains(const Rectangle& r) const noexcept { return (x <= r.x) && ((r.x + r.width) <= (x + width)) && (y <= r.y) && ((r.y + r.height) <= (y + height)); }
        bool Contains(const RECT& rct) const noexcept { return (x <= rct.left) && (rct.right <= (x + width)) && (y <= rct.top) && (rct.bottom <= (y + height)); }

        void Inflate(long horizAmount, long vertAmount) noexcept;

        bool Intersects(const Rectangle& r) const noexcept { return (r.x < (x + width)) && (x < (r.x + r.width)) && (r.y < (y + height)) && (y < (r.y + r.height)); }
        bool Intersects(const RECT& rct) const noexcept { return (rct.left < (x + width)) && (x < rct.right) && (rct.top < (y + height)) && (y < rct.bottom); }

        void Offset(long ox, long oy) noexcept { x += ox; y += oy; }

        // Static functions
        static Rectangle Intersect(const Rectangle& ra, const Rectangle& rb) noexcept;
        static RECT Intersect(const RECT& rcta, const RECT& rctb) noexcept;

        static Rectangle Union(const Rectangle& ra, const Rectangle& rb) noexcept;
        static RECT Union(const RECT& rcta, const RECT& rctb) noexcept;
    };

    //------------------------------------------------------------------------------
    // 2D vector
    struct Vector2 : public XMFLOAT2
    {
        Vector2() noexcept : XMFLOAT2(0.f, 0.f) {}
        constexpr explicit Vector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
        constexpr Vector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
        explicit Vector2(_In_reads_(2) const float* pArray) noexcept : XMFLOAT2(pArray) {}
        Vector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
        Vector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
        explicit Vector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

        Vector2(const Vector2&) = default;
        Vector2& operator=(const Vector2&) = default;

        Vector2(Vector2&&) = default;
        Vector2& operator=(Vector2&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

        // Comparison operators
        bool operator == (const Vector2& V) const noexcept;
        bool operator != (const Vector2& V) const noexcept;

        // Assignment operators
        Vector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
        Vector2& operator+= (const Vector2& V) noexcept;
        Vector2& operator-= (const Vector2& V) noexcept;
        Vector2& operator*= (const Vector2& V) noexcept;
        Vector2& operator*= (float S) noexcept;
        Vector2& operator/= (float S) noexcept;

        // Unary operators
        Vector2 operator+ () const noexcept { return *this; }
        Vector2 operator- () const noexcept { return Vector2(-x, -y); }

        // Vector operations
        bool InBounds(const Vector2& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector2& V) const noexcept;
        void Cross(const Vector2& V, Vector2& result) const noexcept;
        Vector2 Cross(const Vector2& V) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector2& result) const noexcept;

        void Clamp(const Vector2& vmin, const Vector2& vmax) noexcept;
        void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept;

        // Static functions
        static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
        static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;

        static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

        static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

        static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept;
        static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept;
        static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;

        static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept;
        static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;

        static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept;
        static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;

        static void Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept;
        static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

        static void Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
        static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
        static void TransformNormal(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

        // Constants
        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 UnitX;
        static const Vector2 UnitY;
    };

    // Binary operators
    Vector2 operator+ (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator- (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator* (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator* (const Vector2& V, float S) noexcept;
    Vector2 operator/ (const Vector2& V1, const Vector2& V2) noexcept;
    Vector2 operator/ (const Vector2& V, float S) noexcept;
    Vector2 operator* (float S, const Vector2& V) noexcept;

    //------------------------------------------------------------------------------
    // 3D vector
    struct Vector3 : public XMFLOAT3
    {
        Vector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
        constexpr explicit Vector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
        constexpr Vector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
        explicit Vector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
        Vector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
        Vector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
        explicit Vector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

        Vector3(const Vector3&) = default;
        Vector3& operator=(const Vector3&) = default;

        Vector3(Vector3&&) = default;
        Vector3& operator=(Vector3&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

        // Comparison operators
        bool operator == (const Vector3& V) const noexcept;
        bool operator != (const Vector3& V) const noexcept;

        // Assignment operators
        Vector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
        Vector3& operator+= (const Vector3& V) noexcept;
        Vector3& operator-= (const Vector3& V) noexcept;
        Vector3& operator*= (const Vector3& V) noexcept;
        Vector3& operator*= (float S) noexcept;
        Vector3& operator/= (float S) noexcept;

        // Unary operators
        Vector3 operator+ () const noexcept { return *this; }
        Vector3 operator- () const noexcept;

        // Vector operations
        bool InBounds(const Vector3& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector3& V) const noexcept;
        void Cross(const Vector3& V, Vector3& result) const noexcept;
        Vector3 Cross(const Vector3& V) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector3& result) const noexcept;

        void Clamp(const Vector3& vmin, const Vector3& vmax) noexcept;
        void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept;

        // Static functions
        static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
        static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

        static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

        static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

        static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;

        static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

        static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result) noexcept;
        static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result) noexcept;
        static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;

        static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result) noexcept;
        static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;

        static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept;
        static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;

        static void Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept;
        static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept;
        static Vector3 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
        static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

        static void Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept;
        static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
        static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;
        static void TransformNormal(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

        // Constants
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 UnitX;
        static const Vector3 UnitY;
        static const Vector3 UnitZ;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Right;
        static const Vector3 Left;
        static const Vector3 Forward;
        static const Vector3 Backward;
    };

    // Binary operators
    Vector3 operator+ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator- (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V, float S) noexcept;
    Vector3 operator/ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator/ (const Vector3& V, float S) noexcept;
    Vector3 operator* (float S, const Vector3& V) noexcept;

    //------------------------------------------------------------------------------
    // 4D vector
    struct Vector4 : public XMFLOAT4
    {
        Vector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
        constexpr explicit Vector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
        constexpr Vector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        explicit Vector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Vector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Vector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
        explicit Vector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Vector4(const Vector4&) = default;
        Vector4& operator=(const Vector4&) = default;

        Vector4(Vector4&&) = default;
        Vector4& operator=(Vector4&&) = default;

        operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Vector4& V) const noexcept;
        bool operator != (const Vector4& V) const noexcept;

        // Assignment operators
        Vector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Vector4& operator+= (const Vector4& V) noexcept;
        Vector4& operator-= (const Vector4& V) noexcept;
        Vector4& operator*= (const Vector4& V) noexcept;
        Vector4& operator*= (float S) noexcept;
        Vector4& operator/= (float S) noexcept;

        // Unary operators
        Vector4 operator+ () const noexcept { return *this; }
        Vector4 operator- () const noexcept;

        // Vector operations
        bool InBounds(const Vector4& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector4& V) const noexcept;
        void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
        Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector4& result) const noexcept;

        void Clamp(const Vector4& vmin, const Vector4& vmax) noexcept;
        void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept;

        // Static functions
        static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
        static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

        static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

        static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

        static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result) noexcept;
        static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result) noexcept;
        static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;

        static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result) noexcept;
        static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;

        static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept;
        static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;

        static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept;
        static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        // Constants
        static const Vector4 Zero;
        static const Vector4 One;
        static const Vector4 UnitX;
        static const Vector4 UnitY;
        static const Vector4 UnitZ;
        static const Vector4 UnitW;
    };

    // Binary operators
    Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V, float S) noexcept;
    Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator/ (const Vector4& V, float S) noexcept;
    Vector4 operator* (float S, const Vector4& V) noexcept;

    //------------------------------------------------------------------------------
    // 4x4 Matrix (assumes right-handed cooordinates)
    struct Matrix : public XMFLOAT4X4
    {
        Matrix() noexcept
            : XMFLOAT4X4(1.f, 0, 0, 0,
                0, 1.f, 0, 0,
                0, 0, 1.f, 0,
                0, 0, 0, 1.f)
        {
        }
        constexpr Matrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) noexcept
            : XMFLOAT4X4(m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33)
        {
        }
        explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
                r1.x, r1.y, r1.z, 0,
                r2.x, r2.y, r2.z, 0,
                0, 0, 0, 1.f)
        {
        }
        explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
                r1.x, r1.y, r1.z, r1.w,
                r2.x, r2.y, r2.z, r2.w,
                r3.x, r3.y, r3.z, r3.w)
        {
        }
        Matrix(const XMFLOAT4X4& M) noexcept { memcpy(this, &M, sizeof(XMFLOAT4X4)); }
        Matrix(const XMFLOAT3X3& M) noexcept;
        Matrix(const XMFLOAT4X3& M) noexcept;

        explicit Matrix(_In_reads_(16) const float* pArray) noexcept : XMFLOAT4X4(pArray) {}
        Matrix(CXMMATRIX M) noexcept { XMStoreFloat4x4(this, M); }

        Matrix(const Matrix&) = default;
        Matrix& operator=(const Matrix&) = default;

        Matrix(Matrix&&) = default;
        Matrix& operator=(Matrix&&) = default;

        operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

        // Comparison operators
        bool operator == (const Matrix& M) const noexcept;
        bool operator != (const Matrix& M) const noexcept;

        // Assignment operators
        Matrix& operator= (const XMFLOAT3X3& M) noexcept;
        Matrix& operator= (const XMFLOAT4X3& M) noexcept;
        Matrix& operator+= (const Matrix& M) noexcept;
        Matrix& operator-= (const Matrix& M) noexcept;
        Matrix& operator*= (const Matrix& M) noexcept;
        Matrix& operator*= (float S) noexcept;
        Matrix& operator/= (float S) noexcept;

        Matrix& operator/= (const Matrix& M) noexcept;
        // Element-wise divide

    // Unary operators
        Matrix operator+ () const noexcept { return *this; }
        Matrix operator- () const noexcept;

        // Properties
        Vector3 Up() const noexcept { return Vector3(_21, _22, _23); }
        void Up(const Vector3& v) noexcept { _21 = v.x; _22 = v.y; _23 = v.z; }

        Vector3 Down() const  noexcept { return Vector3(-_21, -_22, -_23); }
        void Down(const Vector3& v) noexcept { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

        Vector3 Right() const noexcept { return Vector3(_11, _12, _13); }
        void Right(const Vector3& v) noexcept { _11 = v.x; _12 = v.y; _13 = v.z; }

        Vector3 Left() const noexcept { return Vector3(-_11, -_12, -_13); }
        void Left(const Vector3& v) noexcept { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

        Vector3 Forward() const noexcept { return Vector3(-_31, -_32, -_33); }
        void Forward(const Vector3& v) noexcept { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

        Vector3 Backward() const noexcept { return Vector3(_31, _32, _33); }
        void Backward(const Vector3& v) noexcept { _31 = v.x; _32 = v.y; _33 = v.z; }

        Vector3 Translation() const  noexcept { return Vector3(_41, _42, _43); }
        void Translation(const Vector3& v) noexcept { _41 = v.x; _42 = v.y; _43 = v.z; }

        // Matrix operations
        bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept;

        Matrix Transpose() const noexcept;
        void Transpose(Matrix& result) const noexcept;

        Matrix Invert() const noexcept;
        void Invert(Matrix& result) const noexcept;

        float Determinant() const noexcept;

        // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
        Vector3 ToEuler() const noexcept;

        // Static functions
        static Matrix CreateBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr) noexcept;

        static Matrix CreateConstrainedBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
            _In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr) noexcept;

        static Matrix CreateTranslation(const Vector3& position) noexcept;
        static Matrix CreateTranslation(float x, float y, float z) noexcept;

        static Matrix CreateScale(const Vector3& scales) noexcept;
        static Matrix CreateScale(float xs, float ys, float zs) noexcept;
        static Matrix CreateScale(float scale) noexcept;

        static Matrix CreateRotationX(float radians) noexcept;
        static Matrix CreateRotationY(float radians) noexcept;
        static Matrix CreateRotationZ(float radians) noexcept;

        static Matrix CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;

        static Matrix CreatePerspectiveFieldOfViewRH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspectiveRH(float width, float height, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspectiveFieldOfViewLH(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspectiveLH(float width, float height, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
        static Matrix CreateOrthographicRH(float width, float height, float zNearPlane, float zFarPlane) noexcept;
        static Matrix CreateOrthographicOffCenterRH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
        static Matrix CreateOrthographicLH(float width, float height, float zNearPlane, float zFarPlane) noexcept;
        static Matrix CreateOrthographicOffCenterLH(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

        static Matrix CreateLookAtRH(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
        static Matrix CreateLookAtLH(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
        static Matrix CreateLookToRH(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
        static Matrix CreateLookToLH(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;

        static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;

        static Matrix CreateFromQuaternion(const Quaternion& quat) noexcept;

        // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
        static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

        // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
        static Matrix CreateFromYawPitchRoll(const Vector3& angles) noexcept;

        static Matrix CreateShadow(const Vector3& lightDir, const Plane& plane) noexcept;

        static Matrix CreateReflection(const Plane& plane) noexcept;

        static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result) noexcept;
        static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t) noexcept;

        static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result) noexcept;
        static Matrix Transform(const Matrix& M, const Quaternion& rotation) noexcept;

        // Constants
        static const Matrix Identity;
    };

    // Binary operators
    Matrix operator+ (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator- (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M1, const Matrix& M2) noexcept;
    // Element-wise divide
    Matrix operator* (float S, const Matrix& M) noexcept;


    //-----------------------------------------------------------------------------
    // Plane
    struct Plane : public XMFLOAT4
    {
        Plane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
        constexpr Plane(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        Plane(const Vector3& normal, float d) noexcept : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
        Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3) noexcept;
        Plane(const Vector3& point, const Vector3& normal) noexcept;
        explicit Plane(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
        explicit Plane(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Plane(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Plane(const XMFLOAT4& p) noexcept { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
        explicit Plane(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Plane(const Plane&) = default;
        Plane& operator=(const Plane&) = default;

        Plane(Plane&&) = default;
        Plane& operator=(Plane&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Plane& p) const noexcept;
        bool operator != (const Plane& p) const noexcept;

        // Assignment operators
        Plane& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

        // Properties
        Vector3 Normal() const noexcept { return Vector3(x, y, z); }
        void Normal(const Vector3& normal) noexcept { x = normal.x; y = normal.y; z = normal.z; }

        float D() const noexcept { return w; }
        void D(float d) noexcept { w = d; }

        // Plane operations
        void Normalize() noexcept;
        void Normalize(Plane& result) const noexcept;

        float Dot(const Vector4& v) const noexcept;
        float DotCoordinate(const Vector3& position) const noexcept;
        float DotNormal(const Vector3& normal) const noexcept;

        // Static functions
        static void Transform(const Plane& plane, const Matrix& M, Plane& result) noexcept;
        static Plane Transform(const Plane& plane, const Matrix& M) noexcept;

        static void Transform(const Plane& plane, const Quaternion& rotation, Plane& result) noexcept;
        static Plane Transform(const Plane& plane, const Quaternion& rotation) noexcept;
        // Input quaternion must be the inverse transpose of the transformation
    };

    //------------------------------------------------------------------------------
    // Quaternion
    struct Quaternion : public XMFLOAT4
    {
        Quaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        Quaternion(const Vector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
        explicit Quaternion(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
        explicit Quaternion(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Quaternion(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Quaternion(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
        explicit Quaternion(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;

        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Quaternion& q) const noexcept;
        bool operator != (const Quaternion& q) const noexcept;

        // Assignment operators
        Quaternion& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Quaternion& operator+= (const Quaternion& q) noexcept;
        Quaternion& operator-= (const Quaternion& q) noexcept;
        Quaternion& operator*= (const Quaternion& q) noexcept;
        Quaternion& operator*= (float S) noexcept;
        Quaternion& operator/= (const Quaternion& q) noexcept;

        // Unary operators
        Quaternion operator+ () const  noexcept { return *this; }
        Quaternion operator- () const noexcept;

        // Quaternion operations
        float Length() const noexcept;
        float LengthSquared() const noexcept;

        void Normalize() noexcept;
        void Normalize(Quaternion& result) const noexcept;

        void Conjugate() noexcept;
        void Conjugate(Quaternion& result) const noexcept;

        void Inverse(Quaternion& result) const noexcept;

        float Dot(const Quaternion& Q) const noexcept;

        void RotateTowards(const Quaternion& target, float maxAngle) noexcept;
        void __cdecl RotateTowards(const Quaternion& target, float maxAngle, Quaternion& result) const noexcept;

        // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
        Vector3 ToEuler() const noexcept;

        // Static functions
        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;

        // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

        // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
        static Quaternion CreateFromYawPitchRoll(const Vector3& angles) noexcept;

        static Quaternion CreateFromRotationMatrix(const Matrix& M) noexcept;

        static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
        static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

        static void __cdecl FromToRotation(const Vector3& fromDir, const Vector3& toDir, Quaternion& result) noexcept;
        static Quaternion FromToRotation(const Vector3& fromDir, const Vector3& toDir) noexcept;

        static void __cdecl LookRotation(const Vector3& forward, const Vector3& up, Quaternion& result) noexcept;
        static Quaternion LookRotation(const Vector3& forward, const Vector3& up) noexcept;

        static float Angle(const Quaternion& q1, const Quaternion& q2) noexcept;

        // Constants
        static const Quaternion Identity;
    };

    // Binary operators
    Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q, float S) noexcept;
    Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (float S, const Quaternion& Q) noexcept;

    //------------------------------------------------------------------------------
    // Color
    struct Color : public XMFLOAT4
    {
        Color() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
        constexpr Color(float _r, float _g, float _b) noexcept : XMFLOAT4(_r, _g, _b, 1.f) {}
        constexpr Color(float _r, float _g, float _b, float _a) noexcept : XMFLOAT4(_r, _g, _b, _a) {}
        explicit Color(const Vector3& clr) noexcept : XMFLOAT4(clr.x, clr.y, clr.z, 1.f) {}
        explicit Color(const Vector4& clr) noexcept : XMFLOAT4(clr.x, clr.y, clr.z, clr.w) {}
        explicit Color(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Color(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Color(const XMFLOAT4& c) noexcept { this->x = c.x; this->y = c.y; this->z = c.z; this->w = c.w; }
        explicit Color(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        // BGRA Direct3D 9 D3DCOLOR packed color
        explicit Color(const DirectX::PackedVector::XMCOLOR& Packed) noexcept;

        // RGBA XNA Game Studio packed color
        explicit Color(const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept;

        Color(const Color&) = default;
        Color& operator=(const Color&) = default;

        Color(Color&&) = default;
        Color& operator=(Color&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }
        operator const float* () const noexcept { return reinterpret_cast<const float*>(this); }

        // Comparison operators
        bool operator == (const Color& c) const noexcept;
        bool operator != (const Color& c) const noexcept;

        // Assignment operators
        Color& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Color& operator= (const DirectX::PackedVector::XMCOLOR& Packed) noexcept;
        Color& operator= (const DirectX::PackedVector::XMUBYTEN4& Packed) noexcept;
        Color& operator+= (const Color& c) noexcept;
        Color& operator-= (const Color& c) noexcept;
        Color& operator*= (const Color& c) noexcept;
        Color& operator*= (float S) noexcept;
        Color& operator/= (const Color& c) noexcept;

        // Unary operators
        Color operator+ () const noexcept { return *this; }
        Color operator- () const noexcept;

        // Properties
        float R() const noexcept { return x; }
        void R(float r) noexcept { x = r; }

        float G() const noexcept { return y; }
        void G(float g) noexcept { y = g; }

        float B() const noexcept { return z; }
        void B(float b) noexcept { z = b; }

        float A() const noexcept { return w; }
        void A(float a) noexcept { w = a; }

        // Color operations
        DirectX::PackedVector::XMCOLOR BGRA() const noexcept;
        DirectX::PackedVector::XMUBYTEN4 RGBA() const noexcept;

        Vector3 ToVector3() const noexcept;
        Vector4 ToVector4() const noexcept;

        void Negate() noexcept;
        void Negate(Color& result) const noexcept;

        void Saturate() noexcept;
        void Saturate(Color& result) const noexcept;

        void Premultiply() noexcept;
        void Premultiply(Color& result) const noexcept;

        void AdjustSaturation(float sat) noexcept;
        void AdjustSaturation(float sat, Color& result) const noexcept;

        void AdjustContrast(float contrast) noexcept;
        void AdjustContrast(float contrast, Color& result) const noexcept;

        // Static functions
        static void Modulate(const Color& c1, const Color& c2, Color& result) noexcept;
        static Color Modulate(const Color& c1, const Color& c2) noexcept;

        static void Lerp(const Color& c1, const Color& c2, float t, Color& result) noexcept;
        static Color Lerp(const Color& c1, const Color& c2, float t) noexcept;
    };

    // Binary operators
    Color operator+ (const Color& C1, const Color& C2) noexcept;
    Color operator- (const Color& C1, const Color& C2) noexcept;
    Color operator* (const Color& C1, const Color& C2) noexcept;
    Color operator* (const Color& C, float S) noexcept;
    Color operator/ (const Color& C1, const Color& C2) noexcept;
    Color operator* (float S, const Color& C) noexcept;

    //------------------------------------------------------------------------------
    // Ray
    class Ray
    {
    public:
        Vector3 position;
        Vector3 direction;

        Ray() noexcept : position(0, 0, 0), direction(0, 0, 1) {}
        Ray(const Vector3& pos, const Vector3& dir) noexcept : position(pos), direction(dir) {}

        Ray(const Ray&) = default;
        Ray& operator=(const Ray&) = default;

        Ray(Ray&&) = default;
        Ray& operator=(Ray&&) = default;

        // Comparison operators
        bool operator == (const Ray& r) const noexcept;
        bool operator != (const Ray& r) const noexcept;

        // Ray operations
        bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const noexcept;
        bool Intersects(const BoundingBox& box, _Out_ float& Dist) const noexcept;
        bool Intersects(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, _Out_ float& Dist) const noexcept;
        bool Intersects(const Plane& plane, _Out_ float& Dist) const noexcept;
    };

    //------------------------------------------------------------------------------
    // Viewport
    class Viewport
    {
    public:
        float x;
        float y;
        float width;
        float height;
        float minDepth;
        float maxDepth;

        Viewport() noexcept :
            x(0.f), y(0.f), width(0.f), height(0.f), minDepth(0.f), maxDepth(1.f)
        {
        }
        constexpr Viewport(float ix, float iy, float iw, float ih, float iminz = 0.f, float imaxz = 1.f) noexcept :
            x(ix), y(iy), width(iw), height(ih), minDepth(iminz), maxDepth(imaxz)
        {
        }
        explicit Viewport(const RECT& rct) noexcept :
            x(float(rct.left)), y(float(rct.top)),
            width(float(rct.right - rct.left)),
            height(float(rct.bottom - rct.top)),
            minDepth(0.f), maxDepth(1.f)
        {
        }

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
        // Direct3D 11 interop
        explicit Viewport(const D3D11_VIEWPORT& vp) noexcept :
            x(vp.TopLeftX), y(vp.TopLeftY),
            width(vp.Width), height(vp.Height),
            minDepth(vp.MinDepth), maxDepth(vp.MaxDepth)
        {
        }

        operator D3D11_VIEWPORT() noexcept { return *reinterpret_cast<const D3D11_VIEWPORT*>(this); }
        const D3D11_VIEWPORT* Get11() const noexcept { return reinterpret_cast<const D3D11_VIEWPORT*>(this); }
        Viewport& operator= (const D3D11_VIEWPORT& vp) noexcept;
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
        // Direct3D 12 interop
        explicit Viewport(const D3D12_VIEWPORT& vp) noexcept :
            x(vp.TopLeftX), y(vp.TopLeftY),
            width(vp.Width), height(vp.Height),
            minDepth(vp.MinDepth), maxDepth(vp.MaxDepth)
        {
        }

        operator D3D12_VIEWPORT() noexcept { return *reinterpret_cast<const D3D12_VIEWPORT*>(this); }
        const D3D12_VIEWPORT* Get12() const noexcept { return reinterpret_cast<const D3D12_VIEWPORT*>(this); }
        Viewport& operator= (const D3D12_VIEWPORT& vp) noexcept;
#endif

        Viewport(const Viewport&) = default;
        Viewport& operator=(const Viewport&) = default;

        Viewport(Viewport&&) = default;
        Viewport& operator=(Viewport&&) = default;

        // Comparison operators
#if (__cplusplus >= 202002L)
        bool operator == (const Viewport&) const = default;
        auto operator <=> (const Viewport&) const = default;
#else
        bool operator == (const Viewport& vp) const noexcept;
        bool operator != (const Viewport& vp) const noexcept;
#endif

        // Assignment operators
        Viewport& operator= (const RECT& rct) noexcept;

        // Viewport operations
        float AspectRatio() const noexcept;

        Vector3 Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const noexcept;
        void Project(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const noexcept;

        Vector3 Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world) const noexcept;
        void Unproject(const Vector3& p, const Matrix& proj, const Matrix& view, const Matrix& world, Vector3& result) const noexcept;

        // Static methods
#if defined(__dxgi1_2_h__) || defined(__d3d11_x_h__) || defined(__d3d12_x_h__) || defined(__XBOX_D3D12_X__)
        static RECT __cdecl ComputeDisplayArea(DXGI_SCALING scaling, UINT backBufferWidth, UINT backBufferHeight, int outputWidth, int outputHeight) noexcept;
#endif
        static RECT __cdecl ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight) noexcept;
    };

    //#include "yaMath.inl"

    class EngineMath
    {
    public:
        static const float PI;
        static const float PI2;

        static const float D2R;
        static const float R2D;
    };

    class float4
    {
    public:
        static const float4 ONE;
        static const float4 ONENULL;

        static const float4 ZERO;
        static const float4 ZERONULL;
        static const float4 LEFT;
        static const float4 RIGHT;
        static const float4 UP;
        static const float4 DOWN;
        static const float4 FORWARD;
        static const float4 BACKWARD;

        static const float4 WHITE;
        static const float4 RED;
        static const float4 GREEN;
        static const float4 BLUE;

        union
        {
            float Arr1D[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

            struct
            {
                float X;
                float Y;
                float Z;
                float W;
            };

            struct
            {
                float R;
                float G;
                float B;
                float A;
            };

            struct
            {
                float Pos2DX;
                float Pos2DY;
                float Scale2DX;
                float Scale2DY;
            };

            float Arr2D[1][4];
            DirectX::XMVECTOR DirectXVector;
            DirectX::XMFLOAT3 Float3;
            DirectX::XMFLOAT4 Float4;
        };

        float4(DirectX::FXMVECTOR& _DirectXVector)
            : DirectXVector(_DirectXVector)
        {

        }

        float4(DirectX::XMFLOAT3 _Float3)
            : Float3(_Float3)
        {

        }

        float4(float _X = 0.0f, float _Y = 0.0f, float _Z = 0.0f, float _W = 1.0f)
            : X(_X), Y(_Y), Z(_Z), W(_W)
        {

        }

        UINT ColorToUint() const
        {
            UINT Return;

            char* Ptr = reinterpret_cast<char*>(&Return);

            // 0~1
            Ptr[0] = static_cast<int>(R * 255.0f);
            Ptr[1] = static_cast<int>(G * 255.0f);
            Ptr[2] = static_cast<int>(B * 255.0f);
            Ptr[3] = static_cast<int>(A * 255.0f);

            return Return;
        }


        inline int iX() const
        {
            return static_cast<int>(X);
        }

        inline int iY() const
        {
            return static_cast<int>(Y);
        }

        inline UINT uiX() const
        {
            return static_cast<unsigned int>(X);
        }

        inline UINT uiY() const
        {
            return static_cast<unsigned int>(Y);
        }

        inline float hX() const
        {
            return X * 0.5f;
        }

        inline float hY() const
        {
            return Y * 0.5f;
        }


        inline int ihX() const
        {
            return static_cast<int>(hX());
        }

        inline int ihY() const
        {
            return static_cast<int>(hY());
        }

        inline float4 Half() const
        {
            return { hX(), hY(), Z, W };
        }

        float4 ToABS() const
        {
            return DirectX::XMVectorAbs(DirectXVector);
        }

        float4 operator-() const
        {
            float4 ReturnValue = DirectX::XMVectorSet(-X, -Y, -Z, W);
            return ReturnValue;
        }

        float4 operator-(const float4& _Other) const
        {
            float4 ReturnValue;
            ReturnValue.DirectXVector = DirectX::XMVectorSubtract(DirectXVector, _Other.DirectXVector);
            ReturnValue.W = W;
            return ReturnValue;
        }

        float4 EulerDegToQuaternion()
        {
            // 디그리 각도를 쿼터니온으로 변경
            float4 Return = DirectXVector;
            Return *= EngineMath::D2R;
            Return = DirectX::XMQuaternionRotationRollPitchYawFromVector(Return.DirectXVector);
            return Return;
        }

        float4 QuaternionToEulerDeg()
        {
            // 디그리 각도로 바꿔줍니다.
            return QuaternionToEulerRad() * EngineMath::R2D;
        }

        float4 QuaternionToEulerRad()
        {
            // 쿼터니온을 다시 라디안 각도로 변경
            float4 result;

            float sinrCosp = 2.0f * (W * Z + X * Y);
            float cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);
            result.Z = atan2f(sinrCosp, cosrCosp);

            float pitchTest = W * X - Y * Z;
            float asinThreshold = 0.4999995f;
            float sinp = 2.0f * pitchTest;

            if (pitchTest < -asinThreshold)
            {
                result.X = -(0.5f * EngineMath::PI);
            }
            else if (pitchTest > asinThreshold)
            {
                result.X = (0.5f * EngineMath::PI);
            }
            else
            {
                result.X = asinf(sinp);
            }

            float sinyCosp = 2.0f * (W * Y + X * Z);
            float cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
            result.Y = atan2f(sinyCosp, cosyCosp);

            return result;
        }

        float4 operator+(const float4& _Other) const
        {
            float4 ReturnValue;
            ReturnValue.DirectXVector = DirectX::XMVectorAdd(DirectXVector, _Other.DirectXVector);
            ReturnValue.W = W;
            return ReturnValue;
        }

        float4 operator*(const float4& _Other) const
        {
            float4 ReturnValue;
            ReturnValue = DirectX::XMVectorMultiply(DirectXVector, _Other.DirectXVector);
            ReturnValue.W = W;
            return ReturnValue;
        }


        float4 operator*(const float _Value) const
        {
            float4 OtherVector = { _Value , _Value , _Value , 1.0f };
            float4 ReturnValue;
            ReturnValue = DirectX::XMVectorMultiply(DirectXVector, OtherVector.DirectXVector);
            ReturnValue.W = W;
            return ReturnValue;
        }

        float4 operator/(const float4& _Other) const
        {

            float4 ReturnValue = DirectX::XMVectorDivide(DirectXVector, _Other.DirectXVector);
            ReturnValue.W = W;
            return ReturnValue;
        }


        float4 operator/(const float _Value) const
        {
            float4 OtherVecter = { _Value , _Value , _Value , 1.0f };
            float4 ReturnValue = DirectX::XMVectorDivide(DirectXVector, OtherVecter.DirectXVector);
            ReturnValue.W = W;

            return ReturnValue;
        }

        float4 operator%(const float4 _Value) const
        {
            float4 ReturnValue = DirectX::XMVectorMod(DirectXVector, _Value.DirectXVector);
            ReturnValue.W = W;

            return ReturnValue;
        }

        float4& operator/=(const float4 _Value)
        {
            float PrevW = W;
            DirectXVector = DirectX::XMVectorDivide(DirectXVector, _Value.DirectXVector);
            W = PrevW;
            return *this;
        }


        float4& operator/=(const float _Value)
        {
            float4 OtherVecter = { _Value , _Value , _Value , 1.0f };
            float PrevW = W;
            DirectXVector = DirectX::XMVectorDivide(DirectXVector, OtherVecter.DirectXVector);
            W = PrevW;
            return *this;
        }


        float4& operator+=(const float4& _Other)
        {
            float PrevW = W;
            DirectXVector = DirectX::XMVectorAdd(DirectXVector, _Other.DirectXVector);
            W = PrevW;
            return *this;
        }


        float4& operator-=(const float4& _Other)
        {
            float PrevW = W;
            DirectXVector = DirectX::XMVectorSubtract(DirectXVector, _Other.DirectXVector);
            W = PrevW;
            return *this;
        }

        float4& operator*=(const float4& _Other)
        {
            float PrevW = W;
            DirectXVector = DirectX::XMVectorMultiply(DirectXVector, _Other.DirectXVector);
            W = PrevW;
            return *this;
        }

        float4& operator*=(const float _Value)
        {
            float4 OtherVecter = { _Value , _Value , _Value , 1.0f };
            float PrevW = W;
            DirectXVector = DirectX::XMVectorMultiply(DirectXVector, OtherVecter.DirectXVector);
            W = PrevW;
            return *this;
        }

        bool operator==(const float4 _Value) const
        {
            return DirectX::XMVector3Equal(DirectXVector, _Value.DirectXVector);
            //return X == _Value.X &&
            //	Y == _Value.Y &&
            //	Z == _Value.Z;
        }

        inline void Normalize()
        {
            // 길이를 1로 만드는 함수입니다.
            //float Len = Size();

            //if (0.0f == Len)
            //{
            //	// MsgBoxAssert("0으로 나누려고 했습니다.");
            //	return;
            //}

            //X /= Len;
            //Y /= Len;
            //Z /= Len;

            DirectXVector = DirectX::XMVector3Normalize(DirectXVector);
        }

        inline float4 NormalizeReturn() const
        {
            float4 Result = *this;
            Result.Normalize();
            return Result;
        }

        inline float Size()
        {
            //float Value = X * X + Y * Y + Z * Z; // == 빗변 * 빗변
            //return sqrtf(Value);

            float4 Result = DirectX::XMVector3Length(DirectXVector);
            return Result.X;
        }

        float Max2D() const
        {
            return X > Y ? X : Y;
        }

        POINT WindowPOINT()
        {
            return POINT{ iX(), iY() };
        }

        // Debug Code
        std::string ToString(std::string_view _Next = "")
        {
            return "X : " + std::to_string(X) + " Y : " + std::to_string(Y) + " Z : " + std::to_string(Z) + _Next.data() + "\n";
        }

    public:
        inline float Angle2DDeg()
        {
            return Angle2DRad() * EngineMath::R2D;
        }

        inline float Angle2DRad()
        {
            // 2개의 벡터의 각도 구해주는 함수
            float4 Result = DirectX::XMVector2AngleBetweenNormals(DirectXVector, float4::RIGHT.DirectXVector);

            //float4 AngleVector = NormalizeReturn();

            //// 라디안 각도만 나오게 된다. = acosf(AngleVector.X);

            //float Angle = acosf(AngleVector.X);

            //if (0 >= AngleVector.Y)
            //{
            //	Angle = CMath4::PI + CMath4::PI - Angle;
            //}

            return Result.X;
        }


        float4 VectorRotationToDegXReturn(const float _Deg)
        {
            return VectorRotationToDegX(*this, _Deg);
        }

        static float DotProduct3D(const float4& _Left, const float4& _Right)
        {
            // float Result = (_Left.X * _Right.X) + (_Left.Y * _Right.Y) + (_Left.Z * _Right.Z);
            float4 Result = DirectX::XMVector3Dot(_Left.DirectXVector, _Right.DirectXVector);
            return Result.X;
        }


        static float4 Cross3D(const float4& _Left, const float4& _Right)
        {
            float4 Result = DirectX::XMVector3Cross(_Left.DirectXVector, _Right.DirectXVector);
            return Result;
        }

        static float4 VectorRotationToDegX(const float4& _Value, const float _Deg)
        {
            return VectorRotationToRadX(_Value, _Deg * EngineMath::D2R);
        }

        static float4 VectorRotationToRadX(const float4& _Value, const float _Rad);

        float4 VectorRotationToDegYReturn(const float _Deg)
        {
            return VectorRotationToDegY(*this, _Deg);
        }


        static float4 VectorRotationToDegY(const float4& _Value, const float _Deg)
        {
            return VectorRotationToRadY(_Value, _Deg * EngineMath::D2R);
        }

        static float4 VectorRotationToRadY(const float4& _Value, const float _Rad);

        float4 VectorRotationToDegZReturn(const float _Deg)
        {
            return VectorRotationToDegZ(*this, _Deg);
        }

        static float4 VectorRotationToDegZ(const float4& _Value, const float _Deg)
        {
            return VectorRotationToRadZ(_Value, _Deg * EngineMath::D2R);
        }

        static float4 VectorRotationToRadZ(const float4& _Value, const float _Rad);

        void VectorRotationToDegX(const float _Rad)
        {
            *this = VectorRotationToDegX(*this, _Rad);
        }
        void VectorRotationToDegY(const float _Rad)
        {
            *this = VectorRotationToDegY(*this, _Rad);
        }
        void VectorRotationToDegZ(const float _Rad)
        {
            *this = VectorRotationToDegZ(*this, _Rad);
        }


        static float4 GetUnitVectorFromDeg(const float _Degree)
        {
            // 90 => 1.57
            return GetUnitVectorFromRad(_Degree * EngineMath::D2R);
        }
        //                                       90.0f
        static float4 GetUnitVectorFromRad(const float _Rad)
        {
            // cosf(_Rad)반지름의 길이 1일때의 cosf값이 구해집니다.
            // sinf(_Rad)반지름의 길이 1일때의 sinf값이 구해집니다.
            // => 빗변의 길이가 1일때의 결과가 나온다.

            // 
            // 1.57
            return { cosf(_Rad) , sinf(_Rad) };
        }

        // GetUnitVectorFromDeg(45)

        static float4 LerpClamp(const float4& Start, const float4& _End, float _Ratio)
        {
            if (1.0f <= _Ratio)
            {
                _Ratio = 1.0f;
            }

            float4 Result = DirectX::XMVectorLerp(Start.DirectXVector, _End.DirectXVector, _Ratio);
            return Result;
        }

        inline float4 RoundUpReturn() const
        {
            float4 Result = *this;
            Result.X = round(Result.X);
            Result.Y = round(Result.Y);
            Result.Z = round(Result.Z);
            return Result;
        }

        inline float4 RoundDownReturn() const
        {
            float4 Result = *this;
            Result.X = floor(Result.X);
            Result.Y = floor(Result.Y);
            Result.Z = floor(Result.Z);
            return Result;
        }

        inline void Ceil()
        {
            X = ceil(X);
            Y = ceil(Y);
            Z = ceil(Z);
        }

        inline void Round()
        {
            X = round(X);
            Y = round(Y);
            Z = round(Z);
        }

        inline void Floor()
        {
            X = floor(X);
            Y = floor(Y);
            Z = floor(Z);
        }

        float4 operator*(const class float4x4& _Other) const;
        float4& operator*=(const class float4x4& _Other);
    };

    // 4x4 행렬(matrix)을 나타내는 자료형 구조체
    class float4x4
    {
    public:
        static const int MatrixYCount = 4;
        static const int MatrixXCount = 4;

        union
        {
            // 2D 배열로 4x4 단위행렬 표현
            float Arr2D[MatrixYCount][MatrixXCount] =
            {
                // 00   01   02    03
                {1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            };

            // 배열을 사용하여 벡터로 행렬을 표현
            float4 ArrVector[4];

            struct
            {
                float _00;
                float _01;
                float _02;
                float _03;

                float _10;
                float _11;
                float _12;
                float _13;

                float _20;
                float _21;
                float _22;
                float _23;

                float _30;
                float _31;
                float _32;
                float _33;
            };

            float Arr1D[MatrixYCount * MatrixXCount];
            DirectX::XMMATRIX DirectXMatrix;
        };

        float4x4& operator=(DirectX::XMMATRIX _Matrix)
        {
            DirectXMatrix = _Matrix;
            return *this;
        }

        float4x4()
        {
            Identity();
        }

        float4x4(const float4x4& _Matrix)
            : DirectXMatrix(_Matrix.DirectXMatrix)
        {
        }

        float4x4(const DirectX::FXMMATRIX& _Matrix)
            : DirectXMatrix(_Matrix)
        {
        }

        void Identity()
        {
            DirectXMatrix = DirectX::XMMatrixIdentity();
            return;
        }

        void Scale(const float4& _Value)
        {
            // Identity();
            //Arr2D[0][0] = _Value.X;
            //Arr2D[1][1] = _Value.Y;
            //Arr2D[2][2] = _Value.Z;

            DirectXMatrix = DirectX::XMMatrixScalingFromVector(_Value.DirectXVector);
        }

        void RotationDeg(const float4& _Value)
        {
            RotationRad(_Value * EngineMath::D2R);
        }

        void RotationRad(const float4& _Value)
        {
            float4x4 X;
            float4x4 Y;
            float4x4 Z;

            X.RotationXRad(_Value.X);
            Y.RotationYRad(_Value.Y);
            Z.RotationZRad(_Value.Z);

            DirectXMatrix = (X * Y * Z).DirectXMatrix;
        }

        void Compose(float4& _Scale, float4& _RotQuaternion, float4& _Pos)
        {
            // 우리가 알고 있는 크(기)자(전)이(동)공(전)부(모)가
            // 적용된 행렬을 worldMatrix => 정식용어로 아핀행렬이라고 합니다.

            //float4x4 Scale;       
            //float4x4 Rot;         
            //float4x4 Pos;         
            //Scale.Scale(_Scale);  
            //Rot.RotationDeg(_RotQuaternion.QuaternionToEulerDeg());
            //Pos.Position(_Pos);
            //*this = Scale * Rot * Pos;

            // 내부에서 위처럼 Scale * Rot * Pos를 합쳐주는 함수
            float4 Rot = _RotQuaternion;
            Rot.QuaternionToEulerDeg();
            DirectXMatrix = DirectX::XMMatrixAffineTransformation(_Scale.DirectXVector, Rot.DirectXVector, _RotQuaternion.DirectXVector, _Pos.DirectXVector);
        }

        void Decompose(float4& _Scale, float4& _RotQuaternion, float4& _Pos) const
        {
            // 이함수에 대해서 오해하면 안되는점이 한가지 있다.
            // 각도는 같은 기저벡터가 나올수 있으면 아무래도 상관없는 값이 나온다. (ex 0도 == 360도 == -360도 ...)
            // _RotQuaternion <= 사원수는 뭐냐?
            // 쿼터니온이 나온다.
            DirectX::XMMatrixDecompose(&_Scale.DirectXVector, &_RotQuaternion.DirectXVector, &_Pos.DirectXVector, DirectXMatrix);
        }

        float4x4 InverseReturn() const
        {
            // 해당 Matrix의 역행렬을 구해주는 함수
            float4x4 Result;
            Result.DirectXMatrix = DirectX::XMMatrixInverse(nullptr, DirectXMatrix);
            return Result;
        }

        void RotationXDeg(const float _Value)
        {
            RotationXRad(_Value * EngineMath::D2R);
        }

        void RotationXRad(const float _Value)
        {
            // Identity();

            DirectXMatrix = DirectX::XMMatrixRotationX(_Value);

            //// DirectX::XMMatrixRotationX
            //float CosValue = cosf(_Value);
            //float SinValue = sinf(_Value);
            //Arr2D[1][1] = CosValue;
            //Arr2D[1][2] = SinValue;
            //Arr2D[2][1] = -SinValue;
            //Arr2D[2][2] = CosValue;
        }

        void RotationYDeg(const float _Value)
        {
            RotationYRad(_Value * EngineMath::D2R);
        }

        void RotationYRad(const float _Value)
        {
            // Identity();

            DirectXMatrix = DirectX::XMMatrixRotationY(_Value);

            // DirectX::XMMatrixRotationY
            //float CosValue = cosf(_Value);
            //float SinValue = sinf(_Value);
            //Arr2D[0][0] = CosValue;
            //Arr2D[0][2] = -SinValue;
            //Arr2D[2][0] = SinValue;
            //Arr2D[2][2] = CosValue;
        }

        void RotationZDeg(const float _Value)
        {
            RotationZRad(_Value * EngineMath::D2R);
        }

        void RotationZRad(const float _Value)
        {
            DirectXMatrix = DirectX::XMMatrixRotationZ(_Value);
            //Identity();

            //// DirectX::XMMatrixRotationZ
            //float CosValue = cosf(_Value);
            //float SinValue = sinf(_Value);
            //Arr2D[0][0] = CosValue;
            //Arr2D[0][1] = SinValue;
            //Arr2D[1][0] = -SinValue;
            //Arr2D[1][1] = CosValue;


            //					    [cosf(_Rad)][sinf(_Rad)][02][03]
            //					    [-sinf(_Rad)][cosf(_Rad)][12][13]
            //					    [20][21][22][23]
            //					    [30][31][32][33]
            // [x][y][z][w]        = rx  ry  rz  rw

            // [x]*[00] + [y] *[10] + [z] * [20] + [w] * [30]

            //float4 Rot * 행렬;

            //Rot.X = _Value.X * cosf(_Rad) - _Value.Y * sinf(_Rad);
            //Rot.Y = _Value.X * sinf(_Rad) + _Value.Y * cosf(_Rad);
            //Rot.Z = _Value.Z;


            // 회전을 시킬수 있는 행렬이 되어야 할거빈다.

        }

        void Position(const float4& _Value)
        {

            DirectXMatrix = DirectX::XMMatrixTranslationFromVector(_Value.DirectXVector);

            //Identity();
            //Arr2D[3][0] = _Value.X;
            //Arr2D[3][1] = _Value.Y;
            //Arr2D[3][2] = _Value.Z;
        }

        void TransPose()
        {
            // [][][][]
            // [][][][]
            // [][][][]
            // [][][][]

            DirectXMatrix = DirectX::XMMatrixTranspose(DirectXMatrix);


            //float4x4 This = *this;
            //Identity();
            //for (size_t y = 0; y < MatrixYCount; ++y)
            //{
            //	for (size_t x = 0; x < MatrixXCount; ++x)
            //	{
            //		Arr2D[x][y] = This.Arr2D[y][x];
            //	}
            //}
        }

        void LookToLH(const float4& _EyePos, const float4& _EyeDir, const float4& _EyeUp)
        {
            DirectXMatrix = DirectX::XMMatrixLookToLH(_EyePos.DirectXVector, _EyeDir.DirectXVector, _EyeUp.DirectXVector);

            //Identity();

            //float4 EyePos = _EyePos;
            //float4 EyeForward = _EyeDir;
            //float4 EyeUp = _EyeUp;

            //// 카메라의 Z앞
            //EyeForward.Normalize();
            //// 카마라의 Y위 
            //EyeUp.Normalize();
            //// 카마라의 X위 
            //float4 EyeRight = float4::Cross3D(EyeUp, EyeForward);

            //// 회전행렬을 벡터만으로 만드는 방법.
            //// float4x4 RotMat;
            //ArrVector[0] = EyeRight;
            //ArrVector[1] = EyeUp;
            //ArrVector[2] = EyeForward;

            //// 45도 돌아간 카메라라면 
            //// 다른 모든 물체는 -45도 돌아야 한다.
            //TransPose();

            //// XYZ돌아서 어떤 물체를 바라보고 있는 카메라
            //// 회전행렬을 역으로 돌려야 한다.
            //// -X-Y-Z돌아서 어떤 물체를 원점으로 돌리게 만들어야 하는데.
            //// 전치행렬 Transpose를 만들어야 한다..

            //float4 NegEyePos = -EyePos;

            //// 모든 물체가 이동해야할 방향을 구하고 있다.
            //float XValue = float4::DotProduct3D(EyeRight, NegEyePos);
            //float YValue = float4::DotProduct3D(EyeUp, NegEyePos);
            //float ZValue = float4::DotProduct3D(EyeForward, NegEyePos);

            //// 위치
            //ArrVector[3] = { XValue, YValue, ZValue };
        }

        //               보통 모니터 크기를 넣어주는데
        //               그냥 보고싶은 너비와 높이의 수치만 넣어주면 됩니다.
        //                      1280                 720           5000
        void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
        {
            DirectXMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);

            //// DirectX::XMMatrixOrthographicLH
            //Identity();
            ////                     5000 - 0.1
            //float fRange = 1.0f / (_Far - _Near);
            //Arr2D[0][0] = 2.0f / _Width;
            //Arr2D[1][1] = 2.0f / _Height;
            //Arr2D[2][2] = fRange;
            //Arr2D[3][2] = -fRange * _Near;
        }

        //      60도를 본다.                    200              100
        // 수직
        void PerspectiveFovLHDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
        {
            PerspectiveFovLHRad(_FovAngle * EngineMath::D2R, _Width / _Height, _Near, _Far);
        }

        // 수직 시야각 
        // 1000.0f 0.1f
        void PerspectiveFovLHRad(float _FovAngle, float _AspectRatio, float _Near, float _Far)
        {
            DirectXMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngle, _AspectRatio, _Near, _Far);

            //Identity();

            //// DirectX::XMMatrixPerspectiveFovLH()

            //float YFOV = _FovAngle * GameEngineMath::D2R;

            //// 원근 투영행렬에서 특징적인 부분.
            //Arr2D[2][3] = 1.0f;
            //Arr2D[3][3] = 0.0f;

            //// 투영행렬의 규칙은
            //// 모든 오브젝트의 모든 점을 -1 사이의 공간에 넣는것이다.

            //// 요 2값은 제대로된 
            //
            //// x와 곱해질 비율
            //Arr2D[0][0] = 1.0f / (tanf(YFOV / 2.0f) * _AspectRatio); // / 600


            //// y와 곱해질 비율
            //// 1나누기를 하는 이유는?
            //// -1 1사이의 값으로 만들려고.
            //
            //// 근본적인 원근투영의 원리는
            //// z값이 클수록 y값이 줄어든다.
            //// 이 y * 
            //Arr2D[1][1] = 1.0f / tanf(YFOV / 2.0f);  // / 600

            //// 1000 

            ////     100    100   100 * 투영
            ////  * 0.5f          *0.5f
            ////                  50.0f

            //// 범위안에 있는 녀석들 다 0~1사이의 값으로 바꿉니다.
            //// 1000 * 0.9784123f
            ////           1000  / (1000 - 0.1f);
            //Arr2D[2][2] = _Far / (_Far - _Near);

            //// 이동이 이 좀 들어가기는 했는데
            //Arr2D[3][2] = -(_Near * _Far) / (_Far - _Near);
        }


        void ViewPort(float _Width, float _Height, float _Left, float _Right, float _ZMin = 0.0f, float _ZMax = 1.0f)
        {
            Identity();

            Arr2D[0][0] = _Width * 0.5f;
            Arr2D[1][1] = -_Height * 0.5f; // 여기서 y축 반전을 시킨다.
            Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

            Arr2D[3][0] = Arr2D[0][0] + _Left;
            Arr2D[3][1] = -Arr2D[1][1] + _Right;
            Arr2D[3][2] = _ZMax != 0.0f ? 0.0f : _ZMin / _ZMax;
            Arr2D[3][3] = 1.0f;
        }

        float4x4 operator*(const float4x4& _Other)
        {
            const float4x4& A = *this;
            const float4x4& B = _Other;

            return DirectX::XMMatrixMultiply(A.DirectXMatrix, B.DirectXMatrix);
        }
    };
}