//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_WINDOWS_H
#define GLES3JNI_WINDOWS_H


#ifdef _WIN32
	// Link to libraries
	#ifndef __MINGW32__
		#pragma comment(lib, "user32.lib")		// Visual Studio Only
		#pragma comment(lib, "gdi32.lib")		// For other Windows Compilers please add
		#pragma comment(lib, "opengl32.lib")	// these libs to your linker input
		#pragma comment(lib, "gdiplus.lib")
	#else
		// In Code::Blocks, Select C++14 in your build options, and add the
		// following libs to your linker: user32 gdi32 opengl32 gdiplus
		#if !defined _WIN32_WINNT
			#ifdef HAVE_MSMF
				#define _WIN32_WINNT 0x0600 // Windows Vista
			#else
				#define _WIN32_WINNT 0x0500 // Windows 2000
			#endif
		#endif
	#endif


	// Include WinAPI
	#include <windows.h>
	#include <gdiplus.h>

	// OpenGL Extension
	#include <GL/gl.h>

	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t *wglSwapInterval;


#endif //GLES3JNI_WINDOWS_H
