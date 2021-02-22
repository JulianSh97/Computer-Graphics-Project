#define _USE_MATH_DEFINES
#define PHONGSHADING 999
#define WIREFRAME 990
#define TEXTURED 900
#define REFLECTION 909
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <string>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;

static void RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV) {
	float min, max, delta;

	min = fR < fG ? fR : fG;
	min = min < fB ? min : fB;

	max = fR > fG ? fR : fG;
	max = max > fB ? max : fB;

	fV = max;
	delta = max - min;
	if (delta < 0.00001f)
	{
		fS = 0.f;
		fH = 0.f;
	}
	if (max > 0.0f) {
		fS = (delta / max);
	}
	else {
		fS = 0.0f;
		fH = 0;
		return ;
	}
	if (fR >= max)                           // > is bogus, just keeps compiler happy
		fH = (fG - fB) / delta;        // between yellow & magenta
	else
		if (fG >= max)
			fH = 2.0f + (fB - fR) / delta;  // between cyan & yellow
		else
			fH = 4.0f + (fR - fG) / delta;  // between magenta & cyan

	fH *= 60.0;                              // degrees

	if (fH < 0.0)
		fH += 360.0;

	return;
}
glm::vec4 clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.00f);
static float BoundingBoxColor[3] = { 1.f, 0.f, 0.f };
static float NormalsColor[3]= { 0.585f, 0.f, 0.6045f };
static float FacesNormalsColor[3] = { 0.f,0.f,1.f };
static float DModelColor[3]= {0.2f, 0.75f, 0.8f};
static float AModelColor[3]= {0.2f, 0.75f, 0.8f};
static float SModelColor[3]= {0.2f, 0.75f, 0.8f};
int width_;
int height_;
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene, Renderer& renderer);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1960, windowHeight = 1080;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	renderer.LoadShaders();
	Scene scene = Scene();
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene,renderer);
		RenderFrame(window, scene, renderer, io);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetWindowAspectRatio(window, renderer.GetViewportWidth(), renderer.GetViewportHeight());
	width_ = renderer.GetViewportWidth();
	height_ = renderer.GetViewportHeight();


	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		glfwSetWindowAspectRatio(window, width_, height_);
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[65])
		{
			// A key is down
			// Use the ASCII table for more key codes (https://www.asciitable.com/)
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//added

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene,Renderer& renderer)
{
	static bool CameraWindow = true;
	static bool ModelWindow = true;
	static bool LightWindow = false;
	static bool ColorsWindow = true;
	static bool Fog = false;
	static float OrthoWidth;
	static int Shadingtype = 0;
	static int coloringWay = 0;
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Choose Model"))
		{
			if (ImGui::MenuItem("Banana")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\banana.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Beethoven")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\beethoven.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 2;
			}
			if (ImGui::MenuItem("Bishop")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\bishop.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Blob")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\blob.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Bunny")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\bunny.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Camera")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\camera.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Chain")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\chain.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Cow")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\cow.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Demo")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\demo.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Dolphin")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\dolphin.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Feline")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\feline.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Pawn")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\pawn.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Teapot")) {
				scene.AddModel(Utils::LoadMeshModel("C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Data\\teapot.obj"));
				OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
			}
			if (ImGui::MenuItem("Other", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddCamera(std::make_shared<Camera>(glm::vec3(0, 0, 1)));
					scene.AddModel(Utils::LoadMeshModel(outPath));
					OrthoWidth = (scene.GetActiveModel()->GetMinOrtho() + scene.GetActiveModel()->GetMaxOrtho()) / 3;
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}
			}
			ImGui::EndMenu();
			if (scene.GetModelCount())
			{
				scene.AddCamera(std::make_shared<Camera>(scene.GetActiveModel()->GetModelCenter()));
				scene.SetActiveCameraIndex(0);
			}
		}
		if (ImGui::Button("Load Texture"))
		{
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);
			if (result == NFD_OKAY)
			{
				scene.GetActiveModel()->LoadTextures(outPath);
				free(outPath);
			}
		}
		if (ImGui::Button("Clear Model"))
		{
			scene.GetActiveCamera()->ResetTransformations();
			scene.GetActiveModel()->ResetModel();
			scene.ClearActiveModel();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::Button("Camera Window"))
				CameraWindow = true;
			if (ImGui::Button("Model Window"))
				ModelWindow = true;
			if (ImGui::Button("Light Window"))
				LightWindow = true;
			if (ImGui::Button("Control Window"))
				ColorsWindow = true;
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
	
	//***************************************************** Control Window*****************************************************
	static int fogType;
	if (ColorsWindow) {
		ImGui::Begin("Controls Menu");
		// Controls
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::ColorEdit3("Background Color", (float*)&clear_color);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.f);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.f));

		if (ImGui::Button("Close Me"))
			ColorsWindow = false;
		ImGui::PopStyleColor();
		if (ImGui::TreeNode("Add Environment"))
		{
			std::vector<std::string> faces;
			if (ImGui::Button("Mountains"))
			{

			}
			if (ImGui::Button("Random"))
			{
				faces =
				{
					"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\right1.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\left1.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\top1.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\bottom1.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\front1.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\back1.jpg"
				};
				scene.cubemap = std::make_shared<CubeMap>(faces);
				scene.SetenvironmentMap(true);
				scene.GetActiveModel()->SetColorMethod(REFLECTION);
			}
			if (ImGui::Button("Sea"))
			{
				faces=
				{
					"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\right.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\left.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\top.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\bottom.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\front.jpg",
						"C:\\Users\\most_\\OneDrive\\Documents\\GitHub\\computergraphics2021-f-r-i-e-n-d-s\\Skybox\\back.jpg"
				};
				scene.cubemap = std::make_shared<CubeMap>(faces);
				scene.SetenvironmentMap(true);
				scene.GetActiveModel()->SetColorMethod(REFLECTION);
			}

			ImGui::TreePop();
		}
		
		ImGui::End();
	}

	//*******************************************************************************MODEL WINDOW***********************************************************************
	if(scene.GetModelCount()>0)
	{
		auto model = scene.GetActiveModel();
		static float DModelColor[3] = { 0.5f,0.285f,0.285f };
		static float AModelColor[3] = {0.0625f ,0.3f,0.57f };
		static float SModelColor[3]=  {};
		const static char* items[] = { "Scale","Rotate","Translate"};
		const static char* TransformItems[] = {"World Transformation","Local Transformation"};
		const static char* Axis[] = { "X Axis","Y Axis","Z Axis" };
		static int SelectedItem = 0;
		static int SelectedTransform = 0;
		static int SelectedAxis;
		static float ScaleX = 1.f;
		static float ScaleY = 1.f;
		static float ScaleZ = 1.f;
		static float WScaleX = 1.f;
		static float WScaleY = 1.f;
		static float WScaleZ = 1.f;
		static int current_model = 0;
		glm::mat4x4 Transformation;
		static float TranslateX = 0.f;
		static float TranslateY = 0.f;
		static float TranslateZ = 0.f;
		static float WTranslateX = 0.f;
		static float WTranslateY = 0.f;
		static float WTranslateZ = 0.f;
		static float AngleX =0.f;
		static float AngleY =0.f;
		static float AngleZ =0.f;
		static float WAngleX=0.f;
		static float WAngleY=0.f;
		static float WAngleZ=0.f;
		static float alpha = 2.f;

		if (ModelWindow)
		{
			ImGui::Begin("Transformations Window");
			if (ImGui::SliderInt("Active Model Index", &current_model, 0, scene.GetModelCount() - 1))
			{
				scene.SetActiveModelIndex(current_model);
				TranslateX = scene.GetActiveModel()->GetTranslateX();
				TranslateY = scene.GetActiveModel()->GetTranslateY();
				TranslateZ = scene.GetActiveModel()->GetTranslateZ();
				WTranslateX = scene.GetActiveModel()->GetWTranslateX();
				WTranslateY = scene.GetActiveModel()->GetWTranslateY();
				WTranslateZ = scene.GetActiveModel()->GetWTranslateZ();
				AngleX = scene.GetActiveModel()->GetAngleX();
				AngleY = scene.GetActiveModel()->GetAngleY();
				AngleZ = scene.GetActiveModel()->GetAngleZ();
				WAngleX = scene.GetActiveModel()->GetWAngleX();
				WAngleY = scene.GetActiveModel()->GetWAngleY();
				WAngleZ = scene.GetActiveModel()->GetWAngleZ();
				ScaleX = scene.GetActiveModel()->GetScaleX();
				ScaleY =scene.GetActiveModel()->GetScaleY();
				ScaleZ =scene.GetActiveModel()->GetScaleZ();
				WScaleX=scene.GetActiveModel()->GetWScaleX();
				WScaleY=scene.GetActiveModel()->GetWScaleY();
				WScaleZ=scene.GetActiveModel()->GetWScaleZ();
				DModelColor[0] = scene.GetActiveModel()->GetDiffuseColor().x;
				DModelColor[1] = scene.GetActiveModel()->GetDiffuseColor().y;
				DModelColor[2] = scene.GetActiveModel()->GetDiffuseColor().z;
				AModelColor[0] = scene.GetActiveModel()->GetAmbientColor().x;
				AModelColor[1] = scene.GetActiveModel()->GetAmbientColor().y;
				AModelColor[2] = scene.GetActiveModel()->GetAmbientColor().z;
				SModelColor[0] = scene.GetActiveModel()->GetSpecularColor().x;
				SModelColor[1] = scene.GetActiveModel()->GetSpecularColor().y;
				SModelColor[2] = scene.GetActiveModel()->GetSpecularColor().z;
			}
			if (ImGui::Button("Phong Shading"))
				scene.GetActiveModel()->SetColorMethod(PHONGSHADING);
			ImGui::SameLine();
			if (ImGui::Button("Wireframe Model"))
				scene.GetActiveModel()->SetColorMethod(WIREFRAME);
			ImGui::SameLine();
			if (ImGui::Button("Textured Model"))
				scene.GetActiveModel()->SetColorMethod(TEXTURED);
			ImGui::ColorEdit3("Diffuse Model Color", DModelColor);
			scene.GetActiveModel()->SetDiffuseColor(glm::vec3(DModelColor[0], DModelColor[1], DModelColor[2]));
			ImGui::ColorEdit3("Ambient Model Color", AModelColor);
			scene.GetActiveModel()->SetAmbientColor(glm::vec3(AModelColor[0], AModelColor[1], AModelColor[2]));
			ImGui::ColorEdit3("Specular Model Color", SModelColor);
			scene.GetActiveModel()->SetSpecularColor(glm::vec3(SModelColor[0], SModelColor[1], SModelColor[2]));
			scene.GetActiveModel()->SetColors(BoundingBoxColor, FacesNormalsColor, NormalsColor);

			ImGui::SliderFloat("alpha coefficent", &alpha, 1, 10);
			scene.GetActiveModel()->SetAlpha(alpha);
			ImGui::ListBox("World Or Local", &SelectedTransform, TransformItems, IM_ARRAYSIZE(TransformItems), 2);
			ImGui::ListBox("Choose Transformation", &SelectedItem, items, IM_ARRAYSIZE(items), 3);
			if (SelectedTransform) {
				switch (SelectedItem)
				{
				case 0:
					ImGui::SliderFloat("Scale Factor X", &ScaleX, 0.f, 3.f);
					ImGui::SliderFloat("Scale Factor Y", &ScaleY, 0.f, 3.f);
					ImGui::SliderFloat("Scale Factor Z", &ScaleZ, 0.f, 3.f);
					scene.GetActiveModel()->SetScaleX(ScaleX);
					scene.GetActiveModel()->SetScaleY(ScaleY);
					scene.GetActiveModel()->SetScaleZ(ScaleZ);
					Transformation = Transformations::ScalingTransformation(ScaleX, ScaleY, ScaleZ);
					scene.GetActiveModel()->Set_S_m(Transformation);
					break;
				case 1:
					ImGui::ListBox("Choose Axis to rotate around", &SelectedAxis, Axis, IM_ARRAYSIZE(Axis), 3);
					if (SelectedAxis == 0) {
						ImGui::SliderFloat("Rotation Angle", &AngleX, 0, 360);
						scene.GetActiveModel()->SetAngleX(AngleX);
						Transformation = Transformations::XRotationTransformation(AngleX);
					}
					if (SelectedAxis == 1)
					{
						ImGui::SliderFloat("Rotation Angle", &AngleY, 0, 360);
						scene.GetActiveModel()->SetAngleY(AngleY);
						Transformation = Transformations::YRotationTransformation(AngleY);
					}
					if (SelectedAxis == 2)
					{
						ImGui::SliderFloat("Rotation Angle", &AngleZ, 0, 360);
						scene.GetActiveModel()->SetAngleZ(AngleZ);
						Transformation = Transformations::ZRotationTransformation(AngleZ);
					}
					scene.GetActiveModel()->SetRotationMatrix(Transformation, false, SelectedAxis + 1);
					scene.GetActiveModel()->Set_R_m();
					break;
				case 2:
					ImGui::SliderFloat("Translate Factor X", &TranslateX, -220, 220);
					ImGui::SliderFloat("Translate Factor Y", &TranslateY, -220, 220);
					ImGui::SliderFloat("Translate Factor Z", &TranslateZ, -220, 220);
					scene.GetActiveModel()->SetTranslateX(TranslateX);
					scene.GetActiveModel()->SetTranslateY(TranslateY);
					scene.GetActiveModel()->SetTranslateZ(TranslateZ);
					Transformation = Transformations::TranslationTransformation(TranslateX / scene.GetActiveModel()->GetTranslateFactor(), TranslateY / scene.GetActiveModel()->GetTranslateFactor(), TranslateZ / scene.GetActiveModel()->GetTranslateFactor());
					scene.GetActiveModel()->Set_T_m(Transformation);
					break;
				default:
					break;
				}
				scene.GetActiveModel()->SetModelTransformation(scene.GetActiveModel()->Get_S_m() * scene.GetActiveModel()->Get_R_m() * scene.GetActiveModel()->Get_T_m());
			}
			else
			{
				switch (SelectedItem)
				{
				case 0:
					ImGui::SliderFloat("World Scale Factor X", &WScaleX, 0.f, 3.f);
					ImGui::SliderFloat("World Scale Factor Y", &WScaleY, 0.f, 3.f);
					ImGui::SliderFloat("World Scale Factor Z", &WScaleZ, 0.f, 3.f);
					scene.GetActiveModel()->SetWScaleX(WScaleX);
					scene.GetActiveModel()->SetWScaleY(WScaleY);
					scene.GetActiveModel()->SetWScaleZ(WScaleZ);
					Transformation = Transformations::ScalingTransformation(WScaleX, WScaleY, WScaleZ);
					scene.GetActiveModel()->Set_S_w(Transformation);
					break;
				case 1:
					ImGui::ListBox("Choose World Axis to rotate around", &SelectedAxis, Axis, IM_ARRAYSIZE(Axis), 3);
					if (SelectedAxis == 0) {
						ImGui::SliderFloat("Rotation Angle", &WAngleX, 0, 360);
						scene.GetActiveModel()->SetWAngleX(WAngleX);
						Transformation = Transformations::XRotationTransformation(WAngleX);
					}
					if (SelectedAxis == 1)
					{
						ImGui::SliderFloat("Rotation Angle", &WAngleY, 0, 360);
						scene.GetActiveModel()->SetWAngleY(WAngleY);
						Transformation = Transformations::YRotationTransformation(WAngleY);
					}
					if (SelectedAxis == 2)
					{
						ImGui::SliderFloat("Rotation Angle", &WAngleZ, 0, 360);
						scene.GetActiveModel()->SetWAngleZ(WAngleZ);
						Transformation = Transformations::ZRotationTransformation(WAngleZ);
					}
					scene.GetActiveModel()->SetRotationMatrix(Transformation, true, SelectedAxis + 1);
					scene.GetActiveModel()->Set_R_w();
					break;
				case 2:
					ImGui::SliderFloat("World Translate Factor X", &WTranslateX, -220, 220);
					ImGui::SliderFloat("World Translate Factor Y", &WTranslateY, -220, 220);
					ImGui::SliderFloat("World Translate Factor Z", &WTranslateZ, -220, 220);
					scene.GetActiveModel()->SetWTranslateX(WTranslateX);
					scene.GetActiveModel()->SetWTranslateY(WTranslateY);
					scene.GetActiveModel()->SetWTranslateZ(WTranslateZ);
					Transformation = Transformations::TranslationTransformation(WTranslateX / scene.GetActiveModel()->GetTranslateFactor(), WTranslateY / scene.GetActiveModel()->GetTranslateFactor(), WTranslateZ / scene.GetActiveModel()->GetTranslateFactor());
					scene.GetActiveModel()->Set_T_w(Transformation);
					break;
				default:
					break;
				}
				scene.GetActiveModel()->SetWorldTransformation(scene.GetActiveModel()->Get_S_w() * scene.GetActiveModel()->Get_R_w() * scene.GetActiveModel()->Get_T_w());
			}
			scene.GetActiveModel()->SetTransformation();

			if (ImGui::Button("Reset"))
			{
				ScaleX = 1.f;
				ScaleY = 1.f;
				ScaleZ = 1.f;
				WScaleX = 1.f;
				WScaleY = 1.f;
				WScaleZ = 1.f;
				TranslateX = 0;
				TranslateY = 0;
				TranslateZ = 0;
				WTranslateX = 0;
				WTranslateY = 0;
				WTranslateZ = 0;
				AngleX = 0;
				AngleY = 0;
				AngleZ = 0;
				WAngleX = 0;
				WAngleY = 0;
				WAngleZ = 0;
				scene.GetActiveModel()->ResetModel();
			}
			
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.f));
			if (ImGui::Button("Close Me"))
				ModelWindow = false;
			ImGui::PopStyleColor();
			ImGui::End();
		}
		//*******************************************************************************LIGHT WINDOW***********************************************************************
		glm::vec3 DLightC,SLightC,ALightC;
		static float TranslateX_;
		static float TranslateY_;
		static float TranslateZ_;
		static float WTranslateX_;
		static float WTranslateY_;
		static float WTranslateZ_;
		static float AngleX_;
		static float AngleY_;
		static float AngleZ_;
		static float WAngleX_;
		static float WAngleY_;
		static float WAngleZ_;
		static float DlightColor[3] = { 1.f,1.f,1.f };
		static float AlightColor[3] = { 1.f,1.f,1.f };
		static float SlightColor[3] = { 1.f,1.f,1.f };
		static int lightType;
		static float lightIntensity;
		static LightType type;
		static int L_Material;
		static int TransformationType_;
		const static char* TransformItems_[] = { "World Transformation","Local Transformation" };
		const static char* Axis_[] = { "X Axis","Y Axis","Z Axis" };
		static int SelectedItem_ = 0;
		static int SelectedTransform_ = 0;
		static int SelectedAxis_;
		static int IsWorld_;
		static int item_current = 0;
		static float DLL[3],ALL[3],SLL[3];
		static float xDirection;
		static float yDirection;
		static float zDirection;
		if (LightWindow)
		{
			ImGui::Begin("Light Window");
			{
				if (ImGui::TreeNode("Add Light Source")) 
				{
					ImGui::RadioButton("Parallel", &lightType, 0);
					ImGui::SameLine(); ImGui::RadioButton("Point", &lightType, 1);
					ImGui::ColorEdit3("Diffusive Light Color", DlightColor);
					ImGui::ColorEdit3("Ambient Light Color", AlightColor);
					ImGui::ColorEdit3("Specular Light Color", SlightColor);
					DLightC = glm::vec3(DlightColor[0], DlightColor[1], DlightColor[2]);
					SLightC = glm::vec3(SlightColor[0], SlightColor[1], SlightColor[2]);
					ALightC = glm::vec3(AlightColor[0], AlightColor[1], AlightColor[2]);
					if (ImGui::Button("Add Light"))
					{
						glm::vec3 m = model->GetModelCenter();

						std::shared_ptr <Light> new_light;
						if (lightType)
							new_light = std::make_shared<Light>(model->GetModelCenter(), LightType::POINT);
						else
							new_light = std::make_shared<Light>(model->GetModelCenter(), LightType::PARALLEL);
						new_light->SetDiffuseLightColor(DLightC);
						new_light->SetAmbientLightColor(ALightC);
						new_light->SetSpecularLightColor(SLightC);
						scene.AddLight(new_light);
					}
					ImGui::TreePop();
				}

				if (scene.GetLightCount())
				{
					if (ImGui::SliderInt("Active Light Index", &item_current, 0, scene.GetLightCount() - 1))
					{
						scene.SetActiveLightIndex(item_current);
						TranslateX_ = scene.GetActiveLight()->GetLTranslateX();
						TranslateY_ = scene.GetActiveLight()->GetLTranslateY();
						TranslateZ_ = scene.GetActiveLight()->GetLTranslateZ();
						WTranslateX_ = scene.GetActiveLight()->GetWTranslateX();
						WTranslateY_ = scene.GetActiveLight()->GetWTranslateY();
						WTranslateZ_ = scene.GetActiveLight()->GetWTranslateZ();
						AngleX_ = scene.GetActiveLight()->GetLRotationX();
						AngleY_ = scene.GetActiveLight()->GetLRotationY();
						AngleZ_ = scene.GetActiveLight()->GetLRotationZ();
						WAngleX_ = scene.GetActiveLight()->GetWRotationX();
						WAngleY_ = scene.GetActiveLight()->GetWRotationY();
						WAngleZ_ = scene.GetActiveLight()->GetWRotationZ();
						DLL[0] = scene.GetActiveLight()->GetDiffuseLightColor().x;
						DLL[1] = scene.GetActiveLight()->GetDiffuseLightColor().y;
						DLL[2] = scene.GetActiveLight()->GetDiffuseLightColor().z;

						ALL[0] = scene.GetActiveLight()->GetAmbientLightColor().x;
						ALL[1] = scene.GetActiveLight()->GetAmbientLightColor().y;
						ALL[2] = scene.GetActiveLight()->GetAmbientLightColor().z;
						SLL[0] = scene.GetActiveLight()->GetSpecularLightColor().x;
						SLL[1] = scene.GetActiveLight()->GetSpecularLightColor().y;
						SLL[2] = scene.GetActiveLight()->GetSpecularLightColor().z;
						xDirection = scene.GetActiveLight()->GetLightDirection().x;
						yDirection = scene.GetActiveLight()->GetLightDirection().y;
						zDirection = scene.GetActiveLight()->GetLightDirection().z;
					}
					auto& light = scene.GetActiveLight();
					if (ImGui::TreeNode("Control Active Light")) 
					{
						ImGui::ColorEdit3("Active Light Diffuse Color", DLL);
						light->SetDiffuseLightColor(glm::vec3(DLL[0], DLL[1], DLL[2]));
						ImGui::ColorEdit3("Active Light Ambient Color", ALL);
						light->SetAmbientLightColor(glm::vec3(ALL[0], ALL[1], ALL[2]));
						ImGui::ColorEdit3("Active Light Specular Color", SLL);
						light->SetSpecularLightColor(glm::vec3(SLL[0], SLL[1], SLL[2]));
						if (light->GetLightType() == LightType::PARALLEL)
						{
							ImGui::Text("Light Direction");
							if (ImGui::SliderFloat("X Coordinate", &xDirection, -1.f, 1.f))
							{
								light->SetLightDirection(glm::vec3(xDirection, yDirection, zDirection));
							}
							if (ImGui::SliderFloat("Y Coordinate", &yDirection, -1.f, 1.f))
							{
								light->SetLightDirection(glm::vec3(xDirection, yDirection, zDirection));
							}
							if (ImGui::SliderFloat("Z Coordinate", &zDirection, -1.f, 1.f))
							{
								light->SetLightDirection(glm::vec3(xDirection, yDirection, zDirection));
							}
						}
						else
						if (ImGui::TreeNode("Light Transformations"))
						{
							ImGui::RadioButton("Local", &IsWorld_, 0);
							ImGui::SameLine();
							ImGui::RadioButton("World", &IsWorld_, 1);
							ImGui::RadioButton("Translate", &TransformationType_, 0);
							ImGui::SameLine();
							ImGui::RadioButton("Rotate", &TransformationType_, 1);
							if (IsWorld_)
							{
								if (!TransformationType_ && light->GetLightType()!=LightType::PARALLEL)
								{
									ImGui::SliderFloat("Translate in X", &WTranslateX_, -0.1f, 0.1f);
									ImGui::SliderFloat("Translate in y", &WTranslateY_, -0.1f, 0.1f);
									ImGui::SliderFloat("Translate in Z", &WTranslateZ_, -0.1f, 0.1f);
									scene.GetActiveLight()->SetTranslationMatrix(WTranslateX_, WTranslateY_ , WTranslateZ_, true);
								}
								else
									if (TransformationType_)
									{
										ImGui::SliderFloat("Rotate in X", &WAngleX_, 0.f, 360.f);
										ImGui::SliderFloat("Rotate in y", &WAngleY_, 0.f, 360.f);
										ImGui::SliderFloat("Rotate in Z", &WAngleZ_, 0.f, 360.f);
										scene.GetActiveLight()->SetRotationMatrix((WAngleX_), true, 1);
										scene.GetActiveLight()->SetRotationMatrix((WAngleY_), true, 2);
										scene.GetActiveLight()->SetRotationMatrix((WAngleZ_), true, 3);
									}
								scene.GetActiveLight()->SetWorldTransformation();
							}
							else
							{
								if (TransformationType_)
								{
									ImGui::SliderFloat("Rotate in X", &AngleX_, 0.f, 360.f);
									ImGui::SliderFloat("Rotate in y", &AngleY_, 0.f, 360.f);
									ImGui::SliderFloat("Rotate in Z", &AngleZ_, 0.f, 360.f);
									scene.GetActiveLight()->SetRotationMatrix((AngleX_), false, 1);
									scene.GetActiveLight()->SetRotationMatrix((AngleY_), false, 2);
									scene.GetActiveLight()->SetRotationMatrix((AngleZ_), false, 3);
								}
								else
									if (!TransformationType_ && light->GetLightType() != LightType::PARALLEL)
									{
										ImGui::SliderFloat("Translate in X", &TranslateX_, -0.1f, 0.1f);
										ImGui::SliderFloat("Translate in y", &TranslateY_, -0.1f, 0.1f);
										ImGui::SliderFloat("Translate in Z", &TranslateZ_, -0.1f, 0.1f);
										scene.GetActiveLight()->SetTranslationMatrix(TranslateX_ , TranslateY_ ,TranslateZ_, false);
									}
								scene.GetActiveLight()->SetLocalTransformation();
							}
							if (ImGui::Button("Reset Light Transformations"))
							{
								scene.GetActiveLight()->ResetTransformations();
								TranslateX_ = 0;
								TranslateY_ = 0;
								TranslateZ_ = 0;
								WTranslateX_ = 0;
								WTranslateY_ = 0;
								WTranslateZ_ = 0;
								AngleX_ = 0;
								AngleY_ = 0;
								AngleZ_ = 0;
								WAngleX_ = 0;
								WAngleY_ = 0;
								WAngleZ_ = 0;
							}
							ImGui::TreePop();
						}
						ImGui::TreePop();
					}
				}
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.f));
				if (ImGui::Button("Close Me"))
					LightWindow = false;
				ImGui::PopStyleColor();
				ImGui::End();
			}
		}
		//*******************************************************************************CAMERA WINDOW***********************************************************************
		static int Projection = 1;
		static float eye[3] = { scene.GetActiveModel()->GetPreffered_Eye()[0],scene.GetActiveModel()->GetPreffered_Eye()[1],scene.GetActiveModel()->GetPreffered_Eye()[2] };
		static float at[3] = { 0.f,0.f,0.f };
		static float up[3] = { 0.f,1.f,0.f };
		static float cameraTranslationX, cameraTranslationY, cameraTranslationZ;
		static float cameraRotationX, cameraRotationY, cameraRotationZ;
		static float worldTranslationX, worldTranslationY, worldTranslationZ;
		static float worldRotationX, worldRotationY, worldRotationZ;
		static int IsWorld, TransformationType;
		static float fovy = (90.f);
		static float Near = -0.1f;
		static float Far = 1000.f;
		if (CameraWindow)
		{
			ImGui::Begin("Camera Window");
			ImGui::Text("Choose Projection"); ImGui::SameLine();
			ImGui::RadioButton("Perspective", &Projection, 0); ImGui::SameLine();
			ImGui::RadioButton("Orthographic", &Projection, 1);
			if (Projection)
			{
				scene.GetActiveCamera()->SetIsOrthographic(true);
				ImGui::SliderFloat("Orho Width", &OrthoWidth, scene.GetActiveModel()->GetMinOrtho(), scene.GetActiveModel()->GetMaxOrtho());
				scene.GetActiveCamera()->SetOrthographicWidth(OrthoWidth);
			}
			else
			{
				scene.GetActiveCamera()->SetIsOrthographic(false);
				ImGui::SliderFloat("Fov", &fovy, 20, 120);
				scene.GetActiveCamera()->SetFovy(fovy);
				//perspective
			}

			if (ImGui::TreeNode("LookAt Parameters")) {
				ImGui::InputFloat3("Eye", eye, 3);
				ImGui::InputFloat3("At", at, 3);
				ImGui::InputFloat3("Up", up, 3);
				scene.GetActiveCamera()->SetAt(glm::vec3(at[0], at[1], at[2]));
				scene.GetActiveCamera()->SetEye(glm::vec3(eye[0], eye[1], eye[2]));
				scene.GetActiveCamera()->SetUp(glm::vec3(up[0], up[1], up[2]));
				if (ImGui::Button("Look At"))
				{
					scene.GetActiveCamera()->SetCameraLookAt();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Camera Transformations")) {
				ImGui::RadioButton("Local", &IsWorld, 0);
				ImGui::SameLine();
				ImGui::RadioButton("World", &IsWorld, 1);
				ImGui::RadioButton("Translate", &TransformationType, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Rotate", &TransformationType, 1);
				if (IsWorld)
				{
					if (TransformationType)
					{
						ImGui::SliderFloat("Rotate in X", &worldRotationX, 0, 360);
						ImGui::SliderFloat("Rotate in y", &worldRotationY, 0, 360);
						ImGui::SliderFloat("Rotate in Z", &worldRotationZ, 0, 360);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::XRotationTransformation(worldRotationX), true, 1);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::YRotationTransformation(worldRotationY), true, 2);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::ZRotationTransformation(worldRotationZ), true, 3);
					}
					else
					{
						ImGui::SliderFloat("Translate in X", &worldTranslationX, -200, 200);
						ImGui::SliderFloat("Translate in y", &worldTranslationY, -200, 200);
						ImGui::SliderFloat("Translate in Z", &worldTranslationZ, -200, 200);
						scene.GetActiveCamera()->SetWTranslate(Transformations::TranslationTransformation(worldTranslationX / 1000, worldTranslationY / 1000, worldTranslationZ / 1000));
					}
					scene.GetActiveCamera()->SetWTransformation();
				}
				else
				{
					if (TransformationType)
					{
						ImGui::SliderFloat("Rotate in X", &cameraRotationX, 0, 360);
						ImGui::SliderFloat("Rotate in y", &cameraRotationY, 0, 360);
						ImGui::SliderFloat("Rotate in Z", &cameraRotationZ, 0, 360);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::XRotationTransformation(cameraRotationX), false, 1);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::YRotationTransformation(cameraRotationY), false, 2);
						scene.GetActiveCamera()->SetRotationMatrix(Transformations::ZRotationTransformation(cameraRotationZ), false, 3);
					}
					else
					{
						ImGui::SliderFloat("Translate in x", &cameraTranslationX, -200, 200);
						ImGui::SliderFloat("Translate in y", &cameraTranslationY, -200, 200);
						ImGui::SliderFloat("Translate in z", &cameraTranslationZ, -200, 200);
						scene.GetActiveCamera()->SetCTranslate(Transformations::TranslationTransformation(cameraTranslationX / 1000, cameraTranslationY / 1000, cameraTranslationZ / 1000));
					}
					scene.GetActiveCamera()->SetCTransformation();
				}
				scene.GetActiveCamera()->SetTransformations();
				if (ImGui::Button("Reset Transformations"))
				{
					scene.GetActiveCamera()->ResetTransformations();
					cameraTranslationX = 0;
					cameraTranslationY = 0;
					cameraTranslationZ = 0;
					cameraRotationX = 0;
					cameraRotationY = 0;
					cameraRotationZ = 0;
					worldTranslationX = 0;
					worldTranslationY = 0;
					worldTranslationZ = 0;
					worldRotationX = 0;
					worldRotationY = 0;
					worldRotationZ = 0;
				}
				ImGui::TreePop();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.f));
			if (ImGui::Button("Close Me"))
				CameraWindow = false;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}