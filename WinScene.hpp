//
//  WinScene.hpp
//  HatHeistProject
//


#ifndef WinScene_hpp
#define WinScene_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"

class WinScene : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;
};

#endif /* WinScene_hpp */
