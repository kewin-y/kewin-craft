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

        unsigned int get_id() const;
        void bind() const;

private:
        unsigned int id;
        int width;
        int height;
        int channels;
};
} // namespace kwnc

#endif // !TEXTURE_HPP
