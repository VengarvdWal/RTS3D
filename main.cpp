#define STB_IMAGE_IMPLEMENTATION

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <sys/time.h>
#include "X11/Xlib.h"
#include "X11Window.h"
#include "Shader.h"
#include "OpenGLES.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameObject.h"
#include "Game.h"

Game game;


//void CalculateNormals(uint* indices, uint indiceCount, GLfloat* vertices, uint verticeCount, 
//						uint vLength, uint normalOffset)
//{
//	for (size_t i = 0; i < indiceCount; i += 3)
//	{
//		uint in0 = indices[i] * vLength;
//		uint in1 = indices[i + 1] * vLength;
//		uint in2 = indices[i + 2] * vLength;
//
//		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
//		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
//		glm::vec3 normal = glm::cross(v1, v2);
//		normal = glm::normalize(normal);
//
//		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
//		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
//		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
//		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;		
//	}
//
//	for (size_t i = 0; i < verticeCount / vLength; i++)
//	{
//		uint nOffset = i * vLength + normalOffset;
//		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
//		vec = glm::normalize(vec);
//		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
//	}
//}
//
//void CreateObjects()
//{
//	unsigned int indices[] = {
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2
//	};
//
//	GLfloat vertices[] = {
//		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0, 0.0f, 0.0f,
//		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0, 0.0f, 0.0f,
//		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0, 0.0f, 0.0f
//	};
//
//	unsigned int floorIndices[] = {
//		0, 2, 1,
//		1, 2, 3
//	};
//
//	GLfloat floorVertices[] = {
//		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
//		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
//		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
//		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
//	};
//
//
//	CalculateNormals(indices, 12, vertices, 32, 8, 5);
//
//	Mesh* obj1 = new Mesh();
//	obj1->CreateMesh(vertices, indices, 32, 12);
//	meshList.push_back(obj1);
//
//	Mesh* obj2 = new Mesh();
//	obj2->CreateMesh(floorVertices, floorIndices, 32, 6);
//	meshList.push_back(obj2);
//}

//#pragma region ImGUI
//	
//	const char* glsl_version = "#version 100";
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.DisplaySize = ImVec2(properties.width, properties.height);
//
//	ImGui::StyleColorsDark();
//
//	ImGui_ImplOpenGL3_Init(glsl_version);
//
//#pragma endregion

//#pragma region IMGUI
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui::NewFrame();
//
//		Window returnWin;
//
//		int xRoot, yRoot;
//		int xWin, yWin;
//		uint returnMask;
//
//		XQueryPointer(
//			window->GetDisplay(),
//			window->GetNativeWindow(),
//			&returnWin, &returnWin,
//			&xRoot, &yRoot,
//			&xWin, &yWin,
//			&returnMask);
//
//		ImGuiIO& io = ImGui::GetIO(); (void)io;
//		io.MousePos = ImVec2(static_cast<float>(xWin), static_cast<float>(yWin));
//		io.MouseDown[0] = returnMask & (1 << 8);
//		io.MouseDown[1] = returnMask & (1 << 10);
//				
//		ImGui::Begin("Window");
//			ImGui::Text("Test");
//			ImGui::Button("Button");
//		ImGui::End();
//#pragma endregion

//		glUseProgram(0);		
//		ImGui::Render();							
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//	ImGui::DestroyContext();

int main()
{
	game.InitWindow();


	if (game.MainMenu())
	{
		game.Loading();

		game.Init();

		game.Run();
	}	

	return 0;
}