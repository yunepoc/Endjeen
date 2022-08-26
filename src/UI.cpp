#include <UI.hpp>

#include <App.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace ej {

void UI::addDebugBoolean(std::string name, bool* value) {
  debugBooleans.push_back({name, value});
}

void UI::setDebugLabel(std::string name, std::string value) {
  for (auto &p: debugLabels)
    if (p.first == name) {
      p.second = value;
      return;
    }
  debugLabels.push_back({name, value});
}

void UI::gameUI() {
  ImGui::SetNextWindowPos(ImVec2(0,0));
  ImGui::SetNextWindowSize(ImVec2(0,0));
  ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
  auto& buildings = App::instance().getGame().getBuildings();
  for (auto& building: buildings) {
    bool b = ImGui::ImageButton(building->getName().c_str(),(void*)(intptr_t)building->getIcon().handle, {96,96});
    if (b) {
      SystemMsg msg("UI", "build");
      msg.addString(building->getName());
      System::send(msg);
    }
  }
  ImGui::End();
}

void UI::debugUI() {
  glm::uvec2 wsize = App::instance().getWindow().getSize();
  int width = 800;
  int height = 500;
  ImGui::SetNextWindowPos(ImVec2(wsize.x - width, wsize.y - height));
  ImGui::SetNextWindowSize(ImVec2(width, height));
  ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
  // FPS counter
  std::string fps = std::to_string(std::lround(App::instance().getRenderer().getFPS()));
  ImGui::Text(("FPS: " + fps).c_str());
  // Labels
  for (auto &p: debugLabels)
    ImGui::Text((p.first + ":" + p.second).c_str());
  // Boolean debug values
  for (auto &p: debugBooleans)
    ImGui::Checkbox(p.first.c_str(), p.second);
  ImGui::End();
  //ImGui::ShowDemoWindow();
}

void UI::load() {
  ImGui::CreateContext();
  void* handle = App::instance().getWindow().getHandle();
  ImGui_ImplOpenGL3_Init();
  ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(handle),true);
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  std::string path = App::instance().getResourceMgr().getFilePath("/font.ttf");
  io.Fonts->AddFontFromFileTTF(path.c_str(), 48);
}

void UI::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  gameUI();
  debugUI();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
