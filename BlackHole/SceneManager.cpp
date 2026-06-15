#include "SceneManager.h"

SceneManager::SceneManager() : camera() {}

int SceneManager::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    title = "MiNI Black Hole";
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return arrange();
}

int SceneManager::arrange() {
    try {
        skybox = Skybox({ "skybox/space_right.png",
                          "skybox/space_left.png",
                          "skybox/space_up.png",
                          "skybox/space_down.png",
                          "skybox/space_front.png",
                          "skybox/space_back.png" });
        skyboxShader = Shader("skybox.vert", "skybox.frag");
    } catch (std::exception& e) {
        std::cout << e.what();
        return -1;
    }
    return 0;
}

int SceneManager::run() {
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        accumTime += deltaTime;

        std::ostringstream ss;
        ss << "[";
        ss.precision(0);
        ss << std::fixed << ImGui::GetIO().Framerate;
        ss << " FPS] " << title;
        glfwSetWindowTitle(window, ss.str().c_str());
        processInput();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderScene();
        renderUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return terminate();
}

void SceneManager::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        cursor = !cursor;
        if (cursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

void SceneManager::renderUI() {
    ImVec2 size = ImGui::GetIO().DisplaySize;
    float width = 200;
    ImGui::SetNextWindowPos(ImVec2(size.x - width, 0));
    ImGui::SetNextWindowSize(ImVec2(width, 0));
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    float oldDist = camera.Dist;

    ImGui::Spacing();
    ImGui::Text("Distance: "); ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat("##disti", &camera.Dist);
    camera.ClampDist();
    
    ImGui::Spacing();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##dists", &camera.Dist, 0, 100);
    camera.ClampDist();

    if (oldDist != camera.Dist) {
        camera.UpdateCameraVectors();
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Mass: "); ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat("##massi", &mass);
    
    ImGui::Spacing();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##masss", &mass, 0, 10);
    ImGui::Spacing();
    ImGui::End();
}

void SceneManager::renderScene() {
    glm::mat4 projection = glm::perspective(glm::radians(90.f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    // remove translation from the view matrix
    view = glm::mat4(glm::mat3(view));
    skyboxShader.use();
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setVec3("cameraPos", camera.Position);
    skyboxShader.setFloat("mass", mass);
    skybox.Draw();
}

int SceneManager::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    SceneManager* context = static_cast<SceneManager*>(glfwGetWindowUserPointer(window));
    context->width = width;
    context->height = height;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    SceneManager* context = static_cast<SceneManager*>(glfwGetWindowUserPointer(window));

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (context->firstMouse) {
        context->lastX = xpos;
        context->lastY = ypos;
        context->firstMouse = false;
    }

    float xoffset = xpos - context->lastX;
    float yoffset = context->lastY - ypos; // reversed since y-coordinates go from bottom to top

    context->lastX = xpos;
    context->lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        context->camera.ProcessMouseMovement(xoffset, yoffset);
    }
}
