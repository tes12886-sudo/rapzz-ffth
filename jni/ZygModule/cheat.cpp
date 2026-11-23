// RAPZZ — com.dts.freefireth port of the ffadv (Free Fire MAX) native menu.
// Credit: RAPZZ. Offsets resolved by-name from the freefireth dump.cs via il2cpp exports.
#include <ESP.h>
#include <layout.h>
#include <imports.h>
#include <jni.h>
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/MemoryPatch.hpp"

class _BYTE; class _QWORD;
class _DWORD; class _WORD;
#define _QWORD long
#define _DWORD long
#define _BYTE long
#define _WORD long
#include <EGL/egl.h>

// Guarded patch helpers: resolve by name, only write if the method exists in this build.
#define PATCH_RET(ns, cls, name, args) do { \
    void* _m = Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(ns), OBFUSCATE(cls), OBFUSCATE(name), args); \
    if (_m) MemoryPatch::createWithHex((uintptr_t)_m, OBFUSCATE("C0 03 5F D6")).Modify(); \
} while (0)
#define PATCH_ZERO(ns, cls, name, args) do { \
    void* _m = Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(ns), OBFUSCATE(cls), OBFUSCATE(name), args); \
    if (_m) MemoryPatch::createWithHex((uintptr_t)_m, OBFUSCATE("00 00 80 D2 C0 03 5F D6")).Modify(); \
} while (0)

EGLBoolean(*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &egl.width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &egl.height);
    if (!egl.setup) {
        ImGui::CreateContext();
        SetupImgui();
        InitTexture();
        egl.setup = true;
    }
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(egl.width, egl.height);
    ImGui::NewFrame();

    int touchCount = (((int (*)())(Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"))))());
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1)))(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
        io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
        io.MouseDown[0] = true;
    } else {
        io.MouseDown[0] = false;
    }

    menu();
    if (EnableEsp) DrawESP(egl.width, egl.height);
    Aimbott();

    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

void* loadCheat(void* org) {
    while (!getLibraryLoaded(OBFUSCATE("libil2cpp.so")))
        sleep(10);
    Il2CppAttach();

    // ---- freefireth report / ban suppression (all verified present in dump.cs) ----
    PATCH_ZERO("COW", "GameRegionSwitch", "IsBannedBRTrioRank", 0);
    PATCH_ZERO("COW", "GameRegionSwitch", "IsBannedCSTrioRank", 0);
    PATCH_ZERO("COW", "UIModelGroup", "get_IsBannedTrioRank", 0);
    PATCH_RET ("COW", "UIModelGroup", "set_IsBannedTrioRank", 1);

    PATCH_ZERO("COW", "UIModelRecruit", "IsAccountRecruitBanned", 0);
    PATCH_ZERO("COW", "UIModelRecruit", "CheckAccountRecruitBanAndShowTips", 0);
    PATCH_ZERO("COW", "UIModelRecruit", "GetAccountRecruitBanRemainingMinutes", 0);
    PATCH_RET ("COW", "UIModelRecruit", "ShowAccountRecruitBannedTips", 0);
    PATCH_RET ("COW", "UIModelRecruit", "ProcessAccountRecruitBannedNtf", 1);

    PATCH_ZERO("COW", "BanReasonDataManager", "GetBanReasonDesc", 2);
    PATCH_ZERO("COW", "BanReasonDataManager", "GetBanReasonUrl", 1);
    PATCH_ZERO("COW", "BanReasonDataManager", "GetDefaultUrl", 1);

    PATCH_RET("COW", "EventLogger", "LogReportCheat", 10);
    PATCH_RET("COW", "EventLogger", "LogReportCheatInHistory", 8);
    PATCH_RET("COW", "EventLogger", "LogOutGameReport", 4);
    PATCH_RET("COW", "EventLogger", "LogClanReport", 5);
    PATCH_RET("COW", "EventLogger", "LogAndroidApplicationDetection", 1);
    PATCH_RET("COW", "EventLogger", "SendEventTypeHackClientFix", 1);
    PATCH_RET("COW", "EventLogger", "RecordSpecialKill", 2);
    PATCH_RET("COW", "EventLogger", "SendSpecialKillLog", 0);
    PATCH_RET("COW", "EventLogger", "CheckAndSendQuitTimes", 0);
    PATCH_RET("COW", "EventLogger", "LogEventTypeBannedPlayerAppeal", 4);
    PATCH_RET("COW", "EventLogger", "LogEventTypeAntiAddictionBanAck_IP_US", 1);
    PATCH_RET("COW", "EventLogger", "LogEventTypeAntiAddictionBan", 2);
    PATCH_RET("COW", "EventLogger", "SendUGCColliderResourceCheck", 2);
    PATCH_RET("COW", "EventLogger", "LogAIAssistantFAQ", 8);
    PATCH_RET("COW", "EventLogger", "LogScanQRCodeInfo", 5);
    PATCH_RET("COW", "EventLogger", "SendMicAccessPopUp", 1);
    PATCH_RET("COW", "EventLogger", "RecordCDNDownloadInfo", 4);
    PATCH_RET("COW", "EventLogger", "LogTargetTractionMission", 4);
    PATCH_RET("COW", "EventLogger", "LogUGCMapTranslationFail", 2);

    // ffantihack on TH lives in MHJKABKLFEB / DKPDEMLPNOE (different from MAX).
    // Left disabled by default, same caution as the ffadv reference.
    // PATCH_RET("ffantihack", "MHJKABKLFEB", "JGFDFDBKIGD", 1);
    // PATCH_RET("ffantihack", "MHJKABKLFEB", "DHEHFAGCCDM", 1);
    // PATCH_RET("ffantihack", "MHJKABKLFEB", "INICGCCOGEC", 1);
    // PATCH_RET("ffantihack", "MHJKABKLFEB", "JHACIFHANIL", 0);
    // PATCH_RET("ffantihack", "DKPDEMLPNOE", "PCEAEHNPBGI", 1);
    // PATCH_RET("ffantihack", "DKPDEMLPNOE", "DAFGLNFFCML", 1);

    // Player.UpdateBehavior(float,float) — verified in freefireth dump.
    A64HookFunction(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("UpdateBehavior"), 2), (void*)_LateUpdate, (void**)&LateUpdate);

    if (mlovinit()) {
        setShader(OBFUSCATE("_AlphaMask"));
        LogShaders(); Wallhack();
    }

    void* libEGL = dlopen(OBFUSCATE("libEGL.so"), RTLD_NOW | RTLD_GLOBAL);
    if (!libEGL) return nullptr;
    A64HookFunction((void*)dlsym(libEGL, OBFUSCATE("eglSwapBuffers")), (void*)hook_eglSwapBuffers, (void**)&old_eglSwapBuffers);
    return nullptr;
}

static bool g_bootStarted = false;
static void startBoot() {
    if (g_bootStarted) return;
    g_bootStarted = true;
    pthread_t ptid;
    pthread_create(&ptid, nullptr, loadCheat, nullptr);
}

// Root injection path: plain dlopen() does NOT call JNI_OnLoad, so start from a constructor.
__attribute__((constructor)) static void rapzz_ctor() {
    startBoot();
}

// APK path: System.loadLibrary("rapzz") calls JNI_OnLoad.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    startBoot();
    return JNI_VERSION_1_6;
}
