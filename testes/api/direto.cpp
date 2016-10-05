#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
using namespace std;

constexpr int largura = 800;
constexpr int altura = 600;

int main () {
	// conta os tempo de tudo!
	chrono::time_point<chrono::system_clock> inicio, fim;

	sf::RenderWindow window (sf::VideoMode (largura, altura), "Minha janela pocoto");
	sf::Texture tex;
	tex.loadFromFile ("flango.png");

	sf::Sprite objeto (tex);
	objeto.move (largura / 2, altura / 2);
	objeto.setOrigin (71, 105);

	chrono::duration<double> timeDiff;
	double ang = 0;
	while (window.isOpen ()) {
		inicio = chrono::system_clock::now ();

		double sc = 1.5 + sin (ang) / 2; ang += 0.001;
		objeto.rotate (0.01);
		objeto.setScale (sc, sc);

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
