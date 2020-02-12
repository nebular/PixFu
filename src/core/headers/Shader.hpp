//
//  Shader.hpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp
#include "OpenGL.h"
#include "OpenGlUtils.h"

namespace rgl  {

class Shader {
	
	GLuint ID;
	
public:
	
	Shader(std::string name);
	
	void use();
	
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const float *mat2) const;
	void setMat3(const std::string &name, const float *mat3) const;
	void setMat4(const std::string &name, const float *mat4) const;
};

inline Shader::Shader(std::string name) {
	ID = openglutils::load_shader(name);
}
inline void Shader::use() {
	glUseProgram(ID);
}
inline void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
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

inline void  Shader::setMat2(const std::string &name, const float *mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
}
inline void  Shader::setMat3(const std::string &name, const float *mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
}
inline void  Shader::setMat4(const std::string &name, const float *mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat);
}

};
#endif /* Shader_hpp */
