#pragma once

class Timer {
private:
    float time; // tiempo en segundos

public:
    Timer() : time(0.f) {}

    // Reinicia el timer
    void reset(float t) { time = t; }

    // Actualiza el timer cada frame
    void update(float dt) { if (time > 0.f) time -= dt; }

    // Devuelve el tiempo restante
    float getTime() const { return time; }
};