#ifndef HENZRY_CHAMS
#define HENZRY_CHAMS
#include <GLES2/gl2.h>
#include <dlfcn.h>
#include <And64InlineHook.hpp>
static void *handle;
bool shading = false;
bool wireframe = false;
static const char* shaderName;

void setShader(const char* s) {
    shaderName = s;
}
const char* getShader() {
    return shaderName;
}
bool getShadingEnabled() {
    return shading;
}
bool getWireframeEnabled() {
    return wireframe;
}

int (*old_glGetUniformLocation)(GLuint, const GLchar *);
GLint new_glGetUniformLocation(GLuint program, const GLchar *name) {
    return old_glGetUniformLocation(program, name);
}
bool isCurrentShader(const char *shader) {
    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    return old_glGetUniformLocation(currProgram, shader) != -1;
}
void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    old_glDrawElements(mode, count, type, indices);
    if (mode != GL_TRIANGLES || count < 1000) return;
    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    GLint id = old_glGetUniformLocation(currProgram, getShader());
    if (id == -1) return;
    if (getWireframeEnabled()) {
    glDepthRangef(1, 0.5);
    glBlendColor(1.0f, 1.0f, 0.0f, 1);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
    glLineWidth(1.0f);
    old_glDrawElements(GL_LINE_LOOP, count, type, indices);
}
if (getShadingEnabled()) {
    glDepthRangef(1, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glDepthFunc(GL_ALWAYS);
    old_glDrawElements(GL_TRIANGLES, count, type, indices);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glDepthFunc(GL_LESS);
    glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
}

    old_glDrawElements(mode, count, type, indices);
    glDepthRangef(0.5, 1);
    glColorMask(1, 1, 1, 1);
    glDisable(GL_BLEND);
}
bool mlovinit() {
    handle = NULL;
    handle = dlopen(OBFUSCATE("libGLESv2.so"), RTLD_LAZY);
    if (!handle) {
        return false;
    }
    return true;
}

void LogShaders() {
    auto addr_glGetUniformLocation = (uintptr_t)dlsym(handle, OBFUSCATE("glGetUniformLocation"));
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        return;
    } else {
        A64HookFunction((void *)addr_glGetUniformLocation, (void *)new_glGetUniformLocation, (void **)&old_glGetUniformLocation);
    }
}

void Wallhack() {
    auto addr_glDrawElements = (uintptr_t)dlsym(handle, OBFUSCATE("glDrawElements"));
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        return;
    } else {
        A64HookFunction((void *)addr_glDrawElements, (void *)new_glDrawElements, (void **)&old_glDrawElements);
    }
}

#endif

