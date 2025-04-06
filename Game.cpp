//
//  Game.cpp
//  HatHeistProject
/**
* Author: Annay Mongillo
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "Game.hpp"

#include <SDL_opengl.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "glm/gtc/matrix_transform.hpp"

#include "MenuScene.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"
#include "GameOverScene.hpp"
#include "WinScene.hpp"

#define MS_PER_SEC       1000.0f
#define WINDOW_WIDTH     640
#define WINDOW_HEIGHT    480
#define FIXED_TIMESTEP   0.0166666f  // 60 FPS

Game::Game()
    : window(nullptr), effects(nullptr), current_scene(nullptr),
      prev_ticks(0.0f), accumulator(0.0f), running(true) {}

Game::~Game() {
    shutdown();
}

void Game::switch_to_scene(Scene* scene) {
    current_scene = scene;
    current_scene->initialise();
}

void Game::initialise() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    window = SDL_CreateWindow("Hat Heist",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    shader.load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    shader.set_projection_matrix(projection_matrix);
    glUseProgram(shader.get_program_id());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Scene order: [0] Menu, [1] Level1, [2] Level2, [3] Level3, [4] GameOver, [5] Win
    scenes[0] = new MenuScene();
    scenes[1] = new Level1();
    scenes[2] = new Level2();
    scenes[3] = new Level3();
    scenes[4] = new GameOverScene();
    scenes[5] = new WinScene();

    effects = new Effects(projection_matrix, glm::mat4(1.0f));
    switch_to_scene(scenes[0]);  // Start at menu
}

void Game::process_input() {
    if (current_scene->has_player()) {
        current_scene->get_player()->set_movement(glm::vec3(0.0f));
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    running = false;
                    break;

                case SDLK_RETURN:
                    if (current_scene == scenes[0]) {
                        current_scene->get_state().next_scene_id = 1; // Menu → Level1
                    } else if (current_scene == scenes[4] || current_scene == scenes[5]) {
                        current_scene->get_state().next_scene_id = 0; // GameOver/Win → Menu
                    }
                    break;

                case SDLK_SPACE:
                    if (current_scene->has_player() &&
                        current_scene->get_player()->get_collided_bottom()) {
                        current_scene->get_player()->jump();
                        Mix_PlayChannel(-1, current_scene->get_state().jump_sfx, 0);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (current_scene->has_player()) {
        if (keys[SDL_SCANCODE_LEFT])  current_scene->get_player()->move_left();
        if (keys[SDL_SCANCODE_RIGHT]) current_scene->get_player()->move_right();
        if (glm::length(current_scene->get_player()->get_movement()) > 1.0f)
            current_scene->get_player()->normalise_movement();
    }
}

void Game::update() {
    float ticks = (float)SDL_GetTicks() / MS_PER_SEC;
    float delta_time = ticks - prev_ticks;
    prev_ticks = ticks;
    delta_time += accumulator;

    if (delta_time < FIXED_TIMESTEP) {
        accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        current_scene->update(FIXED_TIMESTEP);
        effects->update(FIXED_TIMESTEP);
        delta_time -= FIXED_TIMESTEP;
    }

    accumulator = delta_time;


    view_matrix = glm::mat4(1.0f);
    if (current_scene->has_player()) {
        float player_x = current_scene->get_player()->get_position().x;
        float cam_x = (player_x > 5.0f) ? -player_x : -5.0f;
        view_matrix = glm::translate(view_matrix, glm::vec3(cam_x, 3.75f, 0));
    }

    view_matrix += effects->get_view_offset();
}

void Game::render() {
    shader.set_view_matrix(view_matrix);

    glClearColor(0.2f, 0.55f, 0.9f, 1.0f);  // Soft sky blue background
    glClear(GL_COLOR_BUFFER_BIT);

    current_scene->render(&shader);
    effects->render();

    SDL_GL_SwapWindow(window);
}

void Game::shutdown() {
    SDL_Quit();

    for (auto* scene : scenes) delete scene;
    delete effects;
}

void Game::run() {
    initialise();

    while (running) {
        process_input();
        update();

        if (current_scene->get_state().next_scene_id >= 0) {
            switch_to_scene(scenes[current_scene->get_state().next_scene_id]);
        }

        render();
    }

    shutdown();
}
