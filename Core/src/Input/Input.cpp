#include "Input/Input.h"
#include <unordered_map>

namespace Core {
    static std::unordered_map<Key, bool> s_Current;
    static std::unordered_map<Key, bool> s_Previous;
    static Vec2 s_MouseDelta{ 0.0f };


    void Input::SetKey(Key key, bool pressed) {
        s_Current[key] = pressed;
    }

    bool Input::IsKeyDown(Key key) {
        return s_Current[key];
    }

    bool Input::IsKeyPressed(Key key) {
        return s_Current[key] && !s_Previous[key];
    }

    void Input::SetMouseDelta(float dx, float dy) {
        s_MouseDelta.x += dx;
        s_MouseDelta.y += dy;
    }

    Vec2 Input::GetMouseDelta() {
        return s_MouseDelta;
    }

    void Input::EndFrame() {
        s_Previous = s_Current;
        s_MouseDelta = { 0.0f, 0.0f };
    }
}
