#pragma once
#include <vector>
#include <random>
#include <cmath>  // 圆周率
#include <imgui.h>

// 一个粒子
struct Particle {
    ImVec2 position;  // 位置
    ImVec2 velocity;  // 速度
    float life;  // 还剩多久
    float maxLife;  // 总时间
    float size;  // 大小
    ImU32 color;  // 颜色
    float alpha;  // 透明度
    
    Particle() : position(0, 0), velocity(0, 0), life(1.0f), maxLife(1.0f), size(2.0f), color(IM_COL32(255, 255, 255, 255)), alpha(1.0f) {}
};

// 粒子管理
class ParticleSystem {
private:
    std::vector<Particle> particles;
    std::mt19937 rng;
    std::uniform_real_distribution<float> randomFloat;
    ImVec2 windowSize;
    float spawnRate;
    float timeSinceLastSpawn;
    
public:
    ParticleSystem(int maxParticles = 100, float spawnRate = 0.1f);
    
    void Update(float deltaTime, ImVec2 windowSize);
    void Render(ImDrawList* drawList);
    void SpawnParticle(ImVec2 position = ImVec2(-1, -1));
    void SetWindowSize(ImVec2 size) { windowSize = size; }
    
private:
    void UpdateParticle(Particle& particle, float deltaTime);
    ImU32 GetParticleColor(float life);
    float Random(float min = 0.0f, float max = 1.0f);
};



