//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_LINUX_H
#define GLES3JNI_LINUX_H
#ifdef LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <png.h>
typedef int(glSwapInterval_t) (Display *dpy, GLXDrawable drawable, int interval);
static glSwapInterval_t *glSwapIntervalEXT;
#endif
#endif //GLES3JNI_LINUX_H
