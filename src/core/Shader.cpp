//
//  Shader.cpp
//  PixEngine
//
//  Created by rodo on 13/03/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Shader.hpp"
#include "Camera.hpp"

namespace Pix {

	Shader3D::Shader3D(const std::string& name)
	: Shader(name) {
		
		// projection locators
		LOC_TRANSFORMATIONMATRIX = getLocator("transformationMatrix");
		LOC_VIEWMATRIX = getLocator("viewMatrix");
		LOC_INVVIEWMATRIX = getLocator("invViewMatrix");
		LOC_PROJECTIONMATRIX = getLocator("projectionMatrix");
	}

	 void Shader3D::loadTransformationMatrix(glm::mat4 &matrix) {
		setMat4(LOC_TRANSFORMATIONMATRIX, (float *) &matrix);
	 }

	 void Shader3D::loadViewMatrix(Camera *camera) {
		glm::mat4& viewMatrix = camera->getViewMatrix();
		setMat4(LOC_VIEWMATRIX, (float *) &viewMatrix);
		glm::mat4& invViewMatrix = camera->getInvViewMatrix();
		setMat4(LOC_INVVIEWMATRIX, (float *) &invViewMatrix);
	}

	void Shader3D::loadProjectionMatrix(glm::mat4 &projection) {
		setMat4(LOC_PROJECTIONMATRIX, (float *) &projection);
	}
}
