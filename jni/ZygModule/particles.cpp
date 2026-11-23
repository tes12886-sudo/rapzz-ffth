#include "particles.h"
#include <algorithm>

ParticleSystem::ParticleSystem(int maxParticles, float spawnRate) 
    : rng(std::random_device{}()), randomFloat(0.0f, 1.0f), spawnRate(spawnRate), timeSinceLastSpawn(0.0f) {
    particles.reserve(maxParticles);
}

void ParticleSystem::Update(float deltaTime, ImVec2 windowSize) {
    this->windowSize = windowSize;
    
    // 更新粒子
    for (auto it = particles.begin(); it != particles.end();) {
        UpdateParticle(*it, deltaTime);
        
        // 删除死粒子
        if (it->life <= 0.0f) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }
    
    // 生成新粒子
    timeSinceLastSpawn += deltaTime;
    if (timeSinceLastSpawn >= spawnRate && particles.size() < particles.capacity() && windowSize.x > 0 && windowSize.y > 0) {
        SpawnParticle();
        timeSinceLastSpawn = 0.0f;
    }
}

void ParticleSystem::Render(ImDrawList* drawList) {
    // 连接附近的粒子
    float connectionDistance = 100.0f;  // 最大距离
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            const Particle& p1 = particles[i];
            const Particle& p2 = particles[j];
            
            float distSq = (p1.position.x - p2.position.x) * (p1.position.x - p2.position.x) +
                           (p1.position.y - p2.position.y) * (p1.position.y - p2.position.y);
            
            if (distSq < connectionDistance * connectionDistance) {
                // 按距离算透明度
                float alpha = 1.0f - (sqrt(distSq) / connectionDistance);
                ImU32 lineColor = IM_COL32(255, 255, 255, (int)(alpha * 255 * 1.0f));  // 白色线
                drawList->AddLine(p1.position, p2.position, lineColor, 1.0f);
            }
        }
    }

    for (const auto& particle : particles) {
        // 按生命算颜色
        float alpha = particle.life * particle.alpha;
        ImU32 color = GetParticleColor(particle.life);
        
        // 画圆
        drawList->AddCircleFilled(particle.position, particle.size, color);
        
        // 加一点光
        if (particle.life > 0.5f) {
            float glowAlpha = (particle.life - 0.5f) * 2.0f * 0.3f;  // 最多三成
            ImU32 glowColor = IM_COL32(255, 255, 255, (int)(glowAlpha * 255));
            drawList->AddCircleFilled(particle.position, particle.size * 1.5f, glowColor);
        }
    }
}

void ParticleSystem::SpawnParticle(ImVec2 position) {
    Particle particle;
    
    // 开始位置
    if (position.x < 0 || position.y < 0) {
        particle.position.x = Random(0, windowSize.x);
        particle.position.y = Random(0, windowSize.y);
    } else {
        particle.position = position;
    }
    
    // 随机速度
    float angle = Random(0, 2 * M_PI);
    float speed = Random(10.0f, 50.0f);
    particle.velocity.x = cos(angle) * speed;
    particle.velocity.y = sin(angle) * speed;
    
    // 粒子属性
    particle.life = Random(3.0f, 8.0f);
    particle.maxLife = particle.life;
    particle.size = Random(1.0f, 4.0f);
    particle.alpha = Random(0.3f, 0.8f);
    
    particles.push_back(particle);
}

void ParticleSystem::UpdateParticle(Particle& particle, float deltaTime) {
    // 更新位置
    particle.position.x += particle.velocity.x * deltaTime;
    particle.position.y += particle.velocity.y * deltaTime;
    
    // 更新生命
    particle.life -= deltaTime;
    
    // 简单物理
    particle.velocity.y += 20.0f * deltaTime;  // 小重力
    particle.velocity.x *= 0.99f;  // 空气阻力
    particle.velocity.y *= 0.99f;
    
    // 碰边反弹
    if (particle.position.x < 0 || particle.position.x > windowSize.x) {
        particle.velocity.x *= -0.8f;  // 少一点力
        particle.position.x = std::max(0.0f, std::min(windowSize.x, particle.position.x));
    }
    
    if (particle.position.y < 0 || particle.position.y > windowSize.y) {
        particle.velocity.y *= -0.8f;  // 少一点力
        particle.position.y = std::max(0.0f, std::min(windowSize.y, particle.position.y));
    }
}

ImU32 ParticleSystem::GetParticleColor(float life) {
    // 红色更清楚
    float normalizedLife = life / 8.0f;  // 最长八秒
    // 更清楚
    return IM_COL32(255, 0, 0, (int)(normalizedLife * 255 * 1.0f));  // 红色
}

float ParticleSystem::Random(float min, float max) {
    return min + randomFloat(rng) * (max - min);
}



