#pragma once
#include "importz.h"
#include "aimbot.h"
#include <cstdint>
#include <algorithm>
#include <string>
#include <imagem.h>


bool EspWeaponIcon = false;

ImVec4 LineCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 BoxCol  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 NameCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 DistCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

bool EspCount = true;
int LinePos = 0;
int HealthPos = 0;
int TextStyle = 0;

float LineScale = 1.0f;
float BoxScale = 1.0f;



void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    void* current_Match = Curent_Match();
    void* local_player = GetLocalPlayer(current_Match);
    if (!local_player || !current_Match) return;

    void* camera = Camera_main();

    if (!camera) return;

    void* playerList[256];
    int playerCount = 0;
    CollectPlayers(current_Match, playerList, playerCount);

    int totalEnemiesAlive = 0;

    for (int i = 0; i < playerCount; ++i) {
        void* enemy = playerList[i];
        if (!enemy || enemy == local_player) continue;
        if (get_isLocalTeam(enemy)) continue;
        if (get_IsDieing(enemy)) continue;

        Vector3 toePos = getPosition(enemy);
        Vector3 toeScreen = WorldToScreenPoint(camera, toePos);
        if (toeScreen.z > 1) {
            totalEnemiesAlive++;
        }
    }

    if (EspCount) { 
        char displaySharedText[32];
        bool noEnemies = (totalEnemiesAlive == 0);

        if (noEnemies) {
            sprintf(displaySharedText, "SAFE");
        } else {
            sprintf(displaySharedText, "%d", totalEnemiesAlive);
        }

        ImFont* font = ImGui::GetFont();
        float bigFontSize = ImGui::GetFontSize() * (noEnemies ? 1.35f : 1.65f); 
        ImVec2 textSize = font->CalcTextSizeA(bigFontSize, 1e9f, 0.0f, displaySharedText);

        ImVec2 textPos((screenWidth - textSize.x) * 0.5f, 25.0f);

        ImU32 textClr = noEnemies ? IM_COL32(40, 255, 80, 255) : IM_COL32(235, 35, 35, 255);

        draw->AddText(font, bigFontSize, ImVec2(textPos.x + 1.5f, textPos.y + 1.5f), IM_COL32(0, 0, 0, 200), displaySharedText);
        draw->AddText(font, bigFontSize, textPos, textClr, displaySharedText);
    }

    for (int i = 0; i < playerCount; ++i) {
        void* enemy = playerList[i];
        if (!enemy || enemy == local_player) continue;
        if (get_isLocalTeam(enemy)) continue;
        if (!get_isVisible(enemy)) continue;

        Vector3 toePos = getPosition(enemy);
        Vector3 toeScreen = WorldToScreenPoint(camera, toePos);
        if (toeScreen.z < 1) continue;

        Vector3 headPos = toePos + Vector3(0, 1.7f, 0);
        Vector3 headScreen = WorldToScreenPoint(camera, headPos);
        if (headScreen.z < 1) continue;

        float distance = Vector3::Distance(getPosition(local_player), headPos);  
                
        float height = abs(headScreen.y - toeScreen.y) * (1.2f / 1.1f);
        float width = height * 0.5f;
        Rect rect(headScreen.x - width / 2.f, screenHeight - headScreen.y, width, height);

        bool isKnocked = get_IsDieing(enemy);

        ImColor lineColor = isKnocked ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : LineCol;
        ImColor BoxColor = isKnocked ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : BoxCol;

        if (EspLine) {
            ImVec2 startPoint(screenWidth * 0.5f, screenHeight);
            ImVec2 hitPoint(rect.x + rect.w * 0.5f, rect.y + rect.h);
            draw->AddLine(startPoint, hitPoint, lineColor, LineScale);
        }
        
    /*    if (EspWeaponIcon) {
            void* weaponObj = HawkerGetWeaponHand(enemy); 
            int currentWeaponID = 0;
            if (weaponObj != nullptr) {
                currentWeaponID = *(int*)((uintptr_t)weaponObj + m_WeaponID);
            }
            TextureInfo activeWeapon = GetFreeFireWeaponTexture(currentWeaponID);
            if (activeWeapon.textureId != 0) {
                float iconW = 55.0f; 
                float iconH = 30.0f;

                int renderX = (int)(rect.x + (rect.w / 2.0f) - (iconW / 2.0f));
                int renderY = (int)(rect.y + rect.h + 5.0f); 

                DrawImage(renderX, renderY, (int)iconW, (int)iconH, activeWeapon.textureId);
            }
        }*/

        if (EspBox) {
            float x = rect.x; float y = rect.y; float w = rect.w; float h = rect.h;
            float thickness = BoxScale;
            ImVec2 min(x, y); ImVec2 max(x + w, y + h);

            ImU32 fullFillColor = isKnocked ? IM_COL32(255, 0, 0, 40) : IM_COL32(0, 0, 0, 60);
            draw->AddRectFilled(min, max, fullFillColor);

            if (isKnocked) {
                draw->AddRect(min, max, IM_COL32(255, 0, 0, 255), 0.0f, 0, thickness);
            } else {
                ImU32 cornerColor = BoxColor; float cornerLen = 10.0f;
                draw->AddLine(ImVec2(x, y), ImVec2(x + cornerLen, y), cornerColor, thickness);
                draw->AddLine(ImVec2(x, y), ImVec2(x, y + cornerLen), cornerColor, thickness);
                draw->AddLine(ImVec2(x + w, y), ImVec2(x + w - cornerLen, y), cornerColor, thickness);
                draw->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + cornerLen), cornerColor, thickness);
                draw->AddLine(ImVec2(x, y + h), ImVec2(x + cornerLen, y + h), cornerColor, thickness);
                draw->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - cornerLen), cornerColor, thickness);
                draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - cornerLen, y + h), cornerColor, thickness);
                draw->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - cornerLen), cornerColor, thickness);
            }
        }
    
        if (Nick) {      
            std::string enemyName = GetNameFromPlayer(enemy);      
            if (enemyName.empty()) enemyName = "Unknown";      
            if (enemyName.size() > 999) enemyName = enemyName.substr(0, 999) + "...";      

            ImFont* font = ImGui::GetFont(); float scaledFontSize = ImGui::GetFontSize() * 0.55f;      
            ImVec2 nameSize = font->CalcTextSizeA(scaledFontSize, 1e9f, 0.0f, enemyName.c_str());      
            ImVec2 namePos(rect.x + rect.w * 0.5f - (nameSize.x * 0.5f), rect.y - nameSize.y - 2.0f);      

            draw->AddText(font, scaledFontSize, namePos, ImGui::GetColorU32(NameCol), enemyName.c_str());
            if (TextStyle == 1) {      
                draw->AddRectFilled(ImVec2(namePos.x - 3, namePos.y - 2), ImVec2(namePos.x + nameSize.x + 3, namePos.y + nameSize.y + 2), IM_COL32(0, 0, 0, 120), 2.0f);      
            }      
        }      
        
        if (Dist) {      
            char distanceText[32]; sprintf(distanceText, "%.2fm", distance);      
            ImFont* font = ImGui::GetFont(); float scaledFontSize = ImGui::GetFontSize() * 0.55f;      
            ImVec2 distSize = font->CalcTextSizeA(scaledFontSize, 1e9f, 0.0f, distanceText);      
            ImVec2 distPos(rect.x + rect.w * 0.5f - (distSize.x * 0.5f), rect.y + rect.h + 2.0f);      

            draw->AddText(font, scaledFontSize, distPos, ImGui::GetColorU32(DistCol), distanceText);      
            if (TextStyle == 1) {      
                draw->AddRectFilled(ImVec2(distPos.x - 3, distPos.y - 2), ImVec2(distPos.x + distSize.x + 3, distPos.y + distSize.y + 2), IM_COL32(0, 0, 0, 120), 2.0f);      
            }      
        }       
        
        if (EspVida) {      
            float maxHealth = get_MaxHP(enemy);      
            float currentHealth = GetHp(enemy);      
            float healthPercentage = (maxHealth > 0.0f) ? currentHealth / maxHealth : 0.0f;      
            healthPercentage = std::clamp(healthPercentage, 0.0f, 1.0f);      

            ImVec4 healthColor = isKnocked ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f) : (healthPercentage > 0.5f ? ImLerp(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ImVec4(0.0f, 1.0f, 0.0f, 1.0f), (healthPercentage - 0.5f) / 0.5f) : ImLerp(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 0.0f, 1.0f), healthPercentage / 0.5f));      
            float barWidth = 3.5f; float spacing = 1.0f;      

            auto DrawHealthBar = [&](float xOffset) {      
                draw->AddRectFilled(ImVec2(xOffset, rect.y), ImVec2(xOffset + barWidth, rect.y + rect.h), ImColor(40, 40, 40, 150));      
                draw->AddRectFilled(ImVec2(xOffset, rect.y + rect.h - (rect.h * healthPercentage)), ImVec2(xOffset + barWidth, rect.y + rect.h), ImColor(healthColor));      
            };      
            if (HealthPos == 0) DrawHealthBar(rect.x - barWidth - spacing);      
            else if (HealthPos == 1) DrawHealthBar(rect.x + rect.w + spacing);      
        }      
    }
    
    if (EspCircle) {
        ImVec2 screenCenter(ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f);
        draw->AddCircle(screenCenter, Fov_Aim, ImColor(255, 255, 255, 255), 64, 1.0f);
    }
}


#include <jni.h>

bool AntiRecord = false;

void SetAntiRecordState(JNIEnv* env, jobject activityOrService, bool enable) {
    if (!env || !activityOrService) return;

    jclass serviceClass = env->GetObjectClass(activityOrService);
    if (!serviceClass) return;

    // 方法编号
    jmethodID getWindowMethod = env->GetMethodID(
        serviceClass, "getWindow", "()Landroid/view/Window;"
    );

    if (getWindowMethod != nullptr) {
        jobject windowObj = env->CallObjectMethod(activityOrService, getWindowMethod);
        if (windowObj != nullptr) {
            jclass windowClass = env->GetObjectClass(windowObj);
            if (windowClass != nullptr) {

                // 方法编号
                jmethodID flagMethod = enable
                    ? env->GetMethodID(windowClass, "addFlags", "(I)V")
                    : env->GetMethodID(windowClass, "clearFlags", "(I)V");

                if (flagMethod != nullptr) {
                    // 8192 是安全标记
                    env->CallVoidMethod(windowObj, flagMethod, 8192);
                }

                env->DeleteLocalRef(windowClass);
            }
            env->DeleteLocalRef(windowObj);
        }
    }

    env->DeleteLocalRef(serviceClass);
    AntiRecord = enable;
}
