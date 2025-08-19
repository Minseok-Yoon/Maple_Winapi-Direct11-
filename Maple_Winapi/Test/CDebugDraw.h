#pragma once
#include "../CMath.h"
#include "../Resource/CTexture.h"

using namespace math; 

class CDebugDraw
{
public:
	static void DrawBox(const Vector3& center, const Vector2& size, const TextureColor& color);
	void DrawDot(const Vector3& center, const Vector4& color, float size);
};