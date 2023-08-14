#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Picture.h"

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

int main() {
    if (!glfwInit()) {
        return -1;
    }
    //// Load image
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //unsigned char* data = stbi_load("test.png", &width, &height, &nrChannels, 0);
    //if (!data) {
    //    return -1;
    //}

    GLFWwindow* window = glfwCreateWindow(512, 512, "Picture", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    //create shader 
    GLuint shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    //create resize shader 
    GLuint resizeProgram = LoadShaders("shaders/resizeShader.vert", "shaders/resizeShader.frag");
    std::vector<Picture*> pictures = genPicForAllFiles(imagePath);
    std::vector<Picture*> results;

    for (Picture* pic : pictures) {
        std::cout << pic->GetName() << std::endl;
        std::string resultImagePath = screenShot(pic, shaderProgram, window);
        Picture* result = new Picture(resultImagePath);
        results.emplace_back(result);
    }
    Picture* currRes = results[0];
    resizeWindow(window, currRes->GetWidth() / 2, currRes->GetHeight() / 2);
    while (!glfwWindowShouldClose(window)) {
        //create buffer
        glClear(GL_COLOR_BUFFER_BIT);

        currRes->draw(shaderProgram);

        glfwSwapBuffers(window);
        //pic->saveResizedImage(imagePath+"result.png", window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}