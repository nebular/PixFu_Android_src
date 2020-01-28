//
// Created by rodo on 2020-01-24.
//

#include "linux.h"
#ifdef LINUX

Display* PixelGameEngine::olc_WindowCreate()
{
	XInitThreads();

	// Grab the deafult display and window
	olc_Display		= XOpenDisplay(NULL);
	olc_WindowRoot	= DefaultRootWindow(olc_Display);

	// Based on the display capabilities, configure the appearance of the window
	GLint olc_GLAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	olc_VisualInfo	= glXChooseVisual(olc_Display, 0, olc_GLAttribs);
	olc_ColourMap	= XCreateColormap(olc_Display, olc_WindowRoot, olc_VisualInfo->visual, AllocNone);
	olc_SetWindowAttribs.colormap = olc_ColourMap;

	// Register which events we are interested in receiving
	olc_SetWindowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask;

	// Create the window
	olc_Window		= XCreateWindow(olc_Display, olc_WindowRoot, 30, 30, nScreenWidth * nPixelWidth, nScreenHeight * nPixelHeight, 0, olc_VisualInfo->depth, InputOutput, olc_VisualInfo->visual, CWColormap | CWEventMask, &olc_SetWindowAttribs);

	Atom wmDelete = XInternAtom(olc_Display, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(olc_Display, olc_Window, &wmDelete, 1);

	XMapWindow(olc_Display, olc_Window);
	XStoreName(olc_Display, olc_Window, "OneLoneCoder.com - Pixel Game Engine");

	if (bFullScreen) // Thanks DragonEye, again :D
	{
		Atom wm_state;
		Atom fullscreen;
		wm_state = XInternAtom(olc_Display, "_NET_WM_STATE", False);
		fullscreen = XInternAtom(olc_Display, "_NET_WM_STATE_FULLSCREEN", False);
		XEvent xev{ 0 };
		xev.type = ClientMessage;
		xev.xclient.window = olc_Window;
		xev.xclient.message_type = wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = (bFullScreen ? 1 : 0);   // the action (0: off, 1: on, 2: toggle)
		xev.xclient.data.l[1] = fullscreen;             // first property to alter
		xev.xclient.data.l[2] = 0;                      // second property to alter
		xev.xclient.data.l[3] = 0;                      // source indication
		XMapWindow(olc_Display, olc_Window);
		XSendEvent(olc_Display, DefaultRootWindow(olc_Display), False,
				   SubstructureRedirectMask | SubstructureNotifyMask, &xev);
		XFlush(olc_Display);
		XWindowAttributes gwa;
		XGetWindowAttributes(olc_Display, olc_Window, &gwa);
		nWindowWidth = gwa.width;
		nWindowHeight = gwa.height;
		olc_UpdateViewport();
	}

	// Create Keyboard Mapping
	mapKeys[0x00] = Key::NONE;
	mapKeys[0x61] = Key::A; mapKeys[0x62] = Key::B; mapKeys[0x63] = Key::C; mapKeys[0x64] = Key::D; mapKeys[0x65] = Key::E;
	mapKeys[0x66] = Key::F; mapKeys[0x67] = Key::G; mapKeys[0x68] = Key::H; mapKeys[0x69] = Key::I; mapKeys[0x6A] = Key::J;
	mapKeys[0x6B] = Key::K; mapKeys[0x6C] = Key::L; mapKeys[0x6D] = Key::M; mapKeys[0x6E] = Key::N; mapKeys[0x6F] = Key::O;
	mapKeys[0x70] = Key::P; mapKeys[0x71] = Key::Q; mapKeys[0x72] = Key::R; mapKeys[0x73] = Key::S; mapKeys[0x74] = Key::T;
	mapKeys[0x75] = Key::U; mapKeys[0x76] = Key::V; mapKeys[0x77] = Key::W; mapKeys[0x78] = Key::X; mapKeys[0x79] = Key::Y;
	mapKeys[0x7A] = Key::Z;

	mapKeys[XK_F1] = Key::F1; mapKeys[XK_F2] = Key::F2; mapKeys[XK_F3] = Key::F3; mapKeys[XK_F4] = Key::F4;
	mapKeys[XK_F5] = Key::F5; mapKeys[XK_F6] = Key::F6; mapKeys[XK_F7] = Key::F7; mapKeys[XK_F8] = Key::F8;
	mapKeys[XK_F9] = Key::F9; mapKeys[XK_F10] = Key::F10; mapKeys[XK_F11] = Key::F11; mapKeys[XK_F12] = Key::F12;

	mapKeys[XK_Down] = Key::DOWN; mapKeys[XK_Left] = Key::LEFT; mapKeys[XK_Right] = Key::RIGHT; mapKeys[XK_Up] = Key::UP;
	mapKeys[XK_KP_Enter] = Key::ENTER; mapKeys[XK_Return] = Key::ENTER;

	mapKeys[XK_BackSpace] = Key::BACK; mapKeys[XK_Escape] = Key::ESCAPE; mapKeys[XK_Linefeed] = Key::ENTER;	mapKeys[XK_Pause] = Key::PAUSE;
	mapKeys[XK_Scroll_Lock] = Key::SCROLL; mapKeys[XK_Tab] = Key::TAB; mapKeys[XK_Delete] = Key::DEL; mapKeys[XK_Home] = Key::HOME;
	mapKeys[XK_End] = Key::END; mapKeys[XK_Page_Up] = Key::PGUP; mapKeys[XK_Page_Down] = Key::PGDN;	mapKeys[XK_Insert] = Key::INS;
	mapKeys[XK_Shift_L] = Key::SHIFT; mapKeys[XK_Shift_R] = Key::SHIFT; mapKeys[XK_Control_L] = Key::CTRL; mapKeys[XK_Control_R] = Key::CTRL;
	mapKeys[XK_space] = Key::SPACE;

	mapKeys[XK_0] = Key::K0; mapKeys[XK_1] = Key::K1; mapKeys[XK_2] = Key::K2; mapKeys[XK_3] = Key::K3; mapKeys[XK_4] = Key::K4;
	mapKeys[XK_5] = Key::K5; mapKeys[XK_6] = Key::K6; mapKeys[XK_7] = Key::K7; mapKeys[XK_8] = Key::K8; mapKeys[XK_9] = Key::K9;

	mapKeys[XK_KP_0] = Key::NP0; mapKeys[XK_KP_1] = Key::NP1; mapKeys[XK_KP_2] = Key::NP2; mapKeys[XK_KP_3] = Key::NP3; mapKeys[XK_KP_4] = Key::NP4;
	mapKeys[XK_KP_5] = Key::NP5; mapKeys[XK_KP_6] = Key::NP6; mapKeys[XK_KP_7] = Key::NP7; mapKeys[XK_KP_8] = Key::NP8; mapKeys[XK_KP_9] = Key::NP9;
	mapKeys[XK_KP_Multiply] = Key::NP_MUL; mapKeys[XK_KP_Add] = Key::NP_ADD; mapKeys[XK_KP_Divide] = Key::NP_DIV; mapKeys[XK_KP_Subtract] = Key::NP_SUB; mapKeys[XK_KP_Decimal] = Key::NP_DECIMAL;

	return olc_Display;
}

bool PixelGameEngine::olc_OpenGLCreate()
{
	glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
	glXMakeCurrent(olc_Display, olc_Window, glDeviceContext);

	XWindowAttributes gwa;
	XGetWindowAttributes(olc_Display, olc_Window, &gwa);
	glViewport(0, 0, gwa.width, gwa.height);

	glSwapIntervalEXT = nullptr;
	glSwapIntervalEXT = (glSwapInterval_t*)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");
	if (glSwapIntervalEXT)
		glSwapIntervalEXT(olc_Display, olc_Window, 0);
	else
	{
		printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
		printf("      Don't worry though, things will still work, it's just the\n");
		printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
	}

	return true;
}



	bool PixelGameEngine::olc_OpenGLCreate()
	{
		// Create Device Context
		glDeviceContext = GetDC(olc_hWnd);
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};

		int pf = 0;
		if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return false;
		SetPixelFormat(glDeviceContext, pf, &pfd);

		if (!(glRenderContext = wglCreateContext(glDeviceContext))) return false;
		wglMakeCurrent(glDeviceContext, glRenderContext);

		glViewport(nViewX, nViewY, nViewW, nViewH);

		// Remove Frame cap
		wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapInterval) wglSwapInterval(0);
		return true;
	}

	// Windows Event Handler
	LRESULT CALLBACK PixelGameEngine::olc_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static PixelGameEngine *sge;
		switch (uMsg)
		{
			case WM_CREATE:		sge = (PixelGameEngine*)((LPCREATESTRUCT)lParam)->lpCreateParams;	return 0;
			case WM_MOUSEMOVE:
			{
				uint16_t x = lParam & 0xFFFF;				// Thanks @ForAbby (Discord)
				uint16_t y = (lParam >> 16) & 0xFFFF;
				int16_t ix = *(int16_t*)&x;
				int16_t iy = *(int16_t*)&y;
				sge->olc_UpdateMouse(ix, iy);
				return 0;
			}
			case WM_SIZE:
			{
				sge->olc_UpdateWindowSize(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);
				return 0;
			}
			case WM_MOUSEWHEEL:
			{
				sge->olc_UpdateMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
				return 0;
			}
			case WM_MOUSELEAVE: sge->bHasMouseFocus = false;							return 0;
			case WM_SETFOCUS:	sge->bHasInputFocus = true;								return 0;
			case WM_KILLFOCUS:	sge->bHasInputFocus = false;							return 0;
			case WM_KEYDOWN:	sge->pKeyNewState[mapKeys[wParam]] = true;				return 0;
			case WM_KEYUP:		sge->pKeyNewState[mapKeys[wParam]] = false;				return 0;
			case WM_LBUTTONDOWN:sge->pMouseNewState[0] = true;							return 0;
			case WM_LBUTTONUP:	sge->pMouseNewState[0] = false;							return 0;
			case WM_RBUTTONDOWN:sge->pMouseNewState[1] = true;							return 0;
			case WM_RBUTTONUP:	sge->pMouseNewState[1] = false;							return 0;
			case WM_MBUTTONDOWN:sge->pMouseNewState[2] = true;							return 0;
			case WM_MBUTTONUP:	sge->pMouseNewState[2] = false;							return 0;
			case WM_CLOSE:		bAtomActive = false;									return 0;
			case WM_DESTROY:	PostQuitMessage(0);										return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#endif