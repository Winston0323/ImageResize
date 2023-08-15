#pragma once
#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "core.h"
#include <GLFW/glfw3.h>



////////////////////////////////////////////////////////////////////////////////

class Picture
{
private:
	
	//filename
	bool RGBA;
	std::string imageName;
	int width, height, nrChannels;
	GLuint texture;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	//rendering buffers
	GLuint VAO,VBO, EBO;

public:

	Picture(std::string inputFile);
	~Picture();

	void draw(GLuint shader);
	int GetWidth() { return this->width; }
	int GetHeight() { return this->height; }
	std::string GetName() { return this->imageName; }
	void saveResizedImage(std::string filepath, GLFWwindow* w);
};

////////////////////////////////////////////////////////////////////////////////

#endif
