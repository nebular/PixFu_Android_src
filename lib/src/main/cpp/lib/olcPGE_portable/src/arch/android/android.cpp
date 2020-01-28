//
// Created by rodo on 2020-01-24.
//

#include "../../olcPixelGameEngine.h"
#include "android.h"

#ifdef ANDROID

static unsigned int load_shader();

olc::PixelGameEngine *olc::PixelGameEngine::BOOTINSTANCE = nullptr;

//
// Created by rodo on 2020-01-24.
//

#define NUM_OF_MOUSE_BUTTONS 2
#define NUM_OF_KEYS 69
#define  ACTION_UP 1
#define ACTION_DOWN 0
#define ACTION_MOVE 2

void olc::PixelGameEngine::olca_on_motionevent(MotionEvent_t event) {

	// we have touch information
	// quick: lets simulate a mouse

	olc_UpdateMouse(event.X0, event.Y0);
	cLoneKeys->sync(event.X0, event.Y0, event.X1, event.Y1, pKeyNewState, event.Action == ACTION_UP);

}

void olc::PixelGameEngine::olc_ProcessWindowEvents() {
	pLoneSensor->fetch(tCurrentEvent);
	//	ALOGE("azimuth %f pitch %f roll %f", event.vector.azimuth, event.vector.pitch, event.vector.roll);

}

olc::rcode olc::PixelGameEngine::olc_WindowCreate() {

	// TODO this "4" is the density pixels , have to be in sync with screen scaling
	// 1 would be native resolution

	cLoneKeys = new LoneScreenKey(4);
	LoneScreenKey::currentInstance = cLoneKeys;
	
	pLoneSensor = new LoneSensor();
	pLoneSensor->init();

	return olc::rcode::OK;

}



 void glerror(std::string tag) {

	GLenum err = 0;

	while((err = glGetError())) {
		std::cerr << tag << "OpenGL error: " << err << std::endl;
		ALOGE("[%s] openGL error %d",tag.c_str(),err);
	}
}

bool olc::PixelGameEngine::olc_OpenGLCreate() {
	// RLP when overrided, a shader can be filled up already
	if (shader == 0) shader = load_shader();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glerror("olc_create bindvertex");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glerror("olc_create pos");

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glerror("olc_create color");

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
						  (void *) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glerror("olc_create texcoord");

	glClearColor(0.0, 0.0, 0.0, 1);
	return true;
}

/*
bool olc::PixelGameEngine::olc_OpenGLCreate() {
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

	// 1. set the vertex attributes pointers
	// Position Attribute
	GLint mPosition=glGetAttribLocation(shader, "aPos");
	glVertexAttribPointer(mPosition, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(mPosition);
	glerror("olc_openGlCreate");

	GLint mColor=glGetAttribLocation(shader, "aColor");
	glVertexAttribPointer(mColor, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(mColor);
	//Texture Coordinate Attribute

	GLint mTextCoord=glGetAttribLocation(shader, "aTexCoord");
	glVertexAttribPointer(mTextCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(mTextCoord);

	glClearColor(0.0, 0.0, 0.0, 1);
	glerror("olc_openGlCreate-O");
	return true;
}
*/

void close_window() {}

void refresh_window() {}

void close_application() {}




static unsigned int compile_shader(unsigned int type, const std::string &source) {
	unsigned int shader = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char *message = (char *) alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Shader Failed to Compile: " << std::endl;
		std::cout << message << std::endl;
	}

	return shader;
}

static unsigned int
load_shader(const std::string &vertexShader, const std::string &fragementShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragementShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

static unsigned int load_shader() {
	return load_shader(
			R"(
							   #version 300 es
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
							   #version 300 es
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

#endif
