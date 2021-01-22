//Espinoza Jimenez Francisco Javier 
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void animate();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
//Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
Camera  camera(glm::vec3(3.0f, 2.0f, 7.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;

float lux=0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
bool active;
bool animacionav;

//Para modelo virus
float movVirusY = 5.0f;
//For Sherksss
float rotShrek = 0.0f;
//Variables para animacion de Avion
float movAvion_x = -35.0f,
movAvion_y = 1.5f,
movAvion_z = -60.0f,
avionRotY = 180.0f;
//Variables para animacion del Lambo
bool lambo = false;
float movAutoX = -25.0f,
movAutoY = -1.7f,
movAutoZ = -55.0f,
radio = 13.2f,
rotModel = 0.0f;

//Animacion Cohete
float mov_x = 0.0,
mov_y = 0.0f,
radioC = 2.0f, rotC = 0.0f;
bool cohete = false;

//Animacion globo -11, 2, -10
float
movGlobo_x = -12.f,
movGlobo_y = 1.0f,
movGlobo_z = -10.0f,
orientaG = 0.0f;
bool	animacionG = false,
recorridoG1 = true,
recorridoG2 = false,
recorridoG3 = false,
recorridoG4 = false,
recorridoG5 = false;


void sonido() {
	//sndPlaySound("s.wav", SND_ASYNC);
}

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0;
float rotRodDer = 0, rotBraIzq = 0, rotBraDer = 0, rotInc1=0, rotInc2=0, rotInc3=0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;




void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	


	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;


}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;


}




int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto EJFJ", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");


	Model Piso((char*)"Models/Carro/Piso.obj");
	Model Helicopter((char*)"Models/helicoptero/helicop.obj");
	Model ModelAlberca((char*)"Models/alberca/pool.obj");
	Model ModelCasa((char*)"Models/depas/casa.obj");
	Model ModelEdificio((char*)"Models/Edificio/Building.obj");
	Model ModelEdificio2((char*)"Models/edificio02/edificio2.obj");
	Model ModelHouseF ((char*)"Models/house/house.obj");
	Model ModelArbol((char*)"Models/arbol/arbol.obj");
	Model ModelArbol1((char*)"Models/arbol1/arbol1.obj");
	Model ModelArbolGnomo((char*)"Models/arbol2/arbol2.obj");
	Model ModelPalmerita((char*)"Models/palmera1/palmera1.obj");
	Model ModelLampara((char*)"Models/lampara/lampara.obj");
	Model ModelShrek((char*)"Models/Shrek/Shrek.obj");
	Model ModelArbolSaul((char*)"Models/arbol3/arbol3.obj");
	Model ModelSauce((char*)"Models/arbol4/arbol4.obj");
	Model ModelPlanta((char*)"Models/planta/planta.obj");
	Model ModelFuente((char*)"Models/fuente/fuente.obj");
	Model ModelAvion((char*)"Models/avion/avion.obj");
	Model ModelPlataforma((char*)"Models/plataforma/plataforma.obj");
	Model ModelLambo((char*)"Models/Lambo/carroseria.obj");
	Model ModelLlantas ((char*)"Models/Lambo/Wheel.obj");
	Model ModelVirus((char*)"Models/virus/covid.obj");
	Model ModelGlobo((char*)"Models/globo/globo.obj");
	Model ModelNina((char*)"Models/nina/nina.obj");
	Model ModelRes((char*)"Models/resbaladilla/resbaladilla.obj");
	Model ModelBote((char*)"Models/bote/bote.obj");
	Model ModelJuego((char*)"Models/juego/juego.obj");
	Model ModelCrash((char*)"Models/crash/crash.obj");
	Model ModelCohete((char*)"Models/Cohete/cohete.obj");
	Model ModelBanca((char*)"Models/banca/banca2.obj");


	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
	
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/posx.jpg");
	faces.push_back("SkyBox/negx.jpg");
	faces.push_back("SkyBox/posy.jpg");
	faces.push_back("SkyBox/negy.jpg");
	faces.push_back("SkyBox/posz.jpg");
	faces.push_back("SkyBox/negz.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		animate();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f+lux, 1.0f+lux, 1.0f+lux);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//model 
		glm::mat4 model(1);
		glm::mat4 origin(1);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		//Temporal para manipulacion del modelo Lambo
		glm::mat4 tmp = glm::mat4(1.0f);
		//Temporal para el covid
		glm::mat4 tmp1 = glm::mat4(1.0f);
		origin = glm::mat4(1.0f);

		////Carga de modelo 
		//Piso
		model = glm::translate(origin, glm::vec3(0.0f, -1.8f, -1.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		//Alberca
		glm::mat4 modelAlberca = glm::translate(origin, glm::vec3(57, -1.7, 38));
		modelAlberca = glm::scale(modelAlberca, glm::vec3(.1, .1f, .1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAlberca));
		ModelAlberca.Draw(lightingShader);
		//Casa
		glm::mat4 modelCasa = glm::translate(origin, glm::vec3(0.0f, -1.7f, -27.0f));
		modelCasa = glm::scale(modelCasa, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa));
		ModelCasa.Draw(lightingShader);
		//Casa 1
		glm::mat4 modelCasa1 = glm::translate(origin, glm::vec3(19.5f, -1.7f, -27.0f));
		modelCasa1 = glm::scale(modelCasa1, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa1));
		ModelCasa.Draw(lightingShader);
		//Casa 2
		glm::mat4 modelCasa2 = glm::translate(origin, glm::vec3(-19.5f, -1.7f, -27.0f));
		modelCasa2 = glm::scale(modelCasa2, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa2));
		ModelCasa.Draw(lightingShader);
		//Casa 3
		glm::mat4 modelCasa3 = glm::translate(origin, glm::vec3(50.0f, -1.7f, 1.0f));
		modelCasa3 = glm::rotate(modelCasa3, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelCasa3 = glm::scale(modelCasa3, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa3));
		ModelCasa.Draw(lightingShader);
		//Casa 4
		glm::mat4 modelCasa4 = glm::translate(origin, glm::vec3(0.0f, -1.7f, -48.0f));
		modelCasa4 = glm::scale(modelCasa4, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa4));
		ModelCasa.Draw(lightingShader);
		//Casa 5
		glm::mat4 modelCasa5 = glm::translate(origin, glm::vec3(20.0f, -1.7f, -48.0f));
		modelCasa5 = glm::scale(modelCasa5, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa5));;
		ModelCasa.Draw(lightingShader);
		//Casa 6
		glm::mat4 modelCasa6 = glm::translate(origin, glm::vec3(-20.0f, -1.7f, -48.0f));
		modelCasa6 = glm::scale(modelCasa6, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa6));
		ModelCasa.Draw(lightingShader);
		//Casa 7
		glm::mat4 modelCasa7 = glm::translate(origin, glm::vec3(50.0f, -1.7f, -19.0f));
		modelCasa7 = glm::rotate(modelCasa7, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelCasa7 = glm::scale(modelCasa7, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasa7));
		ModelCasa.Draw(lightingShader);
		//Edificio 2
		glm::mat4 modelEdificio2 = glm::translate(origin, glm::vec3(0, 8.2, 65));
		modelEdificio2 = glm::rotate(modelEdificio2, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio2 = glm::scale(modelEdificio2, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio2));
		ModelEdificio2.Draw(lightingShader);
		//Edificio 3
		glm::mat4 modelEdificio3 = glm::translate(origin, glm::vec3(-15, 8.2, 65));
		modelEdificio3 = glm::rotate(modelEdificio3, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio3 = glm::scale(modelEdificio3, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio3));
		ModelEdificio2.Draw(lightingShader);
		//Edifico 4
		glm::mat4 modelEdificio4 = glm::translate(origin, glm::vec3(15, 8.2, 65));
		modelEdificio4 = glm::rotate(modelEdificio4, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio4 = glm::scale(modelEdificio4, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio4));
		ModelEdificio2.Draw(lightingShader);
		//Edificio 5
		glm::mat4 modelEdificio5 = glm::translate(origin, glm::vec3(-30, 8.2, 65));
		modelEdificio5 = glm::rotate(modelEdificio5, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio5 = glm::scale(modelEdificio5, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio5));
		ModelEdificio2.Draw(lightingShader);
		//Edificio 6
		glm::mat4 modelEdificio6 = glm::translate(origin, glm::vec3(30, 8.2, 65));
		modelEdificio6 = glm::rotate(modelEdificio6, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio6 = glm::scale(modelEdificio6, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio6));
		ModelEdificio2.Draw(lightingShader);
		//Edificio 7
		glm::mat4 modelEdificio7 = glm::translate(origin, glm::vec3(45, 8.2, 65));
		modelEdificio7 = glm::rotate(modelEdificio7, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		modelEdificio7 = glm::scale(modelEdificio7, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEdificio7));
		ModelEdificio.Draw(lightingShader);
		//HouseF 
		glm::mat4 modelHouseF = glm::translate(origin, glm::vec3(-14.0f, -4.5f, -5.0f));
		modelHouseF = glm::rotate(modelHouseF, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelHouseF = glm::scale(modelHouseF, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouseF));
		ModelHouseF.Draw(lightingShader);
		//HouseF 1
		glm::mat4 modelHouseF1 = glm::translate(origin, glm::vec3(-14.0f, -4.5f, 15.0f));
		modelHouseF1 = glm::rotate(modelHouseF1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelHouseF1 = glm::scale(modelHouseF1, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouseF1));;
		ModelHouseF.Draw(lightingShader);
		//HouseF 2
		glm::mat4 modelHouseF2 = glm::translate(origin, glm::vec3(-14.0f, -4.5f, 35.0f));
		modelHouseF2 = glm::rotate(modelHouseF2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelHouseF2 = glm::scale(modelHouseF2, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouseF2));;
		ModelHouseF.Draw(lightingShader);
		//HouseF 3
		glm::mat4 modelHouseF3 = glm::translate(origin, glm::vec3(-14.0f, -4.5f, 55.0f));
		modelHouseF3 = glm::rotate(modelHouseF3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelHouseF3 = glm::scale(modelHouseF3, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHouseF3));;
		ModelHouseF.Draw(lightingShader);
		//Arbol 
		glm::mat4 modelArbol = glm::translate(origin, glm::vec3(0, -1.8, -30));
		modelArbol = glm::scale(modelArbol, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol));;
		ModelArbol.Draw(lightingShader);
		//Arbol 1
		glm::mat4 modelArbol1 = glm::translate(origin, glm::vec3(0, -1.8, -17));
		modelArbol1 = glm::scale(modelArbol1, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol1));;
		ModelArbol.Draw(lightingShader);
		//Arbol 2
		glm::mat4 modelArbol2 = glm::translate(origin, glm::vec3(14, -1.8, -17));
		modelArbol2 = glm::scale(modelArbol2, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbol2));;
		ModelArbol.Draw(lightingShader);
		//ArbolGnomo
		glm::mat4 modelArbolGnomo = glm::translate(origin, glm::vec3(-50, 5.7, -45));
		modelArbolGnomo = glm::scale(modelArbolGnomo, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbolGnomo));;
		ModelArbolGnomo.Draw(lightingShader);
		//ArbolGnomo 1
		glm::mat4 modelArbolGnomo1 = glm::translate(origin, glm::vec3(-65, 5.7, -50));
		modelArbolGnomo1 = glm::scale(modelArbolGnomo1, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbolGnomo1));;
		ModelArbolGnomo.Draw(lightingShader);
		//ArbolGnomo 2
		glm::mat4 modelArbolGnomo2 = glm::translate(origin, glm::vec3(-60, 5.7, -40));
		modelArbolGnomo2 = glm::scale(modelArbolGnomo2, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbolGnomo2));;
		ModelArbolGnomo.Draw(lightingShader);
		//ArbolGnomo 3
		glm::mat4 modelArbolGnomo3 = glm::translate(origin, glm::vec3(-60, 5.7, -58));
		modelArbolGnomo3 = glm::scale(modelArbolGnomo3, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbolGnomo3));;
		ModelArbolGnomo.Draw(lightingShader);
		//Palmerita 
		glm::mat4 modelPalmerita = glm::translate(origin, glm::vec3(15, 2.7, 0));
		modelPalmerita = glm::scale(modelPalmerita, glm::vec3(5, 5, 5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalmerita));;
		ModelPalmerita.Draw(lightingShader);
		//Palmerita 1
		glm::mat4 modelPalmerita1 = glm::translate(origin, glm::vec3(15, 2.7, 5));
		modelPalmerita1 = glm::scale(modelPalmerita1, glm::vec3(5, 5, 5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalmerita1));;
		ModelPalmerita.Draw(lightingShader);
		//Palmerita  2
		glm::mat4 modelPalmerita2 = glm::translate(origin, glm::vec3(22, 2.7, 3));
		modelPalmerita2 = glm::scale(modelPalmerita2, glm::vec3(5, 5, 5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalmerita2));;
		ModelPalmerita.Draw(lightingShader);
		//Palmerita 3
		glm::mat4 modelPalmerita3 = glm::translate(origin, glm::vec3(-18, 2.7, 5));
		modelPalmerita3 = glm::scale(modelPalmerita3, glm::vec3(5, 5, 5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalmerita3));;
		ModelPalmerita.Draw(lightingShader);
		//Palmerita 4
		glm::mat4 modelPalmerita4 = glm::translate(origin, glm::vec3(-18, 2.7, -10));
		modelPalmerita4 = glm::scale(modelPalmerita4, glm::vec3(5, 5, 5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalmerita4));;
		ModelPalmerita.Draw(lightingShader);
		//Lampara
		glm::mat4 modelLampara = glm::translate(origin, glm::vec3(-11, 2, 12));
		modelLampara = glm::scale(modelLampara, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara));;
		ModelLampara.Draw(lightingShader);
		//Lampara 1
		glm::mat4 modelLampara1 = glm::translate(origin, glm::vec3(-11, 2, -10));
		modelLampara1 = glm::scale(modelLampara1, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara1));;
		ModelLampara.Draw(lightingShader);
		//Lampara 2
		glm::mat4 modelLampara2 = glm::translate(origin, glm::vec3(19, 2, -10));
		modelLampara2 = glm::scale(modelLampara2, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara2));;
		ModelLampara.Draw(lightingShader);
		//Lampara 3
		glm::mat4 modelLampara3 = glm::translate(origin, glm::vec3(19, 2, 12));
		modelLampara3 = glm::scale(modelLampara3, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara3));;
		ModelLampara.Draw(lightingShader);
		//Lampara 4
		glm::mat4 modelLampara4 = glm::translate(origin, glm::vec3(-19.5, 2, -18.5));
		modelLampara4 = glm::scale(modelLampara4, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara4));;
		ModelLampara.Draw(lightingShader);
		//Lampara 5
		glm::mat4 modelLampara5 = glm::translate(origin, glm::vec3(-9.5, 2, -18.5));
		modelLampara5 = glm::scale(modelLampara5, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara5));;
		ModelLampara.Draw(lightingShader);
		//Lampara 6
		glm::mat4 modelLampara6 = glm::translate(origin, glm::vec3(0.5, 2, -18.5));
		modelLampara6 = glm::scale(modelLampara6, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara6));;
		ModelLampara.Draw(lightingShader);
		//Lampara 7
		glm::mat4 modelLampara7 = glm::translate(origin, glm::vec3(10.5, 2, -18.5));
		modelLampara7 = glm::scale(modelLampara7, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara7));;
		ModelLampara.Draw(lightingShader);
		//Lampara 8
		glm::mat4 modelLampara8 = glm::translate(origin, glm::vec3(20.5, 2, -18.5));
		modelLampara8 = glm::scale(modelLampara8, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara8));;
		ModelLampara.Draw(lightingShader);
		//Lampara 9
		glm::mat4 modelLampara9 = glm::translate(origin, glm::vec3(30.5, 2, -18.5));
		modelLampara9 = glm::scale(modelLampara9, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLampara9));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 4
		glm::mat4 modelLamparaAtras4 = glm::translate(origin, glm::vec3(-17.5, 2, -38.5));
		modelLamparaAtras4 = glm::scale(modelLamparaAtras4, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras4));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 5
		glm::mat4 modelLamparaAtras5 = glm::translate(origin, glm::vec3(-7.5, 2, -38.5));
		modelLamparaAtras5 = glm::scale(modelLamparaAtras5, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras5));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 6
		glm::mat4 modelLamparaAtras6 = glm::translate(origin, glm::vec3(2.5, 2, -38.5));
		modelLamparaAtras6 = glm::scale(modelLamparaAtras6, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras6));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 7
		glm::mat4 modelLamparaAtras7 = glm::translate(origin, glm::vec3(12.5, 2, -38.5));
		modelLamparaAtras7 = glm::scale(modelLamparaAtras7, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras7));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 8
		glm::mat4 modelLamparaAtras8 = glm::translate(origin, glm::vec3(22.5, 2, -38.5));
		modelLamparaAtras8 = glm::scale(modelLamparaAtras8, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras8));;
		ModelLampara.Draw(lightingShader);
		//LamparaAtras 9
		glm::mat4 modelLamparaAtras9 = glm::translate(origin, glm::vec3(32.5, 2, -38.5));
		modelLamparaAtras9 = glm::scale(modelLamparaAtras9, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamparaAtras9));;
		ModelLampara.Draw(lightingShader);
		//Fuente 
		glm::mat4 modelFuente = glm::translate(origin, glm::vec3(-7.5f, -2.0f, 22.5f));
		modelFuente = glm::scale(modelFuente, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFuente));;
		ModelFuente.Draw(lightingShader);
		// Arbol Saul 
		glm::mat4 modelArbolSaul = glm::translate(origin, glm::vec3(50, -2, 23.0));
		modelArbolSaul = glm::scale(modelArbolSaul, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelArbolSaul));;
		ModelArbolSaul.Draw(lightingShader);
		// Sauce
		glm::mat4 modelSauce = glm::translate(origin, glm::vec3(50, -2, -40));
		modelSauce = glm::scale(modelSauce, glm::vec3(.25f, .35f, .25f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSauce));;
		ModelSauce.Draw(lightingShader);
		// Planta 15
		glm::mat4 modelPlanta = glm::translate(origin, glm::vec3(-37, 2.3, 25));
		modelPlanta = glm::scale(modelPlanta, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlanta));;
		ModelPlanta.Draw(lightingShader);
		// Planta 2
		glm::mat4 modelPlanta2 = glm::translate(origin, glm::vec3(-37, 2.3, -3));
		modelPlanta2 = glm::scale(modelPlanta2, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlanta2));;
		ModelPlanta.Draw(lightingShader);
		// Planta 3
		glm::mat4 modelPlanta3 = glm::translate(origin, glm::vec3(-37, 2.3, 5));
		modelPlanta3 = glm::scale(modelPlanta3, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPlanta3));;
		ModelPlanta.Draw(lightingShader);
		//CasaLamp 4
		glm::mat4 modelCasaLamp4 = glm::translate(origin, glm::vec3(-40, 2, -20));
		modelCasaLamp4 = glm::scale(modelCasaLamp4, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp4));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 5
		glm::mat4 modelCasaLamp5 = glm::translate(origin, glm::vec3(-40, 2, 0));
		modelCasaLamp5 = glm::scale(modelCasaLamp5, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp5));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 6
		glm::mat4 modelCasaLamp6 = glm::translate(origin, glm::vec3(-40, 2, 20));
		modelCasaLamp6 = glm::scale(modelCasaLamp6, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp6));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 7
		glm::mat4 modelCasaLamp7 = glm::translate(origin, glm::vec3(-40, 2, 40));
		modelCasaLamp7 = glm::scale(modelCasaLamp7, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp7));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 8
		glm::mat4 modelCasaLamp8 = glm::translate(origin, glm::vec3(9, 2, 48));
		modelCasaLamp8 = glm::scale(modelCasaLamp8, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp8));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 9
		glm::mat4 modelCasaLamp9 = glm::translate(origin, glm::vec3(24, 2, 48));
		modelCasaLamp9 = glm::scale(modelCasaLamp9, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp9));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 10
		glm::mat4 modelCasaLamp10 = glm::translate(origin, glm::vec3(-6, 2, 48));
		modelCasaLamp10 = glm::scale(modelCasaLamp10, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp10));;
		ModelLampara.Draw(lightingShader);
		//CasaLamp 11
		glm::mat4 modelCasaLamp11 = glm::translate(origin, glm::vec3(-21, 2, 48));
		modelCasaLamp11 = glm::scale(modelCasaLamp11, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCasaLamp11));;
		ModelLampara.Draw(lightingShader);
		//Avion
		glm::mat4 modelAvion = glm::scale(modelAvion, glm::vec3(0.5f, 0.5f, 0.5f));
		modelAvion = glm::translate(origin, glm::vec3(movAvion_x, movAvion_y, movAvion_z));
		modelAvion = glm::rotate(modelAvion, glm::radians(avionRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAvion));;
		ModelAvion.Draw(lightingShader);
		//Plataforma
		glm::mat4 modelPlataforma = glm::scale(modelPlataforma, glm::vec3(1.0f, 1.0f, 1.0f));
		modelAvion = glm::translate(origin, glm::vec3(-35.0f, -0.5f, -55.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAvion));;
		ModelPlataforma.Draw(lightingShader);
		//Lambo
		glm::mat4 modelLambo = glm::translate(origin, glm::vec3(movAutoX, movAutoY, movAutoZ));
		modelLambo = glm::rotate(modelLambo, glm::radians(-rotModel - 1800.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = modelLambo = glm::scale(modelLambo, glm::vec3(1.5f, 1.5f, 1.5f));
		modelLambo = glm::scale(modelLambo, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLambo));
		ModelLambo.Draw(lightingShader);
		////Izq trase
		glm::mat4 modelLlantas = glm::translate(tmp, glm::vec3(0.85f, 0.25f, 1.29f));
		modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLlantas));
		ModelLlantas.Draw(lightingShader);
		//Izq delantera
		modelLlantas = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, 1.29f));
		modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
		modelLlantas = glm::rotate(modelLlantas, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLlantas));
		ModelLlantas.Draw(lightingShader);
		//Der delantera
		modelLlantas = glm::translate(tmp, glm::vec3(-0.85f, 0.25f, -1.45f));
		modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
		modelLlantas = glm::rotate(modelLlantas, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLlantas));
		ModelLlantas.Draw(lightingShader);
		//Der trasera
		modelLlantas = glm::translate(tmp, glm::vec3(0.85f, 0.25f, -1.45f));
		modelLlantas = glm::scale(modelLlantas, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLlantas));
		ModelLlantas.Draw(lightingShader);
		//Globo 
		glm::mat4 modelGlobo = glm::rotate(modelGlobo, glm::radians(0.0f), glm::vec3(5.0f, 5.3f, 5.0f));
		modelGlobo = glm::translate(glm::mat4(1.0f), glm::vec3(movGlobo_x, movGlobo_y, movGlobo_z));
		modelGlobo = glm::scale(modelGlobo, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGlobo));
		ModelGlobo.Draw(lightingShader);
		//nina 
		glm::mat4 modelNina = glm::translate(glm::mat4(1.0f), glm::vec3(-12.7f, -1.7f, -8.5f));
		modelNina = glm::rotate(modelNina, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelNina = glm::scale(modelNina, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelNina));
		ModelNina.Draw(lightingShader);
		//Res 
		glm::mat4 modelRes = glm::translate(origin, glm::vec3(53.0f, -1.0f, 30.0f));
		modelRes = glm::rotate(modelRes, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRes = glm::scale(modelRes, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRes));
		ModelRes.Draw(lightingShader);
		//Bote 
		glm::mat4 modelBote = glm::translate(origin, glm::vec3(55.0f, -1.7f, 21.0f));
		modelBote = glm::rotate(modelBote, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBote = glm::scale(modelBote, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBote));
		ModelBote.Draw(lightingShader);
		//Bote 1 
		glm::mat4 modelBote1 = glm::translate(origin, glm::vec3(55.0f, -1.7f, 19.0f));
		modelBote1 = glm::rotate(modelBote1, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBote1 = glm::scale(modelBote1, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBote1));
		ModelBote.Draw(lightingShader);
		// Juego 
		glm::mat4 modelJuego = glm::translate(origin, glm::vec3(44.0f, -0.8f, 25.0f));
		modelJuego = glm::rotate(modelJuego, glm::radians(rotShrek), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelJuego));
		ModelJuego.Draw(lightingShader);
		//Crash  
		glm::mat4 modelCrash = glm::translate(origin, glm::vec3(-20.0f, 0.0f, 45.0f));
		modelCrash = glm::rotate(modelCrash, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCrash = glm::scale(modelCrash, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCrash));
		ModelCrash.Draw(lightingShader);
		//Crash 1 
		glm::mat4 modelCrash1 = glm::translate(origin, glm::vec3(0.0f, 0.0f, 45.0f));
		modelCrash1 = glm::rotate(modelCrash1, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCrash1 = glm::scale(modelCrash1, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCrash1));
		ModelCrash.Draw(lightingShader);
		//Crash 2
		glm::mat4 modelCrash2 = glm::translate(origin, glm::vec3(10.0f, 0.0f, 45.0f));
		modelCrash2 = glm::rotate(modelCrash2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCrash2 = glm::scale(modelCrash2, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCrash1));
		ModelCrash.Draw(lightingShader);
		//Crash 3
		glm::mat4 modelCrash3 = glm::translate(origin, glm::vec3(25.0f, 0.0f, 45.0f));
		modelCrash3 = glm::rotate(modelCrash3, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCrash3 = glm::scale(modelCrash3, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCrash3));
		ModelCrash.Draw(lightingShader);
		//Crash 4 
		view = camera.GetViewMatrix();
		glm::mat4 modelCrash4 = glm::translate(modelCrash4, glm::vec3(0.0f, 10.0f, 45.0f));
		modelCrash4 = glm::translate(modelCrash4, glm::vec3(posX, posY, posZ));
		modelCrash4 = glm::rotate(modelCrash4, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCrash4 = glm::scale(modelCrash4, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCrash4));
		ModelCrash.Draw(lightingShader);

		//Crohete 
		glm::mat4 modelCohete = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, -6.0f, 35.0f));
		modelCohete = glm::translate(modelCohete, glm::vec3(mov_x, mov_y, 0.0f));
		modelCohete = glm::rotate(modelCohete, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCohete = glm::rotate(modelCohete, glm::radians(rotC - 360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCohete));
		ModelCohete.Draw(lightingShader);
		//Blanca 
		glm::mat4 modelBanca = glm::translate(origin, glm::vec3(46.0f, -6.0f, -17.5f));
		//modelCrash3 = glm::scale(modelCrash3, glm::vec3(1.5.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBanca));
		ModelBanca.Draw(lightingShader);
		// Blanca 1
		glm::mat4 modelBanca1 = glm::translate(origin, glm::vec3(20.0f, -6.0f, -37.0f));
		//modelCrash3 = glm::scale(modelCrash3, glm::vec3(1.5.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBanca1));
		ModelBanca.Draw(lightingShader);

		float movX = 5.0f,
			movZ = 5.0f;
		glm::mat4 modelVirus = glm::translate(origin, glm::vec3(-20.0f, 0.0f, 0.0f));
		//tmp1 = modelVirus;
		tmp = modelVirus;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				modelVirus = glm::translate(modelVirus, glm::vec3(0.0f, movVirusY, movZ));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelVirus));
				ModelVirus.Draw(lightingShader);
				//modelVirus = glm::translate(tmp, glm::vec3(movX, movVirusY, 0.0f));

			}

			tmp = glm::translate(tmp, glm::vec3(movX, 0.0f, 0.0f));
			modelVirus = tmp;
		}
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
		model = glm::translate(model, glm::vec3(0, 15, 2));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Helicopter.Draw(lightingShader);


		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;


				i_curr_steps++;
			}

		}
	}


int mov_auto = 0,
mov_avion = 0,
mov_covid = 0,
mov_cohete = 0;
float velocidad = 1.0;

void animate(void)
{
	rotShrek += 6;
	if (cohete) {
		if (mov_cohete == 0)
		{
			mov_y += velocidad;
			if (mov_y >= 25.0f)
				mov_cohete = 1;

		}

		if (mov_cohete == 1)
		{
			rotC += 2.0f;
			mov_x = radioC * cos(glm::radians(rotC));
			mov_y = 24.0f + radioC * sin(glm::radians(rotC));
			if (rotC >= 360.0f) {
				rotC = 0.0f;
				mov_cohete = 2;
			}

		}

		if (mov_cohete == 2) {
			rotC = 26.56f;
			mov_x += 0.5;
			mov_y += 0.5;
			if (mov_x >= 20.0f)
				mov_cohete = 3;
		}
		if (mov_cohete == 3) {
			rotC += 2.0f;
			mov_x = 19.0f + radio * cos(glm::radians(rotC));
			mov_y = 42.0f + radio * sin(glm::radians(rotC));
			if (rotC >= 160.56f) {
				//rotModel = 0.0f;
				mov_cohete = 4;
			}
		}
		if (mov_cohete == 4) {
			rotC = 160.0f;
			mov_x += 0.5;
			mov_y -= 0.5;
			if (mov_y <= 10.0f)
				mov_cohete = 5;
		}

	}

	if (mov_covid == 0)
	{
		movVirusY += 0.3;
		if (movVirusY >= 5.0f)
			mov_covid = 1;

	}
	if (mov_covid == 1) {
		movVirusY -= 0.3;
		if (movVirusY <= 3.0f)
			mov_covid = 0;
	}
	if (lambo) {
		if (mov_auto == 0) {
			movAutoZ += velocidad;
			if (movAutoZ >= -35.50f)
				mov_auto = 1;
		}
		if (mov_auto == 1) {
			rotModel = -90;
			movAutoX += velocidad;
			if (movAutoX >= 39.0f)
				mov_auto = 2;
		}
		if (mov_auto == 2) {
			rotModel = 0;
			movAutoZ += velocidad;
			if (movAutoZ >= 23.5f)
				mov_auto = 3;
		}

		if (mov_auto == 3) {
			rotModel = 90.0f;
			movAutoX -= velocidad;
			if (movAutoX <= -2.0f)
				mov_auto = 4;
		}

		if (mov_auto == 4) {
			rotModel += 3.0f;
			movAutoX = (radio * cos(glm::radians(rotModel)) - 2.0f);
			movAutoZ = 23.0f + (radio * sin(glm::radians(rotModel)) - radio);
			if (rotModel == 360.0f) {
				rotModel = 0.0f;
				mov_auto = 4;
			}
		}


	}
	if (animacionav) {
		if (mov_avion == 0)
		{
			//avionRotY = 0.0f;
			movAvion_y += velocidad;
			if (movAvion_y >= 35.0f) {
				mov_avion = 1;
			}
		}

		if (mov_avion == 1)
		{
			movAvion_z += velocidad;
			//avionRotY = 90.0f;
			if (movAvion_z >= 35.0f)
				mov_avion = 2;
		}

		if (mov_avion == 2)
		{
			movAvion_x += velocidad;
			avionRotY = -90.0f;
			if (movAvion_x >= 40.0f)
				mov_avion = 3;
		}

		if (mov_avion == 3)
		{
			movAvion_z -= velocidad;
			avionRotY = 360.0f;
			if (movAvion_z <= -60.0f)
				mov_avion = 4;
		}

		if (mov_avion == 4)
		{
			movAvion_x -= velocidad;
			avionRotY = 90.0f;
			if (movAvion_x <= -35.0f)
				mov_avion = 5;
		}

		if (mov_avion == 5)
		{
			avionRotY = 180.0f;
			//if (movAvion_z <= 30.0f)
			mov_avion = 1;
		}
	}

	if (animacionG)
	{
		//animacion del Globo
		if (recorridoG1)
		{
			movGlobo_y += 0.3f;//velocidad
			//orientaG = 180.0f;//giro
			if (movGlobo_y > 0.6f)//cuando se mueva hasta..
			{
				recorridoG1 = false;//deja el recorrido1
				recorridoG2 = true;//comienza el recorrido 2
			}
		}

		if (recorridoG2)
		{
			movGlobo_y += 0.15;
			movGlobo_x += 0.25;
			orientaG = 30.96f;
			if (movGlobo_x > 1.5f)
			{
				recorridoG2 = false;
				recorridoG3 = true;
			}
		}

		if (recorridoG3)
		{
			movGlobo_y += 0.15;
			movGlobo_x -= 0.25;
			orientaG = 30.96f;
			if (movGlobo_x < -2.0f)
			{
				recorridoG3 = false;
				recorridoG4 = true;
			}
		}
		if (recorridoG4)
		{
			movGlobo_y += 0.15;
			movGlobo_x += 0.25;
			orientaG = 30.96f;
			if (movGlobo_x > 4.0f)
			{
				recorridoG4 = false;
				recorridoG5 = true;
			}
		}
		if (recorridoG5)
		{
			movGlobo_y += 0.15;
			movGlobo_x -= 0.25;
			orientaG = 30.96f;
			if (movGlobo_x < -2.0f)
			{
				recorridoG5 = false;
				recorridoG1 = true;
			}
		}
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_X]) {
		animacionav = !animacionav;
		////animacion ^= true;
		//if(movAuto_z >= 10.0f)
		//	movAuto_z = 0.0f;
	}
	if (keys[GLFW_KEY_V])
		lambo = !lambo;
	if (keys[GLFW_KEY_Z])
	{
		//animacionG ^= true; -11, 2, -10
		animacionG = !animacionG;
		movGlobo_x = -12.0f;
		movGlobo_y = 1.0f;
		movGlobo_z = -10.0f;
	}
	if (keys[GLFW_KEY_P])
		sonido();
	if (keys[GLFW_KEY_C]) {
		cohete = !cohete;
	}
	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}
	if (keys[GLFW_KEY_M])
	{
		lux += 1.0f;
	}
	if (keys[GLFW_KEY_N])
	{
		lux -= 1.0f;
	}



	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

}