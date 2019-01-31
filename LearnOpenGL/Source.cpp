#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Shaders
const char* vertexShaderSource = R"STRING(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() 
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);
}	
)STRING";

const char* fragmentShaderSource = R"STRING(
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)STRING";

const char* fragment2ShaderSource = R"STRING(
#version 330 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
)STRING";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

GLFWwindow* initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

int loadShader(const char* src, GLenum type, const char* errorHeading)
{
	int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << errorHeading << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		return -1;
	}
	return shader;
}

std::vector<int> loadShaders()
{
	std::vector<int> shaderPrograms;
	// build and compile our shader program
	int vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER, "VERTEX");
	int fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER, "FRAGMENT");
	int frag2Shader = loadShader(fragment2ShaderSource, GL_FRAGMENT_SHADER, "FRAGMENT");

	//link shaders
	int shaderProgram1 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader);
	glLinkProgram(shaderProgram1);
	// Check for linker errors
	int success;
	char infoLog[255];
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return shaderPrograms;
	}

	int shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, frag2Shader);
	glLinkProgram(shaderProgram2);
	// Check for linker errors
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return shaderPrograms;
	}

	// cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(frag2Shader);
	
	shaderPrograms.push_back(shaderProgram1);
	shaderPrograms.push_back(shaderProgram2);
	return shaderPrograms;
}



int main()
{

	GLFWwindow* window = initWindow();
	if (!window)
		return -1;
	std::vector<int> shaderPrograms = loadShaders();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float rectVerts[] = {
		0.5, 0.5, 0, // top right
		0.5, -0.5, 0,// bottom right
		-0.5, -0.5, 0,//bottom left
		-0.5, 0.5, 0 //top left
	};
	unsigned int rectIndices[] = {
		0, 1, 3, //first triangle
		1, 2, 3  //second triangle
	};

	float twoTriangleVerts[] = {
		-1, 0, 0,
		-0.5, 1, 0,
		0, 0, 0,
		0.5, 1, 0,
		1, 0, 0
	};
	unsigned int twoTriangleIndices[] = {
		0, 1, 2,
		2, 3, 4
	};

	float tri1Verts[] = {
		-1,-1,0,
		0,0,0,
		0,-1,0
	};
	float tri2Verts[] = {
		0,0,0,
		1,0,0,
		1,1,0
	};


	/* Exercise 1
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(twoTriangleVerts), twoTriangleVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(twoTriangleIndices), twoTriangleIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindVertexArray(0); // Unbind VAO*/


	//Exercise 2
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri1Verts), tri1Verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri2Verts), tri2Verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	

	// The render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);
		// Rendering commands...
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw the triangle!
		glUseProgram(shaderPrograms[0]);
		glBindVertexArray(VAO[0]);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderPrograms[1]);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}