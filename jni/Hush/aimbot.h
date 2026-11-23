#pragma once

#include "ESP.h"
#include "pointers.h"
#include <thread>
#include "Madrchod~Aimkill.h"


float Fov_Aim = 500;

int Aimtype = 0;
int ChestShotsBeforeHead = 3;

bool AimFire = false;
bool AimScope = false;
bool AimRush = false;
int Aimmode = 0;
float LegitSpeed = 90.0f;

bool Aimbot2 = false;
bool Aimauto2 = false;
bool AimFire2 = false;
bool Socpe2 = false;
bool Crouch2 = false;

float AimSpeed = 10.0f;
float AimHeightPercent = 100.0f;

extern int SetDamage;
extern bool HeadKill;
extern bool aimkillsend;
extern bool autochangesendnew;
extern bool AimKill2;
extern bool AimKill1;
extern int damageCooldown;
extern int delayCounter;
extern int delayLimit;
extern int delayLimit2;
extern int DamageDelay;


bool IsVisible(void* closestEnemy) {
    // freefireth: use the game's own visibility flag (Player.IsVisible, verified in dump)
    // instead of the MAX-only raycast wrapper NCDPPPKFHHO.MOPLNHJHDOH.
    if (!closestEnemy) return false;
    return get_isVisible(closestEnemy);
}

Vector3 GetAdjustedPosition(void* closestEnemy) {
    Vector3 headPos = GetHeadPosition(closestEnemy);
    if (aimPosition == 1) {
        headPos.y += NECK_OFFSET;
    } else if (aimPosition == 2) {
        Vector3 hipPos = GetHipPosition(closestEnemy);
        headPos = Vector3::Lerp(headPos, hipPos, 0.5f);
    } else if (aimPosition == 3) {
        headPos = GetHipPosition(closestEnemy);
    }
    return headPos;
}

Quaternion GetCameraRotation(void* localPlayer) {
    return GetRotation(Component_GetTransform(Camera_main()));
}

bool IsInFov(Vector3 enemyPos, Vector3 playerPos, float fovLimit) {
    float dx = enemyPos.x - playerPos.x;
    float dy = enemyPos.y - playerPos.y;
    float dz = enemyPos.z - playerPos.z;
    float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    if (distance == 0) return false;
    float angle = acosf(dx / distance) * (360.0f / 3.14159f);
    return angle <= fovLimit;
}

// 找目标

void* GetClosestEnemy() {
    float shortestDistance = MAX_DISTANCE;
    void* closestEnemy = NULL; 
    void* get_MatchGame = Curent_Match();
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    if (LocalPlayer != NULL && get_MatchGame != NULL && Startgs) {
        void* playerList[256];
        int playerCount = 0;
        CollectPlayers(get_MatchGame, playerList, playerCount);
        if (playerCount > 0) {
            for (int u = 0; u < playerCount; u++) {
                void* Player = playerList[u];
                if (Player != NULL && !get_isLocalTeam(Player) 
                    && (ignoreKnockedEnemies || !get_IsDieing(Player))
                    && get_isVisible(Player) && get_MaxHP(Player)) {
                    Vector3 PlayerPos = getPosition(Player);
                    Vector3 LocalPlayerPos = getPosition(LocalPlayer);
                    float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
                    Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                    float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * AIM_ANGLE_MULTIPLIER;
                    if (distance < Aimdis && angle <= Fov_Aim) {
                        if (angle < shortestDistance) {
                            shortestDistance = angle;
                            closestEnemy = Player;
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;
}

void* EnemyVisible(void* match) {
    if (!match || !Startgs) return nullptr;

    float shortestDistance = 99999.0f;
    float maxAngle = Fov_Aim;
    void* closestEnemy = nullptr;
    void* LocalPlayer = GetLocalPlayer(match);
    if (!LocalPlayer) return nullptr;

    void* playerList[256];
    int playerCount = 0;
    CollectPlayers(match, playerList, playerCount);
    if (playerCount == 0) return nullptr;

    for (int u = 0; u < playerCount; u++) {
        void* Player = playerList[u];
        if (!Player) continue;

        if (!get_isLocalTeam(Player)
            && !get_IsDieing(Player)
            && get_isVisible(Player)
            && get_MaxHP(Player) > 0) {

            Vector3 PlayerPos = GetHeadPosition(Player);
            Vector3 LocalPlayerPos = GetHeadPosition(LocalPlayer);
            Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);

            float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(Camera_main()))) * 100.0f;
            
            if (!get_God(Player)) {
                if (AimVisible) {
                    if (IsVisible(Player) && angle <= maxAngle && angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                } else {
                    if (angle <= maxAngle && angle < shortestDistance) {
                        shortestDistance = angle;
                        closestEnemy = Player;
                    }
                }
            }
        }
    }
    return closestEnemy;
}

// 自动瞄准

void Aimbott() {
    if (!AimbotEsp) return;
    
    void* CurrentMatch = Curent_Match();
    if (!CurrentMatch) return;  
    
    void* local_player = GetLocalPlayer(CurrentMatch);
    if (!local_player) return;  
    
    void* enemyVisible = EnemyVisible(CurrentMatch);
    if (!enemyVisible) return;  
    
    Vector3 enemyLocation = GetAdjustedPosition(enemyVisible);
    Vector3 playerLocation = CameraMain(local_player);  
    
    float distance = Vector3::Distance(playerLocation, enemyLocation);
    
    Quaternion currentRotation = GetRotation(Component_GetTransform(Camera_main()));  
    Quaternion targetRotation = GetRotationToLocation(enemyLocation, 0.1f, playerLocation);  
    
    bool isFiring = get_IsFiring(local_player);  
    bool isScope = get_IsSighting(local_player);  
    
    bool shouldAim = false;
    if (AimFire && isFiring) shouldAim = true;
    if (AimScope && isScope) shouldAim = true;
    if (AimRush && distance <= 5.0f) shouldAim = true;
    
    if (shouldAim) {
        Quaternion finalRotation = targetRotation;
        if (Aimmode == 0) {
            set_aim(local_player, targetRotation);
        }
        else if (Aimmode == 1) {
            float turnSpeed = LegitSpeed;  
            float deltaTime = ImGui::GetIO().DeltaTime;
            float maxStep = turnSpeed * deltaTime;
            finalRotation = Quaternion::RotateTowards(currentRotation, targetRotation, maxStep);
            set_aim(local_player, finalRotation);
        }
        void* WeaponHand = GetWeaponOnHand(local_player);
        if (WeaponHand != nullptr) {
        }
    }
}

void (*LateUpdate)(void* Player) = nullptr;

void _LateUpdate(void* Player) {
    if (!Player || !LateUpdate) {
        if (LateUpdate) LateUpdate(Player);
        return;
    }
    
    void* match = Curent_Match();
    if (!match) { LateUpdate(Player); return; }
    
    void* localPlayer = GetLocalPlayer(match);
    if (!localPlayer) { LateUpdate(Player); return; }
    
    void* lockedEnemy = GetClosestEnemy();
    if (!lockedEnemy) lockedEnemy = EnemyVisible(match);  // 换一个找敌人的方法
    if (lockedEnemy) {
        Vector3 head = GetHeadPosition(lockedEnemy);
        float estimatedHeight = 1.5f;
        
        if (AimHeightPercent < 100) {
            float percent = AimHeightPercent / 100.0f;
            head.y -= estimatedHeight * (1.0f - percent);
        }

        Vector3 camPos = CameraMain(localPlayer);

        if (Aimbot2 && IsVisible(lockedEnemy)) {
            bool IsScopeOn2 = get_IsSighting(localPlayer);
            bool IsFiring2 = get_IsFiring(localPlayer);
            bool IsCrouching2 = get_IsCrouching(localPlayer);
            
            if (Aimauto2 || (AimFire2 && IsFiring2) || (Socpe2 && IsScopeOn2) || (Crouch2 && IsCrouching2)) {
                Quaternion rot = GetRotationToLocation(head, AimSpeed, camPos);
                set_aim(localPlayer, rot);
            }
        }
    }
    
    void *WeaponHand = GetWeaponOnHand(localPlayer);
         
    if (WeaponHand) {
        if (AimKill1) {
            if (damageCooldown <= 0) {
                StartTakeDamage(lockedEnemy);
                // 发送伤害数据
                StartAimKillSend(lockedEnemy);
                StartonFiring(localPlayer, WeaponHand);
                damageCooldown = DamageDelay;
            } else {
                damageCooldown--;
            }
        }
    }
    
    LateUpdate(Player);
}