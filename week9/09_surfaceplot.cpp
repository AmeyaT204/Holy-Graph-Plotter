/*
  CSX75 Tutorial 9 - Time-Varying Surface Plotter
  z = f(x, y, t), where f is loaded at startup from equation_t.txt
  and can reference t (e.g. "sin(x+t)*cos(y)").

  The mesh is fully rebuilt on the CPU every frame by re-evaluating the
  expression tree over the grid, then re-uploaded to the GPU. Normals
  are estimated per grid vertex via finite differences on the sampled
  heights (no closed-form derivative is available for an arbitrary
  parsed expression).

  Arrow keys / PgUp / PgDn : rotate surface
  W,A,S,D,Q,E               : move camera
  Space                     : play / pause animation
  ] / [                     : speed up / slow down
  R                         : reset t to 0
  M                         : cycle render mode (Phong -> Flat -> Wireframe)
*/

#include "09_surfaceplot.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

double PI = 3.14159265;
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
GLuint uShadeModeLoc;

// ---- object rotation / camera (definitions; declared extern in the .hpp) ----
GLfloat xrot = 0.0f, yrot = 0.0f, zrot = 0.0f;
GLfloat c_xpos = 0.0f, c_ypos = 0.0f, c_zpos = 6.0f;
GLfloat c_up_x = 0.0f, c_up_y = 1.0f, c_up_z = 0.0f;
GLfloat c_xrot = 0.0f, c_yrot = 0.0f, c_zrot = 0.0f;

// ---- week 9 state ----
RenderMode renderMode = RenderMode::Phong;
bool  playing  = true;
float currentT = 0.0f;
float speed    = 1.0f;

ExprPtr surfaceExpr;

//-----------------------------------------------------------------
// Lower resolution than week 6's 100, since this grid is re-evaluated
// through the expression tree every single frame rather than being
// built once. 60 is comfortably fast and still looks smooth.
const int RESOLUTION = 60;
const int GRID = RESOLUTION + 1;
const int num_vertices = RESOLUTION * RESOLUTION * 6; // 2 tris * 3 verts per quad

glm::vec4 v_positions[num_vertices];
glm::vec4 v_normals[num_vertices];

static float     gridZ[GRID][GRID];
static glm::vec3 gridN[GRID][GRID];

float minHeight = -1.0f, maxHeight = 1.0f;

//-----------------------------------------------------------------
// Sample the expression over the grid, then estimate per-vertex
// normals from the sampled heights via central (or one-sided, at the
// boundary) finite differences.
void computeGrid(float range, float t)
{
  float step = (2.0f * range) / RESOLUTION;

  for (int j = 0; j < GRID; ++j)
    for (int i = 0; i < GRID; ++i)
    {
      float x = -range + i * step;
      float y = -range + j * step;
      gridZ[i][j] = static_cast<float>(surfaceExpr->evaluate(x, y, t));
    }

  for (int j = 0; j < GRID; ++j)
    for (int i = 0; i < GRID; ++i)
    {
      float dzdx, dzdy;

      if (i > 0 && i < GRID - 1)
        dzdx = (gridZ[i + 1][j] - gridZ[i - 1][j]) / (2.0f * step);
      else if (i == 0)
        dzdx = (gridZ[i + 1][j] - gridZ[i][j]) / step;
      else
        dzdx = (gridZ[i][j] - gridZ[i - 1][j]) / step;

      if (j > 0 && j < GRID - 1)
        dzdy = (gridZ[i][j + 1] - gridZ[i][j - 1]) / (2.0f * step);
      else if (j == 0)
        dzdy = (gridZ[i][j + 1] - gridZ[i][j]) / step;
      else
        dzdy = (gridZ[i][j] - gridZ[i][j - 1]) / step;

      glm::vec3 tangentX(1.0f, 0.0f, dzdx);
      glm::vec3 tangentY(0.0f, 1.0f, dzdy);
      gridN[i][j] = glm::normalize(glm::cross(tangentX, tangentY));
    }
}

void buildSurface(float range, float t)
{
  computeGrid(range, t);
  float step = (2.0f * range) / RESOLUTION;
  int idx = 0;

  auto push = [&](int i, int j)
  {
    float x = -range + i * step;
    float y = -range + j * step;
    v_positions[idx] = glm::vec4(x, gridZ[i][j], y, 1.0f); // y-up: height in Y
    v_normals[idx]   = glm::vec4(gridN[i][j], 0.0f);
    idx++;
  };

  for (int j = 0; j < RESOLUTION; ++j)
  {
    for (int i = 0; i < RESOLUTION; ++i)
    {
      // Triangle 1
      push(i,     j);
      push(i,     j + 1);
      push(i + 1, j);

      // Triangle 2
      push(i + 1, j);
      push(i,     j + 1);
      push(i + 1, j + 1);
    }
  }
}

//-----------------------------------------------------------------
void loadEquation(void)
{
  std::ifstream file("equation_t.txt");
  if (!file.is_open())
  {
    std::cerr << "Could not open equation_t.txt" << std::endl;
    std::exit(1);
  }
  std::string line;
  std::getline(file, line);
  file.close();

  try
  {
    surfaceExpr = parseEquation(line);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Parse error in equation_t.txt: " << e.what() << std::endl;
    std::exit(1);
  }

  std::cout << "Loaded surface: " << surfaceExpr->toString() << std::endl;
}

//-----------------------------------------------------------------
void initBuffersGL(void)
{
  std::string vertex_shader_file("09_vshader.glsl");
  std::string fragment_shader_file("09_fshader.glsl");

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
  uShadeModeLoc    = glGetUniformLocation(shaderProgram, "uShadeMode");

  loadEquation();
  buildSurface(2.0f * (float)PI, currentT);

  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  // GL_DYNAMIC_DRAW instead of week 6's GL_STATIC_DRAW: this buffer's
  // contents change every frame now.
  glBufferData(GL_ARRAY_BUFFER, sizeof(v_positions) + sizeof(v_normals), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_normals), v_normals);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)));
}

void renderGL(float dt)
{
  if (playing)
    currentT += speed * dt;

  buildSurface(2.0f * (float)PI, currentT);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_normals), v_normals);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot), glm::vec3(1.0f, 0.0f, 0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot), glm::vec3(0.0f, 0.0f, 1.0f));
  model_matrix = rotation_matrix;

  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f, 0.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f, 1.0f, 0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * c_rotation_matrix;
  glm::vec4 c_up  = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * c_rotation_matrix;
  lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

  projection_matrix = glm::perspective(glm::radians(60.0), 1.0, 0.1, 100.0);
  view_matrix = projection_matrix * lookat_matrix;

  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniform1f(minHeightLoc, minHeight);
  glUniform1f(maxHeightLoc, maxHeight);
  glUniform1i(uShadeModeLoc, renderMode == RenderMode::Flat ? 1 : 0);

  modelview_matrix = view_matrix * model_matrix;
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));

  glBindVertexArray(vao[0]);
  glPolygonMode(GL_FRONT_AND_BACK, renderMode == RenderMode::Wireframe ? GL_LINE : GL_FILL);

  glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

//-----------------------------------------------------------------
void key_callback_09(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  else if (key == GLFW_KEY_LEFT)
    yrot -= 1.0f;
  else if (key == GLFW_KEY_RIGHT)
    yrot += 1.0f;
  else if (key == GLFW_KEY_UP)
    xrot -= 1.0f;
  else if (key == GLFW_KEY_DOWN)
    xrot += 1.0f;
  else if (key == GLFW_KEY_PAGE_UP)
    zrot -= 1.0f;
  else if (key == GLFW_KEY_PAGE_DOWN)
    zrot += 1.0f;

  else if (key == GLFW_KEY_W)
    c_xrot -= 1.0f;
  else if (key == GLFW_KEY_S)
    c_xrot += 1.0f;
  else if (key == GLFW_KEY_A)
    c_yrot -= 1.0f;
  else if (key == GLFW_KEY_D)
    c_yrot += 1.0f;
  else if (key == GLFW_KEY_Q)
    c_zrot -= 1.0f;
  else if (key == GLFW_KEY_E)
    c_zrot += 1.0f;

  else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    playing = !playing;

  else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
    speed += 0.25f;
  else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
    speed = std::max(0.0f, speed - 0.25f);

  else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    currentT = 0.0f;

  else if (key == GLFW_KEY_M && action == GLFW_PRESS)
  {
    switch (renderMode)
    {
      case RenderMode::Phong:     renderMode = RenderMode::Flat;      break;
      case RenderMode::Flat:      renderMode = RenderMode::Wireframe; break;
      case RenderMode::Wireframe: renderMode = RenderMode::Phong;     break;
    }
  }
}

//-----------------------------------------------------------------
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

  window = glfwCreateWindow(512, 512, "CS475 Tutorial 9: Time-Varying Surface Plotter", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    std::cerr << "GLEW Init Failed" << std::endl;

  glfwSetKeyCallback(window, key_callback_09);
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  csX75::initGL();
  initBuffersGL();

  double lastTime = glfwGetTime();

  while (glfwWindowShouldClose(window) == 0)
  {
    double now = glfwGetTime();
    float dt = static_cast<float>(now - lastTime);
    lastTime = now;

    renderGL(dt);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
