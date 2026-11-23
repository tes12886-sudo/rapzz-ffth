#pragma once

#include "updates.h"
#include <ctime>
#include <cstdint>


struct message_PVector3IntPos_StaticFields {};
struct message_PVector3IntPos_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCodek;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _6_Serialize;
    VirtualInvokeData _7_UnSerialize;
    VirtualInvokeData _8_CanCache;
    VirtualInvokeData _9_Recycle;
};
struct message_PVector3IntPos_c {
    Il2CppClass_1 _1;
    message_PVector3IntPos_StaticFields* static_fields;
    Il2CppClass_2 _2;
    message_PVector3IntPos_VTable vtable;
};
struct message_PVector3IntPos_o {
    message_PVector3IntPos_c *klass;
    void *monitor;
    bool UDPClientMessageBase_m_GetFromPool;
    int32_t X;
    int32_t Y;
    int32_t Z;
};

struct message_C2S_RUDP_TakeDamage_Req_o2 {
    void *klass;
    void *monitor;
    bool UDPClientMessageBase_m_GetFromPool;
    uint32_t AttackableEntityID;
    uint16_t RealDamageValue;
    uint16_t ShieldDamageValue;
    uint32_t DamageValue;
    uint32_t DamagerID;
    int32_t WeaponDataID;
    uint32_t WeaponUniqueID;
    uint8_t HitBodyPart;
    uint32_t TickCount;
    message_PVector3IntPos_o* FirePos;
    message_PVector3IntPos_o* TargetPos;
    monoList<float*> * CheckParams;
    uint32_t ExtraData;
    float ArmorRatio;
    uint64_t ExtraFlag;
    int8_t SpecialHitType;
    bool ForceNoHeadShot;
    bool IsPlayerBackArea;
    monoList<int*> * DamageData;
    float BaseTime;
    bool ManualReload;
};

struct PlayerID {
    uint32_t m_Value;
    uint32_t m_ID;
    uint8_t m_TeamID;
    uint8_t m_ShortID;
    uint64_t m_IDMask;
};

struct COW_GamePlay_IHAAMHPPLMG_o {
    uint32_t m_Value;
    uint32_t m_ID;
    uint8_t m_TeamID;
    uint8_t m_ShortID;
    uint64_t m_IDMask;
};

struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t BaseDamage;
    int32_t HitColliderType;
    monoString* HitColliderName;
    bool isBackArea;
    COW_GamePlay_IHAAMHPPLMG_o Damager;
    void* Weapon;
    int32_t WeaponDataID;
    Vector3 FirePos;
    Vector3 HitPos;
    Vector3 HitNormal;
    uint16_t SpecialHitType;  // 新版字段
    bool ForceNoHeadshot;
    int32_t ExtraInfo;
    monoDictionary<uint8_t*, void **> *SpecialHitDic;
};

struct GCommon_TimeService_StaticFields {};
struct GCommon_TimeService_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_Finalize;
    VirtualInvokeData _2_GetHashCodek;
    VirtualInvokeData _3_ToString;
};
struct GCommon_TimeService_c {
    Il2CppClass_1 _1;
    GCommon_TimeService_StaticFields* static_fields;
    Il2CppClass_2 _2;
    GCommon_TimeService_VTable vtable;
};
struct GCommon_TimeService_o {
    GCommon_TimeService_c *klass;
    void *monitor;
    float m_GameTime;
    float m_LastGameTime;
    float m_DeltaTime;
    uint32_t m_DeltaTickCount;
    uint32_t m_TickCount;
    uint32_t m_RealDoLogicTickCount;
    bool m_UsingFixedDeltaTime;
    float m_FixedDeltaTime;
};


#define offset_GetWeaponID ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("MIGICJLPKBM"), 0))
#define offset_PNGAJBCPDNJ ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PhyXShield"), OBFUSCATE("FDGFAOHICPO"), 0))
// 注意：基础伤害还没找到
#define offset_BaseDamage ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("HOBPLJGNDEA")))
#define offset_KFMGKCJMCAM ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("JDDGEGLJPPD")))
#define offset_GEGFCFDGGGP ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("AHIECPJOJOL")))
#define offset_PlayerAttributes ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("PPDMHPONIEF")))
// 还没找到
#define offset_AHEKHEAHOPP ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerAttributes"), OBFUSCATE("BBCOIBBKLGL")))
// 还没找到
#define offset_DamageBaseInfo ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("GDBBMKEIFGA"), OBFUSCATE("INBDALGGDLH")))
#define offset_SetStartDamage ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("KIGAOLLBMOB"), 1))
#define offset_JMKMBNIBFNA ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JNHPMNJDKEN"), OBFUSCATE("GNJEAJLMLOD")))
#define offset_GHACJPMCEDK ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JNHPMNJDKEN"), OBFUSCATE("FDLEJHEJIOO")))
#define offset_OJKBBAOPPIN ((uintptr_t)Il2CppGetFieldOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("JNHPMNJDKEN"), OBFUSCATE("AGNKEFKEIKN")))
#define offset_GKHECDLGAJA ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("NHPPCNBLCNM"), 1))
#define offset_LCLHHHKFCFP ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HBIBDMMOOOK"), OBFUSCATE("DECGGFMCIHM"), 4))
#define offset_CFFPIACECIG ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HMGPJFPFGNN"), OBFUSCATE("BFPOOEPICCJ"), 1))
#define offset_LHGGPDLOPAH ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("message"), OBFUSCATE("JIIDLNKDKAG"), OBFUSCATE("OOPBCCOPOMM"), 1))
#define offset_SendFunction ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("Send"), 4))
#define offset_StartFiring ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartFiring"), 1))
#define offset_StopFire ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StopFire"), 1))
#define offset_StartWholeBodyFiring ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("StartWholeBodyFiring"), 1))
#define offset_get_realtimeSinceStartup ((void*)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Time"), OBFUSCATE("get_realtimeSinceStartup"), 0))
#define offset_SimulationTimer ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentGameSimulationTimer"), 0))
#define offset_DPPPFBHIHJA ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("ffantihack"), OBFUSCATE("CLFIIKMGKHI"), OBFUSCATE("FJJKLDNGCPI"), 0))
#define Classdamageinfo Il2CppGetClassType(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HNLIMBLIANL"))
#define BNJIPPIIMDF1616 ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HNLIMBLIANL"), OBFUSCATE(".ctor"), 0))
#define offset_TakeDamage ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("TakeDamage"), 9))
#define SwapWeapon11234 ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("PlayerNetwork"), OBFUSCATE("SwapWeapon"), 2))
#define GetPlayerID111 ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_PlayerID"), 0))
#define offsetsendaimkkll ((void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HNLIMBLIANL"), OBFUSCATE(".ctor"), 0))


#define offset_LocalPlayer (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("CurrentLocalPlayer"), 0)
static void *Current_Local_Player() {
    void *(*_Local_Player)(void *players) = (void *(*)(void *))(offset_LocalPlayer);
    return _Local_Player(NULL);
}


static int GetWeapon(void* enemy) {
    int (*GetWeapon)(void *player) = (int(*)(void *))(offset_GetWeaponID);
    return GetWeapon(enemy);
}

void *GKHECDLGAJA(void *pthis, void* a1) {
    return ((void* (*)(void *,void *))(offset_GKHECDLGAJA))(pthis,a1);
}

monoList<float *> *LCLHHHKFCFP(void *Weapon,void *CAGCICACKCF,void *HFBDJJDICLN,bool LDGHPOPPPNL,DamageInfo2_o *DamageInfo) {
    return ((monoList<float *> * (*)(void*,void*,void*,bool,DamageInfo2_o*))(offset_LCLHHHKFCFP))(Weapon,CAGCICACKCF,HFBDJJDICLN,LDGHPOPPPNL,DamageInfo);
}

monoList<float *> *LCLHHHKFCFP2(void *Weapon,void *CAGCICACKCF,void *HFBDJJDICLN,bool LDGHPOPPPNL, void *DamageInfo) {
    return ((monoList<float *> * (*)(void*,void*,void*,bool,void*))(offset_LCLHHHKFCFP))(Weapon,CAGCICACKCF,HFBDJJDICLN,LDGHPOPPPNL,DamageInfo);
}

uint32_t CFFPIACECIG(COW_GamePlay_IHAAMHPPLMG_o id) {
    // 直接读编号
    return id.m_Value;
}

message_PVector3IntPos_o *PackVector3Pos(Vector3 v) {
    return ((message_PVector3IntPos_o* (*)(Vector3))offset_LHGGPDLOPAH)(v);
}

bool GameFacade_Send(uint32_t messageID,void *msg,uint8_t sendOption,bool cacheMsgAnyWay) {
    return ((bool (*)(uint32_t,void *,uint8_t,bool))offset_SendFunction)(messageID,msg,sendOption,cacheMsgAnyWay);
}

static void StartFiring(void *Player, void *Weapon) {
    void (*_StartFiring)(void *, void *) = (void (*)(void *, void *))(offset_StartFiring);
    _StartFiring(Player, Weapon);
}

static void StopFire1(void* Player,void* Weapon){
    void(*_StopFire1)(void*,void*) = (void(*)(void*,void*))(offset_StopFire);
    _StopFire1(Player,Weapon);
}

void StartWholeBodyFiring(void* Player,void* Weapon){
    void(*_StartWholeBodyFiring)(void*,void*) = (void(*)(void*,void*))(offset_StartWholeBodyFiring);
    _StartWholeBodyFiring(Player,Weapon);
}

static void StartonFiring(void *LocalPlayer, void *WeaponHand) {
    StartFiring(LocalPlayer, WeaponHand);
    StartWholeBodyFiring(LocalPlayer, WeaponHand);
    StopFire1(LocalPlayer, WeaponHand);
}

static float get_realtimeSinceStartup() {
    float (*_get_realtimeSinceStartup)(void *nuls) = (float (*)(void *))(offset_get_realtimeSinceStartup);
    return _get_realtimeSinceStartup(NULL);
}

void* CurrentGameSimulationTimer() {
    return ((void* (*)(void *))(offset_SimulationTimer))(NULL);
}

monoList<int*> *DPPPFBHIHJA() {
    return ((monoList<int*> * (*)())(offset_DPPPFBHIHJA))();
}

static int32_t TakeDamage(void *_this, int32_t KOCMLPLOILD, PlayerID HLJDHPGGODB, void* JIIJIFKKCCB, int32_t BOEIBGAABDL, Vector3 NJMFBKNHMBP, Vector3 DOBOBMFMKBJ, monoList<float *> *NBKBEBFNDBE, void* damagerWeaponDynamicInfo, uint32_t damagerVehicleID) {
    return ((int32_t (*)(void*, int32_t, PlayerID, void*, int32_t, Vector3, Vector3, monoList<float *> *, void*, uint32_t))(offset_TakeDamage))(_this, KOCMLPLOILD, HLJDHPGGODB, JIIJIFKKCCB, BOEIBGAABDL, NJMFBKNHMBP, DOBOBMFMKBJ, NBKBEBFNDBE, damagerWeaponDynamicInfo, damagerVehicleID);
}

COW_GamePlay_IHAAMHPPLMG_o GetplayerID(void *_this) {
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))(GetPlayerID111))(_this);
}

static void SwapWeapon11(void* player, int POFFNNMOOBM, bool GDKLMFLNNGM) {
    void (*_SwapWeapon11)(void* player, int POFFNNMOOBM, bool GDKLMFLNNGM) = (void (*)(void *, int, bool))(SwapWeapon11234);
    _SwapWeapon11(player, POFFNNMOOBM, GDKLMFLNNGM);
}


#define offset_get_gameObject (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.CoreModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Component"), OBFUSCATE("get_gameObject"), 0)
void *get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))(offset_get_gameObject))(Pthis);
}

namespace Save {
    void* DamageInfo = nullptr;
    clock_t AimDelay = 0;
    int AimFPS = (1000000 / 120);
}

// 用系统方法新建对象
static DamageInfo2_o* CreateDamageInfo() {
    auto obj = (DamageInfo2_o*)Il2CppCreateClassInstance(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("HNLIMBLIANL"));
    if (!obj) return nullptr;
    ((void (*)(void*))(BNJIPPIIMDF1616))((void*)obj);  // 初始化对象
    return obj;
}

int SetDamage = 1;

// freefireth guard: MAX-only internals (weapon fields, object pool, damage pack) may not
// resolve by name on TH. Bail out cleanly instead of dereferencing offset -1 / null.
static inline bool _okm(void* p) { return p != nullptr; }
static inline bool _okf(uintptr_t o) { return o != (uintptr_t)-1 && o != 0; }
static bool AimKillOffsetsReady() {
    return _okm(offset_GetWeaponID) && _okf(offset_BaseDamage) && _okf(offset_GEGFCFDGGGP)
        && _okf(offset_KFMGKCJMCAM) && _okm(offset_SetStartDamage) && _okm(offset_PNGAJBCPDNJ)
        && _okf(offset_JMKMBNIBFNA) && _okf(offset_GHACJPMCEDK) && _okf(offset_OJKBBAOPPIN)
        && _okm(offset_GKHECDLGAJA) && _okm(offset_LCLHHHKFCFP) && _okm(offset_TakeDamage)
        && _okm((void*)Classdamageinfo) && _okm(offset_LHGGPDLOPAH) && _okm(offset_SendFunction);
}

bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon) {
    if (player != nullptr && enemy != nullptr && weapon != nullptr) {
        if (!_okm((void*)offset_JDGGIFMKIKF)) return false;
        Vector3 EnemyHeadPosition = GetHeadPosition(enemy);
        Vector3 PlayerHeadPosition = GetHeadPosition(player);
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float range = get_Range(weapon);
        if (distance <= range) return true;
    }
    return false;
}

void StartTakeDamage(void *ClosestEnemy) {
    if (ClosestEnemy == nullptr || !Aimbot) return;
    if (!AimKillOffsetsReady()) return;
    void* trandau = Curent_Match();
    if (!trandau) return;
    void* LocalPlayer = GetLocalPlayer(trandau);
    if (!LocalPlayer) return;
    if (get_IsDieing(LocalPlayer) || get_IsDieing(ClosestEnemy)) return;

    void *WeaponHand = GetWeaponOnHand(LocalPlayer);
    if (!WeaponHand) return;

    // 防止崩溃
    int baseDamage = (offset_BaseDamage == (uintptr_t)-1) ? 0 : *(int*)((uintptr_t)WeaponHand + offset_BaseDamage);
    if (baseDamage <= 0) baseDamage = 100;

    int weaponID = GetWeapon(WeaponHand);
    void* objectPool = *(void**)((uintptr_t)LocalPlayer + offset_GEGFCFDGGGP);
    if (!objectPool) return;

    void* headCollider = Player_GetHeadCollider(ClosestEnemy);
    if (!headCollider) return;

    if (!isEnemyInRangeWeapon(LocalPlayer, ClosestEnemy, WeaponHand)) return;

    void *Class_message_DamageInfo_c = Classdamageinfo;
    if (!Class_message_DamageInfo_c) return;

    auto DamageInfo = CreateDamageInfo();  // 新对象
    if (!DamageInfo) return;

    auto damagePacket = (message_C2S_RUDP_TakeDamage_Req_o2*)((void* (*)(void*))(offset_PNGAJBCPDNJ))(LocalPlayer);
    if (!damagePacket) return;

    Vector3 firePos = GetHeadPosition(LocalPlayer);
    Vector3 hitPos  = GetHeadPosition(ClosestEnemy);
    auto PlayerID2  = *(PlayerID *)((uintptr_t)LocalPlayer + offset_KFMGKCJMCAM);

    ((void (*)(void*, void*))offset_SetStartDamage)(WeaponHand, objectPool);
    ((void* (*)(void*))offset_PNGAJBCPDNJ)(LocalPlayer);

    DamageInfo->BaseDamage      = baseDamage;
    DamageInfo->HitColliderType = 1;
    DamageInfo->Weapon          = WeaponHand;
    DamageInfo->WeaponDataID    = weaponID;
    DamageInfo->FirePos         = firePos;
    DamageInfo->HitPos          = hitPos;
    DamageInfo->SpecialHitType  = 0;
    DamageInfo->ForceNoHeadshot = false;
    DamageInfo->ExtraInfo       = 0;
    DamageInfo->isBackArea      = false;
    DamageInfo->Damager         = GetplayerID(LocalPlayer);

    *(void**)((uintptr_t)objectPool + offset_JMKMBNIBFNA) = get_gameObject(headCollider);
    *(void**)((uintptr_t)objectPool + offset_GHACJPMCEDK) = headCollider;
    *(int*)((uintptr_t)objectPool + offset_OJKBBAOPPIN)   = 1;

    void* sl3 = GKHECDLGAJA(LocalPlayer, objectPool);
    if (!sl3) return;

    monoList<float*>* paramCheck = LCLHHHKFCFP(WeaponHand, sl3, headCollider, false, DamageInfo);
    if (!paramCheck) return;

    GCommon_TimeService_o *GameSimulation = (GCommon_TimeService_o *)CurrentGameSimulationTimer();
    if (!GameSimulation) return;

    TakeDamage(ClosestEnemy, baseDamage, PlayerID2, DamageInfo, weaponID,
               firePos, hitPos, paramCheck, nullptr, 0);

    StartonFiring(LocalPlayer, WeaponHand);
}

void StartAimKillSend(void* ClosestEnemy) {
    if (ClosestEnemy == nullptr || !Aimbot) return;
    if (!AimKillOffsetsReady()) return;

    void* localPlayer = Current_Local_Player();
    if (!localPlayer) return;
    if (get_IsDieing(localPlayer) || get_IsDieing(ClosestEnemy)) return;

    void* weapon = GetWeaponOnHand(localPlayer);
    if (!weapon) return;

    // 防止崩溃
    int baseDamage = (offset_BaseDamage == (uintptr_t)-1) ? 0 : *(int*)((uintptr_t)weapon + offset_BaseDamage);
    if (baseDamage <= 0) baseDamage = 100;

    void* objectPool = *(void**)((uintptr_t)localPlayer + offset_GEGFCFDGGGP);
    if (!objectPool) return;

    void* headCollider = Player_GetHeadCollider(ClosestEnemy);
    if (!headCollider) return;

    if (!isEnemyInRangeWeapon(localPlayer, ClosestEnemy, weapon)) return;

    void *Class_message_DamageInfo_c = Classdamageinfo;
    if (!Class_message_DamageInfo_c) return;

    auto damagePacket = (message_C2S_RUDP_TakeDamage_Req_o2*)((void* (*)(void*))offset_PNGAJBCPDNJ)(localPlayer);
    if (!damagePacket) return;

    auto damageBaseInfo = CreateDamageInfo();  // 新对象
    if (!damageBaseInfo) return;

    Vector3 firePos = GetHeadPosition(localPlayer);
    Vector3 hitPos  = GetHeadPosition(ClosestEnemy);
    int weaponID    = GetWeapon(weapon);

    ((void (*)(void*, void*))offset_SetStartDamage)(weapon, objectPool);
    ((void* (*)(void*))offset_PNGAJBCPDNJ)(localPlayer);

    damageBaseInfo->BaseDamage      = baseDamage;
    damageBaseInfo->HitColliderType = 1;
    damageBaseInfo->Weapon          = weapon;
    damageBaseInfo->WeaponDataID    = weaponID;
    damageBaseInfo->FirePos         = firePos;
    damageBaseInfo->HitPos          = hitPos;
    damageBaseInfo->Damager         = GetplayerID(localPlayer);
    damageBaseInfo->SpecialHitType  = 0;
    damageBaseInfo->ForceNoHeadshot = false;

    *(void**)((uintptr_t)objectPool + offset_JMKMBNIBFNA) = get_gameObject(headCollider);
    *(void**)((uintptr_t)objectPool + offset_GHACJPMCEDK) = headCollider;
    *(int*)((uintptr_t)objectPool + offset_OJKBBAOPPIN)   = 1;

    void* sl3 = GKHECDLGAJA(localPlayer, objectPool);
    if (!sl3) return;

    monoList<float*>* paramCheck = LCLHHHKFCFP(weapon, sl3, headCollider, false, damageBaseInfo);
    if (!paramCheck) return;

    damagePacket->AttackableEntityID = CFFPIACECIG(GetplayerID(ClosestEnemy));
    damagePacket->DamagerID          = CFFPIACECIG(GetplayerID(localPlayer));
    damagePacket->DamageValue        = baseDamage;
    damagePacket->RealDamageValue    = (uint16_t)baseDamage;
    damagePacket->ShieldDamageValue  = 0;
    damagePacket->HitBodyPart        = SetDamage;
    damagePacket->WeaponDataID       = weaponID;
    damagePacket->WeaponUniqueID     = *(uint32_t*)((uint64_t)weapon + 0x10);
    damagePacket->TickCount          = 0;
    damagePacket->FirePos            = PackVector3Pos(firePos);
    damagePacket->TargetPos          = PackVector3Pos(hitPos);
    damagePacket->CheckParams        = paramCheck;
    damagePacket->ExtraData          = 0;
    damagePacket->ArmorRatio         = 1.0f;
    damagePacket->ExtraFlag          = 0;
    damagePacket->SpecialHitType     = 0;
    damagePacket->ForceNoHeadShot    = false;
    damagePacket->IsPlayerBackArea   = false;
    damagePacket->DamageData         = nullptr;
    damagePacket->BaseTime           = 0.0f;
    damagePacket->ManualReload       = false;

    if (!damagePacket->FirePos || !damagePacket->TargetPos) return;

    GameFacade_Send(106, damagePacket, 2, 0);
}

int damageCooldown = 0;
int delayCounter   = 0;
int delayLimit     = 2;  // 安全又快
int delayLimit2    = 0;
int DamageDelay    = 2;  // 安全又快

bool AimKill1;
