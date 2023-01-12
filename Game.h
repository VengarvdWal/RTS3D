#pragma once


class Input;


class Game
{

	

private:

		
	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	std::vector<GameObject*> gameObjects;

	Camera camera;

	X11Window* window;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformRoughness = 0;

	Texture defaultTexture;
	Material defaultMaterial;

	DirectionalLight mainLight;
	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];


	const char* vShader = "../../../Shaders/shader.vert";
	const char* fShader = "../../../Shaders/shader.frag";

	const char* dvShader = "../../../Shaders/debugShader.vert";
	const char* dfShader = "../../../Shaders/debugShader.frag";

	Shader* objectShader;
	Shader* debugShader;

	double deltaTime = 0.0;

	uint pointLightCount = 0;
	uint spotLightCount = 0;
			
	void CreateShaders();
	void RenderScene();
	void RenderPass(glm::mat4 projection, glm::mat4 viewMatrix);
	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);

	WindowProperties properties;
	btDiscreteDynamicsWorld* dynamicsWorld;

	enum GameState { mainMenu, loadingScreen, gameplay };
	GameState gameState = mainMenu;
	Input* input;
	
public:

	Game();
	void Init();
	void InitWindow();
	bool MainMenu();
	void Loading();
	void InitLights();
	void InitPhysics();
	void Run();
};




