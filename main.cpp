#include <sstream>
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::W,   // Player1 Fire
    sf::Keyboard::A,   // Player1 Left
    sf::Keyboard::D,  // Player2 Right
};

//Parameters
const int game_width = 800;
const int game_height = 600;
const float time_step = 0.017f; //60 fps
int score[2] = { 0,0 };

//Objects
sf::Text text;
sf::Font font;
sf::Sprite invader;

//Textures
sf::Texture spritesheet;


void reset() {
}

void init() {
    // Load font-face from res dir
    font.loadFromFile("resources/fonts/RobotoMono-Regular.ttf");
    // Set text element to use font
    text.setFont(font);
    // set the character size to 24 pixels
    text.setCharacterSize(24);
    text.setColor(sf::Color(255, 255, 255));

    //Load invader
    if (!spritesheet.loadFromFile("/resources/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }
    invader.setTexture(spritesheet);
    invader.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));

    reset();
}


void update(float dt) {

}



void render(sf::RenderWindow& window) {
    // Draw Everything
    window.draw(text);
    window.draw(invader);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ game_width, game_height }), "INV");
    window.setVerticalSyncEnabled(true);
    init();
    //initialise and load
    while (window.isOpen()) {
        // Reset clock, recalculate deltatime
        static sf::Clock clock;
        float dt = clock.restart().asSeconds();
        // check and consume events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }
        // Quit Via ESC Key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.clear();
        update(dt);
        render(window);
        //wait for the time_step to finish before displaying the next frame.
        sf::sleep(sf::seconds(time_step));
        //Wait for Vsync
        window.display();
    }
    return 0;
}