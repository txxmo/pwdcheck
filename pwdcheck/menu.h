#include "ImGui/ImGui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui//imgui_stdlib.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <random>


namespace menu {
	void render();
	extern ImFont* iconFont;

	extern ImVec2 screenSize;
}

extern ImDrawList* draw;