//
//  MenuScene.hpp
//  HatHeistProject
//


#ifndef MenuScene_hpp
#define MenuScene_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"

class MenuScene : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;
};
#endif /* MenuScene_hpp */
