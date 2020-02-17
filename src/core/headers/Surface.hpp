//
//  Surface.hpp
//  PixEngine
//
//  Created by rodo on 17/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#ifndef PrimarySurface_hpp
#define PrimarySurface_hpp

#include "png.h"
#include "Utils.hpp"
#include "Shader.hpp"
#include "Layer.hpp"
#include "Texture2D.hpp"
#include "OpenGlUtils.h"


namespace rgl {

	class Canvas2D;

	class Surface : public PixFuExtension, public Layer {

		static std::string TAG;

	public:
		static constexpr float VERTICES[32]{
				// positions          // colors           // texture coords
				1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
				1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
				-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
		};

		static constexpr unsigned int INDICES[6]{
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
		};

	private:
		const int nWidth, nHeight;       // texture size

		Shader *pShader;
		Canvas2D *pCanvas;

		Texture2D *pActiveTexture;       // opengl texture
		std::string sSamplerName;        // name of the uniform sampler in the shader

	public:

		Surface(int width,
				int height,
				const std::string &shaderName = "default",
				const std::string &samplerName = "glbuffer");

		virtual ~Surface();

		virtual bool init(PixFu *engine) override;

		virtual void tick(PixFu *engine, float fElapsedTime) override;

		// get the backing memory buffer
		Drawable *buffer();

		// gets the shader
		Shader *shader();

		Canvas2D *canvas();

	};

	inline Drawable *Surface::buffer() { return pActiveTexture->buffer(); }

	inline Shader *Surface::shader() { return pShader; }

	inline Canvas2D *Surface::canvas() { return pCanvas; }

}
#endif /* PrimarySurface_hpp */
