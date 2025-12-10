#pragma once

class Timer {
private:
    float time;
public:
    Timer(): time(0.f) {}
    void reset(float t) { time = t; }
    void update(float dt) { if (time > 0.f) time -= dt; if (time < 0.f) time = 0.f; }
    float getTime() const { return time; }
};