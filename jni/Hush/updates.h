#pragma once
#include <cstdint>
#define getRealOffset(offset) AgetAbsoluteAddress("libil2cpp.so",offset)

static uintptr_t libBase;
bool isGameLibLoaded = false;

long AfindLibrary(const char *library) {
    char filename[0xFF] = {0},
    buffer[1024] = {0};
    FILE *fp = NULL;
    long address = 0;
    sprintf(filename, OBFUSCATE("/proc/self/maps"));
    fp = fopen(filename, OBFUSCATE("rt"));
    if (fp == NULL) {
        perror(OBFUSCATE("fopen"));
        goto done;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (long) strtoul(buffer, NULL, 16);
            goto done;
        }
    }
    done:
    if (fp) {
        fclose(fp);
    }
    return address;
}

long AClibBase;
long AgetAbsoluteAddress(const char* libraryName, long relativeAddr) {
    if (AClibBase == 0) {
        AClibBase = AfindLibrary(libraryName);
        if (AClibBase == 0) {
            AClibBase = 0;
        }
    }
    return AClibBase + relativeAddr;
}
class Vvector3 {
public:
float X;
float Y;
float Z;
Vvector3() : X(0), Y(0), Z(0) {}
Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
Vvector3(const Vvector3 &v);
~Vvector3();
};

Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}

__attribute__((visibility("hidden")))
uintptr_t string2Offset(const char* s) {
    using conv_fn_t = unsigned long (*)(const char*, char**, int);
    conv_fn_t conv_fn = reinterpret_cast<conv_fn_t>(dlsym(RTLD_DEFAULT, OBFUSCATE("strtoul")));
    if constexpr (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return conv_fn(s, nullptr, 16);
    } else {
        using conv64_fn_t = unsigned long long (*)(const char*, char**, int);
        auto conv64_fn = reinterpret_cast<conv64_fn_t>(dlsym(RTLD_DEFAULT, OBFUSCATE("strtoull")));
        return conv64_fn(s, nullptr, 16);
    }
}

uintptr_t basePtr12,basePtr13,basePtr14,basePtr15;

__attribute__((visibility("hidden")))
auto LocalizarInderecoBase(const char* lib)
{
    uintptr_t InderecoBase = 0;
    char line[1024];
    char filename[0xFF] = {0};
    sprintf(filename,OBFUSCATE("/proc/self/maps"));
    FILE* fp = fopen(filename, OBFUSCATE("re"));
    if(fp) {
        while(fgets(line, sizeof line, fp)) {
            if(strstr(line, lib)) {
                InderecoBase = std::stoul(line, nullptr, 16);
                return InderecoBase;
            }
        }
    }
    return InderecoBase;
}


__attribute__((visibility("hidden")))
void* getAddressIL2CPP(uintptr_t relativeAddr2, bool recheck2 = false)
{
    while(basePtr13 == 0)
    {
        basePtr13 = LocalizarInderecoBase(OBFUSCATE("libil2cpp.so"));
        //LOGD(WRAPPER_MARCO("basePtr1: %p"), basePtr1);
    }
    if(recheck2)
        basePtr13 = LocalizarInderecoBase(OBFUSCATE("libil2cpp.so"));
    return (void*)(basePtr13 + relativeAddr2);
}

std::string UTF16ToUTF8(const std::u16string& src) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(src);
}

std::string GetNameFromPlayer(void* player) {
    if (!player) return "Player";
    auto get_NickName = (void* (*)(void*))Il2CppGetMethodOffset("Assembly-CSharp.dll","COW.GamePlay","Player","get_NickName",0);
    if (!get_NickName) return "Player";
    void* nickNamePtr = get_NickName(player);
    if (!nickNamePtr) return "Player";
    int length = *(int*)((uintptr_t)nickNamePtr + 0x10);
    if (length <= 0) return "Player";
    std::u16string str16((char16_t*)((uintptr_t)nickNamePtr + 0x14), length);
    return UTF16ToUTF8(str16);
}

#define Class_Camera__get_main (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_main"))

void* get_main() {
    return reinterpret_cast<void* (__fastcall*)()>(Class_Camera__get_main)();
}

#define Class_Input__get_touchCount (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_touchCount"))

#define Class_Input__GetTouch (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1)

#define Class_Input__get_mousePosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("get_mousePosition"))

#define Class_Screen__get_width (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_width"))
int get_width() {
    return reinterpret_cast<int(__fastcall*)()>(Class_Screen__get_width)();
}

#define Class_Screen__get_height (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_height"))
int get_height() {
    return reinterpret_cast<int(__fastcall*)()>(Class_Screen__get_height)();
}

#define Class_Screen__get_density (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Screen"), OBFUSCATE("get_dpi"))

#define Camera_get_fieldOfView (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("get_fieldOfView"))
float get_fieldOfView() {
    return reinterpret_cast<float(__fastcall*)(void*)>(Camera_get_fieldOfView)(get_main());
}

#define Camera_set_fieldOfView (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("set_fieldOfView"), 1)
void* set_fieldOfView(float value) {
    return reinterpret_cast<void* (__fastcall*)(void*, float)>(Camera_set_fieldOfView)(get_main(), value);
}

#define ForWard (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_forward"), 0)
static Vector3 GetForward(void* player) {
    Vector3(*_GetForward)(void* players) = (Vector3(*)(void*))(ForWard);
    return _GetForward(player);
}

#define Class_Transform__GetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
static Vector3 Transform_GetPosition(void* player) {
    Vector3 out = Vector3::zero();
    void (*_Transform_GetPosition)(void* transform, Vector3 * out) = (void (*)(void*, Vector3*))(Class_Transform__GetPosition);
    _Transform_GetPosition(player, &out);
    return out;
}

#define Class_Transform__SetPosition Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("set_position_Injected"), 1)
static void Transform_INTERNAL_SetPosition(void* player, Vvector3 inn) {
    void (*Transform_INTERNAL_SetPosition)(void* transform, Vvector3 in) = (void (*)(void*, Vvector3))(Class_Transform__SetPosition);
    Transform_INTERNAL_SetPosition(player, inn);
}

#define Class_Transform__Position Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position"), 0)
Vector3 get_position(void* player) {
    Vector3(*_get_position)(void* players) = (Vector3(*)(void*))(Class_Transform__Position);
    return _get_position(player);
}

#define Class_Transform__Rotation Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_rotation"), 0)
static Quaternion GetRotation(void* player) {
Quaternion (*_GetRotation)(void* players) = (Quaternion(*)(void *))(Class_Transform__Rotation);
return _GetRotation(player);
}

#define Class_Camera__WorldToScreenPoint (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Camera"), OBFUSCATE("WorldToScreenPoint"), 1)
static Vector3 WorldToScreenPoint(void* WorldCam, Vector3 WorldPos) {
    Vector3(*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector3(*)(void*, Vector3)) (Class_Camera__WorldToScreenPoint);
    return _WorldToScreenScene(WorldCam, WorldPos);
}

// freefireth: match class JMAGGLCNGIG (dump.cs TypeDefIndex 32130)
#define ListPlayer (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("NOLEDCGOJKC"))

// Dictionary<BEADLMGGGGL, Player> @ 0x128 / 0x130 / 0x140 / 0x150
#define ListPlayer_B (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("NJCGFLANJLH"))
#define ListPlayer_C (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("GOFLDILNLBA"))
#define ListPlayer_D (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JMAGGLCNGIG"), OBFUSCATE("KAENAIKJLDD"))

static void CollectPlayers(void* match, void** out, int& outCount) {
    outCount = 0;
    if (!match) return;
    const uintptr_t offs[4] = {ListPlayer, ListPlayer_B, ListPlayer_C, ListPlayer_D};
    for (int k = 0; k < 4; ++k) {
        if (offs[k] == (uintptr_t)-1) continue;
        MyDictionary* dict = *(MyDictionary**)((uintptr_t)match + offs[k]);
        if (!dict || !dict->entries) continue;
        int total = dict->count;
        if (total < 0 || total > 512) continue;
        if (total > dict->entries->max_length) total = dict->entries->max_length;
        for (int i = 0; i < total; ++i) {
            DictionaryEntry entry = dict->entries->vector[i];
            if (entry.hashCode < 0) continue;
            void* p = entry.value;
            if (!p) continue;
            bool dup = false;
            for (int j = 0; j < outCount; ++j) if (out[j] == p) { dup = true; break; }
            if (!dup && outCount < 256) out[outCount++] = p;
        }
    }
}

#define EnemyUpdate (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("LateUpdate"), 0)

#define MainCam (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("MainCameraTransform"))
static Vector3 CameraMain(void* player) {
    return get_position(*(void**)((uint64_t)player + MainCam));
}

#define Match (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentMatch"), 0)
static void* Curent_Match() {
    void* (*_Curent_Match) (void* nuls) = (void* (*)(void*))(Match);
    return _Curent_Match(NULL);
}

#define Local (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("UIHudDetectorController"), OBFUSCATE("GetLocalPlayer"), 0)
static void* GetLocalPlayer(void* Game) {
    void* (*_GetLocalPlayer)(void* match) = (void* (*)(void*))(Local);
    return _GetLocalPlayer(Game);
}

#define Visible (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsVisible"), 0)
static bool get_isVisible(void* player) {
    bool (*_get_isVisible)(void* players) = (bool (*)(void*))(Visible);
    return _get_isVisible(player);
}

#define Team (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsLocalTeammate"), 1)
static bool get_isLocalTeam(void* player) {
    using fnGetIsLocalTeam = bool(*)(void*, bool);
    auto _get_isLocalTeam = reinterpret_cast<fnGetIsLocalTeam>(Team);
    return _get_isLocalTeam(player, false);
}

#define Die (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsDieing"), 0)
static bool get_IsDieing(void* player) {
    bool (*_get_die)(void* players) = (bool (*)(void*))(Die);
    return _get_die(player);
}

#define CurHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_CurHP"), 0)
static int GetHp(void* player) {
    int (*_GetHp)(void* players) = (int(*)(void*))(CurHP);
    return _GetHp(player);
}

#define MaxHP (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_MaxHP"), 0)
static int get_MaxHP(void* enemy) {
    int (*_get_MaxHP)(void* player) = (int(*)(void*))(MaxHP);
    return _get_MaxHP(enemy);
}

#define Name (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_NickName"), 0)
static monoString* get_NickName(void* player) {
    monoString* (*_get_NickName)(void* players) = (monoString * (*)(void*))(Name);
    return _get_NickName(player);
}

#define Aim (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("SetAimRotation"), 2)
static void set_aim(void* player, Quaternion look) {
    using fnSetAim = void(*)(void*, Quaternion, bool);
    auto _set_aim = reinterpret_cast<fnSetAim>(Aim);

    _set_aim(player, look, false);
}

#define LAimCollider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("AttackableEntity"), OBFUSCATE("set_LockedAimingCollider"), 1)
static void SetAimCollider(void* enemy, void* collider) {
    using fnSetCollider = void(*)(void*, void*);
    auto _set_collider = reinterpret_cast<fnSetCollider>(LAimCollider);
    _set_collider(enemy, collider);
}

#define Scope (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsSighting"),0 )
static bool get_IsSighting(void* player) {
    bool (*_get_IsSighting)(void* players) = (bool (*)(void*))(Scope);
    return _get_IsSighting(player);
}

#define Fire (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsFiring"), 0)
static bool get_IsFiring(void* player) {
    bool (*_get_IsFiring)(void* players) = (bool (*)(void*))(Fire);
    return _get_IsFiring(player);
}

#define Head (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHeadTF"), 0)
static void* GetHeadPositions(void* player) {
    void* (*_GetHeadPositions)(void* players) = (void* (*)(void*))(Head);
    return _GetHeadPositions(player);
}

#define CharGet (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("mscorlib.dll"), OBFUSCATE("System"), OBFUSCATE("String"), OBFUSCATE("get_Chars"), 1)
char get_Chars(monoString* str, int index) {
    char (*_get_Chars)(monoString * str, int index) = (char (*)(monoString*, int))(CharGet);
    return _get_Chars(str, index);
}



#define HeadColider (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_HeadCollider"))
static void* Player_GetHeadCollider(void* player) {
    void* (*_Player_GetHeadCollider)(void* players) = (void* (*)(void*))(HeadColider);
    return _Player_GetHeadCollider(player);
}

#define Hip (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetHipTF"), 0)
static void* GetHipPositions(void* player) {
void* (*_GetHipPositions)(void* players) = (void*(*)(void*))(Hip);
return _GetHipPositions(player);
}

#define Class_Compent__Transform Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_transform"), 0)
static void *Component_GetTransform(void *player) {
void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))(Class_Compent__Transform);
return _Component_GetTransform(player);
}


static void *Camera_main() {
void *(*_Camera_main)(void *nuls) = (void *(*)(void *))(Class_Camera__get_main);
return _Camera_main(nullptr);
}

Vector3 getPosition(void *transform) {
return get_position(Component_GetTransform(transform));
}
static Vector3 GetHeadPosition(void* player) {
return get_position(GetHeadPositions(player));
}
static Vector3 GetHipPosition(void* player) {
return get_position(GetHipPositions(player));
}

float get_density() {
return reinterpret_cast<float(__fastcall *)()>(Class_Screen__get_density)();
}


#define Imo (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetActiveWeapon"))
static void* get_imo(void* player) {
    void* (*_GetImo)(void* players) = (void* (*)(void*))Imo;
    return _GetImo(player);
}

#define Esp2 (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelMiniSentry"), OBFUSCATE("KGFPDMPEJAM"), 2)
static void set_esp2(void* imo, Vector3 x, Vector3 y) {
    void (*_SetEsp2)(void* imo, Vector3 X, Vector3 Y) = (void (*)(void*, Vector3, Vector3))Esp2;
    _SetEsp2(imo, x, y);
}

#define CenterWS (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetAttackableCenterWS"))

static Vector3 GetAttackableCenterWS(void* player) {
    Vector3 (*_GetAttackableCenterWS)(void*) = (Vector3 (*)(void*))CenterWS;
    return _GetAttackableCenterWS(player);
}

#define Raycast (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("NCDPPPKFHHO"), OBFUSCATE("MOPLNHJHDOH"), 4)
static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
    bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool(*)(Vector3, Vector3, unsigned int, void*))(Raycast);
    return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

#define TranGetPosition (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Transform"), OBFUSCATE("get_position_Injected"), 1)
static Vector3 Transform_INTERNAL_GetPosition(void *player) {
    Vector3 out = Vector3::zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))(TranGetPosition);
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

// 武器射程
#define offset_JDGGIFMKIKF (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("LLKBKJNHFAO"), OBFUSCATE("CFDNKILHLNC"), 0)

static float get_Range(void *pthis)
{
    return ((float (*)(void *))(offset_JDGGIFMKIKF))(pthis);
}
	
#define WeaponOnHand (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("GetWeaponOnHand"))
void *GetWeaponOnHand(void *player)
{
    return ((void* (*)(void*))(WeaponOnHand))(player);
}



#define offset_isGod (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsGod"), 0)
static bool get_God(void *player) {
    bool (*_get_God)(void *players) = (bool (*)(void *))(offset_isGod);
    return _get_God(player);
}

#define m_Fixed (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("m_FixedDeltaTime"))
#define m_Simulation (uintptr_t) Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("BaseGame"), OBFUSCATE("m_SimulationTimer"))
#define m_Timer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameSimulationTimer"), 0)
static void* GetSimulationTimer()
{
    void* (*_GetSimulationTimer) () = reinterpret_cast<void* (*)()>(m_Timer);
    return _GetSimulationTimer();
}

#define m_GetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("get_FixedDeltaTime"), 0)
static float GetTimer(void* timeServiceInstance)
{
    using fnGetFixedDelta = float(*)(void*);
    auto _GetTimer = reinterpret_cast<fnGetFixedDelta>(m_GetTimer);
    return _GetTimer(timeServiceInstance);
}

#define m_SetTimer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("GCommon"), OBFUSCATE("TimeService"), OBFUSCATE("UseFixedDeltaTime"), 1)
static void SetTimer(void* timeServiceInstance, float fixedDelta)
{
    using fnSetFixed = void(*)(void*, float);
    auto _set_fixed = reinterpret_cast<fnSetFixed>(m_SetTimer);
    _set_fixed(timeServiceInstance, fixedDelta);
}



Vector3 GetForwardAdjusted(void* player, float adjustAngle = 0.0f) {
    Vector3 forward = GetForward(player);

    float len = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (len > 0.0001f)
        forward = forward / len;

    if (std::abs(adjustAngle) > 0.0001f) {
        float rad = adjustAngle * (M_PI / 180.0f);
        float cosA = cos(rad);
        float sinA = sin(rad);

        float newX = forward.x * cosA - forward.z * sinA;
        float newZ = forward.x * sinA + forward.z * cosA;

        forward.x = newX;
        forward.z = newZ;
    }

    return forward;
}

#define Crush (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("IsCrouching"), 0)


static bool get_IsCrouching(void *player) {
    bool (*_get_IsCrouching)(void *players) = (bool (*)(void *))(Crush);
    return _get_IsCrouching(player);
}
