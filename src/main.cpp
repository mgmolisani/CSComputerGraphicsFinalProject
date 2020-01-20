#include <iostream>
#include <vector>
#include "buffer.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "object.hpp"
#include "program.hpp"
#include "sdl_impl.h"
#include "vertex_array.hpp"

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  unsigned int width = 960;
  unsigned int height = 675;
  SDL_Window* window = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, width, height,
                                        SDL_WINDOW_OPENGL);

  if (window == NULL) {
    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (context == NULL) {
    std::cout << "Could not create OpenGL context: " << SDL_GetError()
              << std::endl;
  }

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "Could not initialize GLAD " << std::endl;
  }

  Program grass_shader_program(
      "shaders/grass/vertex.glsl", "shaders/grass/domain.glsl",
      "shaders/grass/geometry.glsl", "shaders/grass/fragment.glsl");

  Program default_shader_program(
      "shaders/default/vertex.glsl", "shaders/default/domain.glsl",
      "shaders/default/geometry.glsl", "shaders/default/fragment.glsl");

  Program* program = &default_shader_program;

  Object bunny("objects/bunny.obj");
  Object square;

  Object* obj = &square;

  bool done = false;
  GLenum polygon_mode = GL_FILL;
  GLuint density = 1;

  GLint max_density;
  glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &max_density);

  while (!done) {
    /* Handle Events */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          done = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
              polygon_mode = polygon_mode == GL_FILL ? GL_LINE : GL_FILL;
              break;
            case SDL_SCANCODE_DOWN:
              density = density > 1 ? density - 1 : density;
              break;
            case SDL_SCANCODE_UP:
              density = density < max_density ? density + 1 : density;
              break;
            case SDL_SCANCODE_1:
              program = program == &grass_shader_program
                            ? &default_shader_program
                            : &grass_shader_program;
              break;
            case SDL_SCANCODE_2:
              obj = obj == &bunny ? &square : &bunny;
              density = 1;
              break;
            default:
              break;
          }
        default:
          break;
      }
    }

    /* Update and render */
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glEnable(GL_MULTISAMPLE);

    program->Use();
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    GLfloat density_f = GLfloat(density);
    GLfloat patch_outer_level[] = {density_f, density_f, density_f};
    glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, patch_outer_level);
    GLfloat patch_inner_level[] = {density_f};
    glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, patch_inner_level);

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f), float(width) / float(height), 0.1f, 100.0f);
    program->SetUniformMatrix4f("projection", projection);

    glm::mat4 view =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(2.0f));

    GLfloat grass_scale = 0.1f;

    if (obj == &square) {
      model =
          glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model =
          glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
      glm::mat4 view =
          glm::lookAt(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));

      grass_scale = 1.0f;
    }

    program->SetUniformMatrix4f("model", model);
    program->SetUniformMatrix4f("view", view);
    program->SetUniform1f("grass_scale", grass_scale);

    GLfloat amplitude = 1.0;
    program->SetUniform1f("amplitude", amplitude);

    GLfloat frequency = 0.25;
    program->SetUniform1f("frequency", frequency);

    GLfloat phase = -GLfloat(SDL_GetTicks()) /
                    500.0f;  // Neagtive pushes the wind in the +x direction
    program->SetUniform1f("phase", phase);

    std::vector<GLfloat> vertices = obj->vertices();
    std::vector<GLuint> indices = obj->indices();

    VertexArray vertex_array;
    vertex_array.Bind();

    Buffer vertex_buffer;
    vertex_buffer.Bind(GL_ARRAY_BUFFER);
    Buffer::Data(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
                 vertices.data(), GL_STATIC_DRAW);

    Buffer element_buffer;
    element_buffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    Buffer::Data(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
                 indices.data(), GL_STATIC_DRAW);

    VertexArray::AttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                               6 * sizeof(vertices[0]), 0);
    VertexArray::AttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                               6 * sizeof(vertices[0]),
                               (void*)(3 * sizeof(vertices[0])));

    VertexArray::EnableAttribArray(0);
    VertexArray::EnableAttribArray(1);

    // grass_shader_program.Validate();

    glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}