/// Teste de um ModuleManager, que conecta com os módulos e talz
#pragma once

#include "ModuleConnection.hpp"
#include "ModuleHandle.hpp"
#include <iostream>
#include <utility>
#include <cstring>
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
		ModuleHandle h;
		for (auto & it : modulos) {
			h.set (it.second);
			cout << "Mandando quit pro \"" << it.first << '"' << endl;
			h ("quit").resp ();
			
			delete it.second;
		}
	}

	void readConfig (const string & fname) {
		auto config = YAML::LoadFile (fname);
		for (auto mod : config) {
			auto nome = mod.first.as<string> ();
			cout << "Achei módulo: " << nome << endl;
			auto opts = mod.second;
			auto endereco = opts["endereco"].as<string> ();
			// Tenta ler arquivo de config específico do módulo, que é melhor né
			try {
				ostringstream os;
				if (const char *prefix = getenv ("GEDI_MODULE_PATH")) {
					os << prefix << '/';
				}
				os << nome << '/' << nome << ".yml";
				opts = YAML::LoadFile (os.str ());
				cout << "\tUsando config específica do módulo" << endl;
			} catch (...) {}
			// e conecta
			ModuleHandle h;
			if (strncmp (endereco.data (), "inproc", 6) == 0 && opts["thread"]) {
				cout << "\tCriando novo thread pro módulo" << endl;
				auto no = opts["thread"];
				h = threadedConnection (nome, endereco, no["lib"].as<string> ()
						, no["open"].as<string> ());
			}
			else if (strncmp (endereco.data (), "ipc", 3) == 0 || opts["spawn"]) {
				cout << "\tRodando comando do módulo" << endl;
				h = spawnConnection (nome, endereco, opts["spawn"].as<string> ());
			}
			else {
				cout << "\tAguardando conexão" << endl;
				h = addConnection (nome, endereco);
			}
			// sincroniza com o novo módulo
			h ("open").sync ();
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

	ModuleHandle threadedConnection (const string & nome, const string & conexao
			, const string & nomeLib, const string & simbolo) {
		auto it = modulos.emplace (nome, new ModuleConnectionThread (ctx, conexao, nomeLib, simbolo)).first;
		return move (ModuleHandle (it->second));
	}

	/// Pega módulo referente ao nome dado, nullptr se não existir
	ModuleHandle get (const string & nome) {
		auto it = modulos.find (nome);
		if (it == modulos.end ()) {
			throw GEDI_API_EXCEPTION ("ModuleManager", "Não achei módulo de nome \""
					+ nome + "\" =/");
		}
		return move (ModuleHandle (it->second));
	}

private:
	/// Context
	context_t ctx;

	/// Mapa dos módulos
	unordered_map<string, ModuleConnection *> modulos;
};
