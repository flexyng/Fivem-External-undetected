#pragma once

#include <string>

#include "../../../FrameWork/Dependencies/ImGui/imgui.h"
#include "../../../FrameWork/Dependencies/ImGui/imgui_internal.h"
#include <cmath>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


struct render_t
{

#define vec_ref const ImVec2&
#define decl static auto

	inline static ImFont* verdana_12{ NULL };
	inline static ImFont* verdana_bold{ NULL };

	decl rect_filled_blur(vec_ref from, vec_ref to, ImU32 col, float rounding = 0.f, int layers = 5, float offset = 0.5f) {
		auto draw = ImGui::GetBackgroundDrawList();
		for (int i = 0; i < layers; ++i) {
			ImVec2 offset_pos{ offset * i, offset * i };
			draw->AddRectFilled(from - offset_pos, to + offset_pos, col, rounding);
		}
	}

	decl rect(vec_ref from, vec_ref to, ImU32 col, float rounding = 0.f) {
		ImGui::GetBackgroundDrawList()->AddRect(from, to, col, rounding);
	}

	void rect_filled(vec_ref from, vec_ref to, ImU32 col, float rounding = 0.f) {
		ImVec2 rounded_from = { std::round(from.x), std::round(from.y) };
		ImVec2 rounded_to = { std::round(to.x), std::round(to.y) };
		ImGui::GetBackgroundDrawList()->AddRectFilled(rounded_from, rounded_to, col, rounding);
	}

	decl outlined_rect_blur(vec_ref pos, vec_ref size, ImU32 col, float blur_intensity = 0.5f, int layers = 30) {
		auto draw = ImGui::GetBackgroundDrawList();

		for (int i = 1; i <= layers; ++i) {
			float alpha_factor = 1.0f - (float(i) / layers);
			ImU32 faded_col = ImGui::ColorConvertFloat4ToU32(ImVec4(
				(col >> IM_COL32_R_SHIFT & 0xFF) / 255.0f,
				(col >> IM_COL32_G_SHIFT & 0xFF) / 255.0f,
				(col >> IM_COL32_B_SHIFT & 0xFF) / 255.0f,
				alpha_factor * blur_intensity
			));

			ImVec2 offset(i * 0.5f, i * 0.5f);
			draw->AddRect(pos - offset, pos + size + offset, faded_col, 0.0f);
			draw->AddRect(pos + offset, pos + size - offset, faded_col, 0.0f);
		}

		draw->AddRect(pos, pos + size, col, 0.0f);
	}

	void outlined_rect(vec_ref pos, vec_ref size, ImU32 col, float rounding = 0.f) {
		ImVec2 rounded_pos = ImVec2(std::round(pos.x), std::round(pos.y));
		ImVec2 rounded_size = ImVec2(std::round(size.x), std::round(size.y));

		auto draw = ImGui::GetBackgroundDrawList();
		const ImRect rect_bb(rounded_pos, rounded_pos + rounded_size);

		float max_rounding = std::min(rounded_size.x, rounded_size.y) / 2.0f;
		rounding = std::min(rounding, max_rounding);

		draw->AddRect(rect_bb.Min, rect_bb.Max, IM_COL32(0, 0, 0, col >> 24), rounding);
		draw->AddRect(rect_bb.Min - ImVec2{ 2.f, 2.f }, rect_bb.Max + ImVec2{ 2.f, 2.f }, IM_COL32(0, 0, 0, col >> 24), rounding);
		draw->AddRect(rect_bb.Min - ImVec2{ 1.f, 1.f }, rect_bb.Max + ImVec2{ 1.f, 1.f }, col, rounding);
	}



	decl cornered_rect(vec_ref pos, vec_ref size, ImU32 col)
	{
		auto draw = ImGui::GetBackgroundDrawList();

		float X = pos.x;
		float Y = pos.y;
		float W = size.x;
		float H = size.y;

		float lineW = (size.x / 4);
		float lineH = (size.y / 4);
		float lineT = 1;

		auto outline = IM_COL32(0, 0, 0, col >> 24);

		float topOffset = 1.0f;

		draw->AddLine({ roundf(X - lineT + 1.f), roundf(Y - lineT - topOffset) }, { roundf(X + lineW), roundf(Y - lineT - topOffset) }, outline);
		draw->AddLine({ roundf(X - lineT), roundf(Y - lineT - topOffset) }, { roundf(X - lineT), roundf(Y + lineH) }, outline);

		draw->AddLine({ roundf(X + W - lineW), roundf(Y - lineT - topOffset) }, { roundf(X + W + lineT), roundf(Y - lineT - topOffset) }, outline);
		draw->AddLine({ roundf(X + W + lineT), roundf(Y - lineT - topOffset) }, { roundf(X + W + lineT), roundf(Y + lineH) }, outline);

		draw->AddLine({ roundf(X + W + lineT), roundf(Y + H - lineH) }, { roundf(X + W + lineT), roundf(Y + H + lineT) }, outline);
		draw->AddLine({ roundf(X + W - lineW), roundf(Y + H + lineT) }, { roundf(X + W + lineT), roundf(Y + H + lineT) }, outline);

		draw->AddLine({ roundf(X - lineT), roundf(Y + H - lineH) }, { roundf(X - lineT), roundf(Y + H + lineT) }, outline);
		draw->AddLine({ roundf(X - lineT), roundf(Y + H + lineT) }, { roundf(X + lineW), roundf(Y + H + lineT) }, outline);

		{
			draw->AddLine({ roundf(X - (lineT - 3)), roundf(Y - (lineT - 2) - topOffset) }, { roundf(X + lineW), roundf(Y - (lineT - 2) - topOffset) }, outline);
			draw->AddLine({ roundf(X - (lineT - 2)), roundf(Y - (lineT - 2) - topOffset) }, { roundf(X - (lineT - 2)), roundf(Y + lineH) }, outline);

			draw->AddLine({ roundf(X - (lineT - 2)), roundf(Y + H - lineH) }, { roundf(X - (lineT - 2)), roundf(Y + H + (lineT - 2)) }, outline);
			draw->AddLine({ roundf(X - (lineT - 2)), roundf(Y + H + (lineT - 2)) }, { roundf(X + lineW), roundf(Y + H + (lineT - 2)) }, outline);

			draw->AddLine({ roundf(X + W - lineW), roundf(Y - (lineT - 2) - topOffset) }, { roundf(X + W + (lineT - 2)), roundf(Y - (lineT - 2) - topOffset) }, outline);
			draw->AddLine({ roundf(X + W + (lineT - 2)), roundf(Y - (lineT - 2) - topOffset) }, { roundf(X + W + (lineT - 2)), roundf(Y + lineH) }, outline);

			draw->AddLine({ roundf(X + W + (lineT - 2)), roundf(Y + H - lineH) }, { roundf(X + W + (lineT - 2)), roundf(Y + H + (lineT - 2)) }, outline);
			draw->AddLine({ roundf(X + W - lineW), roundf(Y + H + (lineT - 2)) }, { roundf(X + W + (lineT - 2)), roundf(Y + H + (lineT - 2)) }, outline);
		}

		draw->AddLine({ roundf(X), roundf(Y - topOffset) }, { roundf(X), roundf(Y + lineH) }, col);
		draw->AddLine({ roundf(X + 1.f), roundf(Y - topOffset) }, { roundf(X + lineW), roundf(Y - topOffset) }, col);

		draw->AddLine({ roundf(X + W - lineW), roundf(Y - topOffset) }, { roundf(X + W), roundf(Y - topOffset) }, col);
		draw->AddLine({ roundf(X + W), roundf(Y - topOffset) }, { roundf(X + W), roundf(Y + lineH) }, col);

		draw->AddLine({ roundf(X), roundf(Y + H - lineH) }, { roundf(X), roundf(Y + H) }, col);
		draw->AddLine({ roundf(X), roundf(Y + H) }, { roundf(X + lineW), roundf(Y + H) }, col);

		draw->AddLine({ roundf(X + W - lineW), roundf(Y + H) }, { roundf(X + W), roundf(Y + H) }, col);
		draw->AddLine({ roundf(X + W), roundf(Y + H - lineH) }, { roundf(X + W), roundf(Y + H) }, col);
	}

	void text(vec_ref pos, const std::string& text, ImU32 col) {
		ImGuiIO& io = ImGui::GetIO();
		ImFont* defaultFont = io.FontDefault;
		ImGui::GetBackgroundDrawList()->AddText(defaultFont, 15.f, pos, col, text.c_str());
	}

	void health_bar(float max, float current, vec_ref pos, vec_ref size, float alpha_factor, float x_pad = 4.f, float bar_width = 3.f, bool blue = false) {

		int clamped_health = std::min(max, current);
		float box_outline_adjustment = 2.0f;
		float bar_height = ((size.y + box_outline_adjustment) * clamped_health) / max;
		float full_height = std::round(size.y + box_outline_adjustment);


		int currentHealth = static_cast<int>(current);
		auto adjusted_left = ImVec2{ std::round(pos.x + size.x), std::round(pos.y + full_height) };
		auto adjusted_right = ImVec2{ std::round(pos.x + size.x), std::round(pos.y) };
		std::string healthText = std::to_string(static_cast<int>(current));
		ImVec2 textSize = ImGui::CalcTextSize(healthText.c_str());
		ImVec2 textPos = ImVec2(pos.x + (bar_width - textSize.x) / 2.0f, pos.y + size.y - bar_height - textSize.y - -11.5f);
		if (currentHealth < 100) {
			ImGui::GetForegroundDrawList()->AddText(textPos, IM_COL32(255, 255, 255, static_cast<int>(255 * alpha_factor)), healthText.c_str());
		}



		rect_filled({ adjusted_left.x - bar_width - x_pad + 0.5f, adjusted_right.y + 0.5f }, { adjusted_left.x - x_pad - 0.5f, adjusted_left.y - 0.5f }, IM_COL32(60, 60, 60, 200 * alpha_factor));

		//ImU32 health_bar_color = IM_COL32(static_cast<int>(state::health_bar_color[0] * 255), static_cast<int>(state::health_bar_color[1] * 255), static_cast<int>(state::health_bar_color[2] * 255), static_cast<int>(255 * alpha_factor));

		rect_filled({ adjusted_left.x - bar_width - x_pad + 1.0f, adjusted_left.y - 1.0f }, { adjusted_left.x - x_pad - 1.0f, adjusted_left.y - bar_height + 0.8f }, 255, 255);

		rect({ adjusted_left.x - bar_width - x_pad, adjusted_left.y - 0.5f }, { adjusted_left.x - x_pad, adjusted_left.y - 0.5f }, IM_COL32(10, 10, 10, 255 * alpha_factor));

		rect({ adjusted_left.x - bar_width - x_pad, adjusted_right.y }, { adjusted_left.x - x_pad, adjusted_left.y }, IM_COL32(5, 5, 5, 255 * alpha_factor));

	}


	void shield_bar(float max, float current, vec_ref pos, vec_ref size, float alpha_factor) {
		auto adj_pos_x = std::round(pos.x) - 1;
		auto adj_size_x = std::round(size.x) + 2;

		auto adj_from = ImVec2(adj_pos_x, std::round(pos.y) + 5.f);
		auto adj_to = ImVec2(adj_pos_x + adj_size_x, std::round(pos.y) + 8.f);

		auto bar_width = current * adj_size_x / max;

		rect_filled(adj_from, adj_to, IM_COL32(60, 60, 60, 200 * alpha_factor));

		ImColor col(52, 103, 235, static_cast<int>(255 * alpha_factor));

		rect_filled(
			{ adj_pos_x + 1.f, std::round(pos.y) + 5.f },
			{ adj_pos_x + adj_size_x - bar_width - 1.f, std::round(pos.y) + 8.f },
			col
		);

		rect({ adj_from.x - 1.f, adj_from.y - 1.f }, { adj_to.x + 1.f, adj_from.y - 1.f }, IM_COL32(0, 0, 0, 255 * alpha_factor));

		rect({ adj_from.x - 1.f, adj_to.y }, { adj_to.x + 1.f, adj_to.y + 2.f }, IM_COL32(0, 0, 0, 255 * alpha_factor));
	}


	decl text_shadowed(vec_ref pos, const std::string& text, ImU32 col, float font_size = 10.f)
	{

		auto alpha = col >> 24;

		if (alpha > 0)
		{

			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(-1.f, 1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());
			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(1.f, -1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());

			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(1.f, 1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());
			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(-1.f, -1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());

			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(0.f, 1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());
			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(0.f, -1.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());

			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(1.f, 0.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());
			ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos + ImVec2(-1.f, 0.f), IM_COL32(0, 0, 0, col >> 24), text.c_str());

		}

		ImGui::GetBackgroundDrawList()->AddText(nullptr, font_size, pos, col, text.c_str());

	}

	decl line(vec_ref from, vec_ref to, ImU32 col, float thickness) {
		ImGui::GetBackgroundDrawList()->AddLine(from, to, col, thickness);
	}

	decl line_segment(vec_ref from, vec_ref to, ImU32 col, float thickness, float segments = 1.f) {

		if (segments > 1)
		{

			auto draw_list = ImGui::GetBackgroundDrawList();

			float segment_length = 1.0f / segments;
			ImVec2 delta = to - from;

			for (int i = 0; i < segments; ++i)
			{

				float alpha = segment_length * i;

				ImVec2 segment_pos = ImVec2(from.x + delta.x * alpha, from.y + delta.y * alpha);
				draw_list->AddCircleFilled(segment_pos, thickness, col);

			}

		}

	}

	decl circle(vec_ref pos, ImU32 col, float radius, int segments = 6) {
		ImGui::GetBackgroundDrawList()->AddCircle(pos, radius, col, segments);
	}

	decl circle_filled(vec_ref pos, ImU32 col, float radius) {
		ImGui::GetBackgroundDrawList()->AddCircleFilled(pos, radius, col, 128);
	}

	decl initialize()
	{

	}

}; inline render_t render{};