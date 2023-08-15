# Unity Based Data Generator

# 1.Description
This is a tool for resize an NxN image (N is power of 2 for simplicity) and returns an image that is N/2 x N/2. The program use `glTexParameteri` as the algorithm of resizing since `GL_LINEAR` will use the four nearest pixel to produce the result, which is the same as required. I also implement a resize shader. Using such shader will also perform the required task using GLSL, which is also paralleling the code. I think both way will create fast result, rather than using CPU to compute. And for the parts that are not in shader, for example reading in images, I used omp to paraellel them and mark critical for the not thread safe parts. 

# 2. How to use
1. Create a `Image` folder and a `Result` folder
2. Place all your NxN images inside `Image` folder, the program will read RGB and RGBA format images
3. Start the program
4. All resized images will be present inside `Result` folder, result image name will be `originalname_result.png`
5. Use left and right arrow key to switch between images, on left side is the original image and the right corner shows the result image. The showing picture's name will be printed in the terminal
<p align="center">
  <img src="Samples/howtouse.png" alt="Logo" width="768" height="512">
</p>
# 3. Build Instruction
[Please refer to UC SanDiego CSE167 starter code build instruction](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20)
# 4. Examples:
## Input image:
<p align="center">
  <img src="Samples/copy_0.png" alt="Logo" width="512" height="512">
</p>

## Output image:
<p align="center">
  <img src="Samples/copy_0_result.png" alt="Logo" width="256" height="256">
</p>

# 5. Reference
### C++
- [How to get file number in a directory(std::filesystem)](https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard)
- [omp](https://www.openmp.org/wp-content/uploads/OpenMP3.1-CCard.pdf)
- [stb](https://github.com/nothings/stb/tree/master)
## OpenGL
- [How GL_LINEAR perform](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml)
- [Texture Mapping](https://learnopengl.com/Getting-started/Textures)
- [Save frame buffer into image](https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/)
## Shader.cpp / Shader.h 
 - [UC SanDiego CSE167 starter code](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20)

