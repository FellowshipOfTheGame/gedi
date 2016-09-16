/// Teste de um ModuleManager, que conecta com os módulos e talz
#pragma once

#include <iostream>
#include <utility>
#include <unordered_map>
#include <zmq.hpp>

using namespace zmq;
using namespace std;

class ModuleManager {
public:
	/// Ctor
	ModuleManager () {}

	/// Dtor
	~ModuleManager () {}

	/// Adiciona uma conexão com módulo
	ModuleHandle & addConnection (const string & nome, const string & conexao) {
		auto it = modulos.emplace (piecewise_construct
				, forward_as_tuple (nome)
				, forward_as_tuple (ctx, conexao)).first;
		return it->second;
	}

	/// Pega módulo referente ao nome dado, nullptr se não existir
	ModuleHandle & get (const string & nome) {
		auto it = modulos.find (nome);
		if (it == modulos.end ()) {
			throw "Não achei módulo com tal nome =/";
		}
		return it->second;
	}

private:
	/// Context
	context_t ctx;

	/// Mapa dos módulos
	unordered_map<string, ModuleHandle> modulos;
};
