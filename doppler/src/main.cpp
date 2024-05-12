#include <SFML/Graphics/CircleShape.hpp>
#include <vector>
#include <SFML/Graphics.hpp>

const sf::Vector2f FRAME(1275, 957);

int main() {
    sf::RenderWindow window(sf::VideoMode(FRAME.x, FRAME.y), "Doppler Effect Simulation");
    window.setFramerateLimit(60);

    unsigned int time = 0;

	std::vector<sf::CircleShape> wavefronts;

	sf::CircleShape source(10);
	source.setPosition(sf::Vector2f(250, FRAME.y / 2));
	source.setFillColor(sf::Color(255, 0, 0));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the window with black color
        window.clear(sf::Color(255, 255, 255));

        if (time % 5 == 0) {
			sf::CircleShape wavefront(10);
			wavefront.setOutlineThickness(2);
			wavefront.setPosition(source.getPosition());
			wavefront.setOutlineColor(sf::Color(50, 50, 255));
			wavefront.setFillColor(sf::Color(0, 0, 0, 0));
			wavefronts.push_back(wavefront);
        }
        for (size_t i = 0; i < wavefronts.size(); i++) {
			wavefronts[i].setRadius(wavefronts[i].getRadius() + 3);
			wavefronts[i].move(sf::Vector2f(-3, -3));
			if (wavefronts[i].getRadius() > 200) wavefronts[i].setPointCount(300);
			window.draw(wavefronts[i]);
        }
	
		source.move(sf::Vector2f(2.3, 0));
        window.draw(source);

        // Display the updated contents of the window
        window.display();
        time++;
    }

    return 0;
}

