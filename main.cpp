/**
* Author: Annay Mongillo
* Assignment: Rise of the AI
* Date due: 2025-04-05, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f  // 60 FPS

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Effects.hpp"
#include "Scene.h"
#include "MenuScene.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"
#include "GameOverScene.hpp"
#include "WinScene.hpp"


constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr float MS_PER_SEC = 1000.0f;

SDL_Window* g_window;
ShaderProgram g_shader;
glm::mat4 g_view_matrix, g_projection_matrix;

Scene* g_current_scene = nullptr;
Scene* g_scenes[6];
Effects* g_effects;

float g_prev_ticks = 0.0f;
float g_accumulator = 0.0f;
bool g_running = true;

void switch_to_scene(Scene* scene) {
    g_current_scene = scene;
    g_current_scene->initialise();
}

void initialise() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_window = SDL_CreateWindow("Hat Heist",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_window);
    SDL_GL_MakeCurrent(g_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    g_shader.load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    g_shader.set_projection_matrix(g_projection_matrix);
    glUseProgram(g_shader.get_program_id());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_scenes[0] = new MenuScene();
    g_scenes[1] = new Level1();
    g_scenes[2] = new Level2();
    g_scenes[3] = new Level3();
    g_scenes[4] = new GameOverScene();
    g_scenes[5] = new WinScene();

    g_effects = new Effects(g_projection_matrix, glm::mat4(1.0f));
    switch_to_scene(g_scenes[0]);
}

void process_input() {
    if (g_current_scene->has_player()) {
        g_current_scene->get_player()->set_movement(glm::vec3(0.0f));
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            g_running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    g_running = false;
                    break;
                case SDLK_RETURN:
                    if (g_current_scene == g_scenes[0]) {
                        g_current_scene->get_state().lives = 3; // RESET LIVES FROM MENU
                        g_current_scene->get_state().next_scene_id = 1; // Menu → Level1
                    }
                    else if (g_current_scene == g_scenes[4] || g_current_scene == g_scenes[5]) {
                        g_scenes[0]->get_state().lives = 3; // RESET LIVES FROM WIN/LOSE
                        g_current_scene->get_state().next_scene_id = 0; // → Menu
                    }
                    break;
                case SDLK_SPACE:
                    if (g_current_scene->has_player() &&
                        g_current_scene->get_player()->get_collided_bottom()) {
                        g_current_scene->get_player()->jump();
                        Mix_PlayChannel(-1, g_current_scene->get_state().jump_sfx, 0);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (g_current_scene->has_player()) {
        if (keys[SDL_SCANCODE_LEFT]) g_current_scene->get_player()->move_left();
        if (keys[SDL_SCANCODE_RIGHT]) g_current_scene->get_player()->move_right();
        if (glm::length(g_current_scene->get_player()->get_movement()) > 1.0f)
            g_current_scene->get_player()->normalise_movement();
    }
}

void update() {
    float ticks = (float)SDL_GetTicks() / MS_PER_SEC;
    float delta_time = ticks - g_prev_ticks;
    g_prev_ticks = ticks;
    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP) {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        g_current_scene->update(FIXED_TIMESTEP);
        g_effects->update(FIXED_TIMESTEP);
        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;

    // Camera follow
    g_view_matrix = glm::mat4(1.0f);
    if (g_current_scene->has_player()) {
        float x = g_current_scene->get_player()->get_position().x;
        float cam_x = x > 5 ? -x : -5;
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(cam_x, 3.75f, 0));
    }
    g_view_matrix += g_effects->get_view_offset();
}

void render() {
    g_shader.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);
    g_current_scene->render(&g_shader);
    g_effects->render();
    SDL_GL_SwapWindow(g_window);
}

void shutdown() {
    SDL_Quit();
    for (Scene* scene : g_scenes) delete scene;
    delete g_effects;
}

int main(int argc, char* argv[]) {
    initialise();

    while (g_running) {
        process_input();
        update();
        if (g_current_scene->get_state().next_scene_id >= 0) {
            switch_to_scene(g_scenes[g_current_scene->get_state().next_scene_id]);
        }
        render();
    }

    shutdown();
    return 0;
}
