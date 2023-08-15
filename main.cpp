#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Picture.h"
#include <omp.h>
#include "shader.h"
#include <filesystem>
namespace fs = std::filesystem;

std::string imagePath = "Image/";
std::string resultPath = "Result/";

void resizeWindow(GLFWwindow *window, int width, int height ) {

    glfwSetWindowSize(window, width, height);

    // Set the callback
    glViewport(0, 0, width, height);
}

std::vector<Picture*> genPicForAllFiles(std::string directory) {
    std::cout << "Generating picture for image folder" << std::endl;
    std::vector<Picture*> pictures;
    // Convert the string to a path and check if it exists and is a directory
    std::filesystem::path dirPath(directory);
    if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) {
        // Iterate over each entry in the directory
        for (auto entry : std::filesystem::directory_iterator(dirPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                std::string filename = imagePath + entry.path().filename().string();
                Picture* currPic = new Picture(filename);
                pictures.emplace_back(currPic);
            }
        }
    }
    return pictures;
}

std::string screenShot(Picture* picture, GLuint shader , GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    picture->draw(shader);
    glfwSwapBuffers(window);
    std::string resultImagePath = resultPath + picture->GetName().substr(imagePath.size(), picture->GetName().size() - imagePath.size() - 4) + "_result.png";
    picture->saveResizedImage(resultImagePath, window);

    return resultImagePath;
}

Picture* switchDisplayPicture(int index, std::vector<Picture*> results, GLFWwindow* window) {
    Picture* currRes = results[index];
    resizeWindow(window, currRes->GetWidth(), currRes->GetHeight());
    return currRes;
}
int main() {
    std::cout << "Initializing" << std::endl;
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(512, 512, "Picture", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cout << "window create fail" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    std::cout << "Loading Shaders" << std::endl;
    //create shader 
    GLuint shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    //create resize shader 
    GLuint resizeProgram = LoadShaders("shaders/resizeShader.vert", "shaders/resizeShader.frag");
    std::vector<Picture*> pictures = genPicForAllFiles(imagePath);
    std::vector<Picture*> results;
    std::cout << "resizing images" << std::endl;
    for (int i = 0; i < pictures.size(); i++) {
        Picture* pic = pictures[i];
        std::cout << pic->GetName() << std::endl;
        resizeWindow(window, pic->GetWidth() / 2, pic->GetHeight() / 2);
        std::string resultImagePath = screenShot(pic, shaderProgram, window);
        Picture* result = new Picture(resultImagePath);
        results.emplace_back(result);
    }
    int currResultIndex = 0;
    Picture* currPicture = switchDisplayPicture(currResultIndex, results, window);
    Picture* currResult = switchDisplayPicture(currResultIndex, results, window);
    
    int leftPressed = false;
    int rightPressed = false;
    std::cout << "Showing images" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        //create buffer
        glfwMakeContextCurrent(window);
        glUseProgram(shaderProgram);
        glClear(GL_COLOR_BUFFER_BIT);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS&& !leftPressed) {
            // Handle left arrow key press here
            currResultIndex = std::max(0, --currResultIndex);
            std::cout << "Showing Image: " << currResultIndex << std::endl;
            currResult = switchDisplayPicture(currResultIndex, results, window);
            leftPressed = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
            leftPressed = false;
        
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !rightPressed) {
            // Handle left arrow key press here
            int maxIndex = results.size() - 1;
            currResultIndex = std::min(maxIndex, ++currResultIndex);
            std::cout << "Showing Image: " << currResultIndex << std::endl;
            currResult = switchDisplayPicture(currResultIndex, results, window);
            rightPressed = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
            rightPressed = false;

        }
        
        currResult->draw(shaderProgram);

        glfwSwapBuffers(window);
        //pic->saveResizedImage(imagePath+"result.png", window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}