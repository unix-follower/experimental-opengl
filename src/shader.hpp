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
};
