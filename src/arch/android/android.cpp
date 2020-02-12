//
// Created by rodo on 2020-01-24.
//

#ifdef ANDROID

#define NUM_OF_MOUSE_BUTTONS 2
#define NUM_OF_KEYS 69

#include "android.h"

unsigned int load_shader();

olc::PixelGameEngine *olc::PixelGameEngine::BOOTINSTANCE = nullptr;

static constexpr int ACTION_UP = 1;
static constexpr int ACTION_DOWN = 0;
static constexpr int ACTION_MOVE = 2;
static constexpr int ACTION_CANCEL = 3;

// other than primary finger
static constexpr int ACTION_POINTER_DOWN = 5;
static constexpr int ACTION_POINTER_UP = 6;

void glerror(std::string tag);

bool olc::PixelGameEngine::bHasWindowFocus = false;

void olc::PixelGameEngine::olca_thread_OnPause(int status) {
	ALOGV("AndroidpPGE: OnPause %d", status);
//	if (status == 0 || status == 1)
//		bHasWindowFocus = !status;
//	else {
//		// restart context
//	}
}

olc::rcode olc::PixelGameEngine::olc_WindowCreate() {

	cLoneKeys = new LoneScreenKey(1);
	LoneScreenKey::currentInstance = cLoneKeys; // todo singleton

	nWindowHeight = nScreenHeight;
	nWindowWidth = nScreenWidth;

	pLoneSensor = new LoneSensor();
	pLoneSensor->init();
	cLoneKeys->reset();

	return olc::rcode::OK;

}

unsigned int olc::PixelGameEngine::olc_LoadShader() {
	return load_shader();
}

// Called from loop to initialize openGl context
bool olc::PixelGameEngine::olc_OpenGLCreate() {

	shader = olc_LoadShader();

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

// Called from loop to update system devices
void olc::PixelGameEngine::olc_ProcessWindowEvents() {

	// In the case of Android, the motion events arrive in the callback above from the Java Side
	// so here we don´t do anything regarding keys

	// update gyroscope
	pLoneSensor->fetch(tCurrentEvent);

}

// called from Launcher to send a MotionEvent (Touch)
void olc::PixelGameEngine::olca_on_motionevent(MotionEvent_t event) {

	// we have touch information
	// lets simulate a mouse
	//
	// I tried an elaborate routine to simulate clicks on touch but it was getting too complicated and prone to fail
	// I came up with this simple approach that kind of works pretty well. It uses LoneKeys to paint
	// 2 virtual mouse buttons on screen, and multitouch is supported so you can move the pointer with one finger
	// and use the buttons with other. For implementing another mouse behavior, you have all details in tCurrentMotionEvent
	//

	tCurrentMotionEvent = event;
	int action = event.RawAction;

	// Iterate through virtual keys and set their state.
	// will return 0 -> key pressed with primary pointer (X0Y0), 1 -> with secondary (X1Y1), -1 No key pressed.
	int keysRegion = cLoneKeys->sync(tCurrentMotionEvent.X0, tCurrentMotionEvent.Y0,
									 tCurrentMotionEvent.X1, tCurrentMotionEvent.Y1, pKeyNewState,
									 tCurrentMotionEvent.Action == ACTION_UP);

	bool twoFingers = event.PointersCount > 1;
	// ALOGV ("Event: buttons %d, action %d, index %d", tCurrentMotionEvent.PointersCount, tCurrentMotionEvent.RawAction, tCurrentMotionEvent.PointerId);

	switch (action) {

		case ACTION_POINTER_UP:

			for (int i = 0; i < NUM_OF_MOUSE_BUTTONS; i++)
				pMouseNewState[i] = false;

			break;

		case ACTION_UP:
		case ACTION_CANCEL:

			// end touch (no fingers on screen)
			// reset virtual mouse button status
			cLoneKeys->reset();

			// break through to update coordinates and real state (that we have reset here)

		case ACTION_DOWN:
		case ACTION_POINTER_DOWN:

			if (twoFingers)
				for (int i = 0; i < NUM_OF_MOUSE_BUTTONS; i++)
					pMouseNewState[i] = cLoneKeys->GetFakeMouse(i);

		case ACTION_MOVE:

			// These actions update the engine mouse button status from
			// our local copy in cLoneKeys

			if (twoFingers)
				for (int i = 0; i < NUM_OF_MOUSE_BUTTONS; i++)
					pMouseNewState[i] = cLoneKeys->GetFakeMouse(i);

			// then updates the right coordinates: One of them is the finger in the buttons area,
			// the other the pointer finger. We select the right one !

			if (keysRegion != 0)
				olc_UpdateMouse(tCurrentMotionEvent.X0, tCurrentMotionEvent.Y0);
			else if (twoFingers) // may happen that only one finger is pressing buttons but none is pointing
				olc_UpdateMouse(tCurrentMotionEvent.X1, tCurrentMotionEvent.Y1);
			break;
		default:
			break;
	}
}

void close_window() {}

void refresh_window() {}

void close_application() {}

void glerror(std::string tag) {

	GLenum err = 0;

	while ((err = glGetError())) {
		std::cerr << tag << "OpenGL error: " << err << std::endl;
		ALOGE("[%s] openGL error %d", tag.c_str(), err);
	}
}

#endif
