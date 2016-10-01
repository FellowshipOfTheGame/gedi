#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

using namespace std;

constexpr int raio = 100;

int main () {
	// conta os tempo de tudo!
	chrono::time_point<chrono::system_clock> inicio, fim;

	sf::RenderWindow window (sf::VideoMode (800, 600), "Minha janela pocoto");
	sf::CircleShape primero (raio);
	primero.setOrigin (raio, raio);

	sf::CircleShape segundo (raio / 2);
	segundo.setFillColor (sf::Color::Yellow);

	chrono::duration<double> timeDiff;
	while (window.isOpen ()) {
		inicio = chrono::system_clock::now ();

		sf::Event ev;
		while (window.pollEvent (ev)) {
			switch (ev.type) {
				case sf::Event::Closed:
					window.close ();
					break;

				default:
					break;
			}
		}

		window.draw (primero);
		window.draw (segundo);
		window.display ();

		fim = chrono::system_clock::now ();
		timeDiff = fim - inicio;
		cout << timeDiff.count () << endl;
	}
}
