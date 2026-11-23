#pragma once

#include "imagens/aimboticon.h"
#include "imagens/visualicon.h"
#include "imagens/logsicon.h"
#include "imagens/bombicon.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "WEAPON/AKM.h"
#include "WEAPON/AUG.h"
#include "WEAPON/AWM.h"
#include "WEAPON/Desert_Eagle.h"
#include "WEAPON/FIST.h"
#include "WEAPON/GROZA.h"
#include "WEAPON/KAR98K.h"
#include "WEAPON/M24.h"
#include "WEAPON/PAN.h"
#include "WEAPON/SCARL.h"
#include "WEAPON/SKS.h"
#include "WEAPON/UMP45.h"
#include "WEAPON/VECTOR.h"
#include "WEAPON/VSS.h"
#include "WEAPON/MP5.h"


struct TextureInfo {
    ImTextureID textureId;
    int x, y, w, h;
};

void DrawImage(int x, int y, int w, int h, ImTextureID Texture) {
    ImGui::GetForegroundDrawList()->AddImage(
        Texture,
        ImVec2(x, y),
        ImVec2(x + w, y + h)
    );
}

TextureInfo createTexture(char* ImagePath) {
    TextureInfo tex{};
    int w, h, n;

    stbi_uc* data = stbi_load(ImagePath, &w, &h, &n, 4);
    if (!data) return tex;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < w * h; i++) {
        unsigned char* pixel = &data[i * 4];

        unsigned char r = pixel[0];
        unsigned char g = pixel[1];
        unsigned char b = pixel[2];

        if (r < 10 && g < 10 && b < 10) {
            pixel[3] = 0;
        } else {
            pixel[3] = 255;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    tex.textureId = (ImTextureID)(intptr_t)texture;
    tex.w = w;
    tex.h = h;

    return tex;
}

TextureInfo CreateTexture(const unsigned char* buf, int len) {
    TextureInfo image{};

    unsigned char* image_data = stbi_load_from_memory(buf, len, &image.w, &image.h, NULL, 4);
    if (!image_data) {
        image.textureId = 0;
        return image;
    }

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

    for (int i = 0; i < image.w * image.h; i++) {
        unsigned char* pixel = &image_data[i * 4];

        unsigned char r = pixel[0];
        unsigned char g = pixel[1];
        unsigned char b = pixel[2];

        if (r < 10 && g < 10 && b < 10) {
            pixel[3] = 0;
        } else {
            pixel[3] = 255;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.w, image.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image_data);

    image.textureId = (ImTextureID)(intptr_t)image_texture;
    return image;
}


/*
void InitTexture() {
    
}*/



static struct WeaponHandTextures {
    TextureInfo AKM;
    TextureInfo AUG;
    TextureInfo AWM;
    TextureInfo Desert_Eagle;
    TextureInfo FIST;
    TextureInfo GROZA;
    TextureInfo KAR98K;
    TextureInfo M24;
    TextureInfo PAN;
    TextureInfo SCARL;
    TextureInfo SKS;
    TextureInfo UMP45;
    TextureInfo VECTOR;
    TextureInfo VSS;
    TextureInfo MP5;
} weaponHand;

TextureInfo CreateTexture(const unsigned char* buf, int len);

void InitTexture() {
    weaponHand.AKM          = CreateTexture(AKM_data, sizeof(AKM_data));
    weaponHand.AUG          = CreateTexture(AUG_data, sizeof(AUG_data));
    weaponHand.AWM          = CreateTexture(AWM_data, sizeof(AWM_data));
    weaponHand.Desert_Eagle = CreateTexture(Desert_Eagle_data, sizeof(Desert_Eagle_data));
    weaponHand.FIST         = CreateTexture(FIST_data, sizeof(FIST_data));
    weaponHand.GROZA        = CreateTexture(GROZA_data, sizeof(GROZA_data));
    weaponHand.KAR98K       = CreateTexture(KAR98K_data, sizeof(KAR98K_data));
    weaponHand.M24          = CreateTexture(M24_data, sizeof(M24_data));
    weaponHand.PAN          = CreateTexture(PAN_data, sizeof(PAN_data));
    weaponHand.SCARL        = CreateTexture(SCARL_data, sizeof(SCARL_data));
    weaponHand.SKS          = CreateTexture(SKS_data, sizeof(SKS_data));
    weaponHand.UMP45        = CreateTexture(UMP45_data, sizeof(UMP45_data));
    weaponHand.VECTOR       = CreateTexture(VECTOR_data, sizeof(VECTOR_data));
    weaponHand.VSS          = CreateTexture(VSS_data, sizeof(VSS_data));
    weaponHand.VSS          = CreateTexture(MP5_data, sizeof(MP5_data));
}

TextureInfo GetFreeFireWeaponTexture(int weaponId) {
    switch (weaponId) {
        case 102: return weaponHand.AKM;
        case 111: return weaponHand.AUG;
        case 106: return weaponHand.GROZA;
        case 104: return weaponHand.SCARL;
        case 301: return weaponHand.UMP45;
        case 308: return weaponHand.VECTOR;
        case 303: return weaponHand.VSS;
        case 903: return weaponHand.MP5;
        case 203: return weaponHand.AWM;
        case 204: return weaponHand.KAR98K;
        case 202: return weaponHand.SKS;
        case 208: case 205: return weaponHand.M24;
        case 503: return weaponHand.Desert_Eagle; 
        case 701: return weaponHand.PAN;
        case 0:   return weaponHand.FIST;
        default: { TextureInfo empty{}; empty.textureId = 0; return empty; }
    }
}
