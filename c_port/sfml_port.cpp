/**
 * Very simple port of a specific subset of SFML,
 * coded specifically for this experimental project.
 */

#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>

// this is not a perfect test of the size of double, but it's good enough for our purposes
static_assert(sizeof(double) == 8, "double must be 64 bits");

// Works with SFML 2 (e.g. 2.6) and SFML 3. Where their APIs differ, the code
// goes through the helpers below or an `#if SFML_VERSION_MAJOR` block.
namespace compat {
#if SFML_VERSION_MAJOR >= 3
    sf::VideoMode videoMode(unsigned width, unsigned height) { return sf::VideoMode({width, height}); }
    bool loadFont(sf::Font& font, const char* path) { return font.openFromFile(path); }
    sf::Angle degrees(float angle) { return sf::degrees(angle); }
#else
    sf::VideoMode videoMode(unsigned width, unsigned height) { return sf::VideoMode(width, height); }
    bool loadFont(sf::Font& font, const char* path) { return font.loadFromFile(path); }
    float degrees(float angle) { return angle; }
#endif
}

namespace internal {

    struct State final {
        sf::RenderWindow window;
        sf::Clock clock;

        struct EventsState final {
            bool arrow_up_pressed = false;
            bool arrow_down_pressed = false;
            bool arrow_left_pressed = false;
            bool arrow_right_pressed = false;
            
            void reset() {
                arrow_up_pressed = false;
                arrow_down_pressed = false;
                arrow_left_pressed = false;
                arrow_right_pressed = false;
            }
        };
        EventsState events_state;

        State(): 
            window(compat::videoMode(1200, 800), "SFML Duckling Experiment")
        {
            window.setFramerateLimit(120); // make it api call later
        }
    };

    std::optional<State> state;

    void key_pressed(sf::Keyboard::Key code) {
        switch (code) {
            case sf::Keyboard::Key::Up:
                state->events_state.arrow_up_pressed = true;
                break;
            case sf::Keyboard::Key::Down:
                state->events_state.arrow_down_pressed = true;
                break;
            case sf::Keyboard::Key::Left:
                state->events_state.arrow_left_pressed = true;
                break;
            case sf::Keyboard::Key::Right:
                state->events_state.arrow_right_pressed = true;
                break;
            default:
                break;
        }
    }
}

extern "C" {
    double sfml_frame_clock() {
        if (!internal::state) return 0.0;
        auto elapsed = internal::state->clock.restart().asSeconds();

        // draw fps on screen in bottom left corner:
        sf::Font font;
        if (!compat::loadFont(font, "/usr/share/fonts/truetype/freefont/FreeMono.ttf")) { // for now
            // error handling
        }
        else {
            // SFML 3 has no sf::Text constructor without a font.
#if SFML_VERSION_MAJOR >= 3
            sf::Text text(font);
#else
            sf::Text text;
            text.setFont(font);
#endif
            text.setString("FPS: " + std::to_string(static_cast<int>(1.0 / elapsed)));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setPosition(sf::Vector2f(20.0f, 20.0f));

            internal::state->window.draw(text);
        }

        return elapsed;
    }

    void sfml_port_init() {
        internal::state.emplace();
    };

    void sfml_port_deinit() {
        internal::state.reset();
    };

    bool sfml_port_is_open() {
        if (!internal::state) return false;
        return internal::state->window.isOpen();
    }

    void sfml_port_windows_clear() {
        if (!internal::state) return;
        internal::state->window.clear();
    }

    void sfml_port_display() {
        if (!internal::state) return;
        internal::state->window.display();
    }

    void sfml_port_poll_events() {
        if (!internal::state) return;

        internal::state->events_state.reset();

        // SFML 3 returns events from pollEvent as std::optional of a variant-like sf::Event,
        // SFML 2 fills an sf::Event union; both paths pass the key to the same switch.
#if SFML_VERSION_MAJOR >= 3
        while (const std::optional event = internal::state->window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                internal::state->window.close();
            }
            else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                internal::key_pressed(key_pressed->code);
            }
        }
#else
        sf::Event event;
        while (internal::state->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                internal::state->window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                internal::key_pressed(event.key.code);
            }
        }
#endif
    }

    void sfml_draw_rectangle(double x, double y, double width, double height, double rotation) {
        if (!internal::state) return;
        sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));

        rectangle.setOrigin(sf::Vector2f(static_cast<float>(width / 2), static_cast<float>(height / 2)));
        rectangle.setPosition(sf::Vector2f(static_cast<float>(x + width / 2), static_cast<float>(y + height / 2)));
        rectangle.setRotation(compat::degrees(static_cast<float>(rotation)));
        
        rectangle.setFillColor(sf::Color::Green);

        internal::state->window.draw(rectangle);
    }

    // For debug purposes:
    void sfml_draw_debug_arrow(double x1, double y1, double x2, double y2) {
        if (!internal::state) return;
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(static_cast<float>(std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))), 5.0f));

        line.setOrigin(sf::Vector2f(0.0f, 2.5f));
        line.setPosition(sf::Vector2f(static_cast<float>(x1 ), static_cast<float>(y1)));
        line.setRotation(compat::degrees(static_cast<float>(std::atan2(y2 - y1, x2 - x1) * 180.0 / 3.14159265358979323846)));
        line.setFillColor(sf::Color::Red);

        internal::state->window.draw(line);

        // add arrow head:
        sf::CircleShape arrow_head(10.0f, 3);
        arrow_head.setOrigin(sf::Vector2f(10.0f, 10.0f));
        arrow_head.setPosition(sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2)));
        arrow_head.setRotation(compat::degrees(-30 + static_cast<float>(std::atan2(y2 - y1, x2 - x1) * 180.0 / 3.14159265358979323846)));
        arrow_head.setFillColor(sf::Color::Red);

        internal::state->window.draw(arrow_head);
    }

    // @TODO: transition this to key-id based system instead of hardcoding arrow keys
    bool sfml_port_was_arrow_down() {
        if (!internal::state) return false;
        return internal::state->events_state.arrow_down_pressed;
    }

    bool sfml_port_was_arrow_up() {
        if (!internal::state) return false;
        return internal::state->events_state.arrow_up_pressed;
    }

    bool sfml_port_was_arrow_left() {
        if (!internal::state) return false;
        return internal::state->events_state.arrow_left_pressed;
    }

    bool sfml_port_was_arrow_right() {
        if (!internal::state) return false;
        return internal::state->events_state.arrow_right_pressed;
    }

    double math_port_sqrt(double x) {
        return std::sqrt(x);
    }

    double math_port_sin(double x) {
        return std::sin(x);
    }

    double math_port_cos(double x) {
        return std::cos(x);
    }

    double sfml_port_mouse_x_pos() {
        if (!internal::state) return 0.0;
        return static_cast<double>(sf::Mouse::getPosition(internal::state->window).x);
    }

    double sfml_port_mouse_y_pos() {
        if (!internal::state) return 0.0;
        return static_cast<double>(sf::Mouse::getPosition(internal::state->window).y);
    }

}