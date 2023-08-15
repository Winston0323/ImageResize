#include "picture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
/// <summary>
/// constructor of Picture
/// </summary>
/// <param name="inputFile"> the image name </param>
Picture::Picture(std::string inputFile)
{
	imageName = inputFile;
	float vertices[] = {
		// positions         // texture coords
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
	0, 1, 3,  // first triangle
	1, 2, 3   // second triangle
	};

	// loading image
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(imageName.c_str(), &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "Cannot read images:"<< inputFile << std::endl;
	}

	//check the format of the image
	if (nrChannels == 3) {
		RGBA = false;
	}
	else if (nrChannels == 4) {
		RGBA = true;
	}
	else {
		std::cout << "weird channel number " << nrChannels << std::endl;
	}
	#pragma omp critical //cannot multithreading when reading texture and binding buffers
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//using GL_Linear filter averging nears four pixel 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (RGBA) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		stbi_image_free(data);  // Free memory
	}
}

/// <summary>
/// the destructor of picture 
/// </summary>
Picture::~Picture()
{
	// Delete the VBOs and the VAO.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
}
/// <summary>
/// drawing function of the picture, should be called each frame
/// </summary>
/// <param name="shader"> the shader we are using </param>
void Picture::draw(GLuint shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Here you'd typically set up and draw your quad to which the texture is applied
	// This example assumes you have some method for drawing a quad
	glUseProgram(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}
/// <summary>
/// save the resized image into png
/// </summary>
/// <param name="filepath">the desired storing location </param>
/// <param name="w"> the buffer window </param>
void Picture::saveResizedImage(std::string filepath, GLFWwindow* w) {

		int width, height;
		glfwGetFramebufferSize(w, &width, &height);
		GLsizei stride = nrChannels * width;
		stride += (stride % 4) ? (4 - stride % 4) : 0;
		GLsizei bufferSize = stride * height;
		std::vector<char> buffer(bufferSize);
		#pragma omp critical
		{
		glPixelStorei(GL_PACK_ALIGNMENT, 4);
		glReadBuffer(GL_FRONT);
		if (RGBA) {
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
		}
		else {
			glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
		}
		
		stbi_flip_vertically_on_write(true);
		stbi_write_png(filepath.c_str(), width, height, nrChannels, buffer.data(), stride);
	}
}


