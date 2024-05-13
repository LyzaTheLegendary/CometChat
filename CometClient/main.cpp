#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {


	if (!glfwInit()) 
		return -1;
	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "CometChat", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	errno_t errcode = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!errcode)
	{
		std::cout << "Failed to initialize GLAD error code: " << errcode << std::endl;
		return -1;
	}

	return 0;
}