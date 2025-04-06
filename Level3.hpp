//
//  Level3.hpp
//  HatHeistProject
//


#ifndef Level3_hpp
#define Level3_hpp

#include <stdio.h>
#pragma once

#include "Scene.h"
#include <vector"

class Level3 : public Scene {
public:
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* shader) override;

private:
    std::vector<Entity*> enemies;
    std::vector<Entity*> hats;
};

#endif /* Level3_hpp */
