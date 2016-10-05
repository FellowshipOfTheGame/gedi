#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
using namespace std;

constexpr int raio = 100;

int main () {
	// conta os tempo de tudo!
	chrono::time_point<chrono::system_clock> inicio, fim;

	sf::RenderWindow window (sf::VideoMode (800, 600), "Minha janela pocoto");
	sf::Texture tex;
	tex.loadFromFile ("flango.png");

	sf::Sprite objeto (tex);

	chrono::duration<double> timeDiff;
	while (window.isOpen ()) {
		inicio = chrono::system_clock::now ();

		objeto.move (0.1, 0.1);
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

		window.clear ();
		window.draw (objeto);
		window.display ();

		fim = chrono::system_clock::now ();
		timeDiff = fim - inicio;
		cout << timeDiff.count () << endl;
	}
}
