#include "shader.hpp"

#include <filesystem>

std::string getFileContent(const char* filename)
{
    if (std::ifstream in(filename, std::ios::binary); in) {
        std::string content;
        in.seekg(0, std::ios::end);
        content.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&content[0], content.size());
        in.close();
        return content;
    }
    std::error_code code = std::make_error_code(std::errc::no_such_file_or_directory);
    throw std::filesystem::filesystem_error("The file is not found.", filename, code);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = getFileContent(vertexFile);
    std::string fragmentCode = getFileContent(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate() const
{
    glUseProgram(id);
}

void Shader::deleteShader() const
{
    glDeleteProgram(id);
}
