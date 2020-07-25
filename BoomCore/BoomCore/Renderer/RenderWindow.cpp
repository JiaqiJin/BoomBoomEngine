#include "RenderWindow.h"

#include <iostream>

namespace Kawaii
{
	// mouse pos, frame delta time, key pressed record.
	 glm::vec2 RenderWindow::m_cursorPos;
	 glm::vec2 RenderWindow::m_deltaCurPos;
	 bool RenderWindow::m_keysPressed[1024] = {0};
	 float RenderWindow::m_deltaTime = 0.0f;
	 float RenderWindow::m_lastFrame = 0.0f;
	 bool RenderWindow::m_buttonPressed[GLFW_MOUSE_BUTTON_LAST] = { 0 };
	 template<> RenderWindow::ptr Singleton<RenderWindow>::_instance = nullptr;

	 RenderWindow::ptr RenderWindow::getSingleton()
	 {
		 if (!_instance)
			 _instance = std::make_shared<RenderWindow>();
		 return _instance;
	 }

	 bool RenderWindow::initialize(std::string title, int width, int height, bool debugEnable)
	 {
		 m_width = width;
		 m_height = height;
		 m_lastFrame = 0.0f;
		 m_deltaTime = 0.0f;
		 m_debugMode = debugEnable;
		 m_cursorPos.x = width / 2.0f;
		 m_cursorPos.y = height / 2.0f;

		 // context initialization.
		 glfwInit();
		 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		 glfwWindowHint(GLFW_SAMPLES, 16);
		 glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		 // debug context.
		 if (m_debugMode)
			 glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		 else
			 glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);

		 m_windowHandler = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
		 if (m_windowHandler == nullptr)
		 {
			 std::cout << "Failed to create GLFW window." << std::endl;
			 glfwTerminate();
			 return false;
		 }

		 glfwMakeContextCurrent(m_windowHandler);
		 glfwSetInputMode(m_windowHandler, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		 // load OpenGL context.
		 glewExperimental = GL_TRUE;
		 if (glewInit() != GLEW_OK)
		 {
			 std::cout << "Failed to initialize GLEW" << std::endl;
			 return false;
		 }

		 if (m_debugMode)
			 initializeDebugContex();

		 // check error.
		 GLenum error = glGetError();
		 if (error != GL_NO_ERROR)
		 {
			 std::cout << "OpenGL Error Code: " << error << std::endl;
		 }

		 // callback functions setting.
		
		 glfwSetCursorPosCallback(m_windowHandler, cursorPositionCallback);
		 glfwSetMouseButtonCallback(m_windowHandler, mouseButtonCallback);
		 glfwSetKeyCallback(m_windowHandler, keyCallback);
		 glfwSetFramebufferSizeCallback(m_windowHandler, framebufferSizeCallback);
		 glfwSetScrollCallback(m_windowHandler, scrollCallback);

		 glEnable(GL_DEPTH_TEST);

		 //render sys
		 m_renderSys = std::make_shared<RenderSys>();
		 m_renderSys->initialize(width, height);

		 return true;
	 }


	 bool RenderWindow::run()
	 {
		 if (m_windowHandler == nullptr)
		 {
			 std::cout << "Window was not created!\n";
			 return false;
		 }
		 return !glfwWindowShouldClose(m_windowHandler);
	 }

	 void RenderWindow::beginFrame()
	 {
		 processInput();
	 }


	 void RenderWindow::endFrame()
	 {
		 // deleta time calculation.
		 float currentFrame = glfwGetTime();
		 m_deltaTime = currentFrame - m_lastFrame;
		 m_lastFrame = currentFrame;

		 glfwSwapBuffers(m_windowHandler);
		 glfwPollEvents();

	 }

	 bool RenderWindow::shutdown()
	 {
		 glfwTerminate();
		 return false;
	 }


	 void RenderWindow::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
		 GLsizei length, const GLchar* message, const void* userParam)
	 {
		 if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		 std::cout << "---------------" << std::endl;
		 std::cout << "Debug message (" << id << "): " << message << std::endl;

		 switch (source)
		 {
		 case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		 case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		 case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		 case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		 case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		 case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		 }
		 std::cout << std::endl;

		 switch (type)
		 {
		 case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		 case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		 case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		 case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		 case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		 case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		 case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		 case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		 case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		 }
		 std::cout << std::endl;

		 switch (severity)
		 {
		 case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		 case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		 case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		 case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		 }
		 std::cout << std::endl;
		 std::cout << std::endl;
	 }


	 void RenderWindow::initializeDebugContex()
	 {
		 GLint flags;
		 glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		 if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		 {
			 std::cout << "OpenGL Debug Mode Launched.\n";
			 glEnable(GL_DEBUG_OUTPUT);
			 // makes sure errors are displayed synchronously.
			 glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			 glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, nullptr);
			 glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		 }
	 }
	 
	 void RenderWindow::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	 {
		 static bool first = true;
		 if ((!m_buttonPressed[GLFW_MOUSE_BUTTON_LEFT] && !m_buttonPressed[GLFW_MOUSE_BUTTON_RIGHT]))
		 {
			 first = true;
			 return;
		 }

		 // delta cursor position.
		 glm::vec2 lastPos = m_cursorPos;
		 if (first)
		 {
			 lastPos = glm::vec2(xpos, ypos);
			 first = false;
		 }
		 m_deltaCurPos.x = xpos - lastPos.x;
		 m_deltaCurPos.y = lastPos.y - ypos;
		 m_cursorPos = glm::vec2(xpos, ypos);
	 }

	 void RenderWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	 {
		 if (action == GLFW_PRESS)
			 m_buttonPressed[button] = true;
		 else
			 m_buttonPressed[button] = false;
	 }

	 void RenderWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	 {
		 if (action == GLFW_PRESS)
			 m_keysPressed[key] = true;
		 else if (action == GLFW_RELEASE)
			 m_keysPressed[key] = false;
	 }

	 void RenderWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
	 {
		 getSingleton()->getRenderSys()->resize(width, height);
	 }

	 void RenderWindow::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	 {
		 getSingleton()->getRenderSys()->getCamera()->onWheelMove(yoffset);
	 }

	 void RenderWindow::processInput()
	 {
		 // shutdown.
		 if (glfwGetKey(m_windowHandler, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			 glfwSetWindowShouldClose(m_windowHandler, true);

		 RenderWindow::ptr window = getSingleton();

		 // key pressed action.
		 if (m_keysPressed[GLFW_KEY_W])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'W');
		 if (m_keysPressed[GLFW_KEY_A])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'A');
		 if (m_keysPressed[GLFW_KEY_S])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'S');
		 if (m_keysPressed[GLFW_KEY_D])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'D');
		 if (m_keysPressed[GLFW_KEY_Q])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'Q');
		 if (m_keysPressed[GLFW_KEY_E])
			 window->getRenderSys()->getCamera()->onKeyPress(m_deltaTime, 'E');

		 // mouse action.
		 if (m_buttonPressed[GLFW_MOUSE_BUTTON_LEFT])
			 window->getRenderSys()->getCamera()->onMouseMove(m_deltaCurPos.x, m_deltaCurPos.y, "LEFT");
		 else if (m_buttonPressed[GLFW_MOUSE_BUTTON_RIGHT])
			 window->getRenderSys()->getCamera()->onMouseMove(m_deltaCurPos.x, m_deltaCurPos.y, "RIGHT");
		 m_deltaCurPos = glm::vec2(0.0f);
	 }

}