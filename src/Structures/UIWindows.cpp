#include "Structures/UIWindows.h"
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>

size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
}

// Vector para almacenar mensajes de log
std::vector<std::string> logMessages;

// Funci�n para agregar mensajes al log
void AddLogMessage(const std::string& message) {
    logMessages.push_back(message);
}

void ShowConsoleWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Consola", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::TextWrapped("Aqu� aparecer�n los mensajes de la consola...");

        // Mostrar todos los mensajes de log en la consola
        for (const auto& message : logMessages) {
            ImGui::TextWrapped("%s", message.c_str());
        }

        ImGui::End();
    }
}

void ShowConfigWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Configuraci�n", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        static float valores_fps[100] = {};
        static int indice_fps = 0;
        valores_fps[indice_fps] = ImGui::GetIO().Framerate;
        indice_fps = (indice_fps + 1) % 100;
        ImGui::PlotLines("FPS", valores_fps, 100);

        if (ImGui::CollapsingHeader("Configuraci�n de M�dulos")) {
            ImGui::Text("Renderizador, Ventana, Entrada, Texturas...");
        }

        if (ImGui::CollapsingHeader("Informaci�n del Sistema")) {
            size_t memoriaUso = GetMemoryUsage();
            ImGui::Text("Uso de Memoria: %zu MB", memoriaUso);
            ImGui::Text("Mondongo");
            ImGui::Text("Versi�n de OpenGL: 4.3");
        }
        ImGui::End();
    }
}

void ShowInspectorWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (*p_open && ImGui::Begin("Inspector", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("GameObject: BakedHouse" );

        ImGui::Text("Position: 0  0  0");
        ImGui::Text("Rotation: 0  0  0");
        ImGui::Text("Scale:  1  1  1");

        ImGui::Text("Number of vertices: 10000");
        ImGui::Text("Number of triangles: 2000" );

        ImGui::Text("Texture Size: 30x30");
        ImGui::Text("Texture Path C:\Users\Paco\Documents\GitHub\GameEngine\Assets\Textures");
        ImGui::End();
    }
}

void ShowAboutPopup() {
    if (ImGui::BeginPopup("AboutPopup")) {
        ImGui::Text("Editor del Motor de Juego\nVersi�n 1.0\nPor Ropuce");
        ImGui::EndPopup();
    }
}

void ShowHerarkiWindow(bool* p_open) {
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    if (*p_open && ImGui::Begin("Jerarqu�a", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("Camara");
        ImGui::Text("BakerHouse");
        /*ImGui::Text("Objeto 3");
        if (ImGui::TreeNode("Grupo de Objetos")) {
            ImGui::Text("Objeto 4");
            ImGui::Text("Objeto 5");
            ImGui::TreePop();
        }*/
        ImGui::End();
    }
}

void InitializeGeometryLoading() {
    AddLogMessage("LOG: Iniciando carga de geometr�a desde ASSIMP...");
    // C�digo de carga...
    AddLogMessage("LOG: Carga de geometr�a completada.");
}

void InitializeLibraries() {
    AddLogMessage("LOG: Iniciando inicializaci�n de bibliotecas externas...");
    // C�digo de inicializaci�n...
    AddLogMessage("LOG: Inicializaci�n de bibliotecas completada.");
}