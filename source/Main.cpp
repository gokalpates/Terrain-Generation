#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Debug.h"
#include "Shader.h"
#include "Camera.h"
#include "ProcuderalTerrain.h"

int screenWidth = 2560, screenHeight = 1440;
float deltaTime = 0.0, currentFrame, lastFrame = 0.f, fpsUpdateTime = 0.f;
unsigned long long fpsTick = 0ull;
int FPS = 0;

void windowSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, windowSizeCallback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	glClearColor(0.008, 0.8, 0.8, 1.f);
	glEnable(GL_DEPTH_TEST);

	Camera camera(window);
	camera.setCameraSpeed(45.f);

	glm::mat4 model(1.f);
	glm::mat4 view(1.f);
	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)(screenWidth) / (float)(screenHeight), 0.1f, 10000.f);

	Shader shader("shader/main.vert", "shader/main.frag");

	TerrainInfo info;
	info.triYTriScale = 80.f;
	info.mapSize = 4096;
	ProcuderalTerrain* terrain = new ProcuderalTerrain(info);

	std::cout << "vertices count: " << terrain->getVertexCount() << std::endl;
	std::cout << "normals count: " << terrain->getNormalCount() << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsUpdateTime += deltaTime;
		if (fpsUpdateTime > 1.0)
		{
			FPS = fpsTick;
			fpsUpdateTime = 0.0;
			fpsTick = 0ull;
		}

		glfwPollEvents();
		if ( glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) && glfwGetKey(window, GLFW_KEY_X))
		{
			glfwSetWindowShouldClose(window, true);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static bool cullfaceOption = false;
			ImGui::Begin("OpenGL Renderer");
			ImGui::Text("FPS: %d\n", FPS);
			ImGui::Text("Press E or R to enable/disable face culling.");
			ImGui::Text("Press Left Control + X to exit.");

			ImGui::Checkbox("Cull Face", &cullfaceOption);

			if (glfwGetKey(window,GLFW_KEY_E))
			{
				cullfaceOption = true;
			}
			if (glfwGetKey(window, GLFW_KEY_R))
			{
				cullfaceOption = false;
			}

			if (cullfaceOption)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			ImGui::End();
		}

		camera.update();
		view = camera.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setAllMat4(model, view, projection);
		terrain->draw(shader);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		fpsTick++;
	}

	delete terrain;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}
