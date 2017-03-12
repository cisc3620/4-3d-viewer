// Local Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"          // to print vectors and matrices

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define RED 1.0f, 0.0f, 0.0f            // define macros for convenience
#define BLUE 0.0f, 0.0f, 1.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define YELLOW 1.0f, 1.0f, 0.0f

// device coordinates
#define LEFT -0.5f
#define TOP 0.5f
#define RIGHT 0.5f
#define BOTTOM -0.5f
#define MIDDLE 0.0f

const GLchar* vertexSource =
"#version 150 core\n"             // glsl version
"in vec3 position;"               // expects 3 values for position
"in vec3 color;"                  // and 3 values for color
"out vec3 Color;"                 // will pass color along pipeline
"uniform mat4 model;"             // uniform = the same for all vertices
"void main()"
"{"
"    Color = color;"              // just pass color along without modifying it
"    gl_Position = model * vec4(position, 1.0);"   // gl_Position is special variable for final position
"}";                                                    // must be in homogeneous coordinates -- put in 0 for z and 1 for w
// multiply by model matrix to transform
const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"out vec4 outColor;"
"void main()"
"{"
"    outColor = vec4(Color, 1.0);"
"}";

// vertex data
GLfloat vertices [] = {
   0.0f,   1.0f,  0.0f,   RED,
   0.0f,  -0.5f,  0.866f, BLUE,
   0.75f, -0.5f, -0.433f, GREEN,
  -0.75f, -0.5f, -0.433f, YELLOW,
};

bool controlIsPressed(GLFWwindow* window) {
	return glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS; 
}

// callback for keyboard input
// move camera when arrow keys are pressed, rotate it when arrow keys are pressed with control
void key_callback(GLFWwindow* mWindow, int key, int scancode, int action, int mods)
{
  std::cout << (mods == GLFW_MOD_SHIFT) << std::endl;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
    if (mods == GLFW_MOD_SHIFT) {
		std::cout << "rotating camera left\n"; // rotate camera
	    glRotatef(1.0f, 0.0f, 1.0f, 0.0f); 
	}
    else std::cout << "moving camera left\n";  // move camera
    // etc.
  }
}

int main(int argc, char * argv[]) {
  // Load GLFW and Create a Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  auto mWindow = glfwCreateWindow(200, 200, "CISC3620", nullptr, nullptr);
  // Check for Valid Context
  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }
  
  // callbacks
  glfwSetKeyCallback(mWindow, key_callback);
  
  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(mWindow);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
  
  // Create Vertex Array Object: this will store all the information about the vertex data that we are about to specify
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Create a Vertex Buffer Object and copy the vertex data to it
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  // INDEX BUFFER
  GLuint IBO;
  unsigned int Indices[] = { 0, 1, 2,
                             1, 2, 3,
                             2, 3, 0,
                             3, 0, 1 };
						   
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

  // Create and compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  // Create and compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  // Specify the layout of the vertex data
  // position
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);  // attribute location, # values, value type, normalize?, stride, offset
  // color
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  
  // model matrix
  GLint modelTransform = glGetUniformLocation(shaderProgram, "model");
  glm::mat4 scale_model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f,0.5f));
	glm::mat4 model = scale_model;
  glUniformMatrix4fv(modelTransform, 1, GL_FALSE, glm::value_ptr(model));
  
  // Rendering Loop
  
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  
  while (glfwWindowShouldClose(mWindow) == false) {
    
    // Background Fill Color
    glClearColor(0.85f, 0.65f, 0.65f, 0.8f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw triangle
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	
	for(int i = 0; i < 4; i++) {
		GLfloat tmp = vertices[6*i];
		vertices[6*i] = vertices[6*i]*cos(0.00004) + vertices[6*i+2]*sin(0.00004);
		vertices[6*i+2] = vertices[6*i+2]*cos(0.00004) - tmp*sin(0.00004);
	}
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }   glfwTerminate();
  // clean up
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  
  return EXIT_SUCCESS;
}
