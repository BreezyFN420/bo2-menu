#include "drawing.h"

namespace fonts
{
	DWORD* normal_font = *(DWORD**)0x2959390;
	DWORD* extra_small_font = *(DWORD**)0x2959394;
	void* small_dev_font = RegisterFont("fonts/smalldevfont", 7);
}

void drawing_c::draw_string(const char* text, void* font, float x, float y, float w, color_t color)
{
	char buffer[255] = "";

	va_list va_alist;
	va_start(va_alist, text);
	vsnprintf(buffer, sizeof(buffer), text, va_alist);
	va_end(va_alist);

	CL_DrawTextPhysical(text, 0x7FFFFFFF, font, x, y, w, 1.0f, 1.0f, color, 0, 1.0);
}

void drawing_c::draw_filled_rect(float x, float y, float w, float h, color_t color)
{
	DrawShaderStretched(x, y, w, h, 0, 1, 1, 0, color, white_shader);
}

void drawing_c::draw_rotated_line(float x1, float y1, float x2, float y2, color_t color)
{
	float l1 = x2 - x1, h = y2 - y1, l2 = sqrt(l1 * l1 + h * h);
	CG_DrawRotatedPicPhysical(ScrPlace(0), x1 + ((l1 - l2) / 2.f), y1 + (h / 2.f), l2, 1.0f, (180.f / M_PI * atan(h / l1)), COL_WHITE, white_shader);
}

void drawing_c::draw_line(int x1, int y1, int x2, int y2, float* color, float size)
{
	float x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = (float)atan(h1 / l1) * (180 / 3.14159265358979323846);

	DrawRotatedPic(x, y, l2, size, 0, 0, 0, 0, angle, color, white_shader);
}

void drawing_c::draw_box(float x, float y, float w, float h, float size, color_t color)
{
	draw_filled_rect(x, y, w, size, color);
	y += size;
	draw_filled_rect(x, y, size, h - (2 * size), color);
	y += (h - (2 * size));
	draw_filled_rect(x, y, w, size, color);
	y -= (h - (2 * size));
	x += (w - size);
	draw_filled_rect(x, y, size, h - (2 * size), color);
}

void drawing_c::draw_circle(int centerX, int centerY, float radius, float* color, float thickness, int segments)
{
	if (segments < 8) segments = 8; // minimum segments for a rough circle

	const float step = (2.0f * 3.14159265358979323846f) / segments;

	for (int i = 0; i < segments; i++)
	{
		float angle1 = step * i;
		float angle2 = step * (i + 1);

		int x1 = centerX + (int)(cosf(angle1) * radius);
		int y1 = centerY + (int)(sinf(angle1) * radius);
		int x2 = centerX + (int)(cosf(angle2) * radius);
		int y2 = centerY + (int)(sinf(angle2) * radius);

		draw_line(x1, y1, x2, y2, color, thickness);
	}
}
