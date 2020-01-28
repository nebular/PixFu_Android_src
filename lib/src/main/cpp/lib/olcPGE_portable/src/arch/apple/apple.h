//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_APPLE_H
#define GLES3JNI_APPLE_H
#ifdef __APPLE__
#include "cocoawindowing.h"
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <OpenGL/gl3.h>
#include <png.h>
#endif
#endif //GLES3JNI_APPLE_H
