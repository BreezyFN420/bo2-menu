#pragma once
#include "engine.h"

#include <cstdio>
#include <math.h>

using color_t = float[4];

static color_t COL_WHITE = { 1.f, 1.f, 1.f, 1.f };
static color_t COL_RED = { 1.f, 0.f, 0.f, 1.f };

class drawing_c
{
public:
	void draw_string(const char* text, void* font, float x, float y, float w, color_t color);
	void draw_filled_rect(float x, float y, float w, float h, color_t color);
	void draw_rotated_line(float x1, float y1, float x2, float y2, color_t color);
	void draw_line(int x1, int y1, int x2, int y2, float* color, float size);
	void draw_box(float x, float y, float w, float h, float size, color_t color);
	void draw_circle(int centerX, int centerY, float radius, float* color, float thickness, int segments);
}; inline drawing_c draw;

struct font_s
{
	char uk0[4];
	int Height;
};

namespace fonts
{
	extern DWORD* normal_font;
	extern DWORD* extra_small_font;
	extern void* small_dev_font;
}

// Engine drawing functions
using RegisterShader_t = void* (__cdecl*)(const char* shader, int a2);
static RegisterShader_t RegisterShader = reinterpret_cast<RegisterShader_t>(0x734CC0);

// default shader
static void* white_shader = RegisterShader("white", 7);

using RegisterFont_t = void* (__cdecl*)(const char* font, int a2);
static RegisterFont_t RegisterFont = reinterpret_cast<RegisterFont_t>(0x4FF570);

using DrawTextPhysical_t = void (__cdecl*)(
	const char* text, int maxChars, void* font, 
	float x, float y, float w, 
	float xScale, float yScale, float* color, 
	int style, float padding
	);
static DrawTextPhysical_t CL_DrawTextPhysical = reinterpret_cast<DrawTextPhysical_t>(0x4048B0);

using DrawShaderStretched_t = void (__cdecl*)(
	float x, float y, float w, float h, 
	float TileX, float shaderwidth, float shaderheight, 
	float TileY, float* color, void* pShader);
static DrawShaderStretched_t DrawShaderStretched = reinterpret_cast<DrawShaderStretched_t>(0x710D60);

using ScrPlace_GetView_t = int (__cdecl*)(int a1);
static ScrPlace_GetView_t ScrPlace = reinterpret_cast<ScrPlace_GetView_t>(0x6F6AF0);

using CG_DrawRotatedPicPhysical_t = int (__cdecl*)(int scrPlace, float x, float y, float w, float h, int angle, float* color, void* pShader);
static CG_DrawRotatedPicPhysical_t CG_DrawRotatedPicPhysical = reinterpret_cast<CG_DrawRotatedPicPhysical_t>(0x5A7080);

using CG_DrawRotatedPic_t = int (__cdecl*)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float angle, float* color, void* material);
static CG_DrawRotatedPic_t DrawRotatedPic = reinterpret_cast<CG_DrawRotatedPic_t>(0x710DF0);

using UI_TextWidth_t = int (__cdecl*)(int clientNum, const char* text, int maxChars, void* font, float scale);
static UI_TextWidth_t UI_TextWidth = reinterpret_cast<UI_TextWidth_t>(0x41F490);