#ifndef INTRO_H
#define INTRO_H

#include <string>
#include <vector>
#include "text.h"

struct Slide {
    std::string text;
    float duration;
    float fade_time;
};

class Intro {
    public:
        std::vector<Slide> slides;

        int current_slide = 0;
        float timer = 0.0f;
        bool done = false;

        Intro(std::vector<Slide> slides);

        void update(float delta_time);
        void render(TextRenderer& text_renderer, unsigned int width, unsigned int height);
        float get_alpha();
};

#endif