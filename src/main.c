#include "core/core_platform.h"
#include "scenes/scene_manager.h"

int main(void) {
    Core_Init();
    SM_Init();

    while (!Core_ShouldClose()) {
        Core_UpdateMusic();
        SM_Update();
        SM_Draw();
    }

    SM_Deinit();
    Core_Close();
    
    return 0;
}