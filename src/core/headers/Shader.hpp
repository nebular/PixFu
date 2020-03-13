//
//  Shader.hpp
//  PixFu
//
//  The shader class abstracts communication with the shader providing methods to set the
//  uniforms, and to start and stop the shader.
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "OpenGL.h"
#include "OpenGlUtils.h"
#include "Texture2D.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>

namespace Pix {

	class Shader {

		GLuint ID;
		
	protected:
		GLuint getLocator(const std::string &name);
		
	public:

		Shader(const std::string &name);

		// activates the shader
		void use();

		void stop();

		void cleanup();

		void textureUnit(std::string sampler2d, Texture2D *texture);

		// utility uniform functions

		void setBool (const std::string &name, bool value) const;
		void setInt  (const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec2 (const std::string &name, float x, float y) const;
		void setVec3 (const std::string &name, float x, float y, float z) const;
		void setVec4 (const std::string &name, float x, float y, float z, float w) const;
		void setVec4 (const std::string &name, glm::vec4 v) const;
		void setVec3 (const std::string &name, glm::vec3 v) const;
		void setMat2 (const std::string &name, const float *mat2) const;
		void setMat3 (const std::string &name, const float *mat3) const;
		void setMat4 (const std::string &name, const float *mat4) const;

		// optimized functions
		void setBool (GLint locator, bool value) const;
		void setInt  (GLint locator, int value) const;
		void setFloat(GLint locator, float value) const;
		void setVec2 (GLint locator, float x, float y) const;
		void setVec3 (GLint locator, float x, float y, float z) const;
		void setVec4 (GLint locator, float x, float y, float z, float w) const;
		void setVec4 (GLint locator, glm::vec4 v) const;
		void setVec3 (GLint locator, glm::vec3 v) const;
		void setMat2 (GLint locator, const float *mat2) const;
		void setMat3 (GLint locator, const float *mat3) const;
		void setMat4 (GLint locator, const float *mat4) const;

		void bindAttribute(GLuint attribute, std::string variableName);

	};

	inline GLuint Shader::getLocator(const std::string &name) {
		GLuint locator = glGetUniformLocation(ID, name.c_str());
		return locator;
	}

	inline void Shader::textureUnit(std::string sampler2d, Texture2D *texture) {
		setInt(sampler2d, texture->unit());
	}

	inline void Shader::bindAttribute(GLuint attribute, std::string variableName) {
		glBindAttribLocation(ID, attribute, variableName.c_str());
	}

	inline Shader::Shader(const std::string &name) {
		ID = OpenGlUtils::loadShader(name);
	}

	inline void Shader::use() {
		glUseProgram(ID);
	}


	inline void Shader::stop() {
		glUseProgram(0);
	}

	inline void Shader::cleanup() {
		stop();
//	glDetachShader(ID, vertexShaderID);
//	glDetachShader(ID, fragmentShaderID);
//	glDeleteShader(vertexShaderID);
//	glDeleteShader(fragmentShaderID);
		glDeleteProgram(ID);
	}


	inline void Shader::setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
	}

	inline void Shader::setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	inline void Shader::setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	inline void Shader::setVec2(const std::string &name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	inline void Shader::setVec3(const std::string &name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	inline void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	inline void Shader::setVec4(const std::string &name, glm::vec4 v) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z, v.w);
	}

	inline void Shader::setVec3(const std::string &name, glm::vec3 v) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
	}

	inline void Shader::setMat2(const std::string &name, const float *mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
	}

	inline void Shader::setMat3(const std::string &name, const float *mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
	}

	inline void Shader::setMat4(const std::string &name, const float *mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
	}


	

	inline void Shader::setBool(const GLint locator, bool value) const {
		glUniform1i(locator, (int) value);
	}

	inline void Shader::setInt(const GLint locator, int value) const {
		glUniform1i(locator, value);
	}

	inline void Shader::setFloat(const GLint locator, float value) const {
		glUniform1f(locator, value);
	}

	inline void Shader::setVec2(const GLint locator, float x, float y) const {
		glUniform2f(locator, x, y);
	}

	inline void Shader::setVec3(const GLint locator, float x, float y, float z) const {
		glUniform3f(locator, x, y, z);
	}

	inline void Shader::setVec4(const GLint locator, float x, float y, float z, float w) const {
		glUniform4f(locator, x, y, z, w);
	}

	inline void Shader::setVec4(const GLint locator, glm::vec4 v) const {
		glUniform4f(locator, v.x, v.y, v.z, v.w);
	}

	inline void Shader::setVec3(const GLint locator, glm::vec3 v) const {
		glUniform3f(locator, v.x, v.y, v.z);
	}

	inline void Shader::setMat2(const GLint locator, const float *mat) const {
		glUniformMatrix2fv(locator, 1, GL_FALSE, mat);
	}

	inline void Shader::setMat3(const GLint locator, const float *mat) const {
		glUniformMatrix3fv(locator, 1, GL_FALSE, mat);
	}

	inline void Shader::setMat4(const GLint locator, const float *mat) const {
		glUniformMatrix4fv(locator, 1, GL_FALSE, mat);
	}

};
