#ifndef UI_WINDOWS_H
#define UI_WINDOWS_H

#include <imgui.h>
#include <Component/GameObject.h>�



// Declaraci�n de funciones
void ShowConsoleWindow(bool* p_open);
void ShowConfigWindow(bool* p_open);
void ShowInspectorWindow(bool* p_open);
void ShowAboutPopup();
void ShowHerarkiWindow(bool* p_open);
void DisplayGameObjectsInHierarchy(std::shared_ptr<GameObject>& go);

// Nueva funci�n para a�adir mensajes formateados al log
void AddLogMessage(const char* format, ...);

#endif