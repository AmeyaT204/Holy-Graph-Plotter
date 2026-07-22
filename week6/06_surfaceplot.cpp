/*
  CSX75 Tutorial 6 - Surface Plotter
  z = sin(x)*cos(y), colored by height, smooth per-vertex normals.

  Arrow keys / PgUp / PgDn : rotate surface
  W,A,S,D,Q,E               : move camera
  P                         : toggle wireframe
*/

#include "06_surfaceplot.hpp"

double PI=3.14159265;
GLuint shaderProgram;
GLuint vbo[1], vao[1];

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;
glm::mat4 model_matrix;
glm::mat4 view_matrix;
glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;
GLuint minHeightLoc, maxHeightLoc;

//-----------------------------------------------------------------
const int RESOLUTION = 100;                 // grid subdivisions per axis
const int num_vertices = RESOLUTION * RESOLUTION * 6; // 2 tris * 3 verts per quad

int v_idx = 0;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];   // unused by shader now (color computed from height) but kept for layout parity
glm::vec4 v_normals[num_vertices];

float minHeight = -1.0f, maxHeight = 1.0f;

//-----------------------------------------------------------------
float surfaceZ(float x, float y)
{
  return sin(x) * cos(y);
}

glm::vec3 surfaceNormal(float x, float y)
{
  float dzdx = cos(x) * cos(y);
  float dzdy = -sin(x) * sin(y);
  glm::vec3 tangentX(1.0f, 0.0f, dzdx);
  glm::vec3 tangentY(0.0f, 1.0f, dzdy);
  return glm::normalize(glm::cross(tangentX, tangentY));
}

void addVertex(float x, float y)
{
  float z = surfaceZ(x, y);
  glm::vec3 n = surfaceNormal(x, y);

  v_positions[v_idx] = glm::vec4(x, z, y, 1.0f); // y-up world: height goes in Y
  v_normals[v_idx]   = glm::vec4(n, 0.0f);
  v_colors[v_idx]    = glm::vec4(1.0); // placeholder, shader colors by height
  v_idx++;
}

void buildSurface(float range)
{
  float step = (2.0f * range) / RESOLUTION;

  for (int j = 0; j < RESOLUTION; ++j)
  {
    for (int i = 0; i < RESOLUTION; ++i)
    {
      float x0 = -range + i * step;
      float x1 = x0 + step;
      float y0 = -range + j * step;
      float y1 = y0 + step;

      // Triangle 1
      addVertex(x0, y0);
      addVertex(x0, y1);
      addVertex(x1, y0);

      // Triangle 2
      addVertex(x1, y0);
      addVertex(x0, y1);
      addVertex(x1, y1);
    }
  }
}

//-----------------------------------------------------------------
void initBuffersGL(void)
{
  std::string vertex_shader_file("06_vshader.glsl");
  std::string fragment_shader_file("06_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);

  GLuint vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  GLuint vNormal   = glGetAttribLocation(shaderProgram, "vNormal");
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  normalMatrix     = glGetUniformLocation(shaderProgram, "normalMatrix");
  viewMatrix       = glGetUniformLocation(shaderProgram, "viewMatrix");
  minHeightLoc     = glGetUniformLocation(shaderProgram, "minHeight");
  maxHeightLoc     = glGetUniformLocation(shaderProgram, "maxHeight");

  buildSurface(2.0f * (float)PI);

  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions) + sizeof(v_normals), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_normals), v_normals);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)));
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot), glm::vec3(0.0f,0.0f,1.0f));
  model_matrix = rotation_matrix;

  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up  = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

  projection_matrix = glm::perspective(glm::radians(60.0), 1.0, 0.1, 100.0);
  view_matrix = projection_matrix * lookat_matrix;

  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniform1f(minHeightLoc, minHeight);
  glUniform1f(maxHeightLoc, maxHeight);

  modelview_matrix = view_matrix * model_matrix;
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

  glBindVertexArray(vao[0]);
  if (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

int main(int argc, char** argv)
{
  GLFWwindow* window;
  glfwSetErrorCallback(csX75::error_callback);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(512, 512, "CS475 Tutorial 6: Surface Plotter", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    std::cerr << "GLEW Init Failed" << std::endl;

  glfwSetKeyCallback(window, csX75::key_callback);
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  csX75::initGL();
  initBuffersGL();

  while (glfwWindowShouldClose(window) == 0)
  {
    renderGL();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}