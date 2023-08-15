# Unity Based Data Generator

# 1.Description
This is a tool for resize an NxN image (N is power of 2 for simplicity) and returns an image that is N/2 x N/2. The program use `glTexParameteri` as the algorithm of resizing since `GL_LINEAR` will use the four nearest pixel to produce the result, which is the same as required. I also implement a resize shader. Using such shader will also perform the required task using GLSL, which is also paralleling the code. I think both way will create fast result, rather than using CPU to compute. 

# 2. How to use
1. Place all your NxN images inside `Image` folder
2. Start the program
3. All resized images will be present inside `Result` folder, result image name will be `originalname_result.png`
4. Use left and right arrow key to switch between result images, the showing picture's name will be printed in the terminal
# 3. Build Instruction
[Please refer to UC SanDiego CSE167 starter code build instruction](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20)
# 4. Examples:
## Input image:
<p align="center">
  <img src="Image/copy_0.png" alt="Logo" width="512" height="512">
</p>

## output image:
<p align="center">
  <img src="Result/copy_0_result.png" alt="Logo" width="256" height="256">
</p>

# 5. Reference
### C++
- [How to get file number in a directory(std::filesystem)](https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard)
- [omp](https://www.openmp.org/wp-content/uploads/OpenMP3.1-CCard.pdf)
## OpenGL
- [GL_LINEAR](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml)
- [Texture Mapping](https://learnopengl.com/Getting-started/Textures)
## Shader.cpp / Shader.h 
 - [UC SanDiego CSE167 starter code](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20)
