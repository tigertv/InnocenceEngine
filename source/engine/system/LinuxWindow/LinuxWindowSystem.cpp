#include "LinuxWindowSystem.h"
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/glx.h>

#include "../ICoreSystem.h"

extern ICoreSystem* g_pCoreSystem;

typedef GLXContext (*glXCreateContextAttribsARBProc) (Display*, GLXFBConfig, GLXContext, Bool, const int*);

INNO_PRIVATE_SCOPE LinuxWindowSystemNS
{
	bool setup();
	bool initialize();
	bool update();
	bool terminate();

	ObjectStatus m_objectStatus = ObjectStatus::SHUTDOWN;
	ButtonStatusMap m_buttonStatus;
	Display* m_display;
	Window m_window;
	GLint m_attributes[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, true,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		None
	};
}

bool LinuxWindowSystemNS::setup()
{
	auto l_screenResolution = g_pCoreSystem->getVisionSystem()->getRenderingFrontend()->getScreenResolution();

	m_display = XOpenDisplay(0);

	if(m_display == nullptr)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "LinuxWindowSystem: Can't connect to X server!");
		LinuxWindowSystemNS::m_objectStatus = ObjectStatus::STANDBY;
		return false;
	}

	m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display),
	0, 0,   /* x, y */
	(unsigned int)l_screenResolution.x, (unsigned int)l_screenResolution.y, /* width, height */
	0, 0,     /* border_width, border */
	0);       /* background */

	int num_fbc = 0;
	GLXFBConfig *fbc = glXChooseFBConfig(m_display, DefaultScreen(m_display), m_attributes, &num_fbc);
	if (!fbc)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "LinuxWindowSystem: glXChooseFBConfig() failed!");
		LinuxWindowSystemNS::m_objectStatus = ObjectStatus::STANDBY;
		return false;
	}

	/* Create old OpenGL context to get correct function pointer for
	glXCreateContextAttribsARB() */
	XVisualInfo *vi = glXGetVisualFromFBConfig(m_display, fbc[0]);
	GLXContext ctx_old = glXCreateContext(m_display, vi, 0, GL_TRUE);
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB =
	(glXCreateContextAttribsARBProc)
	glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
	/* Destroy old context */
	glXMakeCurrent(m_display, 0, 0);
	glXDestroyContext(m_display, ctx_old);
	if (!glXCreateContextAttribsARB)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "LinuxWindowSystem: glXCreateContextAttribsARB() not found!");
		LinuxWindowSystemNS::m_objectStatus = ObjectStatus::STANDBY;
		return false;
	}

	/* Set desired minimum OpenGL version */
	static int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 5,
		None
	};
	/* Create modern OpenGL context */
	GLXContext ctx = glXCreateContextAttribsARB(m_display, fbc[0], NULL, true, context_attribs);
	if (!ctx)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "LinuxWindowSystem: Failed to create OpenGL context!");
		LinuxWindowSystemNS::m_objectStatus = ObjectStatus::STANDBY;
		return false;
	}

	/* Show_the_window
	--------------- */
	glXMakeCurrent(m_display, m_window, ctx);
	XMapWindow(m_display, m_window);

	auto l_windowName = g_pCoreSystem->getGameSystem()->getGameName();

	XStoreName(m_display, m_window, l_windowName.c_str());
	XSelectInput(m_display, m_window, ExposureMask | StructureNotifyMask);

	LinuxWindowSystemNS::m_objectStatus = ObjectStatus::ALIVE;
	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "LinuxWindowSystem setup finished.");

	return true;
}

bool LinuxWindowSystemNS::initialize()
{
	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "LinuxWindowSystem has been initialized.");
	return true;
}

bool LinuxWindowSystemNS::update()
{
	return true;
}

bool LinuxWindowSystemNS::terminate()
{
	LinuxWindowSystemNS::m_objectStatus = ObjectStatus::SHUTDOWN;
	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "LinuxWindowSystem has been terminated.");
	return true;
}

bool LinuxWindowSystem::setup(void* hInstance, void* hwnd)
{
	return LinuxWindowSystemNS::setup();
}

bool LinuxWindowSystem::initialize()
{
	return LinuxWindowSystemNS::initialize();
}

bool LinuxWindowSystem::update()
{
	return LinuxWindowSystemNS::update();
}

bool LinuxWindowSystem::terminate()
{
	return LinuxWindowSystemNS::terminate();
}

ObjectStatus LinuxWindowSystem::getStatus()
{
	return LinuxWindowSystemNS::m_objectStatus;
}

ButtonStatusMap LinuxWindowSystem::getButtonStatus()
{
	return LinuxWindowSystemNS::m_buttonStatus;
}

bool LinuxWindowSystem::sendEvent(unsigned int umsg, unsigned int WParam, int LParam)
{
	return true;
}

void LinuxWindowSystem::swapBuffer()
{
}