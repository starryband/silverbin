#include <engine_events/intro.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

Intro::Intro(std::vector<Slide> slides) : slides(slides) {}

void Intro::update(float delta_time) {
    if (done)
        return;

    timer += delta_time;

    Slide& slide = slides[current_slide];
    float total = slide.fade_time * 2 + slide.duration;

    if (timer >= total) {
        timer = 0.0f;
        current_slide++;

        if (current_slide >= slides.size()) {
            done = true;
        }
    }
}

float Intro::get_alpha() {
    if (done)
        return 0.0f;

    Slide& slide = slides[current_slide];

    float fade = slide.fade_time;
    float duration = slide.duration;

    if (timer < fade) {
        return timer / fade;
    } else if (timer < fade + duration) {
        return 1.0f;
    } else {
        return 1.0f - (timer - fade - duration) / fade;
    }
}

void Intro::render(TextRenderer& text_renderer, unsigned int width, unsigned int height) {
    if (done)
        return;

    float alpha = get_alpha();
    std::string text = slides[current_slide].text;

    float x = width / 2.0f - (text.size() * 12.0f);
    float y = height / 2.0f;

    text_renderer.render(text, x, y, 1.0f, glm::vec4(alpha, alpha, alpha, alpha));
}