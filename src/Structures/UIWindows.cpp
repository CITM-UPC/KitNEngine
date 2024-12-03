#include "Structures/UIWindows.h"
#include <windows.h>
#include <psapi.h>
#include <sstream>
#include <vector>
#include <string>
#include "Component/Camera.h"
#include <format> 
#include <cstdarg> 
size_t GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
}

// Vector para almacenar mensajes de log
std::vector<std::string> logMessages;

// Funci?n para agregar mensajes al log
void AddLogMessage(const char* format, ...) {
    char buffer[1024]; // Buffer para el mensaje formateado
    va_list args;      // Argumentos variables
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Añadir el mensaje formateado al vector
    logMessages.push_back(buffer);
}

void ShowConsoleWindow(bool* p_open) {
    if (!*p_open) return;

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Consola", p_open)) {
        if (ImGui::SmallButton("Limpiar")) {
            logMessages.clear();
        }

        ImGui::Separator();

        // Área de desplazamiento
        ImGui::BeginChild("Área de desplazamiento", ImVec2(0, 0), false,
            ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        for (const auto& message : logMessages) {
            // Detectar el tipo de mensaje si se tiene un sistema de prefijos
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Blanco por defecto
            if (message.find("[INFO]") == 0) {
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Verde
            }
            else if (message.find("[WARNING]") == 0) {
                color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Amarillo
            }
            else if (message.find("[ERROR]") == 0) {
                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Rojo
            }
            else if (message.find("[ASSIMP]") == 0) {
                color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cian
            }
            else if (message.find("[OK]") == 0) {
                color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Verde (OK)
            }

            ImGui::TextColored(color, "%s", message.c_str());
            if (logMessages.size() > Console_cap) {
                logMessages.erase(logMessages.begin(), logMessages.begin() + 100);
            }

        }

        // Desplazamiento automático si está en el fondo
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
    }
    ImGui::End();
}


void ShowConfigWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Configuracion", p_open)) {
        static float valores_fps[100] = {};
        static int indice_fps = 0;
        valores_fps[indice_fps] = ImGui::GetIO().Framerate;
        indice_fps = (indice_fps + 1) % 100;
        ImGui::PlotLines("FPS", valores_fps, 100);

        if (ImGui::CollapsingHeader("Configuracion de Modulos")) {
            ImGui::Text("Renderizador, Ventana, Entrada, Texturas...");
        }

        if (ImGui::CollapsingHeader("Informacion del Sistema")) {
            size_t memoriaUso = GetMemoryUsage();
            ImGui::Text("Uso de Memoria: %zu MB", memoriaUso);

            // Reemplazamos "Mondongo" por algo relacionado con la detecci?n de hardware
            ImGui::Text("Deteccion de Hardware:");

            // Aqu? podr?as agregar m?s detalles espec?ficos, como por ejemplo:
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            ImGui::Text("Procesador: %u nucleos", si.dwNumberOfProcessors);
            // Tambi?n puedes mostrar m?s informaci?n de hardware aqu? si lo deseas.

            ImGui::Text("Version de OpenGL: 4.3");
        }
    }
    ImGui::End();
}
void ShowInspectorWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Inspector", p_open)) {

        /* if (selectedItem == 0) {  // Si se ha seleccionado la C?mara
            ImGui::Text("Posici?n de la C?mara:");

            // Comprobamos si activeCamera est? v?lido antes de acceder a position
            
                //glm::vec3 camPos = Camera::activeCamera->position;
                //ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", camPos.x, camPos.y, camPos.z);
        
            
        }
        else if (selectedItem == 1) {  // Si se ha seleccionado "BakedHouse"
            ImGui::Text("GameObject: BakedHouse");

            ImGui::Text("Posici?n: 0  0  0");
            ImGui::Text("Rotaci?n: 0  0  0");
            ImGui::Text("Escala:  1  1  1");

            ImGui::Text("N?mero de v?rtices: 10000");
            ImGui::Text("N?mero de tri?ngulos: 2000");

            ImGui::Text("Tama?o de la textura: 30x30");
            ImGui::Text("Ruta de la textura: C:\\Users\\Paco\\Documents\\GitHub\\GameEngine\\Assets\\Textures");
        }*/


    }
    ImGui::End();
}


void ShowAboutPopup() {
    if (ImGui::BeginPopup("AboutPopup")) {
        ImGui::Text("Editor del Motor de Juego\nVersi?n 1.0\nPor Ropuce");
        ImGui::EndPopup();
    }
}

void ShowHerarkiWindow(bool* p_open) {
    if (!*p_open) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 220), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Jerarquia", p_open)) {
        ImGui::PushID("Jerarquia");
        for (std::shared_ptr<GameObject>& g :  GameObject::gameObjects) {
            DisplayGameObjectsInHierarchy(g);
        }
        ImGui::PopID();
    }
    ImGui::End();
}


void DisplayGameObjectsInHierarchy(std::shared_ptr<GameObject>& go){
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    auto nodeName = go->GetName();
    ImGui::PushID(go.get());
    
    auto list = go->GetChildren();
    if (list.empty())                           flags |= ImGuiTreeNodeFlags_Leaf;
    if (GameObject::selectedGameObject == go)   flags |= ImGuiTreeNodeFlags_Selected;
    
    if (ImGui::TreeNodeEx(nodeName.c_str(),flags))
    {
        
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            AddLogMessage(go->GetName()+" seleccionado desde jerarquia");
            GameObject::selectedGameObject = go;
        }
        
        for (std::shared_ptr<GameObject>& g : go->GetChildren())
        {
            DisplayGameObjectsInHierarchy(g);
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}
//void InitializeGeometryLoading() {
//    AddLogMessage("LOG: Iniciando carga de geometr?a desde ASSIMP...");
//    // C?digo de carga...
//    AddLogMessage("LOG: Carga de geometr?a completada.");
//}
//
//void InitializeLibraries() {
//    AddLogMessage("LOG: Iniciando inicializaci?n de bibliotecas externas...");
//    // C?digo de inicializaci?n...
//    AddLogMessage("LOG: Inicializaci?n de bibliotecas completada.");
//}