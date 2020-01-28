#ifdef __APPLE__

#include "olcPixelGameEngine.h"
//
// Created by rodo on 2020-01-24.
//

#define NUM_OF_MOUSE_BUTTONS 2
#define NUM_OF_KEYS 69

void olc::PixelGameEngine::olc_ProcessWindowEvents()
{
	float mouseX = get_mouse_position_x();
	float mouseY = get_mouse_position_y();
	olc_UpdateMouse(mouseX, mouseY);

	bHasInputFocus = window_is_focused();

	for (char i = 0; i < NUM_OF_MOUSE_BUTTONS; i++) {
		pMouseNewState[i] = get_mouse_button(i);
	}

	olc_UpdateMouseWheel(get_mouse_scroll_y());

	for (int i = 0; i < NUM_OF_KEYS; i++) {
		if  (i < 55) {
			if (get_key_down(::Key::All[i])) {
				pKeyNewState[i] = true;
			}
			if (get_key_up(::Key::All[i])) {
				pKeyNewState[i] = false;
			}
		}
		else {
			switch (i) {
				case Key::SHIFT :
					if (get_modifier_key_down(::SHIFT)) pKeyNewState[i] = true;
					if (get_modifier_key_up(::SHIFT)) pKeyNewState[i] = false;
					break;
				case Key::CTRL :
					if (get_modifier_key_down(::CONTROL)) pKeyNewState[i] = true;
					if (get_modifier_key_up(::CONTROL)) pKeyNewState[i] = false;
					break;
				case Key::INS :
					if (get_key_down(::Key::KEY_INSERT_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_INSERT_)) pKeyNewState[i] = false;
					break;
				case Key::DEL :
					if (get_key_down(::Key::KEY_DEL_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_DEL_)) pKeyNewState[i] = false;
					break;
				case Key::HOME :
					if (get_key_down(::Key::KEY_HOME_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_HOME_)) pKeyNewState[i] = false;
					break;
				case Key::END :
					if (get_key_down(::Key::KEY_END_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_END_)) pKeyNewState[i] = false;
					break;
				case Key::PGUP :
					if (get_key_down(::Key::KEY_PGUP_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_PGUP_)) pKeyNewState[i] = false;
					break;
				case Key::PGDN :
					if (get_key_down(::Key::KEY_PGDN_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_PGDN_)) pKeyNewState[i] = false;
					break;
				case Key::BACK :
					if (get_key_down(::Key::KEY_DELETE)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_DELETE)) pKeyNewState[i] = false;
					break;
				case Key::ESCAPE :
					if (get_key_down(::Key::KEY_ESCAPE)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_ESCAPE)) pKeyNewState[i] = false;
					break;
				case Key::RETURN :
					if (get_key_down(::Key::KEY_RETURN)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_RETURN)) pKeyNewState[i] = false;
					break;
				case Key::ENTER :
					if (get_key_down(::Key::KEY_ENTER)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_ENTER)) pKeyNewState[i] = false;
					break;
				case Key::PAUSE :
					if (get_key_down(::Key::KEY_PAUSE_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_PAUSE_)) pKeyNewState[i] = false;
					break;
				case Key::SCROLL :
					if (get_key_down(::Key::KEY_SCROLL_)) pKeyNewState[i] = true;
					if (get_key_up(::Key::KEY_SCROLL_)) pKeyNewState[i] = false;
					break;
			}
		}
	}
}

olc::rcode olc::PixelGameEngine::olc_WindowCreate()
{
	try {
		init_application();
	} catch (...) {}

	nWindowWidth = nScreenWidth * nPixelWidth;
	nWindowHeight = nScreenHeight * nPixelHeight;
	create_window("Tests", nWindowWidth, nWindowHeight);

	set_window_background_color(0, 0, 0, 1);
	set_window_title_bar_hidden (false);
	set_window_title_hidden (false);
	set_window_resizable(false);

	bool outOfBounds = false;
	while ((nWindowWidth >= get_screen_width() || nWindowHeight >= get_screen_height()) && (nPixelWidth > 0 || nPixelHeight > 0)) {
		outOfBounds = true;
		nWindowWidth = nScreenWidth * (--nPixelWidth);
		nWindowHeight = nScreenHeight * (--nPixelHeight);
		set_window_size(nWindowWidth, nWindowHeight);
	}

	if (outOfBounds) {
		if (nPixelWidth == 0 || nPixelHeight == 0) {
			std::cout << std::endl << "The screen dimension given cannot fit on your screen." << std::endl << std::endl;
			return olc::rcode::FAIL;
		}
		else {
			std::cout << std::endl << "Had to lower pixel dimension in order to fit on screen." << std::endl << std::endl;
		}
	}

	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "OpenGL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	return olc::rcode::OK;
}

	static unsigned int compile_shader (unsigned int type, const std::string& source)
	{
		unsigned int shader = glCreateShader(type);
		const char *src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca( length * sizeof(char));
			glGetShaderInfoLog( shader, length, &length, message);
			std::cout << "Shader Failed to Compile: " << std::endl;
			std::cout << message << std::endl;
		}

		return shader;
	}

	static unsigned int load_shader (const std::string& vertexShader, const std::string& fragementShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compile_shader( GL_VERTEX_SHADER, vertexShader );
		unsigned int fs = compile_shader( GL_FRAGMENT_SHADER, fragementShader );

		glAttachShader( program, vs);
		glAttachShader( program, fs);
		glLinkProgram( program );
		glValidateProgram( program );

		glDetachShader( program, vs );
		glDetachShader( program, fs );

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	static unsigned int load_shader() {
		return load_shader(
						   R"(
						   #version 330 core
						   layout (location = 0) in vec3 aPos;
						   layout (location = 1) in vec3 aColor;
						   layout (location = 2) in vec2 aTexCoord;

						   out vec3 ourColor;
						   out vec2 TexCoord;

						   void main() {
						   gl_Position = vec4(aPos, 1.0);
						   ourColor = aColor;
						   TexCoord = vec2(aTexCoord.x, 1.0f - aTexCoord.y);
						   }
						   )",
						   R"(
						   #version 330 core
						   out vec4 FragColor;

						   in vec3 ourColor;
						   in vec2 TexCoord;

						   // texture sampler
						   uniform sampler2D glbuffer;

						   void main() {
						   FragColor = texture(glbuffer, TexCoord);
						   }
						   )"
						   );
	}

	bool olc::PixelGameEngine::olc_OpenGLCreate()
	{
		// RLP when overrided, a shader can be filled up already
		if (shader == 0) shader = load_shader();

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glClearColor(0.0, 0.0, 0.0, 1);
		return true;
	}



#endif
