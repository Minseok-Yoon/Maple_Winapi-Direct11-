#pragma once
#include "CMath.h"

class CMath4
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
        Return *= CMath4::D2R;
        Return = DirectX::XMQuaternionRotationRollPitchYawFromVector(Return.DirectXVector);
        return Return;
    }

    float4 QuaternionToEulerDeg()
    {
        // 디그리 각도로 바꿔줍니다.
        return QuaternionToEulerRad() * CMath4::R2D;
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
            result.X = -(0.5f * CMath4::PI);
        }
        else if (pitchTest > asinThreshold)
        {
            result.X = (0.5f * CMath4::PI);
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
        return Angle2DRad() * CMath4::R2D;
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
        return VectorRotationToRadX(_Value, _Deg * CMath4::D2R);
    }

    static float4 VectorRotationToRadX(const float4& _Value, const float _Rad);

    float4 VectorRotationToDegYReturn(const float _Deg)
    {
        return VectorRotationToDegY(*this, _Deg);
    }


    static float4 VectorRotationToDegY(const float4& _Value, const float _Deg)
    {
        return VectorRotationToRadY(_Value, _Deg * CMath4::D2R);
    }

    static float4 VectorRotationToRadY(const float4& _Value, const float _Rad);

    float4 VectorRotationToDegZReturn(const float _Deg)
    {
        return VectorRotationToDegZ(*this, _Deg);
    }

    static float4 VectorRotationToDegZ(const float4& _Value, const float _Deg)
    {
        return VectorRotationToRadZ(_Value, _Deg * CMath4::D2R);
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
        return GetUnitVectorFromRad(_Degree * CMath4::D2R);
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