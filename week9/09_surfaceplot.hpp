#ifndef _SURFACEPLOT09_HPP_
#define _SURFACEPLOT09_HPP_

#define ESCAPE 27

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "DSL.hpp"
#include "Parser.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Object rotation (same as week 6)
extern GLfloat xrot, yrot, zrot;
// Camera position/rotation (same as week 6)
extern GLfloat c_xpos, c_ypos, c_zpos;
extern GLfloat c_up_x, c_up_y, c_up_z;
extern GLfloat c_xrot, c_yrot, c_zrot;

// New in week 9: one mode cycled with the M key, instead of a separate
// wireframe bool.
enum class RenderMode { Phong, Flat, Wireframe };
extern RenderMode renderMode;

// New in week 9: animation state.
extern bool  playing;   // Space toggles
extern float currentT;  // R resets to 0
extern float speed;     // ] / [ adjust

#endif
