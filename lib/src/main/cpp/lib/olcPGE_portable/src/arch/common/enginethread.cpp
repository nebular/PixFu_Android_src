#include "olcPixelGameEngine.h"

//
// Created by rodo on 2020-01-24.
//


namespace olc {

	void PixelGameEngine::EngineThread() {

		if (!olca_thread_init()) {
			// error
			std::cerr << "Error:  engine init" << std::endl;
			return;
		}

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();


		while (bAtomActive) {

			// Run as fast as possible
			while (bAtomActive) {

				// Handle Timing
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;

				// Our time per frame coefficient
				float fElapsedTime = elapsedTime.count();

				bAtomActive = olca_thread_tick(fElapsedTime);

				// Update Title Bar
				fFrameTimer += fElapsedTime;
				nFrameCount++;

				if (fFrameTimer >= 1.0f) {

					fFrameTimer -= 1.0f;

					std::string sTitle =
							"OneLoneCoder.com - Pixel Game Engine - " + sAppName + " - FPS: " +
							std::to_string(nFrameCount);

#ifdef _WIN32
					#ifdef UNICODE
							SetWindowText(olc_hWnd, ConvertS2W(sTitle).c_str());
						#else
							SetWindowText(olc_hWnd, sTitle.c_str());
						#endif
#elif __APPLE__
					set_window_name(sTitle.c_str());
#elif ANDROID
					std::cout << "TODO set window name" << std::endl;
#elif LINUX
					XStoreName(olc_Display, olc_Window, sTitle.c_str());
#endif

					nFrameCount = 0;
				}
			}

			// Allow the user to free resources if they have overrided the destroy function
			if (OnUserDestroy()) {
				// User has permitted destroy, so exit and clean up
			} else {
				// User denied destroy for some reason, so continue running
				bAtomActive = true;
			}
		}

		olca_thread_deinit();

	}

	olc::rcode PixelGameEngine::Start() {

		// Construct the window
		if (!olc_WindowCreate())
			return olc::FAIL;

		// Start the thread

		bAtomActive = true;

		#if defined LINUX || defined _WIN32
			std::thread t = std::thread(&PixelGameEngine::EngineThread, this);
		#elif defined __APPLE__
			EngineThread();
		#elif defined ANDROID
			return olc::OK;
		#endif

		#ifdef _WIN32
			// Handle Windows Message Loop
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		#endif

		#if defined LINUX || defined _WIN32
			// this is win32 or linux
			t.join();
		#elif defined __APPLE__
			// Wait for thread to be exited
			close_window();
			close_application();
		#endif
		return olc::OK;
	}

	bool PixelGameEngine::olca_thread_init() {

		// Start OpenGL, the context is owned by the game thread
		olc_OpenGLCreate();


		//glChannel1 = loadTexture(getSpriteBuffer());

		glGenTextures(1, &glBuffer);
		glBindTexture(GL_TEXTURE_2D, glBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

#if !defined __APPLE__ && !defined ANDROID
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#endif

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nScreenWidth, nScreenHeight, 0, GL_RGBA,
					 GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());


#if defined __APPLE__ || defined ANDROID

		glUseProgram(shader);
		glUniform1i(glGetUniformLocation(shader, "glbuffer"), 0);

#endif

		// Create user resources as part of this thread
		if (!OnUserCreate())
			return false;

		// ------------------
		// RLP: As derived classes can load textures in OnUserCreate, we
		// reselect primary texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glBuffer);
		// ------------------

		return true;
	}

	bool olc::PixelGameEngine::olca_thread_tick(float fElapsedTime) {

#ifdef __APPLE__

		process_window_events();
			if (get_window_is_closing()) {
				bAtomActive = false;
			}
			olc_UpdateViewport();
			olc_ProcessWindowEvents();

			glClear(GL_COLOR_BUFFER_BIT);

#elif ANDROID

		// todo
		olc_UpdateViewport();
		olc_ProcessWindowEvents();
		glClear(GL_COLOR_BUFFER_BIT);

//		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#elif LINUX

		// Handle Xlib Message Loop - we do this in the
			// same thread that OpenGL was created so we dont
			// need to worry too much about multithreading with X11
			XEvent xev;

			while (XPending(olc_Display))
			{
				XNextEvent(olc_Display, &xev);
				if (xev.type == Expose)
				{
					XWindowAttributes gwa;
					XGetWindowAttributes(olc_Display, olc_Window, &gwa);
					nWindowWidth = gwa.width;
					nWindowHeight = gwa.height;
					olc_UpdateViewport();
					glClear(GL_COLOR_BUFFER_BIT); // Thanks Benedani!
				}
				else if (xev.type == ConfigureNotify)
				{
					XConfigureEvent xce = xev.xconfigure;
					nWindowWidth = xce.width;
					nWindowHeight = xce.height;
				}
				else if (xev.type == KeyPress)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					pKeyNewState[mapKeys[sym]] = true;
					XKeyEvent *e = (XKeyEvent *)&xev; // Because DragonEye loves numpads
					XLookupString(e, NULL, 0, &sym, NULL);
					pKeyNewState[mapKeys[sym]] = true;
				}
				else if (xev.type == KeyRelease)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					pKeyNewState[mapKeys[sym]] = false;
					XKeyEvent *e = (XKeyEvent *)&xev;
					XLookupString(e, NULL, 0, &sym, NULL);
					pKeyNewState[mapKeys[sym]] = false;
				}
				else if (xev.type == ButtonPress)
				{
					switch (xev.xbutton.button)
					{
						case 1:	pMouseNewState[0] = true; break;
						case 2:	pMouseNewState[2] = true; break;
						case 3:	pMouseNewState[1] = true; break;
						case 4:	olc_UpdateMouseWheel(120); break;
						case 5:	olc_UpdateMouseWheel(-120); break;
						default: break;
					}
				}
				else if (xev.type == ButtonRelease)
				{
					switch (xev.xbutton.button)
					{
						case 1:	pMouseNewState[0] = false; break;
						case 2:	pMouseNewState[2] = false; break;
						case 3:	pMouseNewState[1] = false; break;
						default: break;
					}
				}
				else if (xev.type == MotionNotify)
				{
					olc_UpdateMouse(xev.xmotion.x, xev.xmotion.y);
				}
				else if (xev.type == FocusIn)
				{
					bHasInputFocus = true;
				}
				else if (xev.type == FocusOut)
				{
					bHasInputFocus = false;
				}
				else if (xev.type == ClientMessage)
				{
					bAtomActive = false;
				}
			}
#endif

		// Handle User Input - Keyboard
		for (int i = 0; i < 256; i++) {
			pKeyboardState[i].bPressed = false;
			pKeyboardState[i].bReleased = false;

			if (pKeyNewState[i] != pKeyOldState[i]) {
				if (pKeyNewState[i]) {
					pKeyboardState[i].bPressed = !pKeyboardState[i].bHeld;
					pKeyboardState[i].bHeld = true;
				} else {
					pKeyboardState[i].bReleased = true;
					pKeyboardState[i].bHeld = false;
				}
			}

			pKeyOldState[i] = pKeyNewState[i];
		}

		// Handle User Input - Mouse
		for (int i = 0; i < 5; i++) {
			pMouseState[i].bPressed = false;
			pMouseState[i].bReleased = false;

			if (pMouseNewState[i] != pMouseOldState[i]) {
				if (pMouseNewState[i]) {
					pMouseState[i].bPressed = !pMouseState[i].bHeld;
					pMouseState[i].bHeld = true;
				} else {
					pMouseState[i].bReleased = true;
					pMouseState[i].bHeld = false;
				}
			}

			pMouseOldState[i] = pMouseNewState[i];
		}

		// Cache mouse coordinates so they remain
		// consistent during frame

		nMousePosX = nMousePosXcache;
		nMousePosY = nMousePosYcache;

		nMouseWheelDelta = nMouseWheelDeltaCache;
		nMouseWheelDeltaCache = 0;

#ifdef OLC_DBG_OVERDRAW
		olc::Sprite::nOverdrawCount = 0;
#endif

		// Handle Frame Update
		bAtomActive = OnUserUpdate(fElapsedTime);

		//glDrawArrays();

		// Display Graphics
#if not defined __APPLE__ && not defined ANDROID
		glViewport(nViewX, nViewY, nViewW, nViewH);
#endif

		// TODO: This is a bit slow (especially in debug, but 100x faster in release mode???)
		// Copy pixel array into texture
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE,
						pDefaultDrawTarget->GetData());

#if defined  __APPLE__ || defined ANDROID
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glBuffer);

		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
		// Display texture on screen
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
			glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f + (fSubPixelOffsetX),  1.0f + (fSubPixelOffsetY), 0.0f);
			glTexCoord2f(1.0, 0.0); glVertex3f( 1.0f + (fSubPixelOffsetX),  1.0f + (fSubPixelOffsetY), 0.0f);
			glTexCoord2f(1.0, 1.0); glVertex3f( 1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
			glEnd();
#endif

		// Present Graphics to screen
#ifdef _WIN32
		SwapBuffers(glDeviceContext);
#elif  defined __APPLE__ || defined ANDROID
		refresh_window();
#else
		glXSwapBuffers(olc_Display, olc_Window);
#endif

		return bAtomActive;

	}


	void PixelGameEngine::olca_thread_deinit() {

		bAtomActive = false;

#ifdef _WIN32
		wglDeleteContext(glRenderContext);
					PostMessage(olc_hWnd, WM_DESTROY, 0, 0);
#elif defined __APPLE__ || defined ANDROID
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
#else
		glXMakeCurrent(olc_Display, None, NULL);
								glXDestroyContext(olc_Display, glDeviceContext);
								XDestroyWindow(olc_Display, olc_Window);
								XCloseDisplay(olc_Display);
#endif

	}

}
