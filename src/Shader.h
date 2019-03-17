#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath)
      : program(loadProgram(vertexPath, fragmentPath)) {}

  void use() { glUseProgram(program); }

  // set bool value for "name" attribute in vertex shader
  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
  }

  void setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y,
                value.z);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
  }

  void setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
  }

  const GLuint program;

private:
  bool readShaderSource(const char *file_path, std::vector<GLchar> &buffer) {
    if (file_path == NULL)
      return false;

    // open source file
    std::ifstream file(file_path, std::ios::binary);
    if (file.fail()) {
      std::cerr << "cannot open source file: " << file_path << '\n';
      return false;
    }

    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());

    buffer.resize(length + 1);

    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if (file.fail()) {
      std::cerr << "Error: cannot read source file: " << file_path << '\n';
      return false;
    }

    file.close();
    return true;
  }

  // print result of shader compile
  GLboolean printShaderInfoLog(GLuint shader, const char *str) {
    // get compile result
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
      std::cerr << "Compile Error in " << str << std::endl;

    // get length of log
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
      std::vector<GLchar> infoLog(bufSize);
      GLsizei length;
      glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
      std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
  }

  // print result of program object link
  GLboolean printProgramInfoLog(GLuint program) {
    // get result of link
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
      std::cerr << "Link Error." << std::endl;

    // get length of log
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
      std::vector<GLchar> infoLog(bufSize);
      GLsizei length;
      glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
      std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
  }

  // create vertex/fragment shader
  GLuint createProgram(const char *vsrc, const char *fsrc) {
    const GLuint program = glCreateProgram();

    if (vsrc != NULL) {
      const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
      glShaderSource(vobj, 1, &vsrc, NULL);
      glCompileShader(vobj);

      if (printShaderInfoLog(vobj, "vertex shader"))
        glAttachShader(program, vobj);
      glDeleteShader(vobj);
    }

    if (fsrc != NULL) {
      const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
      glShaderSource(fobj, 1, &fsrc, NULL);
      glCompileShader(fobj);
      if (printShaderInfoLog(fobj, "fragment shader"))
        glAttachShader(program, fobj);
      glDeleteShader(fobj);
    }

    // link program object
    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "color");
    glBindAttribLocation(program, 2, "texture");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    if (printProgramInfoLog(program))
      return program;

    glDeleteProgram(program);
    return 0;
  }

  GLuint loadProgram(const char *vert, const char *frag) {
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));
    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));

    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
  }
};

#endif
