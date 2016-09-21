#include "Module.hpp"
#include <SFML/Graphics.hpp>

int main () {
	Module M ("ipc://teste");
	sf::RenderWindow window;

	M.on ("quit", [&] (Arguments & args) {
		window.close ();
		M.close ();
	});
	M.on ("window", [&] (Arguments & args) {
		int width = args.asInt (0);
		int height = args.asInt (1);
		string title = args.defaultString (2, "Teste");
		window.create (sf::VideoMode (width, height), title);
		window.display ();
	});

	while (M.isOpen ()) {
		M.handle ();
	}
}
