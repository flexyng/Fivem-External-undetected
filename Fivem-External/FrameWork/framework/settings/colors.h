#pragma once
#include "imgui.h"

class c_colors
{
public:

	ImVec4 accent = ImColor(255, 129, 216); // deafault main color da

	struct
	{
		ImVec4 background = ImColor(1, 2, 1); // deafault Hintergrund 
		ImVec4 name = ImColor(255, 255, 255);
		ImVec4 year = ImColor(75, 83, 117);
	} window;

	struct
	{
		ImVec4 label = ImColor(255, 255, 255);
		ImVec4 background = ImColor(0, 0, 1); // die Tabs
		ImVec4 stroke = ImColor(2, 2, 2);
		ImVec4 line = ImColor(255, 129, 216);
		ImVec4 favorite_inactive = ImColor(38, 42, 63);
		ImVec4 favorite_active = ImColor(255, 129, 216);
	} child;

	struct
	{
		ImVec4 label = ImColor(235, 238, 255);
		ImVec4 label_inactive = ImColor(75, 83, 117);
		ImVec4 background = ImColor(1, 1, 1);
	} widgets;

	struct
	{
		ImVec4 icon = ImColor(79, 85, 118);
	} section;

	struct
	{
		ImVec4 label = ImColor(79, 85, 118);
	} favorite_button;

	struct
	{
		ImVec4 background = ImColor(9, 10, 16);
	} dropdown;

	struct
	{
		ImVec4 background = ImColor(9, 10, 16);
	} colorpicker;

	struct
	{
		ImVec4 background = ImColor(9, 10, 16);
		ImVec4 button_bg = ImColor(13, 15, 24);
	} keybind;

	struct
	{
		ImVec4 background = ImColor(1, 1, 1);
		ImVec4 label = ImColor(255, 129, 216);
	} notify;

	struct
	{
		ImVec4 background = ImColor(1, 1, 1);
		ImVec4 text = ImColor(235, 238, 255);
		ImVec4 line = ImColor(255, 129, 216);
	} watermark;
};

inline c_colors* clr = new c_colors();