//
//  Level2.hpp
//  HatHeistProject
//

#ifndef Level2_hpp
#define Level2_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"
#include <vector>

class Level2 : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;

private:
    std::vector<Entity*> enemies;
    std::vector<Entity*> hats;  
};

#endif /* Level2_hpp */
