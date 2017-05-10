#include "GameBase.h"

#include <thread>

#include <Shiny.h>

#include <ren/RenderState.h>
#include <sys/Json.h>
#include <sys/Time_.h>
#include <sys/ThreadPool.h>
#include <ui/BaseElement.h>
#include <ui/Renderer.h>

#include "FlowControl.h"
#include "GameStateManager.h"
#include "TimedInput.h"

GameBase::GameBase(int w, int h, const char *local_dir) : width(w), height(h) {
    terminated = false;

#if !defined(__EMSCRIPTEN__)
    size_t num_threads = (size_t)std::max(int(std::thread::hardware_concurrency()) - 1, 1);
    auto pool = std::make_shared<sys::ThreadPool>(num_threads);
    AddComponent(THREAD_POOL_KEY, pool);
#endif

    auto state_manager = std::make_shared<GameStateManager>();
    AddComponent(STATE_MANAGER_KEY, state_manager);

    auto input_manager = std::make_shared<InputManager>();
    input_manager->SetConverter(InputManager::RAW_INPUT_P1_MOVE, [w, h](InputManager::Event &evt) {
        evt.move.dx *= 100.0f / w;
        evt.move.dy *= 100.0f / w;
    });
    input_manager->SetConverter(InputManager::RAW_INPUT_P2_MOVE, [w, h](InputManager::Event &evt) {
        evt.move.dx *= 100.0f / w;
        evt.move.dy *= 100.0f / w;
    });
    AddComponent(INPUT_MANAGER_KEY, input_manager);

    auto flow_control = std::make_shared<FlowControl>(2*NET_UPDATE_DELTA, NET_UPDATE_DELTA);
    AddComponent(FLOW_CONTROL_KEY, flow_control);

    JsObject config;
    config[ui::GL_DEFINES_KEY] = "";
    auto ui_renderer = std::make_shared<ui::Renderer>(config);
    AddComponent(UI_RENDERER_KEY, ui_renderer);

    auto ui_root = std::make_shared<ui::RootElement>(glm::ivec2(w, h));
    AddComponent(UI_ROOT_KEY, ui_root);
}

GameBase::~GameBase() {

}

void GameBase::Resize(int w, int h) {
    width = w; height = h;

    R::Resize(width, height);

    auto ui_root = GetComponent<ui::RootElement>(UI_ROOT_KEY);
    ui_root->set_zone({width, height});
    ui_root->Resize(nullptr);
}

void GameBase::Start() {

}

void GameBase::Frame() {
    auto state_manager = GetComponent<GameStateManager>(STATE_MANAGER_KEY);
    auto input_manager = GetComponent<InputManager>(INPUT_MANAGER_KEY);

    PROFILE_FUNC();

    static int frame_count = 0;
    static int fps_time_acc = 0;
    FrameInfo &fr = fr_info_;

    fr.cur_time = sys::GetTicks();
    if (fr.cur_time < fr.prev_time)fr.prev_time = 0;
    fr.delta_time = fr.cur_time - fr.prev_time;
    if (fr.delta_time > 200) {
        fr.delta_time = 200;
    }
    fr.prev_time = fr.cur_time;
    fr.time_acc += fr.delta_time;
    fps_time_acc += fr.delta_time;

    sys::cached_time = fr.cur_time - fr.time_acc;

    {   PROFILE_BLOCK(Update);
        while (fr.time_acc >= UPDATE_DELTA) {
            InputManager::Event evt;
            while (input_manager->PollEvent(sys::cached_time, evt)) {
                state_manager->HandleInput(evt);
            }

            state_manager->Update(UPDATE_DELTA);
            fr.time_acc -= UPDATE_DELTA;

            sys::cached_time += UPDATE_DELTA;
        }
    }

    if (fps_time_acc > 1000) {
        int fps = frame_count;
        frame_count = 0;
        //if (sc_) {
        //    sc_->SetCVAR("r_fps", (double) fps);
        //}
        fps_time_acc -= 1000;
    }

    fr.time_fract = float(fr.time_acc) / UPDATE_DELTA;

    {   PROFILE_BLOCK(Draw);
        state_manager->Draw(0.001f * fr_info_.delta_time);
    }

    frame_count++;
}

void GameBase::Quit() {
    terminated = true;
}
