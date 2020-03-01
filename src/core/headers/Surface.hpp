//
//  Surface.hpp
//  PixEngine
//
//  This extension is the primary surface. Provides a canvas to draw into, that gets rendered
//  full-screen by openGL as the topmost texture (any extensions drawing will be behind). This
//  surface s used for the UI and got generic 2D graphics.
//
//  You may also start your PixFu instance without the surface extension, if you don't need it.
//
//  Created by rodo on 17/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#pragma once

#include "Shader.hpp"
#include "PixFuExtension.hpp"
#include "LayerVao.hpp"

namespace rgl {

	class Canvas2D;

	class Shader;

	class Texture2D;

	class Surface : public PixFuExtension, public LayerVao {

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
		const bool bBlend;				 // whether to blend (if not topmost texture)

		Shader *pShader;
		Canvas2D *pCanvas;

		Texture2D *pActiveTexture;       // opengl texture
		std::string sSamplerName;        // name of the uniform sampler in the shader

	public:

		Surface(int width,
				int height,
				const std::string &shaderName = "default",
				const std::string &samplerName = "glbuffer",
				bool blend = false);

		virtual ~Surface();

		virtual bool init(PixFu *engine) override;

		virtual void tick(PixFu *engine, float fElapsedTime) override;

		// get the backing memory buffer
		Drawable *buffer();

		// gets the shader
		Shader *shader();

		// gets a 2D canvas
		Canvas2D *canvas();

	};

	inline Drawable *Surface::buffer() { return pActiveTexture->buffer(); }

	inline Shader *Surface::shader() { return pShader; }

	inline Canvas2D *Surface::canvas() { return pCanvas; }

}
