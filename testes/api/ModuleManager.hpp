/// Teste de um ModuleManager, que conecta com os módulos e talz
#pragma once

#include <iostream>
#include <utility>
#include <unordered_map>
#include <zmq.hpp>
#include <yaml-cpp/yaml.h>

using namespace zmq;
using namespace std;

class ModuleManager {
public:
	/// Ctor
	ModuleManager () {}

	/// Dtor
	~ModuleManager () {
		for (auto & con : modulos) {
			delete con.second;
		}
	}

	void readConfig (const string & fname) {
		auto config = YAML::LoadFile (fname);
		for (auto mod : config) {
			auto nome = mod.first.as<string> ();
			cout << "Achei módulo: " << nome << endl;
			auto opts = mod.second;
			auto endereco = opts["endereco"].as<string> ();
			if (opts["spawn"]) {
				spawnConnection (nome, endereco, opts["spawn"].as<string> ());
			}
			else {
				addConnection (nome, endereco);
			}
		}
	}

	/// Adiciona uma conexão com módulo
	ModuleHandle addConnection (const string & nome, const string & conexao) {
		auto it = modulos.emplace (nome, new ModuleConnection (ctx, conexao)).first;
		return move (ModuleHandle (it->second));
	}

	/// Spawna uma conexão com módulo
	ModuleHandle spawnConnection (const string & nome, const string & conexao
			, const string & comando) {
		auto it = modulos.emplace (nome, new ModuleConnectionProcess (ctx, conexao, comando)).first;
		return move (ModuleHandle (it->second));
	}

	/// Pega módulo referente ao nome dado, nullptr se não existir
	ModuleHandle get (const string & nome) {
		auto it = modulos.find (nome);
		if (it == modulos.end ()) {
			throw "Não achei módulo com tal nome =/";
		}
		return move (ModuleHandle (it->second));
	}

private:
	/// Context
	context_t ctx;

	/// Mapa dos módulos
	unordered_map<string, ModuleConnection *> modulos;
};
