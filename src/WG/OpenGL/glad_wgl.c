/*

    WGL loader generated by glad 0.1.36 on Fri Jun  7 12:54:58 2024.

    Language/Generator: C/C++
    Specification: wgl
    APIs: wgl=1.0
    Profile: -
    Extensions:
        WGL_ARB_create_context,
        WGL_ARB_extensions_string,
        WGL_EXT_extensions_string,
        WGL_EXT_swap_control
    Loader: False
    Local files: False
    Omit khrplatform: False
    Reproducible: False

    Commandline:
        --api="wgl=1.0" --generator="c" --spec="wgl" --no-loader --extensions="WGL_ARB_create_context,WGL_ARB_extensions_string,WGL_EXT_extensions_string,WGL_EXT_swap_control"
    Online:
        https://glad.dav1d.de/#language=c&specification=wgl&api=wgl%3D1.0&extensions=WGL_ARB_create_context&extensions=WGL_ARB_extensions_string&extensions=WGL_EXT_extensions_string&extensions=WGL_EXT_swap_control
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WG/OpenGL/glad_wgl.h>

static HDC GLADWGLhdc = (HDC)INVALID_HANDLE_VALUE;

static int get_exts(void) {
    return 1;
}

static void free_exts(void) {
    return;
}

static int has_ext(const char *ext) {
    const char *terminator;
    const char *loc;
    const char *extensions;

    if(wglGetExtensionsStringEXT == NULL && wglGetExtensionsStringARB == NULL)
        return 0;

    if(wglGetExtensionsStringARB == NULL || GLADWGLhdc == INVALID_HANDLE_VALUE)
        extensions = wglGetExtensionsStringEXT();
    else
        extensions = wglGetExtensionsStringARB(GLADWGLhdc);

    if(extensions == NULL || ext == NULL)
        return 0;

    while(1) {
        loc = strstr(extensions, ext);
        if(loc == NULL)
            break;

        terminator = loc + strlen(ext);
        if((loc == extensions || *(loc - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0'))
        {
            return 1;
        }
        extensions = terminator;
    }

    return 0;
}
int GLAD_WGL_VERSION_1_0 = 0;
int GLAD_WGL_ARB_create_context = 0;
int GLAD_WGL_ARB_extensions_string = 0;
int GLAD_WGL_EXT_extensions_string = 0;
int GLAD_WGL_EXT_swap_control = 0;
PFNWGLCREATECONTEXTATTRIBSARBPROC glad_wglCreateContextAttribsARB = NULL;
PFNWGLGETEXTENSIONSSTRINGARBPROC glad_wglGetExtensionsStringARB = NULL;
PFNWGLGETEXTENSIONSSTRINGEXTPROC glad_wglGetExtensionsStringEXT = NULL;
PFNWGLSWAPINTERVALEXTPROC glad_wglSwapIntervalEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC glad_wglGetSwapIntervalEXT = NULL;
static void load_WGL_ARB_create_context(GLADloadproc load) {
	if(!GLAD_WGL_ARB_create_context) return;
	glad_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)load("wglCreateContextAttribsARB");
}
static void load_WGL_ARB_extensions_string(GLADloadproc load) {
	if(!GLAD_WGL_ARB_extensions_string) return;
	glad_wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)load("wglGetExtensionsStringARB");
}
static void load_WGL_EXT_extensions_string(GLADloadproc load) {
	if(!GLAD_WGL_EXT_extensions_string) return;
	glad_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)load("wglGetExtensionsStringEXT");
}
static void load_WGL_EXT_swap_control(GLADloadproc load) {
	if(!GLAD_WGL_EXT_swap_control) return;
	glad_wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)load("wglSwapIntervalEXT");
	glad_wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)load("wglGetSwapIntervalEXT");
}
static int find_extensionsWGL(void) {
	if (!get_exts()) return 0;
	GLAD_WGL_ARB_create_context = has_ext("WGL_ARB_create_context");
	GLAD_WGL_ARB_extensions_string = has_ext("WGL_ARB_extensions_string");
	GLAD_WGL_EXT_extensions_string = has_ext("WGL_EXT_extensions_string");
	GLAD_WGL_EXT_swap_control = has_ext("WGL_EXT_swap_control");
	free_exts();
	return 1;
}

static void find_coreWGL(HDC hdc) {
	GLADWGLhdc = hdc;
}

int gladLoadWGLLoader(GLADloadproc load, HDC hdc) {
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)load("wglGetExtensionsStringARB");
	wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)load("wglGetExtensionsStringEXT");
	if(wglGetExtensionsStringARB == NULL && wglGetExtensionsStringEXT == NULL) return 0;
	find_coreWGL(hdc);

	if (!find_extensionsWGL()) return 0;
	load_WGL_ARB_create_context(load);
	load_WGL_ARB_extensions_string(load);
	load_WGL_EXT_extensions_string(load);
	load_WGL_EXT_swap_control(load);
	return 1;
}

