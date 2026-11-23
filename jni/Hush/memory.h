#pragma once
//#include "KittyMemory/MemoryPatch.hpp"


const float MAX_DISTANCE = 99999.0f;
const float AIM_ANGLE_MULTIPLIER = 70.0f;
static int aimPosition = 0;
const float NECK_OFFSET = -0.15f;
const float CHEST_OFFSET = -0.35f;
static int aimSmoothing = 0;
static bool ignoreKnockedEnemies = false;
float Aimdis = 150;

inline bool IsValidPointer(void* ptr) {
    return ptr != nullptr && ptr > (void*)0x10000 && ptr < (void*)0x7FFFFFFF0000;
}

bool Aimbot = false;
bool AimVisible = false;
bool aimbotAim = false;
bool aimbotAuto = false;

float Fov = 0;
//int Aimdis = 130;
bool EnableEsp = false;
bool EspNick = false;
bool Startgs = true;
bool Funcoes = true;
bool AimbotEsp = false;
bool AimSilent = false;
bool TelekillTeste = false;
bool EspLine = false;
bool EspBox = false;
bool EspDistancia = false;
bool EspCircle = false;
bool EspVida = false;
bool EspInfo = false;
bool Nick = false;
bool Dist = false;
bool ResetGuest1 = false;
bool SpeedHack = false;
bool GhostHack = false;
const int TOP_LEFT = 0;
const int TOP = 1;
const int BOTTOM = 2;
const int CENTER = 3;
const int DISABLED = 4;
int currentLinePosition = TOP;

const int BOX_TYPE_1 = 0;
const int BOX_TYPE_2 = 1;
const int BOX_TYPE_3 = 2;
const int BOX_TYPE_4 = 3;
const int BOX_DISABLED = 4;
int currentBoxType = BOX_TYPE_1;

int selectedStyle = 1;
static int currentStyleIndex = 0;

bool Camera = true;
bool Aaa = false;
bool Unlock120 = false;


// 瞄准击杀
bool AimKill = false;
static int TimerTakeDamageinit = 0;
float maxDistanceAimkill = 100.0f;


bool(*CAMERAV)(void* camera); 
bool _CAMERAV(void* camera) { 
if (Camera) {
return true;
}
return CAMERAV(camera);
}

bool(*CAMERAVV)(void* aaa); 
bool _CAMERAVV(void* aaa) { 
if (Aaa) {
return true;
}
return CAMERAV(aaa);
}


int (*souza_ori)(void * a1, int *a2, char *a3, socklen_t a4);
int souza_cry(void * al, int *a2, char *a3, socklen_t a4){
return 0;
}





bool Paraquedas;
bool(*PARAQUEDAS)(void* paraqueda); 
bool _PARAQUEDAS(void* paraqueda) { 
if (Paraquedas) {
return true;
}
return PARAQUEDAS(paraqueda);
}
/*
bool Bala;

MemoryPatch magica; 


if (Bala) {
magica.Modify();
} else {
magica.Restore();
}
*
uintptr_t string2Offset(const char *c) {
int base = 16;
static_assert(sizeof(uintptr_t) == sizeof(unsigned long) || sizeof(uintptr_t) == sizeof(unsigned long long), "Please add string to handle conversion for this architecture.");
if (sizeof(uintptr_t) == sizeof(unsigned long)) {
return strtoul(c, nullptr, base);
}
return strtoull(c, nullptr, base);
}
std::string int_to_string(int num) {
std::string str = std::to_string(num);
return str;
}

/*
int (*orig_doni)(void *a1, int *a2, char *a3, socklen_t a4);
int hook_doni(void *a1, int *a2, char *a3, socklen_t a4) {
return 0;
}
*/






