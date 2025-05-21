#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

namespace kwnc
{
class Shader
{
        public:

                Shader(const std::string &vertex_path, const std::string &fragment_path);
                ~Shader();

                unsigned int get_id();
        private:
                unsigned int id;
                std::string get_file_content(const std::string &path);
                void check_compile_errors(unsigned int shader);
                void check_linking_errors();
};

} // namespace kwnc

#endif // !SHADER_HPP
