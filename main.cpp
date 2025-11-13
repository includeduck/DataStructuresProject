#include <SFML/Graphics.hpp>

int main()
{
    // Create a 800x600 window with title
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    // Create a red circle shape
    sf::CircleShape circle(50.0f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(100.f, 100.f);

    // Movement speed
    float speed = 100.f;

    // Clock to measure time per frame
    sf::Clock clock;

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move the circle over time
        float deltaTime = clock.restart().asSeconds();
        circle.move(speed * deltaTime, 0.f);

        // Wrap around the screen
        if (circle.getPosition().x > window.getSize().x)
            circle.setPosition(-circle.getRadius() * 2, circle.getPosition().y);

        // Draw everything
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}