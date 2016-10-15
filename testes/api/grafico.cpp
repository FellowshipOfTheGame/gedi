#include "Module.hpp"
#include "GameObject.hpp"
#include "Exception.hpp"
#include <unordered_map>
#include <utility>
#include <cstdint>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace zmq;

struct Ispraite {
	sf::Drawable *drawable;
	sf::Shader *shader {nullptr};

	Ispraite (sf::Drawable *d) : drawable (d) {}
	~Ispraite () {
		delete drawable;
	}

	void draw (sf::RenderTarget & target) {
		target.draw (*drawable, shader ? shader : sf::RenderStates::Default);
	}
};

using ObjMap = unordered_map<ID, Ispraite *>;
using TextureMap = unordered_map<string, sf::Texture>;
using ShaderMap = unordered_map<string, sf::Shader *>;

extern "C" void abre (context_t * ctx, const char * endereco);

int main () {
	context_t ctx;
	abre (&ctx, "ipc://teste");
	return 0;
}

extern "C" void abre (context_t * ctx, const char * endereco) {
	// módulo do gráfico
	Module M (*ctx, endereco);
	ObjMap objetos;
	TextureMap texturas;
	ShaderMap shaders;

	sf::RenderWindow window;
	//--  Sai da janela  --//
	M.on ("quit", [&] (Arguments & args) {
		window.close ();
		M.close ();
	});
	M.on ("open", [&] (Arguments & args) {
		M.sync ();
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
		auto sprite = new sf::Sprite (tex);
		objetos.emplace (id, new Ispraite (sprite));
	});
	//--  Transformables  --//
	M.on ("setOrigin", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			int x = args.as<int> (1);
			int y = args.as<int> (2);
			obj->setOrigin (x, y);
		}
	});
	M.on ("setPosition", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double x = args.as<double> (1);
			double y = args.as<double> (2);
			obj->setPosition (x, y);
		}
	});
	M.on ("setRotation", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double angulo = args.as<double> (1);
			obj->setRotation (angulo);
		}
	});
	M.on ("setScale", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double sx = args.as<double> (1);
			double sy = args.as<double> (2);
			obj->setScale (sx, sy);
		}
	});
	M.on ("move", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double x = args.as<double> (1);
			double y = args.as<double> (2);
			obj->move (x, y);
		}
	});
	M.on ("rotate", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double angulo = args.as<double> (1);
			obj->rotate (angulo);
		}
	});
	M.on ("scale", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		if (auto * obj = dynamic_cast<sf::Transformable *> (objetos.at (id)->drawable)) {
			double sx = args.as<double> (1);
			double sy = args.as<double> (2);
			obj->scale (sx, sy);
		}
	});
	//--  Shaders  --//
	M.on ("shader", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto arquivo1 = args.as<string> (1);
		auto arquivo2 = args.as<string> (2);
		auto *shader = new sf::Shader;
		if (arquivo2 == "vertex") {
			shader->loadFromFile (arquivo1, sf::Shader::Type::Vertex);
		}
		else if (arquivo2 == "fragment") {
			shader->loadFromFile (arquivo1, sf::Shader::Type::Fragment);
		}
		else if (arquivo2 == "geometry") {
			shader->loadFromFile (arquivo1, sf::Shader::Type::Geometry);
		}
		else if (args.size () > 3) {
			shader->loadFromFile (arquivo1, arquivo2, args.as<string> (3));
		}
		else {
			shader->loadFromFile (arquivo1, arquivo2);
		}
		shaders.emplace (move (nome), shader);
	});
	M.on ("attach", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		auto shader = args.as<string> (1);
		objetos.at (id)->shader = shaders.at (shader);
	});
	M.on ("unattach", [&] (Arguments & args) {
		auto id = args.as<ID> (0);
		objetos.at (id)->shader = nullptr;
	});
	//--  Controle  --//
	M.on ("draw", [&] (Arguments & args) {
		window.clear ();
		for (auto & it : objetos) {
			it.second->draw (window);
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
	for (auto & it : shaders) {
		delete it.second;
	}
}
