#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// windows dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint vaoId, vboId, programShaderId;

GLint uniformModelId;

bool direction = true;
float triOffSet = 0.0f;
float triMaxOffSet = 0.7f;
float triIncrement = 0.0005f;

/* Vertex Shader */

static const char* vShaderSource = "						\n\
		#version 330    			   			        	\n\
		                         							\n\
		layout (location = 0) in vec3 pos;  	       		\n\
												   			\n\
		uniform mat4 model;			     					\n\
												   			\n\
												   			\n\
	    void main () {                                	 	 \n\
			gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0); 	\n\
		}";

/* Fragment Shader */

static const char* fShaderSource = "						\n\
		#version 330    			   			        	\n\
		                         							\n\
		out vec4 colour; 					 	       		\n\
														   	\n\
												   			\n\
	    void main () {                                	 	\n\
			colour = vec4(1.0, 0.0, 0.0, 1.0);           	\n\
		}";

void createTriangle() {
	GLfloat vertices[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	/* inicio bind vertex array */

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* fin bind vertex array */

	glBindVertexArray(0);

}

void addShader(GLuint programId, const char* shaderCode, GLenum shaderType) {
	GLuint shaderId = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shaderId, 1, theCode, codeLength);
	glCompileShader(shaderId);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shaderId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(programId, shaderId);
}

void compileShaders() {

	programShaderId = glCreateProgram();

	if (!programShaderId) {
		std::cerr << "Error creating shader program!" << std::endl;
		return;
	}
	addShader(programShaderId, vShaderSource, GL_VERTEX_SHADER);
	addShader(programShaderId, fShaderSource, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(programShaderId);
	glGetProgramiv(programShaderId, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(programShaderId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(programShaderId);
	glGetProgramiv(programShaderId, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(programShaderId, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error validating program: " << eLog << std::endl;
		return;
	}

	uniformModelId = glGetUniformLocation(programShaderId, "model");
}

int main(int argc, char* args[]) {

	std::cout << "Hola Mundo !!" << std::endl;

	// initialise GLFW
	if (!glfwInit()) {
		std::cout << "GLFW initialisation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// setup GLFW windows properties
	// OpenGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);

	if (!mainWindow) {
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extesions features
	glewExperimental = GL_TRUE;

	if (glewInit()) {
		std::cout << "GLEW initialisation failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);


	createTriangle();
	compileShaders();

	// Loop until window close
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get + Handle user inputs events
		glfwPollEvents();

		if (direction) {
			triOffSet += triIncrement;
		} else {
			triOffSet -= triIncrement;
		}

		if (abs(triOffSet) >= triMaxOffSet) {
			direction = !direction;
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programShaderId);


		glm::mat4 model;
		model = glm::translate(model, glm::vec3(triOffSet, triOffSet, 0));

        glUniformMatrix4fv(uniformModelId, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(vaoId);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);


		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
