#include "plugin.h"
#include "GBH.h"
#include "CTextureManager.h"
#include "CSprite2D.h"
#include "CMenuManager.h"
#include "CStyle.h"
#include "CGame.h"
#include "CGlobal.h"

#include "Hooking.Patterns.h"
#include "..\injector\utility.hpp"

#include <format>
#include <filesystem>

using namespace plugin;

class TextureLoaderII {
public:
    static inline injector::hook_back<int(__stdcall*)(tHardwareTexture*)> setTextureHookBack;
    static inline unsigned short textureIndex = -1;
    static inline unsigned short spriteIndex = -1;

    static inline CSprite2d* textures = {};
    static inline CSprite2d* sprites = {};

    inline enum {
        NUM_TILES = 992, // 1024,
        NUM_SPRITES = 2048
    };

    static inline int __stdcall SetTexture(tHardwareTexture* hwTexture) {
        void* texture = nullptr;

        if (textureIndex != -1 && textureIndex < NUM_TILES)
            texture = textures[textureIndex].m_pTexture;
        else if (spriteIndex != -1 && spriteIndex < NUM_SPRITES)
            texture = sprites[spriteIndex].m_pTexture;

        textureIndex = -1;
        spriteIndex = -1;

        if (texture) {
            RenderStateSet(D3DRENDERSTATE_TEXTUREHANDLE, texture);
            return 0;
        }

        return setTextureHookBack.fun(hwTexture);
    }

    static inline void Shutdown() {
        for (int i = 0; i < NUM_TILES; i++) {
            textures[i].Delete();
        }

        for (int i = 0; i < NUM_SPRITES; i++) {
            sprites[i].Delete();
        }
    }

    static inline void Init() {
        char styName[32] = {};
        const char* s = gGlobal.styleName;
        s += 5;

        int i = 0;
        while (*s) {
            if (*s == '.') {
                styName[i] = '\0';
                break;
            }

            styName[i] = *s;
            i++;
            s++;
        }

        char folder1[MAX_PATH];
        sprintf(folder1, "data/textures/%s/tiles", styName);
        char folder2[MAX_PATH];
        sprintf(folder2, "data/textures/%s/sprites", styName);

        std::filesystem::create_directories(folder1);
        std::filesystem::create_directories(folder2);

        char buff[MAX_PATH];
        for (int i = 0; i < NUM_TILES; i++) {
            sprintf(buff, "%s/GTA2_TILE_%d.dds", folder1, i);
            textures[i].SetTexture(buff);
        }

        for (int i = 0; i < NUM_SPRITES; i++) {
            sprintf(buff, "%s/GTA2_SPRITE_%d.dds", folder2, i);
            sprites[i].SetTexture(buff);
        }
    }

    TextureLoaderII() {
        ThiscallEvent <AddressList<0x462028, H_CALL>, PRIORITY_AFTER, ArgPickNone, void(CGame*)> onGameInit;
        onGameInit += []() {
            Shutdown();
            Init();
        };

        plugin::Events::initEngineEvent += []() {
            textures = new CSprite2d[NUM_TILES];
            sprites = new CSprite2d[NUM_SPRITES];

            ThiscallEvent <AddressList<
                0x46C29A, H_CALL,
                0x46C7C3, H_CALL,
                0x46CE03, H_CALL,
                0x46D334, H_CALL,
                0x46D4C2, H_CALL,
                0x46D656, H_CALL,
                0x46D7E6, H_CALL,
                0x46D972, H_CALL,
                0x46DFAB, H_CALL,
                0x46E28E, H_CALL,
                0x46F33F, H_CALL,
                0x46F790, H_CALL,
                0x46FBE4, H_CALL,
                0x470036, H_CALL,
                0x47021B, H_CALL,
                0x47040B, H_CALL,
                0x4705F7, H_CALL,
                0x4707D7, H_CALL,
                0x4709B4, H_CALL>, PRIORITY_AFTER, ArgPick2N<CTextureManager*, 0, unsigned short*, 1>, void(CTextureManager*, unsigned short*)> onGetTexture;
            onGetTexture += [](CTextureManager* _this, unsigned short* id) {
                textureIndex = *id;
                spriteIndex = -1;
            };

            ThiscallEvent <AddressList<
                0x460E29, H_CALL,
                0x460E77, H_CALL,
                0x4BE142, H_CALL,
                0x4CBD84, H_CALL,
                0x4CC349, H_CALL>, PRIORITY_AFTER, ArgPick5N<CTextureManager*, 0, int, 1, int, 2, int, 3, int, 4>, void(CTextureManager*, int, int, int, int)> onGetSprite;
            onGetSprite += [](CTextureManager* _this, int base, int texture, int mode, int enableAlpha) {
                if (GetFrontendMenuManager())
                    return;

                spriteIndex = GetCurrentStyle()->GetBaseIndex(base, texture);
                textureIndex = -1;

                double w = GetCurrentStyle()->m_pSpriteIndex[spriteIndex].width;
                w = pow(2, ceil(log(w) / log(2)));

                double h = GetCurrentStyle()->m_pSpriteIndex[spriteIndex].height;
                h = pow(2, ceil(log(h) / log(2)));

                // Little hack for uv
                if (void* t = sprites[spriteIndex].m_pTexture) {
                    *(float*)0x673428 = w - 0.000099999997;
                    *(float*)0x673448 = *(float*)0x673428;
                    *(float*)0x67344C = h - 0.000099999997;
                    *(float*)0x67346C = *(float*)0x67344C;
                }
            };

            auto pattern = hook::module_pattern(GetModuleHandle("d3ddll"), "E8 ? ? ? ? 5E C2 10 00");
            setTextureHookBack.fun = injector::MakeCALL(pattern.get_first(0xE02F34 - 0xE04541), SetTexture, true).get();
            injector::MakeCALL(pattern.get_first(0xE02F34 - 0xE04541), SetTexture, true);
        };

        plugin::Events::shutdownEngineEvent += []() {
            Shutdown();
            delete[] textures;
            delete[] sprites;
        };
    };
} textureLoaderII;
