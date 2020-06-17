
#include <iostream>
#include "Renderer/Manager/Shader.h"
#include "Renderer/Manager/Texture.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Manager/Geometry.h"
#include "Renderer/Manager/Mesh.h"
#include "Renderer/Manager/TextureMgr.h"
#include "Renderer/Camera/FPSCamera.h"
#include "Renderer/Camera/Camera3D.h"
#include "Renderer/RenderTarget/RenderTarget.h"
#include "Renderer/Manager/ShaderMgr.h"
#include "Renderer/Manager/MeshMgr.h"
#include "Renderer/RenderDevice.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    /*glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BoomBoomEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Failed to initialize Glew" << std::endl;
    }
    */
    Kawaii::RenderDevice render;
    render.initialize("BoomBoomEngine", SCR_WIDTH, SCR_HEIGHT);
    //glEnable(GL_DEPTH_TEST);

    Kawaii::Shader shader("Shaders/shader.vs", "Shaders/shader.fs");
    Kawaii::Shader skyBoxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
    Kawaii::Cube mesh(8.0f, 8.0f, 8.0f);
    Kawaii::Sphere sphere(1.0f, 36, 18);
    //Kawaii::Texture2D texture("res/wall.jpg");
    unsigned int sphereMeshIndex = Kawaii::MeshMgr::getSingleton()->loadMesh(new Kawaii::Sphere(1.0f, 36, 18));
    Kawaii::FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    //printf(fpsCamera.getPosition());
    
 
    unsigned int testShader = Kawaii::ShaderMgr::getSingleton()->loadShader("test", "Shaders/shader.vs", "Shaders/shader.fs");
    Kawaii::SimpleRender* simpleRender = new Kawaii::SimpleRender(testShader);
    simpleRender->addMesh(sphereMeshIndex);

    unsigned int skyId = Kawaii::TextureMgr::getSingleton()->loadTextureCube("kawaii","res/skybox/", ".png");
    //std::cout << skyId;

    Kawaii::TextureCube textureCube("res/skybox/", ".png");

    
    float skyboxVertices[] = {
        // positions          
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
   

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // shader configuration
     // --------------------
    //shader.bind();
    //shader.setInt("skybox", 0);

    skyBoxShader.bind();
    skyBoxShader.setInt("skybox", 0);

    Kawaii::ShaderMgr::ptr shaderMgr = Kawaii::ShaderMgr::getSingleton();

    // render loop
    // -----------
    while (render.run())
    {
        // per-frame time logic
       // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        render.beginFrame();
       
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        //shader.bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        /*fpsCamera.setPerspectiveProject(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //fpsCamera.lookAt, Kawaii::FPSCamera::LocalUp);
        glm::mat4 projection1 = fpsCamera.getProjectMatrix();
        glm::mat4 view1 = fpsCamera.getViewMatrix();        
        
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection1);
        shader.setVec3("cameraPos", camera.Position);
        // cubes
        //mesh.draw(false, 0);
        
        sphere.draw(false, 0);
        */
        Kawaii::Shader::ptr shad = shaderMgr->getShader(testShader);
        simpleRender->testrender(&camera, shad);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyBoxShader.bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyBoxShader.setMat4("view", view);
        skyBoxShader.setMat4("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        //textureCube.bind(0);
        Kawaii::TextureMgr::getSingleton()->bindTexture("kawaii", skyId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //glfwSwapBuffers(window);
        //glfwPollEvents();
        render.endFrame();
    }

    //glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
   //glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    render.shutdown();

}

