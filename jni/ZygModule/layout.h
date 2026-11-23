#include <imports.h>
#include <imguiconfig.h>
#include "ESP.h"
#include "particles.h"
#include "Firewall1.h"
#include "aimbot.h"
#include <string>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <vector>

#ifndef OBFUSCATE
#define OBFUSCATE(str) str
#endif

#define rgba(r,g,b,a) ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f)




static bool g_isMenuVisible = true;
static bool g_DarkMode = true; 
static int activeTab = 1; 

static bool g_ShowFloatingMenu = false; 
static bool g_FirstLoadNotifyDone = false; 
static bool g_DisclaimerAccepted = false;

struct iOSNotification {
    std::string title;
    std::string message;
    double timestamp;
    float duration;
    ImU32 accentColor;
};
static std::vector<iOSNotification> g_Notifications;

static float g_TabAnimTarget = 0.0f;  
static float g_TabAnimCurrent = 0.0f; 
static float g_TabAlphaAnim[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }; 
static float g_TabPositionsY[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

static int g_ToggleStyle = 0; 

static ImU32 g_MenuAccentColor = IM_COL32(255, 110, 0, 255); 
static int selectedThemeColorIdx = 0; 
static bool g_ShowColorDropdown = false; 

struct ThemeColorProfile {
    const char* name;
    ImU32 color;
};

static ThemeColorProfile g_ThemeProfiles[] = {
    {"Orange",       IM_COL32(255, 110, 0, 255)},
    {"Blue",         IM_COL32(0, 130, 255, 255)},
    {"Purple",       IM_COL32(146, 77, 191, 255)},
    {"Pink",         IM_COL32(255, 90, 160, 255)},
    {"Red",          IM_COL32(240, 35, 35, 255)},
    {"Green",        IM_COL32(40, 215, 85, 255)},
    {"Cyan",         IM_COL32(0, 210, 225, 255)},
    {"Yellow",       IM_COL32(255, 210, 0, 255)},
    {"Gold",         IM_COL32(212, 175, 55, 255)},
    {"Mint Green",   IM_COL32(0, 245, 166, 255)},
    {"Deep Violet",  IM_COL32(92, 0, 255, 255)},
    {"Neon Crimson", IM_COL32(255, 0, 85, 255)},
    {"Ice Blue",     IM_COL32(130, 200, 255, 255)},
    {"Ghost White",  IM_COL32(240, 240, 250, 255)},
    {"Charcoal Gray",IM_COL32(70, 70, 80, 255)},
    {"Sunset Amber", IM_COL32(255, 75, 45, 255)}
};

void PushNotification(const std::string& title, const std::string& message, float duration = 3.0f) {
    iOSNotification notif;
    notif.title = title;
    notif.message = message;
    notif.timestamp = ImGui::GetTime();
    notif.duration = duration;
    notif.accentColor = g_MenuAccentColor;
    g_Notifications.push_back(notif);
}

void DrawiOSNotifications() {
    if (g_Notifications.empty()) return;

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    double currentTime = ImGui::GetTime();
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    float currentY = 25.0f; 

    for (auto it = g_Notifications.begin(); it != g_Notifications.end();) {
        double elapsed = currentTime - it->timestamp;
        if (elapsed > it->duration) {
            it = g_Notifications.erase(it);
            continue;
        }

        float alpha = 1.0f;
        if (elapsed < 0.25f) {
            alpha = (float)(elapsed / 0.25f); 
        } else if (elapsed > (it->duration - 0.3f)) {
            alpha = (float)((it->duration - elapsed) / 0.3f); 
        }

        int calculatedAlpha = (int)(255.0f * alpha);
        if (calculatedAlpha <= 0) { ++it; continue; }

        float width = 310.0f;
        float height = 55.0f;
        float centerX = displaySize.x * 0.5f;
        ImVec2 panelMin(centerX - (width * 0.5f), currentY);
        ImVec2 panelMax(centerX + (width * 0.5f), currentY + height);

        ImU32 bgCol = g_DarkMode ? IM_COL32(22, 22, 26, (int)(240.0f * alpha)) : IM_COL32(255, 255, 255, (int)(245.0f * alpha));
        ImU32 borderCol = g_DarkMode ? IM_COL32(45, 45, 52, (int)(180.0f * alpha)) : IM_COL32(220, 220, 226, (int)(200.0f * alpha));
        ImU32 txtMain = g_DarkMode ? IM_COL32(255, 255, 255, calculatedAlpha) : IM_COL32(18, 18, 24, calculatedAlpha);
        ImU32 txtSub = g_DarkMode ? IM_COL32(165, 165, 172, calculatedAlpha) : IM_COL32(110, 110, 118, calculatedAlpha);
        ImU32 dynamicAccent = (it->accentColor & 0x00FFFFFF) | (calculatedAlpha << 24);

        drawList->AddRectFilled(panelMin, panelMax, bgCol, 18.0f);
        drawList->AddRect(panelMin, panelMax, borderCol, 18.0f, 0, 1.2f);

        drawList->AddRectFilled(ImVec2(panelMin.x + 12, panelMin.y + 14), ImVec2(panelMin.x + 16, panelMax.y - 14), dynamicAccent, 2.0f);

        drawList->AddText(ImVec2(panelMin.x + 26, panelMin.y + 10), txtMain, it->title.c_str());
        drawList->AddText(ImVec2(panelMin.x + 26, panelMin.y + 28), txtSub, it->message.c_str());

        currentY += height + 12.0f; 
        ++it;
    }
}

void DrawVectorWarningIcon(ImVec2 center, float size, ImU32 color) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    float height = size * 0.866f; 
    float halfWidth = size * 0.5f;

    ImVec2 p1(center.x, center.y - (height * 0.6f));
    ImVec2 p2(center.x - halfWidth, center.y + (height * 0.4f));
    ImVec2 p3(center.x + halfWidth, center.y + (height * 0.4f));

    drawList->AddTriangle(p1, p2, p3, color, 3.5f); 
    
    ImU32 innerGlow = (color & 0x00FFFFFF) | (35 << 24);
    drawList->AddTriangleFilled(p1, p2, p3, innerGlow);

    float barTopY = center.y - (height * 0.22f);
    float barBottomY = center.y + (height * 0.05f);
    float thickFactor = size * 0.07f;
    
    drawList->AddLine(ImVec2(center.x, barTopY), ImVec2(center.x, barBottomY), color, thickFactor);

    float dotRadius = size * 0.045f;
    float dotCenterY = center.y + (height * 0.22f);
    drawList->AddCircleFilled(ImVec2(center.x, dotCenterY), dotRadius, color, 12);
}

void DrawDisclaimerOverlay() {
    if (g_DisclaimerAccepted) return;

    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* foregroundDraw = ImGui::GetForegroundDrawList();
    
    foregroundDraw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 160));

    float boxW = 380.0f;
    float boxH = 290.0f;
    ImVec2 screenCenter(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    ImVec2 winPos(screenCenter.x - (boxW * 0.5f), screenCenter.y - (boxH * 0.5f));

    ImGui::SetNextWindowPos(winPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(boxW, boxH), ImGuiCond_Always);

    ImGui::Begin("##TERMS_OF_SERVICE_WINDOW", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    
    ImGui::Spacing(); ImGui::Spacing();
    ImVec2 windowCursorPos = ImGui::GetCursorScreenPos();
    ImVec2 iconCenterPos(windowCursorPos.x + (boxW * 0.5f) - 14.0f, windowCursorPos.y + 25.0f);
    
    DrawVectorWarningIcon(iconCenterPos, 42.0f, g_MenuAccentColor);
    
    ImGui::Dummy(ImVec2(0, 48.0f));
    
    ImGui::SetCursorPosX((boxW - ImGui::CalcTextSize("TERMS OF SERVICE").x) * 0.5f);
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(g_MenuAccentColor), "TERMS OF SERVICE");
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Text, g_DarkMode ? ImVec4(0.80f, 0.80f, 0.85f, 1.0f) : ImVec4(0.20f, 0.20f, 0.25f, 1.0f));
    ImGui::TextWrapped("This software is designed for educational & reverse engineering testing parameters only. Use it at your own risk.");
    ImGui::Spacing();
    ImGui::TextWrapped("The developers hold absolute zero liability towards ban actions or platform protocol violations occurring via execution.");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(24, boxH - 62));
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, g_MenuAccentColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, g_MenuAccentColor);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    if (ImGui::Button("I ACCEPT TERMS", ImVec2(boxW - 48, 42))) {
        g_DisclaimerAccepted = true; 
        PushNotification("RAPZZ", "Welcome back, Operator!");
    }
    
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    ImGui::End();
}

void DrawSubMenuIcons(int iconType, ImVec2 pos, float size, ImU32 color) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    if (iconType == 0) {
        float r = size * 0.25f;
        ImVec2 c1(pos.x - r, pos.y + r*0.3f);
        ImVec2 c2(pos.x, pos.y - r*0.4f);
        ImVec2 c3(pos.x + r, pos.y + r*0.2f);
        
        drawList->AddCircleFilled(c1, r * 0.9f, color, 12);
        drawList->AddCircleFilled(c2, r * 1.3f, color, 12);
        drawList->AddCircleFilled(c3, r * 1.0f, color, 12);
        
        ImVec2 rectMin(pos.x - r * 1.5f, pos.y + r * 0.1f);
        ImVec2 rectMax(pos.x + r * 1.5f, pos.y + r * 1.1f);
        drawList->AddRectFilled(rectMin, rectMax, color, r * 0.4f);
    }
    else if (iconType == 1) {
        drawList->PathLineTo(ImVec2(pos.x, pos.y - size * 0.5f));
        drawList->PathLineTo(ImVec2(pos.x - size * 0.4f, pos.y + size * 0.2f));
        drawList->PathLineTo(ImVec2(pos.x - size * 0.15f, pos.y + size * 0.1f));
        drawList->PathLineTo(ImVec2(pos.x - size * 0.15f, pos.y + size * 0.5f));
        drawList->PathLineTo(ImVec2(pos.x + size * 0.15f, pos.y + size * 0.5f));
        drawList->PathLineTo(ImVec2(pos.x + size * 0.15f, pos.y + size * 0.1f));
        drawList->PathLineTo(ImVec2(pos.x + size * 0.4f, pos.y + size * 0.2f));
        drawList->PathFillConvex(color);
    }
    else if (iconType == 2) {
        drawList->AddCircle(pos, size * 0.45f, color, 16, 1.8f);
        drawList->AddCircle(pos, size * 0.22f, color, 12, 1.2f);
        drawList->AddLine(ImVec2(pos.x - size * 0.45f, pos.y), ImVec2(pos.x + size * 0.45f, pos.y), color, 1.5f);
        drawList->AddLine(ImVec2(pos.x, pos.y - size * 0.45f), ImVec2(pos.x, pos.y + size * 0.45f), color, 1.5f);
    }
    else if (iconType == 3) {
        float h = size * 0.4f;
        float w = size * 0.25f;
        
        drawList->PathLineTo(ImVec2(pos.x - w, pos.y - h));
        drawList->PathLineTo(ImVec2(pos.x, pos.y));
        drawList->PathLineTo(ImVec2(pos.x - w, pos.y + h));
        drawList->PathStroke(color, 0, 2.5f);
        
        drawList->PathLineTo(ImVec2(pos.x, pos.y - h));
        drawList->PathLineTo(ImVec2(pos.x + w, pos.y));
        drawList->PathLineTo(ImVec2(pos.x, pos.y + h));
        drawList->PathStroke(color, 0, 2.5f);
    }
}

struct ThreeFingerState {
    int lastCount = 0;
    double lastFlipTime = 0.0;
};
static ThreeFingerState g_threeFingerState;

static inline int getTouchCount() {
    using Fn = int (*)();
    static Fn fn = nullptr;
    if (!fn) {
        fn = (Fn)Il2CppGetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"),
                                            OBFUSCATE("UnityEngine"),
                                            OBFUSCATE("Input"),
                                            OBFUSCATE("get_touchCount"));
    }
    return fn ? fn() : 0;
}

static inline void UpdateThreeFingerGesture() {
    const int touchCount = getTouchCount();
    if (touchCount >= 5 && g_threeFingerState.lastCount < 5) {
        double currentTime = ImGui::GetTime();
        if (currentTime - g_threeFingerState.lastFlipTime > 0.3) {
            g_isMenuVisible = !g_isMenuVisible;
            g_threeFingerState.lastFlipTime = currentTime;
        }
    }
    g_threeFingerState.lastCount = touchCount;
}

void DrawCustomTabIcon(int tabindex, ImVec2 center, float radius, ImU32 color, float dynamicAlpha){
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int calculatedAlpha = (int)(255.0f * dynamicAlpha);
    if(calculatedAlpha < 5) return;
    ImU32 customColor = (color & 0x00FFFFFF) | (calculatedAlpha << 24);

    if (tabindex == 1) {
        drawList->AddCircleFilled(center, 3.5f, customColor, 12);
        drawList->PathLineTo(ImVec2(center.x - radius + 2, center.y));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x - 4, center.y - radius + 6), ImVec2(center.x + 4, center.y - radius + 6), ImVec2(center.x + radius - 2, center.y), 20);
        drawList->PathStroke(customColor, 0, 2.2f);
        drawList->PathLineTo(ImVec2(center.x - radius + 2, center.y));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x - 4, center.y + radius - 6), ImVec2(center.x + 4, center.y + radius - 6), ImVec2(center.x + radius - 2, center.y), 20);
        drawList->PathStroke(customColor, 0, 2.2f);
    }
    else if (tabindex == 2) {
        drawList->AddCircle(center, radius - 5.0f, customColor, 16, 2.2f);
        drawList->AddLine(ImVec2(center.x - radius + 1, center.y), ImVec2(center.x + radius - 1, center.y), customColor, 2.2f);
        drawList->AddLine(ImVec2(center.x, center.y - radius + 1), ImVec2(center.x, center.y + radius - 1), customColor, 2.2f);
    }
    else if (tabindex == 3) {
        drawList->AddCircleFilled(ImVec2(center.x - 4, center.y - 4), 1.5f, customColor);
        drawList->AddCircleFilled(ImVec2(center.x + 5, center.y - 3), 2.2f, customColor);
        drawList->PathLineTo(ImVec2(center.x, center.y - 8));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x, center.y - 2), ImVec2(center.x, center.y - 2), ImVec2(center.x + 8, center.y));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x + 2, center.y), ImVec2(center.x + 2, center.y), ImVec2(center.x, center.y + 8));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x, center.y + 2), ImVec2(center.x, center.y + 2), ImVec2(center.x - 8, center.y));
        drawList->PathBezierCubicCurveTo(ImVec2(center.x - 2, center.y), ImVec2(center.x - 2, center.y), ImVec2(center.x, center.y - 8));
        drawList->PathFillConvex(customColor); 
    }
    else if (tabindex == 4) {
        drawList->AddCircle(center, 5.0f, customColor, 12, 2.4f);
        for (int i = 0; i < 8; i++) {
            float angle = i * (IM_PI / 4.0f);
            drawList->AddLine(ImVec2(center.x + cosf(angle) * 5.0f, center.y + sinf(angle) * 5.0f), 
                              ImVec2(center.x + cosf(angle) * 8.5f, center.y + sinf(angle) * 8.5f), customColor, 2.4f);
        }
    }
    else if (tabindex == 5) {
        drawList->AddLine(ImVec2(center.x - 9, center.y - 5), ImVec2(center.x + 9, center.y - 5), customColor, 2.0f);
        drawList->AddLine(ImVec2(center.x - 9, center.y), ImVec2(center.x + 9, center.y), customColor, 2.0f);
        drawList->AddLine(ImVec2(center.x - 9, center.y + 5), ImVec2(center.x + 9, center.y + 5), customColor, 2.0f);
        drawList->AddCircleFilled(ImVec2(center.x - 3, center.y - 5), 2.8f, customColor);
        drawList->AddCircleFilled(ImVec2(center.x + 4, center.y), 2.8f, customColor);
        drawList->AddCircleFilled(ImVec2(center.x - 1, center.y + 5), 2.8f, customColor);
    }
}

bool ModernToggle(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    float rowHeight = 48.0f; 
    ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + rowHeight));
    
    ImGui::ItemSize(total_bb, style.FramePadding.y); 
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !*v;
        PushNotification(label, *v ? "Activated successfully" : "Deactivated successfully");
    }

    ImU32 staticLineBg = g_DarkMode ? IM_COL32(20, 20, 22, 255) : IM_COL32(255, 255, 255, 255);
    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, staticLineBg, 14.0f);
    
    ImU32 borderTrackingCol = g_DarkMode ? IM_COL32(38, 38, 42, 255) : IM_COL32(230, 230, 236, 255);
    window->DrawList->AddRect(total_bb.Min, total_bb.Max, borderTrackingCol, 14.0f, 0, 1.2f);

    ImU32 text_color = g_DarkMode ? IM_COL32(245, 245, 250, 255) : IM_COL32(35, 35, 40, 255);
    window->DrawList->AddText(ImVec2(total_bb.Min.x + 18, total_bb.Min.y + (rowHeight - ImGui::GetTextLineHeight()) * 0.5f), text_color, label);

    ImU32 dynamicSpectralGlow = (g_MenuAccentColor & 0x00FFFFFF) | (55 << 24);    

    if (g_ToggleStyle == 0) {
        float switchWidth = 46.0f;
        float switchHeight = 24.0f;
        ImVec2 sMin = ImVec2(total_bb.Max.x - 64, total_bb.Min.y + (rowHeight - switchHeight) * 0.5f);
        ImVec2 sMax = ImVec2(sMin.x + switchWidth, sMin.y + switchHeight);
        float sRounding = switchHeight * 0.5f;

        if (*v) {
            for (int i = 1; i <= 2; i++) {
                window->DrawList->AddRect(ImVec2(sMin.x - i, sMin.y - i), ImVec2(sMax.x + i, sMax.y + i), dynamicSpectralGlow, sRounding + i, 0, 1.0f);
            }
            window->DrawList->AddRectFilled(sMin, sMax, g_MenuAccentColor, sRounding);
            float radius = (switchHeight - 4.0f) * 0.5f;
            window->DrawList->AddCircleFilled(ImVec2(sMax.x - radius - 3.0f, sMin.y + radius + 2.0f), radius, IM_COL32(255, 255, 255, 255));
        } else {
            ImU32 offBg = g_DarkMode ? IM_COL32(45, 45, 48, 255) : IM_COL32(225, 225, 230, 255);
            window->DrawList->AddRectFilled(sMin, sMax, offBg, sRounding);
            float radius = (switchHeight - 4.0f) * 0.5f;
            window->DrawList->AddCircleFilled(ImVec2(sMin.x + radius + 3.0f, sMin.y + radius + 2.0f), radius, g_DarkMode ? IM_COL32(140, 140, 145, 255) : IM_COL32(175, 175, 180, 255));
        }
    } 
    else if (g_ToggleStyle == 1) {
        float boxRadius = 14.0f;
        ImVec2 boxCenter = ImVec2(total_bb.Max.x - 32, total_bb.Min.y + (rowHeight * 0.5f));
        
        if (*v) {
            for (int i = 1; i <= 3; i++) {
                window->DrawList->AddCircle(boxCenter, boxRadius + (i * 2.0f), dynamicSpectralGlow, 32, 1.2f);
            }
            window->DrawList->AddCircleFilled(boxCenter, boxRadius, g_MenuAccentColor, 32);
            window->DrawList->PathLineTo(ImVec2(boxCenter.x - 6.0f, boxCenter.y - 0.5f));
            window->DrawList->PathLineTo(ImVec2(boxCenter.x - 1.5f, boxCenter.y + 4.0f));
            window->DrawList->PathLineTo(ImVec2(boxCenter.x + 6.0f, boxCenter.y - 5.0f));
            window->DrawList->PathStroke(IM_COL32(255, 255, 255, 255), 0, 2.6f);
        } else {
            ImU32 uncheckBg = g_DarkMode ? IM_COL32(40, 40, 42, 255) : IM_COL32(245, 245, 250, 255);
            window->DrawList->AddCircle(boxCenter, boxRadius, g_DarkMode ? IM_COL32(75, 75, 80, 255) : IM_COL32(185, 185, 190, 255), 32, 1.5f);
            window->DrawList->AddCircleFilled(boxCenter, boxRadius - 1.0f, uncheckBg, 32);
        }
    } 
    else if (g_ToggleStyle == 2) {
        ImVec2 bMin = ImVec2(total_bb.Max.x - 102, total_bb.Min.y + 8.0f);
        ImVec2 bMax = ImVec2(total_bb.Max.x - 14, total_bb.Max.y - 8.0f);
        float bRounding = 10.0f;
        
        if (*v) {
            for (int i = 1; i <= 2; i++) {
                window->DrawList->AddRect(ImVec2(bMin.x - i, bMin.y - i), ImVec2(bMax.x + i, bMax.y + i), dynamicSpectralGlow, bRounding + i, 0, 1.0f);
            }
            window->DrawList->AddRectFilled(bMin, bMax, g_MenuAccentColor, bRounding);
            const char* txt = "ON";
            ImVec2 txtSz = ImGui::CalcTextSize(txt);
            window->DrawList->AddText(ImVec2(bMin.x + (88.0f - txtSz.x)*0.5f, bMin.y + (32.0f - txtSz.y)*0.5f), IM_COL32(255, 255, 255, 255), txt);
        } else {
            ImU32 btnOff = g_DarkMode ? IM_COL32(42, 42, 45, 255) : IM_COL32(220, 220, 225, 255);
            window->DrawList->AddRectFilled(bMin, bMax, btnOff, bRounding);
            const char* txt = "OFF";
            ImVec2 txtSz = ImGui::CalcTextSize(txt);
            ImU32 offTxtCol = g_DarkMode ? IM_COL32(150, 150, 155, 255) : IM_COL32(120, 120, 125, 255);
            window->DrawList->AddText(ImVec2(bMin.x + (88.0f - txtSz.x)*0.5f, bMin.y + (32.0f - txtSz.y)*0.5f), offTxtCol, txt);
        }
    }

    return pressed;
}






bool ModernSlider(const char* label, float* v, float v_min, float v_max) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    float rowHeight = 52.0f;
    ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + rowHeight));
    
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    if (held) {
        float mouse_x = ImGui::GetIO().MousePos.x - bb.Min.x - 20;
        float width = bb.GetWidth() - 40;
        *v = v_min + (std::max(0.0f, std::min(mouse_x / width, 1.0f)) * (v_max - v_min));
    }

    ImU32 bgCol = g_DarkMode ? IM_COL32(22, 22, 25, 255) : IM_COL32(235, 235, 240, 255);
    window->DrawList->AddRectFilled(bb.Min, bb.Max, bgCol, 14.0f);
    
    ImU32 outlineColor = g_DarkMode ? IM_COL32(38, 38, 42, 255) : IM_COL32(225, 225, 230, 255);
    window->DrawList->AddRect(bb.Min, bb.Max, outlineColor, 14.0f, 0, 1.2f);

    ImU32 barTrackCol = g_DarkMode ? IM_COL32(44, 44, 48, 255) : IM_COL32(210, 210, 215, 255);
    ImU32 grabCol = g_MenuAccentColor;
    
    float ratio = (*v - v_min) / (v_max - v_min);
    window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 18, bb.Min.y + 30), ImVec2(bb.Max.x - 18, bb.Min.y + 37), barTrackCol, 4.0f);
    
    ImVec2 grabPos = ImVec2(bb.Min.x + 18 + (ratio * (bb.GetWidth() - 42)), bb.Min.y + 33.5f);
    window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 18, bb.Min.y + 30), grabPos, grabCol, 4.0f);
   
    window->DrawList->AddCircleFilled(grabPos, 9.0f, IM_COL32(255, 255, 255, 255));
    window->DrawList->AddCircle(grabPos, 9.0f, grabCol, 32, 2.2f);

    window->DrawList->AddText(ImVec2(bb.Min.x + 18, bb.Min.y + 6), g_DarkMode ? IM_COL32(220,220,225,255) : IM_COL32(45,45,50,255), label);

    return held;
}

void menu() {
    UpdateThreeFingerGesture();

    if (!g_FirstLoadNotifyDone) {
        PushNotification("RAPZZ", "Library Loaded Successfully!", 4.5f);
        g_FirstLoadNotifyDone = true;
    }

    DrawiOSNotifications();

    if (!g_DisclaimerAccepted) {
        DrawDisclaimerOverlay();
        return; 
    }

    if (!g_isMenuVisible) return;

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    if (g_DarkMode) {
        colors[ImGuiCol_WindowBg]             = ImVec4(0.06f, 0.06f, 0.07f, 1.0f);
        colors[ImGuiCol_ChildBg]              = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_Button]               = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.10f, 0.10f, 0.11f, 1.0f);
        colors[ImGuiCol_FrameBg]              = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
        colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.16f, 0.16f, 0.18f, 1.0f);
        colors[ImGuiCol_FrameBgActive]        = ImVec4(0.20f, 0.20f, 0.22f, 1.0f);
        colors[ImGuiCol_PopupBg]              = ImVec4(0.11f, 0.11f, 0.13f, 1.0f);
        colors[ImGuiCol_Text]                 = ImVec4(0.96f, 0.96f, 0.98f, 1.0f);
    } else {
        colors[ImGuiCol_WindowBg]             = ImVec4(0.96f, 0.96f, 0.97f, 1.0f);
        colors[ImGuiCol_ChildBg]              = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_Button]               = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_ButtonHovered]        = ImVec4(0.93f, 0.93f, 0.95f, 1.0f);
        colors[ImGuiCol_ButtonActive]         = ImVec4(0.88f, 0.88f, 0.90f, 1.0f);
        colors[ImGuiCol_Text]                 = ImVec4(0.08f, 0.08f, 0.12f, 1.0f);
    }

    style.WindowBorderSize  = 0.0f;
    style.FrameBorderSize   = 0.0f;
    style.WindowRounding    = 24.0f; 
    style.FrameRounding     = 14.0f;
    style.ItemSpacing       = ImVec2(12, 14); 
    style.WindowPadding     = ImVec2(14, 14);
    style.ScrollbarRounding = 12.0f; 
    style.ScrollbarSize     = 5.0f;  

    ImGui::SetNextWindowSize(ImVec2(650, 460), ImGuiCond_Always);
    ImGui::Begin("##iOSMenuMain", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    float deltaTime = ImGui::GetIO().DeltaTime;
    
    ImGui::BeginChild("SideTabGroup", ImVec2(75, 0), false, ImGuiWindowFlags_NoScrollbar);
    int targetTabs[] = { 1, 2, 3, 4, 5 }; 

    ImDrawList* sideDrawList = ImGui::GetWindowDrawList();
    ImVec2 sideBarPos = ImGui::GetCursorScreenPos();
    
    if (activeTab >= 1 && activeTab <= 5) {
        g_TabAnimTarget = g_TabPositionsY[activeTab - 1];
    }
    
    if (g_TabAnimCurrent == 0.0f && g_TabAnimTarget != 0.0f) {
        g_TabAnimCurrent = g_TabAnimTarget;
    }

    g_TabAnimCurrent += (g_TabAnimTarget - g_TabAnimCurrent) * 12.0f * deltaTime;

    if (g_TabAnimCurrent > 10.0f) {
        ImVec2 indicatorMin = ImVec2(sideBarPos.x, g_TabAnimCurrent);
        ImVec2 indCenter = ImVec2(indicatorMin.x + 29.0f, indicatorMin.y + 29.0f);

        ImU32 bubbleBgColor = g_DarkMode ? g_MenuAccentColor : IM_COL32(255, 255, 255, 255);
        ImU32 indicatorGlow = (g_MenuAccentColor & 0x00FFFFFF) | (45 << 24);
        for(int g = 1; g <= 2; g++) {
            sideDrawList->AddCircle(indCenter, 27.0f + (g * 1.5f), indicatorGlow, 36, 1.0f);
        }
        
        sideDrawList->AddCircleFilled(indCenter, 28.0f, bubbleBgColor);
        ImU32 bubbleBorderColor = g_DarkMode ? (g_MenuAccentColor & 0x00FFFFFF) | (190 << 24) : IM_COL32(225, 225, 232, 255);
        sideDrawList->AddCircle(indCenter, 28.0f, bubbleBorderColor, 36, 1.0f);
    }

    for (int i = 0; i < 5; i++) {
        bool isCurrent = (activeTab == targetTabs[i]);
        g_TabAlphaAnim[i] += ((isCurrent ? 1.0f : 0.0f) - g_TabAlphaAnim[i]) * 14.0f * deltaTime;

        char blockId[24]; sprintf(blockId, "##TabIdx%d", i);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 29.0f); 
        
        ImVec2 buttonPos = ImGui::GetCursorScreenPos();
        g_TabPositionsY[i] = buttonPos.y; 

        ImVec2 iconCenter = ImVec2(buttonPos.x + 29.0f, buttonPos.y + 29.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
        
        if (ImGui::Button(blockId, ImVec2(58, 58))) { activeTab = targetTabs[i]; }
        ImGui::PopStyleColor(3);

        ImU32 inactiveIconColor = g_DarkMode ? IM_COL32(120, 120, 125, 255) : IM_COL32(150, 150, 155, 255);
        ImU32 activeIconColor = g_DarkMode ? IM_COL32(255, 255, 255, 255) : g_MenuAccentColor;
        
        if(g_TabAlphaAnim[i] < 0.95f) { DrawCustomTabIcon(targetTabs[i], iconCenter, 16.0f, inactiveIconColor, 1.0f - g_TabAlphaAnim[i]); }
        if(g_TabAlphaAnim[i] > 0.05f) { DrawCustomTabIcon(targetTabs[i], iconCenter, 16.0f, activeIconColor, g_TabAlphaAnim[i]); }

        ImGui::PopStyleVar(); ImGui::Spacing();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("MainCanvasView", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    ImGui::BeginChild("TopHeaderControl", ImVec2(0, 55), false, ImGuiWindowFlags_NoScrollbar);
    
    const char* stringHeader = "SETTING";
    if (activeTab == 1) stringHeader = "AIM SETTING";
    else if (activeTab == 2) stringHeader = "ESP SETTING";
    else if (activeTab == 3) stringHeader = "RAGE OPTIONS";
    else if (activeTab == 4) stringHeader = "VISUAL SYSTEM";
    else if (activeTab == 5) stringHeader = "UI STYLE"; 

    ImGui::SetCursorPos(ImVec2(4, 10));
    ImGui::PushStyleColor(ImGuiCol_Button, g_DarkMode ? ImVec4(0.12f,0.12f,0.14f,1.0f) : ImVec4(1.0f,1.0f,1.0f,1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(g_MenuAccentColor));
    ImGui::Button(stringHeader, ImVec2(220, 38));
    ImGui::PopStyleColor(2);

    ImDrawList* topDraw = ImGui::GetWindowDrawList();

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 146, 10)); 
    ImVec2 pickerBtnPos = ImGui::GetCursorScreenPos();
    if (ImGui::Button("##ColorPickerMainToggleBtn", ImVec2(38, 38))) { g_ShowColorDropdown = !g_ShowColorDropdown; }
    ImVec2 pickerCenter(pickerBtnPos.x + 19.0f, pickerBtnPos.y + 19.0f);
    topDraw->AddCircle(pickerCenter, 9.0f, g_DarkMode ? IM_COL32(220, 220, 225, 255) : IM_COL32(50, 50, 55, 255), 0, 1.8f);
    topDraw->AddCircleFilled(pickerCenter, 4.5f, g_MenuAccentColor); 

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 98, 10));
    ImVec2 themeBtnPos = ImGui::GetCursorScreenPos();
    if (ImGui::Button("##ThemeToggleBtn", ImVec2(38, 38))) { g_DarkMode = !g_DarkMode; }
    ImVec2 themeCenter(themeBtnPos.x + 19.0f, themeBtnPos.y + 19.0f);
    
    if (!g_DarkMode) { 
        topDraw->AddCircleFilled(themeCenter, 6.0f, IM_COL32(255, 149, 0, 255));
        for (int r = 0; r < 8; r++) {
            float angle = r * (IM_PI / 4.0f);
            topDraw->AddLine(ImVec2(themeCenter.x + cosf(angle) * 8.0f, themeCenter.y + sinf(angle) * 8.0f), ImVec2(themeCenter.x + cosf(angle) * 12.0f, themeCenter.y + sinf(angle) * 12.0f), IM_COL32(255, 149, 0, 255), 1.6f);
        }
    } else { 
        topDraw->AddCircleFilled(themeCenter, 8.0f, IM_COL32(245, 245, 250, 255));
        topDraw->AddCircleFilled(ImVec2(themeCenter.x - 4.0f, themeCenter.y - 2.0f), 7.5f, IM_COL32(16, 16, 18, 255));
    }

    ImGui::SameLine(); ImVec2 closeBtnPos = ImGui::GetCursorScreenPos();
    if (ImGui::Button("##CloseWindowMain", ImVec2(38, 38))) { g_isMenuVisible = false; }
    ImU32 crossColor = g_DarkMode ? IM_COL32(235,235,240,255) : IM_COL32(45,45,50,255);
    topDraw->AddLine(ImVec2(closeBtnPos.x + 13.0f, closeBtnPos.y + 13.0f), ImVec2(closeBtnPos.x + 25.0f, closeBtnPos.y + 25.0f), crossColor, 2.5f);
    topDraw->AddLine(ImVec2(closeBtnPos.x + 25.0f, closeBtnPos.y + 13.0f), ImVec2(closeBtnPos.x + 13.0f, closeBtnPos.y + 25.0f), crossColor, 2.5f);

    ImGui::EndChild();
    ImGui::Spacing();

    if (g_ShowColorDropdown) {
        ImVec2 popSize = ImVec2(240.0f, 240.0f); ImVec2 popPos = ImVec2(pickerBtnPos.x - popSize.x + 40.0f, pickerBtnPos.y + 44.0f);
        topDraw->AddRectFilled(popPos, ImVec2(popPos.x + popSize.x, popPos.y + popSize.y), g_DarkMode ? IM_COL32(20, 20, 22, 255) : IM_COL32(255, 255, 255, 255), 14.0f);
        topDraw->AddRect(popPos, ImVec2(popPos.x + popSize.x, popPos.y + popSize.y), g_DarkMode ? IM_COL32(45, 45, 50, 255) : IM_COL32(220, 220, 225, 255), 14.0f, 0, 1.5f);
        
        float gridStartX = popPos.x + 20.0f; float gridStartY = popPos.y + 24.0f;
        for (int c = 0; c < 16; c++) {
            int r = c / 4; int col = c % 4; ImVec2 cCenter = ImVec2(gridStartX + (col * 50.0f), gridStartY + (r * 50.0f));
            ImGui::SetCursorScreenPos(ImVec2(cCenter.x - 18.0f, cCenter.y - 18.0f));
            char cBtnId[32]; sprintf(cBtnId, "##TopBarClrNode_%d", c);
            
            if (ImGui::InvisibleButton(cBtnId, ImVec2(36.0f, 36.0f))) { selectedThemeColorIdx = c; g_MenuAccentColor = g_ThemeProfiles[c].color; }
            topDraw->AddCircleFilled(cCenter, 14.0f, g_ThemeProfiles[c].color);
            if (selectedThemeColorIdx == c) { topDraw->AddCircle(cCenter, 18.0f, g_DarkMode ? IM_COL32(255, 255, 255, 255) : IM_COL32(25, 25, 30, 255), 0, 2.0f); }
        }
    }

    ImGui::BeginChild("VerticalTouchContainer", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        float dragDeltaY = ImGui::GetIO().MouseDelta.y;
        if (fabsf(dragDeltaY) > 0.5f) {
            float targetScrollY = ImGui::GetScrollY() - dragDeltaY; float maxScrollY = ImGui::GetScrollMaxY();
            if (targetScrollY < 0.0f) targetScrollY = 0.0f; else if (targetScrollY > maxScrollY) targetScrollY = maxScrollY;
            ImGui::SetScrollY(targetScrollY);
        }
    }

    if (activeTab == 1) {
        ModernToggle("Enable Features", &AimbotEsp); ImGui::Spacing();
        ImGui::TextColored(g_DarkMode ? ImVec4(0.70f,0.70f,0.75f,1.0f) : ImVec4(0.30f,0.30f,0.35f,1.0f), "Aim Configuration");
        
        float segmentWidth = (ImGui::GetContentRegionAvail().x - 16) / 2.0f; 
        const char* Aim_Mode_Labels[] = { "Rage", "Legit" };
        for (int m = 0; m < 2; m++) {
            bool isSelectedMode = (Aimmode == m);
            ImGui::PushStyleColor(ImGuiCol_Button, isSelectedMode ? ImGui::ColorConvertU32ToFloat4(g_MenuAccentColor) : (g_DarkMode ? ImVec4(0.12f,0.12f,0.14f,1.0f) : ImVec4(1.0f,1.0f,1.0f,1.0f)));
            ImGui::PushStyleColor(ImGuiCol_Text, isSelectedMode ? ImVec4(1.0f,1.0f,1.0f,1.0f) : (g_DarkMode ? ImVec4(0.55f,0.55f,0.60f,1.0f) : ImVec4(0.35f,0.35f,0.40f,1.0f)));
            if (ImGui::Button(Aim_Mode_Labels[m], ImVec2(segmentWidth, 34))) { 
                Aimmode = m; 
                PushNotification("Aim Mode System", m == 0 ? "Switched to RAGE Engine" : "Switched to LEGIT Engine"); 
            }
            ImGui::PopStyleColor(2); if (m == 0) ImGui::SameLine(0, 12);
        }
        ImGui::Spacing();

        ModernToggle("Aim Bot", &Aimbot2);
        ModernToggle("Aim Fire", &AimFire);
        ModernToggle("Aim Scope", &AimScope);
        ModernToggle("Aim Rush", &AimRush); 
        ModernSlider("Fov", &Fov_Aim, 0.0f, 500.0f); 
        ImGui::Spacing();

        ImGui::Text("Target Position");
        const char* aimpos_options[] = { "Cabeça", "Pescoço", "Corpo", "Quadril" };
        ImGui::PushItemWidth(-1); ImGui::Combo("##AimLocalDropdown", &aimPosition, aimpos_options, IM_ARRAYSIZE(aimpos_options)); ImGui::PopItemWidth();
    }
    else if (activeTab == 2) {
        ModernToggle("Enable ESP", &EnableEsp); 
        ModernToggle("Show Name ", &Nick);
        ModernToggle("Show Line", &EspLine); 
        ModernToggle("Show Box", &EspBox);
        ModernToggle("Show Weapon", &EspWeaponIcon);
        ModernToggle("Show Distance", &Dist); 
        ModernToggle("Show Health Bar", &EspVida);
    }
    else if (activeTab == 3) { 
        ImGui::Text("Rage engine core functions loaded.");
        ModernToggle("AimKill ", &AimKill1);
    }
    else if (activeTab == 4) {
        ImGui::TextColored(g_DarkMode ? ImVec4(0.70f,0.70f,0.75f,1.0f) : ImVec4(0.30f,0.30f,0.35f,1.0f), "Visual modifiers");
        ModernToggle("Wallhack / Chams", &shading);
        ModernToggle("Wireframe", &wireframe);
        ImGui::Spacing();
        ModernToggle("ESP Enemy Count", &EspCount);
        ModernToggle("FOV Circle", &EspCircle);
        ImGui::Spacing();
        ImGui::TextColored(g_DarkMode ? ImVec4(0.55f,0.55f,0.60f,1.0f) : ImVec4(0.40f,0.40f,0.45f,1.0f), "by RAPZZ");
    }
    else if (activeTab == 5) {
        ModernToggle("Enable AntiRecord", &AntiRecord);
        ImGui::Spacing(); 
        ImGui::Separator(); 
        ImGui::Spacing();

        ImGui::TextColored(g_DarkMode ? ImVec4(0.70f,0.70f,0.75f,1.0f) : ImVec4(0.30f,0.30f,0.35f,1.0f), "Choose Global Toggle Style");
        ImGui::Spacing();

        float totalWidth = ImGui::GetContentRegionAvail().x; float btnWidth = (totalWidth - 24.0f) / 3.0f;
        const char* stylesText[] = { "Toggle", "Checkbox", "Button" };
        
        for (int s = 0; s < 3; s++) {
            bool isCurrentStyle = (g_ToggleStyle == s);
            if (isCurrentStyle) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertU32ToFloat4(g_MenuAccentColor)); ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, g_DarkMode ? ImVec4(0.12f, 0.12f, 0.14f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, g_DarkMode ? ImVec4(0.65f, 0.65f, 0.70f, 1.0f) : ImVec4(0.35f, 0.35f, 0.40f, 1.0f));
            }

            char btnId[32]; sprintf(btnId, "%s##StyleBtn_%d", stylesText[s], s);
            if (ImGui::Button(btnId, ImVec2(btnWidth, 40))) { g_ToggleStyle = s; }
            ImGui::PopStyleColor(2);
             if (s < 2) ImGui::SameLine(0, 12);
        }

        ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
        static bool sampleFeature1 = true;
        static bool sampleFeature2 = false;
        
        ModernToggle("Preview Feature Option A", &sampleFeature1);
        ImGui::Spacing();
        ModernToggle("Preview Feature Option B", &sampleFeature2);
        
        ImGui::Spacing();
        ImGui::ColorEdit4("Line Color", (float*)&LineCol, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Box Borders Color", (float*)&BoxCol, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Text Colors", (float*)&NameCol, ImGuiColorEditFlags_NoInputs);
        ImGui::ColorEdit4("Distance Text Color", (float*)&DistCol, ImGuiColorEditFlags_NoInputs);
    }


    ImGui::EndChild(); 
    ImGui::EndChild(); 
    ImGui::End();
}

