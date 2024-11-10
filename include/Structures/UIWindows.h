#ifndef UI_WINDOWS_H
#define UI_WINDOWS_H

#include <imgui.h>

static int selectedItem = -1;

void ShowConsoleWindow(bool* p_open);
void ShowConfigWindow(bool* p_open);
void ShowInspectorWindow(bool* p_open);
void ShowAboutPopup();
void ShowHerarkiWindow(bool* p_open);
void InitializeGeometryLoading();
void InitializeLibraries();

#endif 