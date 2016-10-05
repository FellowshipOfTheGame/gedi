#include "Module.hpp"
#include "GameObject.hpp"
#include "Exception.hpp"
#include <unordered_map>
#include <cstdint>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace zmq;

using ObjMap = unordered_map<ID, sf::Drawable *>;
using TextureMap = unordered_map<string, sf::Texture>;

int main () {
	context_t ctx;
	// módulo do gráfico
	Module M (ctx, "ipc://teste");
	ObjMap objetos;
	TextureMap texturas;

	sf::RenderWindow window;
	//--  Sai da janela  --//
	M.on ("quit", [&] (Arguments & args) {
		window.close ();
		M.close ();
	});
	//--  Criação/instanciação  --//
	M.on ("window", [&] (Arguments & args) {
		int width = args.as<int> (0);
		int height = args.as<int> (1);
		string title = args.asDefault<string> (2, "Teste");
		window.create (sf::VideoMode (width, height), title);
		window.display ();
	});
	M.on ("texture", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto arquivo = args.as<string> (1);
		sf::Texture tex;
		if (!tex.loadFromFile (arquivo)){
			throw runtime_error ("[grafico::texture] Erro ao carregar textura \""
					+ arquivo + "\"");
		}
		texturas.insert (make_pair (move (nome), move (tex)));
	});
	//--  Criação de objetos, com ID  --//
	M.on ("sprite", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		auto nomeTextura = args.as<string> (1);
		const auto & tex = texturas.at (nomeTextura);
		objetos.insert (make_pair (id, new sf::Sprite (tex)));
	});
	//--  Transformables  --//
	M.on ("setOrigin", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id))) {
			int x = args.as<int> (1);
			int y = args.as<int> (2);
			obj->setOrigin (x, y);
		}
	});
	M.on ("setPosition", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id))) {
			double x = args.as<double> (1);
			double y = args.as<double> (2);
			obj->setPosition (x, y);
		}
	});
	M.on ("move", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id))) {
			double x = args.as<double> (1);
			double y = args.as<double> (2);
			obj->move (x, y);
		}
	});
	//--  Shapes  --//
	M.on ("setFillColor", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Shape *> (objetos.at (id))) {
			string cor = args.as<string> (1);
			if (cor == "amarelo") {
				obj->setFillColor (sf::Color::Yellow);
			}
		}
	});
	//--  Controle  --//
	M.on ("draw", [&] (Arguments & args) {
		window.clear ();
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
