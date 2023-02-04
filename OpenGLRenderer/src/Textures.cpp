// Texture functionality - Julian Haurin - 2/4/23

#include "Textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // image loading library

// loads texture data into provided texture ID(?)
int loadTexture(unsigned int& in_texture, const char* in_filePath) {

    // texture1
    glGenTextures(1, &in_texture);
    glBindTexture(GL_TEXTURE_2D, in_texture);

    // setting texture wrapping parameters, uses s, t, r coordinate system
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 2D textures will repeat mirrored

    // determines point (nearest-neighbor) filtering and bilinear filtering for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when scaling down, OpenGL will use bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when scaling up, OpenGL will use bilinear filtering

    // mipmap configuring
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image from disk using stbi
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* textureData = stbi_load(in_filePath, &width, &height, &nrChannels, 0);

    // checks that the image loaded successfully
    if (!textureData) {
        std::cout << "Error: texture1 image failed to load" << std::endl;
        return -1;

    }
    else {

        std::cout << "Loaded texture data successfully" << std::endl;
        // load data from stbi into OpenGL
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixel transfer configuring
        // Allocates mutable storage for a mipmap level of the bound texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData); // pixel unpack operation
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    // frees image memory
    stbi_image_free(textureData);

    return 0; // on success

}