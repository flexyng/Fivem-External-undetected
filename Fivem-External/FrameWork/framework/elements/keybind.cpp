#include "../settings/functions.h"

const char* keys[] =
{
    "None",
    "Mouse 1",
    "Mouse 2",
    "CN",
    "Mouse 3",
    "Mouse 4",
    "Mouse 5",
    "-",
    "Back",
    "Tab",
    "-",
    "-",
    "CLR",
    "Enter",
    "-",
    "-",
    "Shift",
    "CTL",
    "Menu",
    "Pause",
    "Caps",
    "KAN",
    "-",
    "JUN",
    "FIN",
    "KAN",
    "-",
    "Escape",
    "CON",
    "NCO",
    "ACC",
    "MAD",
    "Space",
    "PGU",
    "PGD",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "SEL",
    "PRI",
    "EXE",
    "PRI",
    "INS",
    "Delete",
    "HEL",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "WIN",
    "WIN",
    "APP",
    "-",
    "SLE",
    "Num 0",
    "Num 1",
    "Num 2",
    "Num 3",
    "Num 4",
    "Num 5",
    "Num 6",
    "Num 7",
    "Num 8",
    "Num 9",
    "MUL",
    "ADD",
    "SEP",
    "MIN",
    "Delete",
    "DIV",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "NUM",
    "SCR",
    "EQU",
    "MAS",
    "TOY",
    "OYA",
    "OYA",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "Shift",
    "Shift",
    "Ctrl",
    "Ctrl",
    "Alt",
    "Alt"
};

//bool keybind_selectable(const char* label, bool active)
//{
//    struct selectable_state
//    {
//        float alpha = 0.f;
//        ImVec4 color = clr->selectable.label_disabled;
//    };
//
//    ImGuiWindow* window = GetCurrentWindow();
//    if (window->SkipItems)
//        return false;
//
//    ImGuiContext& g = *GImGui;
//    const ImGuiStyle& style = g.Style;
//    const ImGuiID id = window->GetID(label);
//
//    const float width = GetContentRegionAvail().x;
//    const ImVec2 pos = window->DC.CursorPos;
//    const ImRect rect(pos, pos + ImVec2(width, SCALE(elements->selectable.height)));
//    ItemSize(rect, style.FramePadding.y);
//    if (!ItemAdd(rect, id))
//        return false;
//
//    bool hovered = IsItemHovered();
//    bool pressed = hovered && g.IO.MouseClicked[0];
//    if (pressed)
//        MarkItemEdited(id);
//
//    selectable_state* state = gui->anim_container(&state, id);
//    state->color = ImLerp(state->color, active ? clr->widgets.label : clr->selectable.label_disabled, gui->fixed_speed(12.f));
//    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(8.f) * (active ? 1.f : -1.f)), 0.f, 1.f);
//
//    draw->render_text(window->DrawList, var->font.montserrat[1], rect.Min - SCALE(0, 1), rect.Max - SCALE(0, 1), draw->get_clr(state->color), label, NULL, NULL, ImVec2(0.f, 0.5f));
//    RenderCheckMark(window->DrawList, rect.Max - SCALE(elements->selectable.checkmark_adjust), draw->get_clr(clr->accent, state->alpha), SCALE(8.f));
//
//    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
//    return pressed;
//}

//static float calc_keybind_size(int items_count, float item_size)
//{
//    ImGuiContext& g = *GImGui;
//    if (items_count <= 0)
//        return FLT_MAX;
//    return item_size * items_count + g.Style.ItemSpacing.y * (items_count - 1) + (g.Style.WindowPadding.y * 2);
//}
//

bool key_select(const char* k_id, int* key)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(k_id);

    const ImVec2 pos = window->DC.CursorPos;
    const float width = GetContentRegionAvail().x;

    const ImRect rect(pos, pos + ImVec2(width, SCALE(25)));
    ImGui::ItemSize(rect, style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    char buf_display[64] = "...";

    bool value_changed = false;
    int k = *key;

    std::string active_key = "";
    active_key += keys[*key];

    if (*key != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, active_key.c_str());
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, "...");
    }

    const bool hovered = ItemHoverable(rect, id, 0);

    if (hovered && io.MouseClicked[0])
    {
        if (g.ActiveId != id) {
            // Start edition
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *key = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        // Release focus when we click outside
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    if (g.ActiveId == id) {
        for (auto i = 0; i < 5; i++) {
            if (io.MouseDown[i]) {
                switch (i) {
                case 0:
                    k = 0x01;
                    break;
                case 1:
                    k = 0x02;
                    break;
                case 2:
                    k = 0x04;
                    break;
                case 3:
                    k = 0x05;
                    break;
                case 4:
                    k = 0x06;
                    break;
                }
                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = 0x08; i <= 0xA5; i++) {
                if (io.KeysDown[i]) {
                    k = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }

        if (IsKeyPressed(ImGuiKey_Escape)) {
            *key = 0;
            ImGui::ClearActiveID();
        }
        else {
            *key = k;
        }
    }

    window->DrawList->AddRectFilled(rect.Min, rect.Max, draw->get_clr(clr->keybind.button_bg), SCALE(elements->widgets.rounding));
    draw->render_text(window->DrawList, var->font.inter[2], rect.Min - SCALE(0, 1), rect.Max - SCALE(0, 1), draw->get_clr(clr->widgets.label), buf_display, NULL, NULL, ImVec2(0.5f, 0.5f));

    return value_changed;
}

bool hold_button(std::string_view label, std::string_view icon, bool reset = false)
{
    struct button_state
    {
        float alpha = 0.f;
        bool active = false;
    };

    ImGuiWindow* window = GetCurrentWindow();

    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const float width = GetContentRegionAvail().x;
    const ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + ImVec2(width, SCALE(25.f)));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held);

    button_state* state = gui->anim_container(&state, id);

    if (pressed)
        state->active = true;

    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(8.f) * (state->active ? 1.f : -1.f)), 0.f, 1.f);

    if (state->alpha >= 0.9f)
        state->active = false;

    window->DrawList->AddRectFilled(rect.Min, rect.Max, draw->get_clr(clr->keybind.button_bg, state->alpha), SCALE(elements->widgets.rounding));

    draw->render_text(window->DrawList, var->font.icons[6], rect.Min, ImVec2(rect.Min.x + SCALE(30), rect.Max.y), reset ? draw->get_clr(ImColor(255, 158, 158)) : draw->get_clr(clr->widgets.label), icon.data(), NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->render_text(window->DrawList, var->font.inter[2], rect.Min + SCALE(30, -1), rect.Max - SCALE(0, 1), reset ? draw->get_clr(ImColor(255, 158, 158)) : draw->get_clr(clr->widgets.label), label.data(), NULL, NULL, ImVec2(0.0f, 0.5f));

    return pressed;
}

bool toggle_button(std::string_view label, std::string_view icon, bool* callback)
{
    ImGuiWindow* window = GetCurrentWindow();

    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const float width = GetContentRegionAvail().x;
    const ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + ImVec2(width, SCALE(25.f)));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held);
    if (pressed)
        *callback = !(*callback);

    float* state = gui->anim_container(&state, id);

    *state = ImClamp(*state + (gui->fixed_speed(8.f) * (*callback ? 1.f : -1.f)), 0.f, 1.f);

    window->DrawList->AddRectFilled(rect.Min, rect.Max, draw->get_clr(clr->keybind.button_bg, *state), SCALE(elements->widgets.rounding));

    draw->render_text(window->DrawList, var->font.icons[6], rect.Min, ImVec2(rect.Min.x + SCALE(30), rect.Max.y), draw->get_clr(clr->widgets.label), icon.data(), NULL, NULL, ImVec2(0.5f, 0.5f));
    draw->render_text(window->DrawList, var->font.inter[2], rect.Min + SCALE(30, -1), rect.Max - SCALE(0, 1), draw->get_clr(clr->widgets.label), label.data(), NULL, NULL, ImVec2(0.0f, 0.5f));
    draw->render_text(window->DrawList, var->font.icons[7], ImVec2(rect.Max.x - SCALE(30), rect.Min.y), rect.Max, draw->get_clr(clr->widgets.label, *state), "D", NULL, NULL, ImVec2(0.5f, 0.5f));

    return pressed;
}

bool mode_button_toggle(std::string_view label, bool active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + SCALE(58, 20));
    ItemSize(rect, style.FramePadding.y);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held, 0);

    // Render

    float* state = gui->anim_container(&state, id);

    *state = ImClamp(*state + (gui->fixed_speed(8.f) * (active ? 1.f : -1.f)), 0.f, 1.f);

    window->DrawList->AddRectFilled(rect.Min, rect.Max, draw->get_clr(clr->keybind.background), SCALE(elements->widgets.rounding));
    window->DrawList->AddRectFilled(rect.Min, rect.Max, draw->get_clr(clr->accent, *state), SCALE(elements->widgets.rounding));

    draw->render_text(window->DrawList, var->font.inter[2], rect.Min - SCALE(0, 1), rect.Max - SCALE(0, 1), draw->get_clr(clr->widgets.label), label.data(), NULL, NULL, ImVec2(0.5f, 0.5f));

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

int mode_button(int* mode)
{
    PushStyleVar(ImGuiStyleVar_ItemSpacing, SCALE(4, 12));

    BeginGroup();
    {
        if (mode_button_toggle("Toggle", *mode == 0))
            *mode = 0;

        gui->sameline();

        if (mode_button_toggle("Hold", *mode == 1))
            *mode = 1;
    }
    EndGroup();
    PopStyleVar();

    return *mode;
}

void key_render_checkmark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz)
{
    float thickness = SCALE(1.f);
    pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

    float third = sz / 3.0f;
    float bx = pos.x + third;
    float by = pos.y + sz - third * 0.5f;
    draw_list->PathLineTo(ImVec2(bx - third, by - third));
    draw_list->PathLineTo(ImVec2(bx, by));
    draw_list->PathLineTo(ImVec2(bx + third * 2.0f, by - third * 2.0f));
    draw_list->PathStroke(col, 0, thickness);
}

bool key_checkbox(std::string_view label, bool* callback)
{
    struct checkbox_state
    {
        ImVec4 label_clr = clr->widgets.label_inactive;
        float radius = 0.f;
        float rounding = 0.f;
        float alpha[2] = { 0.f, 0.f };
    };

    ImGuiWindow* window = GetCurrentWindow();
    bool& favorite_state = var->gui.child_states[window->ChildId];

    if (!favorite_state && var->gui.favorite_childs) return false;

    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const ImRect total = gui->widget_rect();
    const ImRect rect(ImVec2(total.Min.x, total.GetCenter().y - SCALE(elements->checkbox.height) / 2), ImVec2(total.Max.x, total.GetCenter().y + SCALE(elements->checkbox.height) / 2));
    const ImRect clickable(ImVec2(rect.Max.x - SCALE(elements->checkbox.height), rect.Min.y), rect.Max);

    ItemSize(total, 0);
    if (!ItemAdd(total, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held);
    if (pressed)
        *callback = !(*callback);

    checkbox_state* state = gui->anim_container(&state, id);
    state->label_clr = ImLerp(state->label_clr, *callback ? clr->widgets.label : clr->widgets.label_inactive, gui->fixed_speed(10.f));
    state->alpha[0] = ImClamp(state->alpha[0] + (gui->fixed_speed(6.f) * (*callback ? 1.f : -1.f)), 0.f, 1.f);
    state->alpha[1] = ImClamp(state->alpha[1] + (gui->fixed_speed(10.f) * (state->rounding <= SCALE(elements->widgets.rounding + 0.5f) ? 1.f : -1.f)), 0.f, 1.f);
    state->radius = ImLerp(state->radius, *callback ? clickable.GetWidth() / 2.f : 0.f, gui->fixed_speed(12.f));
    state->rounding = ImLerp(state->rounding, *callback ? SCALE(elements->widgets.rounding) : SCALE(30.f), gui->fixed_speed(12.f));

    window->DrawList->AddRectFilled(clickable.Min, clickable.Max, draw->get_clr(clr->widgets.background), SCALE(elements->widgets.rounding));

    const int vtx_start = window->DrawList->VtxBuffer.Size;
    window->DrawList->AddRectFilled(clickable.GetCenter() - ImVec2(state->radius, state->radius), clickable.GetCenter() + ImVec2(state->radius, state->radius), draw->get_clr(ImVec4(1.f, 1.f, 1.f, state->alpha[0])), state->rounding);
    const int vtx_end = window->DrawList->VtxBuffer.Size;
    ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vtx_start, vtx_end, clickable.Min, clickable.Max, draw->get_clr(clr->accent), draw->get_clr(ImVec4(clr->accent.x * 0.2f, clr->accent.y * 0.2f, clr->accent.z * 0.2f, 1.f)));

    window->DrawList->PushClipRect(clickable.GetCenter() - ImVec2(state->radius, state->radius), clickable.GetCenter() + ImVec2(state->radius, state->radius), true);
    key_render_checkmark(window->DrawList, clickable.GetCenter() - SCALE(3.5f, 4.f), draw->get_clr(clr->widgets.label, state->alpha[1]), SCALE(8.f));
    window->DrawList->PopClipRect();

    draw->render_text(window->DrawList, var->font.inter[2], total.Min - SCALE(elements->widgets.label_padding), total.Max - SCALE(elements->widgets.label_padding), draw->get_clr(state->label_clr), label.data(), NULL, NULL, ImVec2(0.f, 0.5f));

    return pressed;
}

bool c_gui::keybind(std::string_view label, keybind_state* state)
{
    struct keybind_statee
    {
        bool active[2] = { false, false };
        bool hovered = false;
        float alpha[2] = { 0.f, 0.f };
    };

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const char* modes[3] = { "Hold", "Toggle", "Always" };

    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + SCALE(11, 11));

    ImGui::ItemSize(rect, style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    const bool hovered = ItemHoverable(rect, id, 0);

    keybind_statee* astate = gui->anim_container(&astate, id);

    if (hovered && g.IO.MouseClicked[0] || (astate->active[0] && g.IO.MouseClicked[0] && !astate->hovered))
        astate->active[0] = !astate->active[0];

    astate->alpha[0] = ImClamp(astate->alpha[0] + (8.f * g.IO.DeltaTime * (astate->active[0] ? 1.f : -1.f)), 0.f, 1.f);
    astate->alpha[1] = ImClamp(astate->alpha[1] + (8.f * g.IO.DeltaTime * (astate->active[1] ? 1.f : -1.f)), 0.f, 1.f);

    draw->render_text(window->DrawList, var->font.icons[5], rect.Min, rect.Max, draw->get_clr(clr->widgets.label_inactive), "C", NULL, NULL, ImVec2(0.5f, 0.5f));

    if (!IsRectVisible(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max + ImVec2(0, 2)))
    {
        astate->active[0] = false;
        astate->active[1] = false;
        astate->alpha[0] = 0.f;
        astate->alpha[1] = 0.f;
    }

    if (astate->alpha[0] <= 0.01f)
        astate->active[1] = false;

    gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(10, 10));
    gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(10, 10));
    gui->push_style_color(ImGuiCol_PopupBg, draw->get_clr(clr->keybind.background));
    gui->push_style_var(ImGuiStyleVar_PopupBorderSize, 0.f);
    if (astate->alpha[0] >= 0.01f);
    {
        astate->hovered = (g.HoveredWindow && strstr(g.HoveredWindow->Name, (std::stringstream{} << "keybind_window" << id).str().c_str()) && IsMouseHoveringRect(g.HoveredWindow->Pos, g.HoveredWindow->Pos + g.HoveredWindow->Size, false)) ||
            (g.HoveredWindow && strstr(g.HoveredWindow->Name, (std::stringstream{} << "key_window" << id).str().c_str()) && IsMouseHoveringRect(g.HoveredWindow->Pos, g.HoveredWindow->Pos + g.HoveredWindow->Size, false));

        SetNextWindowSize(SCALE(140, 115));
        SetNextWindowPos(rect.Min);
        gui->push_style_var(ImGuiStyleVar_Alpha, astate->alpha[0]);
        gui->begin((std::stringstream{} << "keybind_window" << id).str(), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse, true, draw->get_clr(clr->accent, 0.3f), SCALE(30.f), 0);
        {
            if (astate->active[0] && IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + GetWindowSize()))
                SetWindowFocus();

            if (hold_button("Copy Lua Path", "A"))
            {
                ///////////
            }

            toggle_button("New Bind", "B", &astate->active[1]);

            if (hold_button("Reset", "C", true))
            {
                state->key = 0;
                state->mode = 0;
                state->value = 0;
            }

        }
        gui->end();
        gui->pop_style_var();

        if (astate->alpha[1] >= 0.01f)
        {
            SetNextWindowSize(SCALE(140, 100));
            SetNextWindowPos(rect.Min + SCALE(150, 7));
            gui->push_style_var(ImGuiStyleVar_Alpha, astate->alpha[1] * astate->alpha[0]);
            gui->begin((std::stringstream{} << "key_window" << id).str(), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse, true, draw->get_clr(clr->accent, 0.3f), SCALE(30.f), 0);
            {
                if (astate->active[1] && IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + GetWindowSize()))
                    SetWindowFocus();

                key_select((std::stringstream{} << GetCurrentWindow()->ID << "key_select").str().c_str(), &state->key);

                mode_button(&state->mode);

                SetCursorPosY(GetCursorPosY() - SCALE(9));

                key_checkbox("Value", &state->value);
            }
            gui->end();
            gui->pop_style_var();
        }
    }
    gui->pop_style_color();
    gui->pop_style_var(3);

    return false;

}
