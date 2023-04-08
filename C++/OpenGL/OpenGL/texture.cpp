//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#include <GL/glew.h>
//
//#include <GLFW/glfw3.h>
//
//
//GLuint loadBMP_custom(const char * imagepath){
//
//	printf("Reading image %s\n", imagepath);
//
//	// Data read from the header of the BMP file
//	unsigned char header[54];
//	unsigned int dataPos;
//	unsigned int imageSize;
//	unsigned int width, height;
//	// Actual RGB data
//	unsigned char * data;
//
//	// Open the file
//	FILE * file = fopen(imagepath,"rb");
//	if (!file){
//		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
//		getchar();
//		return 0;
//	}
//
//	// Read the header, i.e. the 54 first bytes
//
//	// If less than 54 bytes are read, problem
//	if ( fread(header, 1, 54, file)!=54 ){
//		printf("Not a correct BMP file\n");
//		fclose(file);
//		return 0;
//	}
//	// A BMP files always begins with "BM"
//	if ( header[0]!='B' || header[1]!='M' ){
//		printf("Not a correct BMP file\n");
//		fclose(file);
//		return 0;
//	}
//	// Make sure this is a 24bpp file
//	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
//	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
//
//	// Read the information about the image
//	dataPos    = *(int*)&(header[0x0A]);
//	imageSize  = *(int*)&(header[0x22]);
//	width      = *(int*)&(header[0x12]);
//	height     = *(int*)&(header[0x16]);
//
//	// Some BMP files are misformatted, guess missing information
//	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
//	if (dataPos==0)      dataPos=54; // The BMP header is done that way
//
//	// Create a buffer
//	data = new unsigned char [imageSize];
//
//	// Read the actual data from the file into the buffer
//	fread(data,1,imageSize,file);
//
//	// Everything is in memory now, the file can be closed.
//	fclose (file);
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Give the image to OpenGL
//	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
//
//	// OpenGL has now copied the data. Free our own version
//	delete [] data;
//
//	// Poor filtering, or ...
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	// ... nice trilinear filtering ...
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	// ... which requires mipmaps. Generate them automatically.
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}
//
//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library,
//// or do it yourself (just like loadBMP_custom and loadDDS)
////GLuint loadTGA_glfw(const char * imagepath){
////
////	// Create one OpenGL texture
////	GLuint textureID;
////	glGenTextures(1, &textureID);
////
////	// "Bind" the newly created texture : all future texture functions will modify this texture
////	glBindTexture(GL_TEXTURE_2D, textureID);
////
////	// Read the file, call glTexImage2D with the right parameters
////	glfwLoadTexture2D(imagepath, 0);
////
////	// Nice trilinear filtering.
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
////	glGenerateMipmap(GL_TEXTURE_2D);
////
////	// Return the ID of the texture we just created
////	return textureID;
////}
//
//
//
//#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
//#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
//#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
//
//GLuint loadDDS(const char * imagepath){
//
//	unsigned char header[124];
//
//	FILE *fp;
//
//	/* try to open the file */
//	fp = fopen(imagepath, "rb");
//	if (fp == NULL){
//		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
//		return 0;
//	}
//
//	/* verify the type of file */
//	char filecode[4];
//	fread(filecode, 1, 4, fp);
//	if (strncmp(filecode, "DDS ", 4) != 0) {
//		fclose(fp);
//		return 0;
//	}
//
//	/* get the surface desc */
//	fread(&header, 124, 1, fp);
//
//	unsigned int height      = *(unsigned int*)&(header[8 ]);
//	unsigned int width	     = *(unsigned int*)&(header[12]);
//	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
//	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
//	unsigned int fourCC      = *(unsigned int*)&(header[80]);
//
//
//	unsigned char * buffer;
//	unsigned int bufsize;
//	/* how big is it going to be including all mipmaps? */
//	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
//	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
//	fread(buffer, 1, bufsize, fp);
//	/* close the file pointer */
//	fclose(fp);
//
//	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
//	unsigned int format;
//	switch(fourCC)
//	{
//	case FOURCC_DXT1:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//		break;
//	case FOURCC_DXT3:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//		break;
//	case FOURCC_DXT5:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//		break;
//	default:
//		free(buffer);
//		return 0;
//	}
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//
//	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//	unsigned int offset = 0;
//
//	/* load the mipmaps */
//	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
//	{
//		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
//		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
//			0, size, buffer + offset);
//
//		offset += size;
//		width  /= 2;
//		height /= 2;
//
//		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
//		if(width < 1) width = 1;
//		if(height < 1) height = 1;
//
//	}
//
//	free(buffer);
//
//	return textureID;
//
//
//}

#include <iostream>
#include <fstream>
#include "texture.hpp"

#include <GL/glew.h>

#define FOURCC_DXT1 0x31545844 // ASCII to DXT1
#define FOURCC_DXT3 0x33545844 // ASCII to DXT3
#define FOURCC_DXT5 0x35545844 // ASCII to DXT5

const char* readFilee(const char* path) {
	std::ifstream file(path);
	char* content;
	if(!file.is_open()) {
		std::cout << "error " << path << "\n\n";
		return nullptr;
	}

	file.seekg(0, std::ios::end); // go to the end
	long length = file.tellg();    // get location (which is the length)
	file.seekg(0, std::ios::beg); // go back to the beginning

	content = new char[length + 1];
	file.read(content, length);

	content[length] = '\0';

	file.close();

	return content;
}

GLuint loadBMPimage(const char* filePath, bool* success) {
    *success = false;

    std::ifstream file(filePath);
    if(!file.is_open()) {
        std::cerr << "Could not open file " << filePath << ".\n";
        return 0;
    }

    char* header = new char[54];
    file.read(header, 54);

    if(header[0] != 'B' || header[1] != 'M') {
        std::cerr << "File " << filePath << " Doesn't seem to be a proper BMP file.\n";
        file.close();
        return 0;
    }

    unsigned int dataPos = *((unsigned int*) &header[0x0a]);
    unsigned int imageSize = *((unsigned int*) &header[0x22]);
    unsigned int width = *((unsigned int*) &header[0x12]);
    unsigned int height = *((unsigned int*) &header[0x16]);

    if(dataPos == 0) {
        dataPos = 54;
    }
    if(imageSize == 0) {
        imageSize = width * height * 3;
    }

    char* data = new char[imageSize];
    file.read(data, imageSize);
    file.close();


    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // when MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // when MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);

    *success = true;
    return textureID;
}

GLuint loadDDSimage(const char* filePath, bool* success) {
    *success = false;

    std::ifstream file(filePath);
    if(!file.is_open()) {
		char* path = strdup(__FILE__);
		path[strlen(path) - 11] = '\0';
		std::string tmp = std::string(path);
		tmp += filePath;
		file.open(tmp);
		if(!file.is_open()) {
			std::cerr << "Could not open file " << filePath << ".\n";
			return 0;
		}
    }

    char* fileCode = new char[4];
    file.read(fileCode, 4);
    if(strncmp(fileCode, "DDS ", 4) != 0) {
        std::cerr << "File " << filePath << " doesn't seem to be a proper DDS file.\n";
        file.close();
        return 0;
    }

    char* header = new char[124];
    file.read(header, 124);

    unsigned int height = *((unsigned int*) &header[0x08]);
    unsigned int width = *((unsigned int*) &header[0x0c]);
    unsigned int linearSize = *((unsigned int*) &header[0x10]);
    unsigned int mipMapCount = *((unsigned int*) &header[0x18]);
    unsigned int fourCC = *((unsigned int*) &header[0x50]);


    unsigned int bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    char *buffer = new char[bufferSize];
    file.read(buffer, bufferSize);
    file.close();


    unsigned int format;
    switch(fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            delete[] buffer;
            return 0;
    }


    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    for(unsigned int level = 0;level < mipMapCount && (width || height);level++) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }


    delete[] buffer;

    *success = true;
    return textureID;
}


GLuint createTexture(unsigned int fileType, const char* filePath, bool* success) {
//	std::cout << readFilee(filePath);
    switch(fileType) {
        case BMP:
            return loadBMPimage(filePath, success);
        case DDS:
            return loadDDSimage(filePath, success);
        default:
            std::cerr << "Unknown image file tipe \"" << fileType << "\".\n";
            return 0;
            break;
    }
}
