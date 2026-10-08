#include <SFML/Graphics.hpp>


/* Unsure when this is needed.

float playerSpeed = 10.0f;
void Update(double dt) {
  //Good!
  if(moveButton.pressed){
    player.move(playerSpeed * dt);
  }
}

*/

//=============================================================================================
//Pong Params and keybinds
const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::W,
    sf::Keyboard::S,
    sf::Keyboard::Up,
    sf::Keyboard::Down
};

const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int game_width = 800;
const int game_height = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps

sf::CircleShape ball;
sf::RectangleShape paddles[2];

//=============================================================================================
//Pong Code



void init() {
    // Set size and origin of paddles
    for (sf::RectangleShape& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(5.f, 5.f); //Half the ball width and height
    // reset paddle position
    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, game_height / 2.f);
    paddles[1].setPosition( (800.f - (paddleOffsetWall + (paddleSize.x / 2.f))), game_height / 2.f);
    // reset Ball Position
    ball.setPosition(game_width/2.f, game_height/2.f);
}

void update(float dt) {
    // handle paddle movement
    float direction = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(sf::Vector2f(0.f, direction * paddleSpeed * dt));
}

void render(sf::RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

void clean() {
    //free up the memory if necessary.
}

void pong() {
    	//create the window
	sf::RenderWindow window(sf::VideoMode({game_width, game_height}), "PONG");

    // enable or disable vsync - do at start of game, or via options menu
    window.setVerticalSyncEnabled(true);

    //initialise and load
	init();
	while(window.isOpen()){
        static sf::Clock clock;
        const float dt = clock.restart().asSeconds();
		//Calculate dt
		
		window.clear();
		update(dt);
		render(window);
		//wait for the time_step to finish before displaying the next frame.
		sf::sleep(sf::seconds(time_step));


		//Wait for Vsync
        window.display();
	}
	//Unload and shutdown
	clean();
}

//=============================================================================================

int main() {
    pong();
    return 0;
}