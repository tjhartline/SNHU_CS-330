//Tammy Hartline
//V/ersion: 284
//08/07/2023
// CS-330
// Week 7 Final Project
// Adding lighting to a 3D scene
//--------------------------------------------
#pragma warning(disable : 6262) //disable stb_image.h stack warning

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "GLAD/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

using namespace std;



// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;


GLuint ShaderProgram, slShaderProgram;

bool loadSunlightShader(GLuint& slShaderProgram)
{
	//Sunlight shader program

	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;
	//Fragment file
	in_file.open("slFragShader.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load sunlight fragment shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint slFragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Create shader source
	const GLchar* slFragmentSrc = src.c_str();

	//Set shader source
	glShaderSource(slFragShader, 1, &slFragmentSrc, NULL);

	//Compile shader
	glCompileShader(slFragShader);

	//check for compile error
	glGetShaderiv(slFragShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(slFragShader, 512, NULL, infoLog);
		cout << "Fragment shader (sunlight) failed to compile in load shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}

	//sunlight vertex shader source
	temp = "";
	src = "";

	in_file.open("slVertexhader.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load sunlight vertex shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint slVertShader = glCreateShader(GL_VERTEX_SHADER);

	//Create shader source
	const GLchar* slVertSrc = src.c_str();

	//Set shader source
	glShaderSource(slVertShader, 1, &slVertSrc, NULL);

	//Compile shader
	glCompileShader(slVertShader);

	//check for compile error
	glGetShaderiv(slVertShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(slVertShader, 512, NULL, infoLog);
		cout << "Fragment shader (sunlight) failed to compile in load shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}




	//Create program
	slShaderProgram = glCreateProgram();
	glAttachShader(slShaderProgram, slFragShader);
	glAttachShader(slShaderProgram, slVertShader);


	glLinkProgram(slShaderProgram);

	//Check link status
	glGetProgramiv(slShaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{

		glGetProgramInfoLog(slShaderProgram, 512, NULL, infoLog);
		cout << "Program failed to link sun shaders in load shaders function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}

	glUseProgram(0);
	glDeleteShader(slFragShader);
	glDeleteShader(slVertShader);


	return loadSuccess;
}

//---------------------------------------------------------------


//Create shader loading
bool loadShaders(GLuint& ShaderProgram)
{

	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex file
	in_file.open("vertexShaderSource.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load vertex shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Create shader source
	const GLchar* vertexSrc = src.c_str();

	//Set shader source
	glShaderSource(vertexShader, 1, &vertexSrc, NULL);

	//Compile shader
	glCompileShader(vertexShader);

	//check for compile error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "Vertex shader failed to compile in load shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}

	//Fragment shader
	temp = "";
	src = "";

	//Fragment file
	in_file.open("fragmentShaderSource.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load fragment shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Create shader source
	const GLchar* fragmentSrc = src.c_str();

	//Set shader source
	glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);

	//Compile shader
	glCompileShader(fragmentShader);

	//check for compile error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Fragment shader failed to compile in load shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}



	//Create program
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);

	glLinkProgram(ShaderProgram);

	//Check link status
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{

		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		cout << "Program failed to link in load shaders function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}

	//Clean-up
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

//-------------------------------------------------------------------------------
// 
// shadow shaders

//Create shader loading
bool loadShadowShaders(GLuint& shadowProgram)
{

	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex file
	in_file.open("shadowVertexShader.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load vertex shadow shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint shadowVert = glCreateShader(GL_VERTEX_SHADER);

	//Create shader source
	const GLchar* shadowVertexSrc = src.c_str();

	//Set shader source
	glShaderSource(shadowVert, 1, &shadowVertexSrc, NULL);

	//Compile shader
	glCompileShader(shadowVert);

	//check for compile error
	glGetShaderiv(shadowVert, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shadowVert, 512, NULL, infoLog);
		cout << "Vertex shader failed to compile in load shadow shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}

	//Fragment shader
	temp = "";
	src = "";

	//Fragment file
	in_file.open("shadowFragShader.glsl");

	if (in_file.is_open())
	{

		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{

		std::cout << "Failed to load fragment shader file.\n" << std::endl;
		loadSuccess = false;

	}

	in_file.close();


	GLuint shadowFrag = glCreateShader(GL_FRAGMENT_SHADER);

	//Create shader source
	const GLchar* shadFragmentSrc = src.c_str();

	//Set shader source
	glShaderSource(shadowFrag, 1, &shadFragmentSrc, NULL);

	//Compile shader
	glCompileShader(shadowFrag);

	//check for compile error
	glGetShaderiv(shadowFrag, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shadowFrag, 512, NULL, infoLog);
		cout << "Fragment shader failed to compile in load shadow shader function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}



	//Create program
	ShaderProgram = glCreateProgram();
	glAttachShader(shadowProgram, shadowVert);
	glAttachShader(shadowProgram, shadowFrag);

	glLinkProgram(shadowProgram);

	//Check link status
	glGetProgramiv(shadowProgram, GL_LINK_STATUS, &success);

	if (!success)
	{

		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		cout << "Program failed to link in load shadow shaders function." << endl;
		cout << infoLog << endl;
		loadSuccess = false;

	}


	//Clean-up
	glUseProgram(0);
	glDeleteShader(shadowFrag);
	glDeleteShader(shadowVert);

	return loadSuccess;
}

//-----------------------------------------------------------------------------
// Camera position and orientation
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera movement speed and mouse sensitivity
float movementSpeed = 0.5f;
float mouseSensitivity = 0.1f;

// Mouse position tracking
double lastMouseX = 400;
double lastMouseY = 300;
bool firstMouse = true;

// Projection matrices for perspective and orthographic views
glm::mat4 perspectiveProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
glm::mat4 orthographicProjection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);

// Flag to track the current projection type
bool usePerspectiveProjection = true;

// Keyboard input handling
void processInput(GLFWwindow* window)
{
	// Close the window when pressing the escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	unsigned int projectionLoc = glGetUniformLocation(ShaderProgram, "projection");

	// Toggle between Perspective and Orthographic projections using 'P' key
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		usePerspectiveProjection = !usePerspectiveProjection;

		// Update the projection matrix based on the current projection type
		if (usePerspectiveProjection)
			projection = perspectiveProjection;
		else
			projection = orthographicProjection;

		// Pass the updated projection matrix to the shaders
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	//-------------------------------------------------------------------------------------------------------

	// Camera movement using WASD keys
	//added cout statements for debugging to verify the camera movement was correct
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += movementSpeed * cameraFront;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= movementSpeed * cameraFront;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}

	// Camera movement using QE keys for upward and downward
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cameraPos += movementSpeed * cameraUp;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraPos -= movementSpeed * cameraUp;
		std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
	}
}
// Mouse movement callback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xOffset = static_cast<float>(xpos - lastMouseX) * mouseSensitivity;
	float yOffset = static_cast<float>(lastMouseY - ypos) * mouseSensitivity;

	lastMouseX = xpos;
	lastMouseY = ypos;

	float yaw = glm::radians(xOffset);
	float pitch = glm::radians(yOffset);

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));

	cameraFront = glm::normalize(glm::vec3(rotation * glm::vec4(cameraFront, 1.0f)));
}




struct Vertex
{
	glm::vec3 pos;   // vertex position
	glm::vec3 norm;  // vertex normal
	float tu;        // texture coordinates
	float tv;
};

// Function to create the sphere vertices and indices
void CreateSphere(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float fRad, unsigned int slices, unsigned int stacks)
{
	// Calculate vertex positions and normals for the sphere
	for (unsigned int stack = 0; stack <= stacks; stack++)
	{
		float theta = stack * glm::pi<float>() / stacks;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (unsigned int slice = 0; slice <= slices; slice++)
		{
			float phi = slice * 2.0f * glm::pi<float>() / slices;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			Vertex vertex;
			vertex.pos.x = fRad * sinTheta * cosPhi;
			vertex.pos.y = fRad * cosTheta;
			vertex.pos.z = fRad * sinTheta * sinPhi;

			vertex.norm = glm::normalize(vertex.pos);
			vertex.tu = static_cast<float>(slice) / static_cast<float>(slices);
			vertex.tv = static_cast<float>(stack) / static_cast<float>(stacks);

			vertices.push_back(vertex);
		}
	}

	// Calculate indices for the sphere
	for (unsigned int stack = 0; stack < stacks; stack++)
	{
		for (unsigned int slice = 0; slice < slices; slice++)
		{
			unsigned int baseIndex = stack * (slices + 1) + slice;
			indices.push_back(baseIndex);
			indices.push_back(baseIndex + slices + 1);
			indices.push_back(baseIndex + 1);

			indices.push_back(baseIndex + slices + 1);
			indices.push_back(baseIndex + slices + 2);
			indices.push_back(baseIndex + 1);
		}
	}
}

GLuint CreateSphereVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1); // norm
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

	glEnableVertexAttribArray(2); // tu
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tu));

	glEnableVertexAttribArray(3); // tv
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tv));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vao;
}

// Function to create the cone vertices and indices
void CreateCone(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, float fRad, float height, unsigned int slices)
{
	// Calculate vertex positions and normals for the cone
	float angleStep = glm::two_pi<float>() / slices;
	for (unsigned int i = 0; i < slices; ++i)
	{
		float angle = i * angleStep;
		float x = fRad * glm::cos(angle);
		float z = fRad * glm::sin(angle);

		Vertex vertex;
		vertex.pos = glm::vec3(x, 0.0f, z);
		vertex.norm = glm::normalize(glm::vec3(x, -height / 2.0f, z)); // Invert the Y component of the normal
		vertex.tu = static_cast<float>(i) / static_cast<float>(slices);
		vertex.tv = 1.0f;

		vertices.push_back(vertex);
	}

	// Add the top vertex of the cone
	Vertex topVertex;
	topVertex.pos = glm::vec3(0.0f, height, 0.0f);
	topVertex.norm = glm::normalize(glm::vec3(0.0f, height / 2.0f, 0.0f)); // Update the normal for the top vertex
	topVertex.tu = 0.5f;
	topVertex.tv = 0.0f;

	vertices.push_back(topVertex);

	// Calculate indices for the base of the cone
	for (unsigned int i = 0; i < slices; ++i)
	{
		indices.push_back(i);
		indices.push_back((i + 1) % slices);
		indices.push_back(static_cast<unsigned int>(vertices.size()) - 1);
	}


}


GLuint CreateConeVAO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1); // norm
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));

	glEnableVertexAttribArray(2); // tu
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tu));

	glEnableVertexAttribArray(3); // tv
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tv));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vao;
}


//Plane vertices with texture coordinates
std::vector<float> planeVertices = {
	-2.0f, 1.0f, -2.0f, 0.0f, 0.0f, // Vertex 0: x, y, z, u, v
	 2.0f, 1.0f, -2.0f, 1.0f, 0.0f,  // Vertex 1: x, y, z, u, v
	 2.0f, 1.0f,  2.0f, 1.0f, 1.0f,   // Vertex 2: x, y, z, u, v
	-2.0f, 1.0f,  2.0f, 0.0f, 1.0f   // Vertex 3: x, y, z, u, v
};


std::vector<unsigned int> planeIndices = {
	0, 1, 2,
	2, 3, 0
};

GLuint CreatePlaneVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1); // texCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vao;
}
//declare global variables used for lighting

GLuint yellowSphereVAO;
glm::vec3 yellowSpherePos = glm::vec3(-1.5f, 1.5f, -1.5f); // Initial position of the yellow sphere
//scale
glm::vec3 sunScale(2.0f);

//Sun and sunlight color
glm::vec3 objectColor(1.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

//light position and scale
glm::vec3 lightPos(-1.0f, 1.0f, 0.31f);
glm::vec3 lightScale(0.5f);



//set width height and numchannels
int width, height, nrChannels;


int main(int argc, char* argv[])
{

	
	//Initialize window
	glfwInit();

	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed!" << std::endl;
		return -1;
	}

	// Set OpenGL version to 3.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tammy Hartline's 3D Scene ~ Final Project ~ CS330", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Set the mouse callback function
	glfwSetCursorPosCallback(window, mouse_callback);

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set mouse movement callback
	glfwSetCursorPosCallback(window, mouse_callback);

	// Enable mouse cursor capture
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Create the mapped sphere
	std::vector<Vertex> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	GLuint sphereVAO;
	float sphereRadius = 0.6f;
	unsigned int sphereSlices = 50;
	unsigned int sphereStacks = 50;

	CreateSphere(sphereVertices, sphereIndices, sphereRadius, sphereSlices, sphereStacks);

	// Create and bind the Vertex Array Object (VAO) for the sphere
	sphereVAO = CreateSphereVAO(sphereVertices, sphereIndices);

	// Create the cone vertices and indices
	std::vector<Vertex> coneVertices;
	std::vector<unsigned int> coneIndices;
	GLuint coneVAO;
	float coneRadius = 0.4f;
	float coneHeight = 1.0f;
	unsigned int coneSlices = 50;

	CreateCone(coneVertices, coneIndices, coneRadius, coneHeight, coneSlices);

	// Create and bind the Vertex Array Object (VAO) for the cone
	coneVAO = CreateConeVAO(coneVertices, coneIndices);

		//Check shader program initiates as expected
	loadShaders(ShaderProgram);
	if (!loadShaders(ShaderProgram))
	{

		cout << "Sun shader initialization in main: " << glGetError();
		glfwTerminate();

	}

	//Check shader program initiates as expected
	GLuint slShaderProgram;
	loadShaders(slShaderProgram);
	if (!loadShaders(slShaderProgram))
	{

		cout << glGetError();
		glfwTerminate();

	}

	// Create and bind the Vertex Array Object (VAO) for the plane
	GLuint planeVAO = CreatePlaneVAO(planeVertices, planeIndices);

	// Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set mouse movement callback
	glfwSetCursorPosCallback(window, mouse_callback);

	// Enable mouse cursor capture
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Check plane shader program
	GLuint planeShaderProgram;
	loadShaders(planeShaderProgram);
	if (!loadShaders(planeShaderProgram))
	{

		cout << "Plane shader initialization in main: " << glGetError();
		glfwTerminate();

	}

	//Define sphere (sun) for lighting effect
	float yellowSphereRotationSpeed = 1.0f; // Rotation speed of the yellow sphere in degrees per second

	//Create the new sun sphere
	std::vector<Vertex> yellowSphereVertices;
	std::vector<unsigned int> yellowSphereIndices;
	float yellowSphereRadius = sphereRadius / 2.0f;
	CreateSphere(yellowSphereVertices, yellowSphereIndices, yellowSphereRadius, sphereSlices, sphereStacks);

	//Create and bind VAO for sun
	yellowSphereVAO = CreateSphereVAO(yellowSphereVertices, yellowSphereIndices);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Get uniform locations
	GLint modLoc = glGetUniformLocation(slShaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(slShaderProgram, "view");
	GLint projLoc = glGetUniformLocation(slShaderProgram, "projection");
	GLint objColLoc = glGetUniformLocation(slShaderProgram, "objectColor");
	GLint lightColLoc = glGetUniformLocation(slShaderProgram, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(slShaderProgram, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(slShaderProgram, "viewPos");

	// Activate shader program
	glUseProgram(slShaderProgram);

	// Calculate the position of the sphere to touch the edge of the cone at its widest point
	float distanceFromConeTopToSphereCenter = sphereRadius - coneHeight / 1.5f; // The distance from the cone's top to the center of the sphere
	glm::vec3 spherePosition = glm::vec3(0.0f, distanceFromConeTopToSphereCenter, 0.0f);



	// load and create sphere texture
	// -------------------------
	//-------------------------------------------------------------------------------------------------------
	//Sphere (ice cream) texture
	int image_width = 0;
	int image_height = 0;
	unsigned char* image1 = stbi_load("images/sphere.png", &image_width, &image_height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);
	
	
	GLuint sphereTexture;		//this is the texture id
	glGenTextures(1, &sphereTexture);
	glBindTexture(GL_TEXTURE_2D, sphereTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (image1)
	{

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	    glGenerateMipmap(GL_TEXTURE_2D);


	}

	else
	{
	    std::cout << "Failed to load sphere texture." << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image1);
	//---------------------------------------------------------------------------------------------------------
	//Load and create Cone texture
	glUseProgram(ShaderProgram);
	
	unsigned char* image2 = stbi_load("images/cone.png", &image_width, &image_height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);


	GLuint coneTexture;		//this is the texture id
	glGenTextures(1, &coneTexture);
	glBindTexture(GL_TEXTURE_2D, coneTexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (image2)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);


	}

	else
	{
		std::cout << "Failed to load sphere texture." << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image2);

	// Load and create the plane texture
	unsigned char* image3 = stbi_load("images/plane.png", &image_width, &image_height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);

	GLuint planeTexture; // This is the texture id
	glGenTextures(1, &planeTexture);
	glBindTexture(GL_TEXTURE_2D, planeTexture);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (image3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load plane texture." << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image3);

	unsigned int sphereTextureLoc = glGetUniformLocation(ShaderProgram, "sphereTexture");
	unsigned int coneTextureLoc = glGetUniformLocation(ShaderProgram, "coneTexture");
	unsigned int planeTextureLoc = glGetUniformLocation(ShaderProgram, "planeTexture");

	
	// Activate the textures and bind to different texture units
	glUseProgram(ShaderProgram);
	glUniform1i(sphereTextureLoc, 0); // Use texture unit 0 for sphere texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sphereTexture);

	glUniform1i(coneTextureLoc, 1); // Use texture unit 1 for cone texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, coneTexture);

	glUniform1i(planeTextureLoc, 2); // Use texture unit 2 for plane texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planeTexture);

	//Set uniform values for lighting
	glUseProgram(slShaderProgram);

	glUniform3f(glGetUniformLocation(slShaderProgram, "objectColor"), 1.0f, 1.0f, 0.0f);
	glUniform3f(glGetUniformLocation(slShaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(slShaderProgram, "lightPos"), -1.0f, 2.0f, 2.0f);
	glUniform3f(glGetUniformLocation(slShaderProgram, "viewPos"), 0.0f, 0.0f, 0.0f);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{

		glEnable(GL_DEPTH_TEST);

		// Process keyboard input
		processInput(window);

		// Clear the screen
		//glClearColor(0.678f, 0.847f, 0.902f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderProgram);

		// Update the view matrix based on the camera position and orientation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Set the view matrix in the shader
		unsigned int viewLoc = glGetUniformLocation(ShaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// Create Model matrix
		glm::mat4 model = glm::mat4(1.0f);

		// Set the model matrix in the shader
		unsigned int modelLoc = glGetUniformLocation(ShaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Set the projection matrix based on the current projection type
		glm::mat4 projection;
		if (usePerspectiveProjection)
			projection = perspectiveProjection;
		else
			projection = orthographicProjection;

		// Set the projection matrix in the shader
		unsigned int projectionLoc = glGetUniformLocation(ShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Update the position and rotation of the yellow sphere
		float currentTime = glfwGetTime();
		float yellowSphereRotationAngle = currentTime * yellowSphereRotationSpeed;
		glm::mat4 yellowSphereModel = glm::mat4(1.0f);
		yellowSphereModel = glm::translate(yellowSphereModel, yellowSpherePos);
		yellowSphereModel = glm::rotate(yellowSphereModel, glm::radians(yellowSphereRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the Y-axis
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(yellowSphereModel));

		// Draw the yellow sphere
		glUniform1i(glGetUniformLocation(ShaderProgram, "objectType"), 3); // Set objectType to 3 for the yellow sphere
		glBindVertexArray(yellowSphereVAO);
		glDrawElements(GL_TRIANGLES, (sphereSlices * 2 * (sphereStacks - 1)) * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Draw the scoop sphere
		glm::mat4 sphereModel = glm::mat4(1.0f);
		sphereModel = glm::translate(sphereModel, spherePosition); // Apply translation to the sphere
		glUniform1i(glGetUniformLocation(ShaderProgram, "objectType"), 0); // Set objectType to 1 for the original sphere
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel)); // Set model matrix for the sphere
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, (sphereSlices * 2 * (sphereStacks - 1)) * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Draw the cone (point at the bottom)
		glm::mat4 coneModel = glm::mat4(1.0f);
		coneModel = glm::translate(coneModel, glm::vec3(0.0f, -coneHeight / 2.0f, 0.0f)); // Move the cone down by half of its height
		coneModel = glm::rotate(coneModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate the cone 180 degrees around the X-axis
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(coneModel)); // Set model matrix for cone

		glUniform1i(glGetUniformLocation(ShaderProgram, "objectType"), 1); // Set objectType to 0 for the cone
		glBindVertexArray(coneVAO);
		glDrawElements(GL_TRIANGLES, coneIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		//Draw the plane
		
		glUniform1i(glGetUniformLocation(ShaderProgram, "objectType"), 2);
		glBindVertexArray(planeVAO);		
		glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		//Poll events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	// Clean up and exit
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteVertexArrays(1, &coneVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteProgram(ShaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}