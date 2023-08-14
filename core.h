#ifndef _CORE_H_
#define _CORE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#ifndef GLM_ENABLE_EXPERIMENTAL
	#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <ctype.h>
#include <algorithm>
#include <Math.h>
#include <omp.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <direct.h>

#endif