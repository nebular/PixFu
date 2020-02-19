//
//  OpenGL.h
//  javid
//
//  Created by rodo on 30/12/2019.
//  Copyright © 2019 rodo. All rights reserved.
//
#pragma once
#ifdef _WIN32

// OpenGL Extension
#include <GL/gl.h>

#elif __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <OpenGL/gl3.h>
#elif LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#elif ANDROID

#if __ANDROID_API__ >= 24

#include <GLES3/gl32.h>

#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif

#endif

