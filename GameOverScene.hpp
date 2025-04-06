//
//  GameOverScene.hpp
//  HatHeistProject
//


#ifndef GameOverScene_hpp
#define GameOverScene_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"

class GameOverScene : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;
};
#endif /* GameOverScene_hpp */
