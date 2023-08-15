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

/// <summary>
/// resizing an window
/// </summary>
/// <param name="window"> the window you want to resize </param>
/// <param name="width"> the desired width </param>
/// <param name="height"> the desired height </param>
void resizeWindow(GLFWwindow *window, int width, int height ) {
    std::cout << "Window Size: " << width << ", " << height << std::endl;
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);
    
}
/// <summary>
/// Read all images from a directory
/// </summary>
/// <param name="directory"> string of that dir</param>
/// <returns>an array of Picture*</returns>
std::vector<Picture*> genPicForAllFiles(std::string directory) {
    std::vector<Picture*> pictures;

    // Convert the string to a path and check if it exists and is a directory
    std::filesystem::path dirPath(directory);

    if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) {

        // Get the number of files in the directory
        int numFiles = std::distance(std::filesystem::directory_iterator(dirPath), std::filesystem::directory_iterator{});
        pictures.resize(numFiles, nullptr);
        auto entry = std::filesystem::directory_iterator(dirPath);
        #pragma omp parallel for        //Parallelize the loop using OpenMP
        for (int i = 0; i < numFiles; i++) {
            if (std::filesystem::is_regular_file(entry->status())) {
                std::string filename = imagePath + entry->path().filename().string();
                std::cout << "Reading file: " << filename << std::endl;
                Picture* currPic = new Picture(filename);
                pictures[i] = currPic;
            }
            entry++;
        }
    }

    return pictures;
}

/// <summary>
/// take a screen shot of the current window
/// </summary>
/// <param name="picture"> the picture that is currently drawing </param>
/// <param name="shader"> the shader we are using currently </param>
/// <param name="window"> the window of current context </param>
/// <returns>the name of its result</returns>
std::string screenShot(Picture* picture, GLuint shader , GLFWwindow* window) {
    #pragma omp critical
    {
        glClear(GL_COLOR_BUFFER_BIT);
        picture->draw(shader);
        glfwSwapBuffers(window);
        std::string resultImagePath = resultPath + picture->GetName().substr(imagePath.size(), picture->GetName().size() - imagePath.size() - 4) + "_result.png";
        picture->saveResizedImage(resultImagePath, window);

        return resultImagePath;
    }
}
/// <summary>
/// switch to the index image
/// </summary>
/// <param name="index">index of image in the array </param>
/// <param name="picVector"> the array that contains the image </param>
/// <param name="window"> the displaying window </param>
/// <returns>the picture pointer</returns>
Picture* switchDisplayPicture(int index, std::vector<Picture*> picVector, GLFWwindow* window) {
    Picture* currRes = picVector[index];
    resizeWindow(window, currRes->GetWidth() * 3, currRes->GetHeight() * 2);
    return currRes;
}
int main() {
        std::cout << "Loading Shaders" << std::endl;
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(512, 512, "Picture", NULL, NULL);
    if (!window) {
        glfwTerminate();
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
    results.resize(pictures.size(), nullptr);
    std::cout << "resizing images" << std::endl;
    #pragma omp parallel for  
    for (int i = 0; i < pictures.size(); i++) {
        Picture* pic = pictures[i];
        std::cout << pic->GetName() << std::endl;
        //resize the window to half the size
        resizeWindow(window, pic->GetWidth() / 2, pic->GetHeight() / 2);
        //take a screenshot
        std::string resultImagePath = screenShot(pic, shaderProgram, window);
        Picture* result = new Picture(resultImagePath);
        results[i] = result;//place back to the result array
    }
    int currResultIndex = 0;
    Picture* currResult = switchDisplayPicture(currResultIndex, results, window);
    Picture* currOrigin = pictures[currResultIndex];
    int leftPressed = false;
    int rightPressed = false;
    std::cout << "Showing images" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        //create buffer
        glfwMakeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT);

        //use left and right key to switch between pictures
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS&& !leftPressed) {
            // Handle left arrow key press here
            currResultIndex = std::max(0, --currResultIndex);
            std::cout << "Showing Index " << currResultIndex << " Image: " << currOrigin->GetName() << std::endl;

            currOrigin = pictures[currResultIndex];
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
            currOrigin = pictures[currResultIndex];
            std::cout << "Showing Index " << currResultIndex << " Image: " << currOrigin->GetName() << std::endl;
            currResult = switchDisplayPicture(currResultIndex, results, window);
            rightPressed = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
            rightPressed = false;

        }
        
        currResult->resultDraw(shaderProgram);
        currOrigin->originDraw(shaderProgram);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}