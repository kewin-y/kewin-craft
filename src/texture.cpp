#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static GLenum get_texture_format(int num_channels)
{
        switch (num_channels) {
        case 1:
                return GL_RED;
        case 2:
                return GL_RG;
        case 3:
                return GL_RGB;
        case 4:
                return GL_RGBA;
        default:
                return 0;
        }
}

namespace kwnc
{
Texture::Texture(const std::string &path)
{
        stbi_uc *data;
        GLenum format;

        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        format = get_texture_format(channels);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() { glDeleteTextures(1, &id); }

unsigned int Texture::get_id() { return id; }

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, id); }
} // namespace kwnc
