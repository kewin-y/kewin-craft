#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "glad/gl.h"
#include <string>

namespace kwnc
{
class Texture
{
public:
        Texture(const std::string &path);
        ~Texture();

        uint get_id();
        void bind();

private:
        unsigned int id;
        int width;
        int height;
        int channels;

        static GLenum get_texture_format(int channels);
};
} // namespace kwnc

#endif // !TEXTURE_HPP
