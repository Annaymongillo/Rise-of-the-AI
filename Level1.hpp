//
//  Level1.hpp
//  HatHeistProject
//
//

#ifndef Level1_hpp
#define Level1_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"
#include <vector>
#include "Entity.hpp"
class Level1 : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;

private:
    std::vector<Entity*> enemies;
    std::vector<Entity*> collectibles; 
};

#endif /* Level1_hpp */
