#include <SFML/Graphics.hpp>
#include "MAP.h"
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 600), "SFML Test");
    CircleShape circle(50.0f);
    circle.setFillColor(Color::Red);
    circle.setPosition(100.f, 100.f);
    float speed = 100.f;
    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        circle.move(speed * deltaTime, 0.f);

        if (circle.getPosition().x > window.getSize().x) circle.setPosition(circle.getRadius() * 2, circle.getPosition().y);

        window.clear(Color::Black);
        window.draw(circle);
        window.display();
    }

   /* Map<int, int> mp;

    mp.insert(1, 5);
    mp.insert(2, 6);
    mp.insert(3, 7);
    mp.insert(4, 8);

    mp.printInOrder();*/

    return 0;
}