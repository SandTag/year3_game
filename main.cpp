#include <SFML/Graphics.hpp>

//=============================================================================================
//Pong Params and keybinds
const sf::Keyboard::Key controls[5] = {
    sf::Keyboard::W,
    sf::Keyboard::S,
    sf::Keyboard::Up,
    sf::Keyboard::Down,
    sf::Keyboard::Escape
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

sf::Vector2f ball_velocity;
bool is_player1_serving = true;
const float initial_velocity_x = 100.f; //horizontal velocity
const float initial_velocity_y = 60.f; //vertical velocity

const float velocity_multiplier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.
sf::RenderWindow windowMain(sf::VideoMode({ game_width, game_height }), "PONG");

//=============================================================================================
//Pong Code

static void ballBounce() {
    ball_velocity.x = (ball_velocity.x * -velocity_multiplier);
    ball_velocity.y = (ball_velocity.y * -velocity_multiplier);
}

static void resetObjs() {
    // reset paddle position
    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, game_height / 2.f);
    paddles[1].setPosition((800.f - (paddleOffsetWall + (paddleSize.x / 2.f))), game_height / 2.f);
    // reset Ball Position
    ball.setPosition(game_width / 2.f, game_height / 2.f);
}

static void reset() {
    resetObjs();
    ball_velocity = { (is_player1_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
}

static bool paddleCollided(sf::RectangleShape paddle, float bx, float by, bool R) {

    if (R) {//Right
        if (
        bx > paddleSize.x + paddleOffsetWall &&
            by > paddle.getPosition().y - (paddleSize.y * 0.5f) &&
            by < paddle.getPosition().y + (paddleSize.y * 0.5f))
            {
                return true;
    }
    }
    else if (//Left

        //ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddle.getPosition().y - (paddleSize.y * 0.5f) &&
        //ball is above bottom edge of paddle
        by < paddle.getPosition().y + (paddleSize.y * 0.5f)) {
        return true;

        /*
        //ball is inline or behind paddle AND
	    bx < paddle_size.x + paddle_offset_wall && 
	     //ball is below top edge of paddle AND
	    by > paddles[0].getPosition().y - (paddle_size.y * 0.5) &&
	    //ball is above bottom edge of paddle
	    by < paddles[0].getPosition().y + (paddle_size.y * 0.5)){
            //bounces off left paddle
        */
    }
    else {
        return false;
    }
}

static void init() {
    // Set size and origin of paddles
    for (sf::RectangleShape& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(5.f, 5.f); //Half the ball width and height
    reset();
}

static void update(float dt) {
    // handle paddle movement
    float direction0 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])
        &&
        paddles[0].getPosition().y > paddleSize.y / 2.f//lower boundary
        ) {
        direction0--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])
        &&
        paddles[0].getPosition().y < game_height - paddleSize.y / 2.f
        ) {
        direction0++;
    }//Keybinds

    paddles[0].move(sf::Vector2f(0.f, direction0 * paddleSpeed * dt));//Paddle left Calc Movement

    //paddle 2
    float direction1 = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[2])
        &&
        paddles[1].getPosition().y > paddleSize.y / 2.f
        ) {
        direction1--;
    }
    if (sf::Keyboard::isKeyPressed(controls[3])
        &&
        paddles[1].getPosition().y < game_height - paddleSize.y / 2.f
        ) {
        direction1++;
    }//Keybinds

    if (sf::Keyboard::isKeyPressed(controls[4]))//Close the window and exit the program with escape key.
    {
        windowMain.close();
        exit(0);
    }

    paddles[1].move(sf::Vector2f(0.f, direction1 * paddleSpeed * dt));//Paddle right Calc Movement

    ball.move(ball_velocity * dt);//Ball Calc Movement

    // check ball collision after movement
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > game_height) { //bottom wall
        // bottom wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, -10.f));
    }
    else if (by < 0) { //top wall
        // top wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, 10.f));
    }

    else if (
        paddleCollided(paddles[0], bx, by, false)) {
        ballBounce();
        ball.move(sf::Vector2f(+10.f, 0));
    }
    else if (
        paddleCollided(paddles[1], bx, by, true)) {
        ballBounce();
        ball.move(sf::Vector2f(-10.f, 0));
    }
    else if (bx > game_width) {
        // right wall
        reset();
    }
    else if (bx < 0) {
        // left wall
        reset();
    }
    
}

static void render(sf::RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

static void clean() {
    //free up the memory if necessary.
}

static void pong() {
    	//create the window
	//windowMain = sf::RenderWindow(sf::VideoMode({game_width, game_height}), "PONG");

    // enable or disable vsync - do at start of game, or via options menu
    windowMain.setVerticalSyncEnabled(true);

    //initialise and load
	init();
	while(windowMain.isOpen()){
        static sf::Clock clock;
        const float dt = clock.restart().asSeconds();
		//Calculate dt
		
        windowMain.clear();
		update(dt);
		render(windowMain);
		//wait for the time_step to finish before displaying the next frame.
		sf::sleep(sf::seconds(time_step));


		//Wait for Vsync
        windowMain.display();
	}
	//Unload and shutdown
	clean();
}

//=============================================================================================

int main() {
    pong();
    return 0;
}