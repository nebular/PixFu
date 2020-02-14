
//
//  OpenGl.cpp
//  LoneKart
//
//  Created by rodo on 20/01/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "OpenGlUtils.h"
#include "PixFu.hpp"
#include "Utils.hpp"
#include <fstream>
#include <iostream>

// Convenience methods

std::string OpenGlUtils::TAG = "OpenGLUtils";
std::string OpenGlUtils::VERSION = "v330core";

std::string OpenGlUtils::loadShaderFile(const std::string &sFile)
{
	std::string sFilename = rgl::PixFuPlatform::getPath("/opengl/"+VERSION+"/"+sFile);

	std::ifstream file(sFilename, std::ios::in | std::ios::binary);
	std::ifstream file2(sFilename, std::ios::in | std::ios::binary);
	
	if (!file.is_open()) return "";
	
	std::string result;
	
	while(!file.eof()) // To get you all the lines.
	{
		std::string STRING;
		getline(file,STRING); // Saves the line in STRING.
		result += STRING+"\n";
	}
	return result;
}

unsigned int OpenGlUtils::compileShader(unsigned int type, const std::string &source)
{
	unsigned int shader = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	
	int result;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca( length * sizeof(char));
		glGetShaderInfoLog( shader, length, &length, message);

		rgl::LogE(TAG, rgl::SF("Shader Failed to Compile: %s", message));
		exit(1);
	}
	
	return shader;
}

unsigned int OpenGlUtils::loadShader(const std::string &vertexShader,
									 const std::string &fragementShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragementShader);
	
	glAttachShader( program, vs);
	glAttachShader( program, fs);
	glLinkProgram( program );
	glValidateProgram( program );
	
	glDetachShader( program, vs );
	glDetachShader( program, fs );
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	glError("loadshader");
	return program;
}

unsigned int OpenGlUtils::loadShader(const std::string &filename) {
	std::string vertex = loadShaderFile(filename + ".vertex.glsl");
	std::string shader = loadShaderFile(filename + ".fragment.glsl");
	if (vertex.empty()) throw std::runtime_error("Shader files not found or error");
	return loadShader(vertex, shader);
}


void OpenGlUtils::glError(const std::string &tag) {
	
	GLenum err = 0;
	
	while( (err = glGetError()) ) {
		rgl::LogV(TAG, rgl::SF("OpenGL %s, error %s", tag.c_str(), err));
	}
}

GLuint OpenGlUtils::getGlTexture(uint glChannel) { return GL_TEXTURE0 + glChannel - 1; }

GLuint OpenGlUtils::loadTexture(rgl::Drawable *img, GLuint texId, bool repeat) {
	
	if (img==nullptr) return NO_TEXTURE;

	if (texId == NO_TEXTURE) {
		
		glGenTextures(1, &texId);				// generates texture ID

		GLuint texUnit = getGlTexture(texId);
		glActiveTexture(texUnit);
		glBindTexture(GL_TEXTURE_2D, texId);	// any subsequent texture commands will configure the currently bound texture:
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->getData());
		glError("loadtexture " + std::to_string(texId));
	} else {
		GLuint texUnit = getGlTexture(texId);
		glActiveTexture(texUnit);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->width, img->height, GL_RGBA, GL_UNSIGNED_BYTE, img->getData());
		glError("update texture");
	}
	return texId;
}
