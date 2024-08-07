// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdlib.h>
#include <chrono>
#include <thread>

void setImGuiWindowSizeToOSWindow(GLFWwindow* window);

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Tizen Sideloader", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setImGuiWindowSizeToOSWindow(window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            std::string s;
            std::string command;
            static char str0[15] = "";
            static char str1[7] = "";
            static int clicked = 0;
            static std::string filePath;
            static int sideload;
            static std::string filePathName;
            static int tizendialog = 0;
            static char tizenloc[155];
            static int demowindow;

            freopen("out.txt","w",stdout);

            ImGui::Begin("Tizen Sideloader");                         
            ImGui::Text("Enter device IP and Port:");
            ImGui::PushItemWidth(110);               
            ImGui::InputTextWithHint("##IP", "IP", str0, IM_ARRAYSIZE(str0));
            ImGui::SameLine();
            ImGui::Text(":");
            ImGui::SameLine();
            ImGui::PushItemWidth(50);
            ImGui::InputTextWithHint("##Port", "Port", str1, IM_ARRAYSIZE(str1));
            ImGui::PopItemWidth();
            std::ostringstream str2;
            str2 << str0 << ":" << str1;
            //std::cout << str2.str();           
            //std::cout << str0;
            ImGui::SameLine();
            if (ImGui::Button("Connect!"))
                clicked++;

            if (clicked & 1) {
                 std::cout << str0;
                 std::ostringstream command;
                 command << tizenloc << "/tools/sdb connect " << str2.str();
                 int code = system(command.str().c_str());
                 clicked = 0;

                 if (code == 0) {
                     clicked = 0;
                     ImGui::SameLine();
                     ImGui::Text("Device connected! :)");
                 } else {
                     ImGui::SameLine();
                     ImGui::Text("An error occured :(");
                     ImGui::OpenPopup("An error occured :(");
             }

            }

            if (ImGui::Button("Choose WGT", ImVec2(250,100))) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".wgt", config);
            }

            if (ImGui::Button("Sideload!", ImVec2(250,150)))
                sideload++;

            if (ImGui::Button("Configure tizen-studio", ImVec2(250,50)))
                tizendialog++;

            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                    filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    // action
                    std::cout << filePath;

                }
    
            // close
            ImGuiFileDialog::Instance()->Close();
        }

            if (ImGui::Button("Open ImGui demo window", ImVec2(250,20)))
                demowindow++;
  

           if (sideload & 1) {
               std::ostringstream sideloadcommand;
               sideloadcommand << tizenloc <<"/tools/ide/bin/tizen install -n " << filePathName;
               int sideloadcode = system(sideloadcommand.str().c_str());
               sideload = 0;

               if (sideloadcode == 0) {
                   ImGui::OpenPopup("Sideloading Successful :)");
               } else {
                   ImGui::OpenPopup("An error occured sideloading :(");
               }
           }

           if (tizendialog & 1) {
               ImGui::OpenPopup("Configure tizen-studio");
           }

           if (demowindow & 1) {
               ImGui::ShowDemoWindow();
           }
             
           if (ImGui::BeginPopupModal("An error occured :(", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        
                ImGui::Text("An error has occured while connecting to your TV.");
                ImGui::Text("Please ensure you have entered the correct IP and port and your TV is in developer mode.");
                ImGui::Text("Please also ensure that you have set the location of tizen-studio.");
                ImGui::Text("Check the console for more info.");
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    ImGui::EndPopup();
        }

           if (ImGui::BeginPopupModal("Configure tizen-studio", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Enter the location of tizen-studio *CLI*");
                ImGui::Text("You can download it");
                ImGui::SameLine();
                ImGui::TextLinkOpenURL("here", "https://developer.tizen.org/development/tizen-studio/download");
                ImGui::InputText("Please do not include a trailing slash", tizenloc, IM_ARRAYSIZE(tizenloc));
                ImGui::SameLine();;

                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    tizendialog = 0;
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    ImGui::EndPopup();
            }
            


           if (ImGui::BeginPopupModal("An error occured sideloading :(", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("An error has occured while sideloading.");
                ImGui::Text("Please ensure the WGT file is compatible with your TV and your TV is connected to your device.");
                ImGui::Text("Please also ensure that you have set the location of tizen-studio.");
                ImGui::Text("Check the console for more info.");
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    ImGui::EndPopup();
        }

           if (ImGui::BeginPopupModal("Sideloading Successful :)", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        
                ImGui::Text("Sideloading Successful!");
                ImGui::Text("You should now see the app on your TV.");
                ImGui::Text("If you found this tool useful please consider leaving a ☆ on Github :3");
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::SameLine();
                    ImGui::EndPopup();
        }


}

ImGui::End();
        

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void setImGuiWindowSizeToOSWindow(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
}