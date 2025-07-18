#include "shader.hpp"
#include "glad/gl.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace kwnc
{
Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
{
        std::string vertex_src_tmp = get_file_content(vertex_path);
        std::string fragment_src_tmp = get_file_content(fragment_path);

        const char *vertex_src = vertex_src_tmp.c_str();
        const char *vragment_src = fragment_src_tmp.c_str();

        unsigned int vertex_shader, fragment_shader;

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_src, nullptr);
        glCompileShader(vertex_shader);
        check_compile_errors(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &vragment_src, nullptr);
        glCompileShader(fragment_shader);
        check_compile_errors(fragment_shader);

        id = glCreateProgram();
        glAttachShader(id, vertex_shader);
        glAttachShader(id, fragment_shader);
        glLinkProgram(id);
        check_linking_errors();

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
}

Shader::~Shader() { glDeleteProgram(id); }

unsigned int Shader::get_id() { return id; }

void Shader::use() { glUseProgram(id); }

void Shader::uniform_m4(const std::string &name, float *mat) const
{
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                           mat);
}

void Shader::uniform_v3(const std::string &name, float *vec) const
{
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, vec);
}

void Shader::uniform_i(const std::string &name, int i) const
{
        glUniform1i(glGetUniformLocation(id, name.c_str()), i);
}

std::string Shader::get_file_content(const std::string &path)
{
        std::string content;
        std::ifstream file;
        std::stringstream stream;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
                file.open(path);
                stream << file.rdbuf();
                file.close();
                content = stream.str();
        } catch (const std::exception &e) {
                std::cerr << "Error reading shader file:" << e.what()
                          << std::endl;
        }

        return content;
}

void Shader::check_compile_errors(unsigned int shader)
{
        int success;
        char infoLog[1024];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);

                std::cerr << "Error compiling shader: " << infoLog << std::endl;
        }
}

void Shader::check_linking_errors()
{
        int success;
        char infoLog[1024];

        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if (!success) {
                glGetProgramInfoLog(id, 1024, NULL, infoLog);

                std::cerr << "Error linking shader: " << infoLog << std::endl;
        }
}

} // namespace kwnc
