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
#include "Input.h"
#include "Texture.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "GameObject.h"
#include "Game.h"
#include <glm/glm.hpp>
#include "DebugDrawer.h"
#include "thread"
#include "Terrain.h"
#include "Utils.h"


#include <glm/gtc/type_ptr.hpp>

#include "cstdio"
#include "stdlib.h"

DebugDrawer* debug = new DebugDrawer();
Terrain* terrain = new Terrain();


static double GetMilliseconds() {
	static timeval s_tTimeVal;
	gettimeofday(&s_tTimeVal, NULL);
	double time = s_tTimeVal.tv_sec * 1000.0; // sec to ms
	time += s_tTimeVal.tv_usec / 1000.0; // us to ms
	return time;
}

Game::Game()
{
	
}

void Game::CreateShaders()
{
	objectShader =  new Shader();
	objectShader->CreateFromFiles(vShader, fShader, false);
	shaderList.push_back(*objectShader);

	debugShader = new Shader();
	debugShader->CreateFromFiles(dvShader, dfShader, true);
	/*shaderList.push_back(*debugShader);*/



			
}

void Game::AddGameObject(GameObject* go)
{
	gameObjects.push_back(go);
}

//void Game::RemoveGameObject(GameObject &go)
//{
//	
//}

void Game::InitWindow()
{
	properties.width = 1280;
	properties.height = 720;
	window = new X11Window(properties);
	window->InitEGL();

	const char* glsl_version = "#version 300 es";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(properties.width, properties.height);
	ImGui::StyleColorsDark();
		
	ImGui_ImplOpenGL3_Init(glsl_version);

	
}

bool Game::MainMenu()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	glViewport(0, 0, properties.width, properties.height);
	glEnable(GL_DEPTH_TEST);	

	bool clicked = false;

	while (gameState == mainMenu)
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		Window returnWin;
		
				int xRoot, yRoot;
				int xWin, yWin;
				uint returnMask;
		
				XQueryPointer(
					window->GetDisplay(),
					window->GetNativeWindow(),
					&returnWin, &returnWin,
					&xRoot, &yRoot,
					&xWin, &yWin,
					&returnMask);
		
				
				io.MousePos = ImVec2(static_cast<float>(xWin), static_cast<float>(yWin));
				io.MouseDown[0] = returnMask & (1 << 8);
				io.MouseDown[1] = returnMask & (1 << 10);


		//MAIN MENU BUTTONS
		{
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize(ImVec2(properties.width, properties.height));
			ImGui::Begin("Window", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

			
			ImGui::SetCursorPos(ImVec2(properties.width / 2.5, properties.height / 2));
			if (ImGui::Button("Start", ImVec2(250, 50)))
			{				
				clicked = true;			
				
			}
			
			ImGui::SetCursorPos(ImVec2(properties.width / 2.5, properties.height / 1.5));
				if(ImGui::Button("Quit", ImVec2(250, 50)))
				{
					ImGui::DestroyContext();
					return false;
					
				}		
		}	

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		eglSwapBuffers(window->GetEglDisplay(), window->GetSurface());

		if (clicked)
		{
			gameState = loadingScreen;

		}
		
	}
		
}

void Game::Loading()
{
	eglSwapBuffers(window->GetEglDisplay(), window->GetSurface());

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	glViewport(0, 0, properties.width, properties.height);
	glEnable(GL_DEPTH_TEST);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize(ImVec2(properties.width, properties.height));
	ImGui::Begin("LoadingScreen", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		

	float progress = 0.0f; float progressDir = 1.0f;
	
			
	while (gameState == loadingScreen)
	{		
		//PROGRESS BAR
		{
			progress += progressDir * 0.1f * ImGui::GetIO().DeltaTime;


			ImGui::SetCursorPos(ImVec2(properties.width / 6, properties.height / 1.25));
			ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
			ImGui::Text("Loading");

			/*if (progress >= +1.1f) { progress = +1.1f; progressDir *= -1.0f; }
			if (progress <= -0.1f) { progress = -0.1f; progressDir *= -1.0f; }	*/		
			
			
			if (progress >= 1.0f)
			{
				gameState = gameplay;
			}			
		}

	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	eglSwapBuffers(window->GetEglDisplay(), window->GetSurface());
}
	




void Game::InitLights()
{
	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0), 0.0f, 0.0f, 5.0f, 1.0f);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.4, 0.6f, 0.0f, -1.0f, -1.0f);

	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f, 0.1f, 1.0f, -2.0f, 0.0f, 0.0f, 0.3f, 0.2f, 0.1f);
	pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 2.0f, -2.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 20.0);
	spotLightCount++;
}

void Game::InitPhysics()
{	
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);	
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();	
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

void Game::Init()
{	
	InitLights();
	InitPhysics();

	
	terrain->CreateTerrain(16, 16);	

	debug->setDebugMode(1);
	dynamicsWorld->setDebugDrawer(debug);


	glViewport(0, 0, properties.width, properties.height);
	glEnable(GL_DEPTH_TEST);

	defaultTexture = Texture("../../../Textures/DefaultTexture.png");
	defaultTexture.LoadTexture();

	defaultMaterial = Material(0.5f, 8);

	GameObject* object = new GameObject("../../../Models/Tank.obj", 1, btVector3(0, 0, -10));
	dynamicsWorld->addRigidBody(object->getRigidBody());
	AddGameObject(object);	

	/*GameObject* object2 =  new GameObject("../../../Models/Tank.obj", 1, btVector3(5, 0, -10));
	dynamicsWorld->addRigidBody(object2->getRigidBody());
	AddGameObject(object2);

	GameObject* object3 =  new GameObject("../../../Models/Tank.obj", 1, btVector3(-5, 0, -10));
	dynamicsWorld->addRigidBody(object3->getRigidBody());
	AddGameObject(object3);*/

	CreateShaders();
}

void Game::RenderScene()
{
	glm::mat4 matrix(1.0f);
	
	//matrix = glm::translate(matrix, glm::vec3(-3.0f, -1.0f, 0.0));
		
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrix));
	defaultTexture.UseTexture();
	defaultMaterial.UseMaterial(uniformSpecularIntensity, uniformRoughness);
	//terrain->RenderTerrain();


	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		matrix = glm::mat4(1.0f);

		matrix = glm::translate(matrix, BulletToGlm(gameObjects[i]->getTransform()));
		matrix = glm::rotate(matrix, gameObjects[i]->getRotationAngle(), BulletToGlm(gameObjects[i]->getRotationAxis()));
				
		
		//matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 0.5f));		

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrix));
		defaultMaterial.UseMaterial(uniformSpecularIntensity, uniformRoughness);
		gameObjects[i]->Update();
		gameObjects[i]->Render();	
	}
	


}

void Game::RenderPass(glm::mat4 projection, glm::mat4 viewMatrix)
{
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformRoughness = shaderList[0].GetRoughnessLocation();	

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);	
			
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount);
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
	
	RenderScene();
	debug->DebugDraw(uniformProjection, uniformView, projection, viewMatrix);
	/*shaderList[1].UseShader();
	uniformProjection = shaderList[1].GetProjectionLocation();
	uniformView = shaderList[1].GetViewLocation();*/

	//debug->DebugDraw();
	
}

void Game::Run()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)properties.width / properties.height, 0.1f, 100.0f);

	double prevTime = GetMilliseconds();
	double currentTime = GetMilliseconds();
	
	while (true)
	{
		currentTime = GetMilliseconds();
		deltaTime = double(currentTime - prevTime) * 0.001;
		prevTime = currentTime;

		dynamicsWorld->stepSimulation(deltaTime, 5);
		
			

		//camera.KeyControl();
		if (input->keyIsPressed(XK_w)) { camera.KeyControl('w', deltaTime); }
		if (input->keyIsPressed(XK_s)) { camera.KeyControl('s', deltaTime); }
		if (input->keyIsPressed(XK_A)) { camera.KeyControl('a', deltaTime); }
		if (input->keyIsPressed(XK_D)) { camera.KeyControl('d', deltaTime); }
		
		RenderPass(projection, camera.CalculateViewMatrix());
		dynamicsWorld->debugDrawWorld();

		glUseProgram(0);

		eglSwapBuffers(window->GetEglDisplay(), window->GetSurface());

	}
}



