#ifndef _SURFACEPLOT_HPP_
#define _SURFACEPLOT_HPP_

#define ESCAPE 27
#define DELETE 127

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Rotation Parameters (object)
GLfloat xrot=0.0, yrot=0.0, zrot=0.0;
// Camera position/rotation
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 6.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0, c_yrot=0.0, c_zrot=0.0;

bool wireframe=false;

#endif