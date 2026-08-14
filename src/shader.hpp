#pragma once

#include <cerrno>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

std::string getFileContent(const char* filename);

class Shader {
  public:
    GLuint id;
    Shader(const char* vertexFile, const char* fragmentFile);

    void activate() const;
    void deleteShader() const;

  private:
    // Checks if the shaders have compiled properly
    void compileErrors(unsigned int shader, const char* type) const;
};
