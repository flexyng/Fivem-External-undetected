#include "settings/functions.h"
#include <Windows.h>
#include <core.cpp>

void c_gui::render()
{
	
	notify->setup_notify();

	static ImVec2 preview_pos;
	static int active_tab = 0;

	gui->set_next_window_size(SCALE(var->window.size));
	gui->set_next_window_pos(ImGui::GetIO().DisplaySize / 2 - SCALE(var->window.size) / 2, ImGuiCond_Once);
	gui->begin("menu", nullptr, var->window.flags, 0, 0, 0, 0);
	{
		const ImVec2 pos = GetWindowPos();
		const ImVec2 size = GetWindowSize();
		ImDrawList* drawlist = GetWindowDrawList();
		ImGuiStyle* style = &GetStyle();
		preview_pos = pos + ImVec2(size.x + SCALE(10), SCALE(90));

		{
			style->WindowPadding = SCALE(var->window.padding);
			style->ItemSpacing = SCALE(var->window.spacing);
			style->WindowRounding = SCALE(var->window.rounding);
			style->ChildRounding = style->WindowRounding;
			style->WindowBorderSize = SCALE(var->window.border_size);
			style->WindowShadowSize = SCALE(var->window.shadow_size);
			style->ScrollbarSize = SCALE(var->window.scrollbar_size);
		}

		{
			gui->push_style_color(ImGuiCol_ChildBg, draw->get_clr(clr->window.background, 0.5f));

			gui->begin_content("logo_rect", SCALE(var->window.logo_size));
			{
				draw->render_text(GetWindowDrawList(), var->font.icons[0], GetWindowPos(), GetWindowPos() + GetWindowSize(), draw->get_clr(clr->accent), "A", NULL, NULL, ImVec2(0.5f, 0.5f));
			}
			gui->end_content();

			gui->sameline(false);

			gui->begin_content("child_select", SCALE(var->window.child_select_size), SCALE(elements->favorite_button.padding), SCALE(elements->favorite_button.spacing));
			{
				if (gui->favorite_button("Favorites", "A", 1))
					var->gui.favorite_childs = true;

				gui->sameline(false);

				if (gui->favorite_button("Standart", "B", 0))
					var->gui.favorite_childs = false;
			}
			gui->end_content();

			gui->sameline(false);

			gui->begin_group();
			{
				gui->begin_content("gui_size", SCALE(var->window.gui_dpi_size), SCALE(elements->content.padding.x, 5.f), SCALE(elements->content.spacing));
				{
					gui->slider_int("GUI SIZE", &var->stored_dpi, 75, 200, "%d%%", 0, true);

					if (var->dpi != var->stored_dpi / 100.f && IsMouseReleased(ImGuiMouseButton_Left))
						var->dpi_changed = true;
				}
				gui->end_content();

				gui->begin_content("hue_slider", SCALE(var->window.hue_slider_size), SCALE(elements->content.padding.x - 5.f, 10.f), SCALE(elements->content.spacing));
				{
					static float menu_col[4] = { clr->accent.x, clr->accent.y, clr->accent.z, 1.f };
					if (gui->hue_slider("HUE", menu_col))
					{
						clr->accent.x = menu_col[0];
						clr->accent.y = menu_col[1];
						clr->accent.z = menu_col[2];
					}
				}
				gui->end_content();
			}
			gui->end_group();

			gui->begin_content("sections", ImVec2(SCALE(var->window.section_size.x), var->window.section_size.y), SCALE(elements->section.padding), SCALE(elements->section.spacing));
			{
				for (int i = 0; i < IM_ARRAYSIZE(var->gui.section_icons); i++)
					gui->section(var->gui.section_icons[i], i, var->gui.section_count);

				var->window.section_size.y = GetCurrentWindow()->ContentSize.y + GetStyle().WindowPadding.y * 2;
			}
			gui->end_content();

			gui->sameline(false);

			gui->pop_style_color();
		}

		{
			gui->push_style_color(ImGuiCol_ChildBg, draw->get_clr(clr->window.background, 0.5f));

			var->gui.content_alpha = ImClamp(var->gui.content_alpha + (gui->fixed_speed(6.f) * (var->gui.section_count == active_tab ? 1.f : -1.f)), 0.f, 1.f);

			if (var->gui.content_alpha == 0.f)
				active_tab = var->gui.section_count;

			gui->begin_content("functional", GetWindowSize() - SCALE(var->window.section_size.x, var->window.child_select_size.y + var->window.topbar_size.y + GetStyle().ItemSpacing.y) - GetStyle().ItemSpacing, SCALE(elements->content.padding), SCALE(elements->content.spacing), false, false);
			{
				gui->push_style_var(ImGuiStyleVar_Alpha, var->gui.content_alpha);

				if (active_tab == 0)
				{
					gui->begin_group();
					{
						gui->begin_child("Aimbot", "A");
						{
							static keybind_state enabled_state = { 0, 0, 0 };
							static bool enabled = true;
							gui->checkbox_with_key("Enable", &enabled, &enabled_state);
							
							
							gui->checkbox_with_key("Humanizer", &enabled, &enabled_state);

							static int field_of_view = 145;
							gui->slider_int("Smooth", &field_of_view, 0, 180);

							static int Speed = 1;
							gui->slider_int("Speed", &Speed, 0, 7);
						}
						gui->end_child();

						gui->begin_child("Options", "B");
						{
							const char* target_items[3] = { "Head", "Chest", "Legs" };
							static std::vector<int> target_count = { 1, 0, 0 };
							gui->multi_dropdown("Hitbox", target_count, target_items, IM_ARRAYSIZE(target_items));

						}
						gui->end_child();
					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						gui->begin_child("Color", "C");
						{
							const char* history_items[3] = { "Purpl3", "Yellow", "Red" };
							static int history_count = 1;
							gui->dropdown("Outline", &history_count, history_items, IM_ARRAYSIZE(history_items));

						}
						gui->end_child();

						
					}
					gui->end_group();
				}
				else if (active_tab == 1)
				{
					gui->begin_group();
					{
						gui->begin_child("Triggerbot", "F");
						{
							auto& cfg = g_options.esp.player;
							gui->checkbox("Enabled", &cfg.enabled);
							gui->checkbox("Box", &cfg.box);

							
						}
						gui->end_child();

						gui->begin_child("Chams", "G");
						{
							static float chams_color[4] = { 1.f, 1.f, 1.f, 1.f };
							static bool enable_chams = true;
							gui->checkbox_with_color("Chams", &enable_chams, chams_color, true);

							static bool through_walls = false;
							gui->checkbox("Through walls", &through_walls);

							static float backtrack_col[4] = { 0.71f, 0.54f, 0.54f, 1.f };
							static bool backtrack = false;
							gui->checkbox_with_color("Backtrack", &backtrack, backtrack_col, true);
						}
						gui->end_child();
					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						gui->begin_child("Models", "I");
						{
							static bool bomb = true;
							gui->checkbox("Bomb", &bomb);

							static bool weapons = false;
							gui->checkbox("Weapons", &weapons);

							static float grenades_col[4] = { 0.71f, 0.54f, 0.54f, 1.f };
							static bool grenades = true;
							gui->checkbox_with_color("Grenades", &grenades, grenades_col, true);
						}
						gui->end_child();

						/*gui->begin_child("Misc", "M");
						{
							static bool visual_recoil = false;
							gui->checkbox("Visual recoil", &visual_recoil);

							static bool crosshair = false;
							gui->checkbox("Crosshair", &crosshair);

							static keybind_state thirdperson_state = { 0, 0, 0 };
							static bool thirdperson = true;
							gui->checkbox_with_key("Thirdperson", &thirdperson, &thirdperson_state);
						}
						gui->end_child();*/
					}
					gui->end_group();
				}
				else if (active_tab == 2)
				{
					gui->begin_group();
					{
						gui->begin_child("Config - Saver", "L");
						{
							if (gui->button("Save"))
								notify->add_notify("Config,   Saved", false);

							if (gui->button("Delete"))
								notify->add_notify("Config,   Deleted", true);
						}
						gui->end_child();
					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						/*gui->begin_child("Developers", "M");
						{
							static char designer[32] = "Past Owl";
							gui->text_field("Designer", designer, 32);

							static char coder[32] = "asdasd";
							gui->text_field("Coder", coder, 32);
						}
						gui->end_child();*/
					}
					gui->end_group();
				}
				gui->pop_style_var();
			}
			gui->end_content();

			gui->set_cursor_pos(GetCursorPos() + ImVec2(SCALE(var->window.section_size.x) + style->ItemSpacing.x, 0));
			gui->begin_content("topbar_content", SCALE(var->window.topbar_size));
			{
				draw->render_text(GetWindowDrawList(), var->font.inter[2], GetWindowPos() - SCALE(-10, 1), GetWindowPos() + GetWindowSize() - SCALE(0, 1), draw->get_clr(clr->window.name), var->gui.cheat_name.c_str(), NULL, NULL, ImVec2(0.f, 0.5f));
				draw->render_text(GetWindowDrawList(), var->font.inter[2], GetWindowPos() - SCALE(0, 1), GetWindowPos() + GetWindowSize() - SCALE(10, 1), draw->get_clr(clr->window.year), var->gui.cheat_year.c_str(), NULL, NULL, ImVec2(1.f, 0.5f));
			}
			gui->end_content();
			gui->pop_style_color();


		
		}
	}



	gui->end();

	draw->watermark("FiveM Private", "Laxor", "-", "Ok");
}

