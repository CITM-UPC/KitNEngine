#ifndef UI_WINDOWS_H
#define UI_WINDOWS_H

#include <imgui.h>
#include <Component/GameObject.h>

// Declaración de funciones
void ShowConsoleWindow(bool* p_open);
void ShowConfigWindow(bool* p_open);
void ShowInspectorWindow(bool* p_open);
void ShowAboutPopup();
void ShowHerarkiWindow(bool* p_open);
void InitializeGeometryLoading();
void InitializeLibraries();
void DisplayGameObjectsInHierarchy(std::shared_ptr<GameObject>& go);

// Nueva función para añadir mensajes formateados al log
void AddLogMessage(const char* format, ...);

#endif