#include "Module.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>

using namespace std;

using ObjMap = unordered_map<int, sf::Drawable *>;

int main () {
	Module M ("ipc://teste");
	sf::RenderWindow window;
	ObjMap objetos;

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
	M.on ("circle", [&] (Arguments & args) {
		int id = args.asInt (0);
		int raio = args.defaultInt (1, 100);
		auto circulo = new sf::CircleShape (raio);
		objetos.insert (make_pair (id, circulo));
	});
	M.on ("draw", [&] (Arguments & args) {
		for (auto & it : objetos) {
			window.draw (*it.second);
		}
		window.display ();
	});

	while (M.isOpen ()) {
		M.handle ();
	}

	// limpa os rolê
	for (auto & it : objetos) {
		delete it.second;
	}
}
