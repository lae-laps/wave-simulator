#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

sf::Font defaultFont;
//const sf::Vector2f FRAME(1600, 1000);
const sf::Vector2f FRAME(1275, 957);

const int pixelWidth = 4;

struct Source {
	sf::Vector2f position;
	float amplitude;
	float wavelength;
	float velocity;
	unsigned int timeOfCreation;
};



/*
float y(const sf::Vector2f& position, int time, const std::vector<Source>& sources) {
    float height = 0.0f;
    for (const auto& source : sources) {
        float distance = sqrt(pow(source.position.x - position.x, 2) + pow(source.position.y - position.y, 2));
        float phase = (distance / source.wavelength - time * source.velocity / source.wavelength) * 2 * M_PI;
        float decay = (distance != 0) ? 1 / sqrt(distance) : 0;  // Applying the 1/sqrt(r) decay model
        float waveContribution = source.amplitude * sin(phase) * decay;
        height += waveContribution;
    }
    return height;
}*/

float y(const sf::Vector2f& position, int time, const std::vector<Source>& sources) {
    float height = 0.0f;
    for (const auto& source : sources) {
        float distance = sqrt(pow(source.position.x - position.x, 2) + pow(source.position.y - position.y, 2));
        float travelTime = distance / source.velocity; // Time for the wave to reach this point
        if (time - source.timeOfCreation >= travelTime) { // Check if the wave should have reached this point
            float phase = ((distance / source.wavelength) - (time - travelTime) * source.velocity / source.wavelength) * 2 * M_PI;
            float decay = (distance != 0) ? 1 / sqrt(distance) : 0;
            float waveContribution = source.amplitude * sin(phase) * decay;
            height += waveContribution;
        }
    }
    return height;
}

int main() {

	// set antialising
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

    // create main window
    sf::RenderWindow window(sf::VideoMode(FRAME.x, FRAME.y), "Simulador de ondas en R2", sf::Style::Default, settings);

    window.setFramerateLimit(60);

	// load fonts
	
	if (!defaultFont.loadFromFile("/usr/share/fonts/TTF/FiraCode-Regular.ttf")) {
		std::cout << "could not find system fonts : TODO: add local fonts to project" << std::endl;	
	}

	// presets
	
	std::vector<std::vector<sf::RectangleShape>> pixels;
	for (size_t i = 0; i < FRAME.y / pixelWidth; i++) {
		std::vector<sf::RectangleShape> row;
		for (size_t j = 0; j < FRAME.x / pixelWidth; j++) {
			sf::RectangleShape pixel;
			pixel.setSize(sf::Vector2f(pixelWidth, pixelWidth));
			pixel.setPosition(sf::Vector2f(pixelWidth * j, pixelWidth * i));
			pixel.setFillColor(sf::Color(100, 100, 100));
			row.push_back(pixel);
		}
		pixels.push_back(row);
	}

    /*std::vector<Source> sources = {
        {sf::Vector2f(400, 450), 300.0f, 100.0f, 2.0f}, // position, amplitude, wavelength, speed
        {sf::Vector2f(975, 500), 300.0f, 200.0f, 2.0f}
    };*/

	std::vector<Source> sources = {};

	unsigned int time = 0;

	sf::Text timeText;
    timeText.setFont(defaultFont);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10, 10);  // Position at top-left corner

	// predefine event
	sf::Event event;

    // main loop
	while (window.isOpen()) {

		// get new events and parse each one untill there is no more
        while (window.pollEvent(event)) {
            // switch over sf::Event(s) union
            // keypress and mouse first for speed

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			
				Source newSource;
				newSource.position = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
				newSource.amplitude = 200.0;
				newSource.wavelength = 200.0;
				newSource.velocity = 8.0;
				newSource.timeOfCreation = time;

				sources.push_back(newSource);

			} else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {

				time = 0;
				sources = {};

			} else if (event.type == sf::Event::Closed) {
				window.close();
			}
        }
 
        // clear screen
        window.clear(sf::Color(255, 255, 255));

		// process

		for (size_t i = 0; i < pixels.size(); i++) {
            for (size_t j = 0; j < pixels[i].size(); j++) {
                sf::Vector2f pos = pixels[i][j].getPosition();
                float displacement = y(pos, time, sources);
                sf::Color color = sf::Color(100 + displacement, 100 + displacement, 255);
                pixels[i][j].setFillColor(color);
                window.draw(pixels[i][j]);
            }
        }

		time++;

		timeText.setString("time[ms] : " + std::to_string(time) + "\ncreate source [l-click]\nreset [r]");
		window.draw(timeText);

        // update the window
        window.display();
    }

    // cleanup
 
    return 0;
}
