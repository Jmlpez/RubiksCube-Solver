#include <iostream>
#include <ctime>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./include/DisplayCube.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

float lastFrame = glfwGetTime();
float deltaTime = 0;

Camera camera(SCR_WIDTH, SCR_HEIGHT);

using namespace std;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    // Tell GLFW what version of OpenGL we are using (3.3 in this case)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So thats means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object  with a width, height and a name
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    // Error checking
    if (window == NULL)
    {
        cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Callback for resize events
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Callback for scroll events
    glfwSetScrollCallback(window, scroll_callback);

    // load GLAD to get the pointers functions of OpenGL, respect to the current OS
    gladLoadGL();

    // Specify the viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Creates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("./shaders/default.vert", "./shaders/default.frag");

    shaderProgram.Activate();
    //---------------Setting directional light in the scene---------------//
    shaderProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shaderProgram.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    shaderProgram.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    shaderProgram.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    // material settings
    shaderProgram.setFloat("material.shininess", 16.0f);
    //---------------Setting directional light in the scene---------------//

    glEnable(GL_DEPTH_TEST);

    float angle = 0.0f; // degrees
    float rotationCount = 0;
    unsigned int counter = 0;

    DisplayCube cube;
    // cube.setMoving(true);

    // Export model matrix
    glm::mat4 boxModel = glm::mat4(1.0f);
    // boxModel = glm::rotate(boxModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shaderProgram.setMat4("model", boxModel);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        cube.processInput(window);
        // Specify the color of the background
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        // rgb(134 203 249)

        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        counter++;
        if (deltaTime >= 1.0 / 30.0)
        {
            string FPS = "Cubo de Rubick: " + to_string((1.0f / deltaTime) * counter);
            glfwSetWindowTitle(window, FPS.c_str());
            lastFrame = currentFrame;
            counter = 0;
        }
        camera.processInput(window, deltaTime);
        camera.updateMatrix();

        // Tells OpenGL which Shader Program we want to use
        // Activate the shader before exporting uniforms
        shaderProgram.Activate();

        // Exports the camera Position to the Fragment Shader for specular lighting
        shaderProgram.setVec3("camPos", camera.cameraPos);

        // Export the camera matrix to the shader
        camera.getMatrix(shaderProgram);

        // Draw the cube
        cube.handleMove();
        cube.Draw(shaderProgram);
        cube.updateModel(boxModel, shaderProgram);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete all objects we've created
    shaderProgram.Delete();
    // Destroy Window object
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera.updateViewport(width, height);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScroll((float)(yoffset));
}
