#include "Module.hpp"
#include "GameObject.hpp"
#include <map>
#include <cstdint>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace zmq;

using ObjMap = map<ID, sf::Drawable *>;

int main () {
	context_t ctx;
	// módulo do gráfico
	Module M (ctx, "ipc://teste");
	ObjMap objetos;

	sf::RenderWindow window;
	M.on ("quit", [&] (Arguments & args) {
		window.close ();
		M.close ();
	});
	M.on ("window", [&] (Arguments & args) {
		int width = args.as<int> (0);
		int height = args.as<int> (1);
		string title = args.asDefault<string> (2, "Teste");
		window.create (sf::VideoMode (width, height), title);
		window.display ();
	});
	M.on ("circle", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		int raio = args.asDefault<int> (1, 100);
		auto circulo = new sf::CircleShape (raio);
		objetos.insert (make_pair (id, circulo));
	});
	M.on ("setOrigin", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id))) {
			int x = args.as<int> (1);
			int y = args.as<int> (2);
			obj->setOrigin (x, y);
		}
	});
	M.on ("setFillColor", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Shape *> (objetos.at (id))) {
			string cor = args.as<string> (1);
			if (cor == "amarelo") {
				obj->setFillColor (sf::Color::Yellow);
			}
		}
	});
	M.on ("draw", [&] (Arguments & args) {
		for (auto & it : objetos) {
			window.draw (*it.second);
		}
		window.display ();
	});
	M.on ("didClose", [&] (Arguments & args) {
		static sf::Event ev;
		while (window.pollEvent (ev)) {
			switch (ev.type) {
				case sf::Event::Closed:
					M.respond (true);
					break;

				default:
					break;
			}
		}
		M.respond (false);
	});

	while (M.isOpen ()) {
		M.handle ();
	}

	// limpa os rolê
	for (auto & it : objetos) {
		delete it.second;
	}
}
