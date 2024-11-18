#pragma once

typedef struct _tagRectangle
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectangle() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}
}RECTANGLE, * PRECTANGLE;

typedef struct _tagPixel
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}PIXEL, * PPIXEL;