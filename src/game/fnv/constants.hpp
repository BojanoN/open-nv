#pragma once

namespace Constants {

namespace FNV {
    inline constexpr const unsigned int noLoadingScreenTextures                            = 4;
    inline constexpr const char*        pathLoadingScreenTextures[noLoadingScreenTextures] = {
        "textures\\interface\\loading\\loading_screen01.dds",
        "textures\\interface\\loading\\loading_screen_legal.dds",
        "textures\\interface\\loading\\loading_screen_bgs.dds",
        "textures\\interface\\loading\\loading_screen_bethsoft.dds"
    };
    inline constexpr const char* fsLoadingScreen = "loadingscreen.fs";
    inline constexpr const char* vsLoadingScreen = "loadingscreen.vs";

    inline constexpr const char* relpathIntroVideo = "./Data/Video/FNVIntro.bik";
}
}
