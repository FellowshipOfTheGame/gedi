/// Teste de um Module, que conversa com o módulo e recebe resposta (se precisar)
#pragma once

#include "ModuleConnection.hpp"
#include "Arguments.hpp"
#include <iostream>

using namespace std;

/// Handler de módulos, pra podermos usar polimorfismo =]
class ModuleHandle {
public:
	ModuleHandle () : ModuleHandle (nullptr) {}
	ModuleHandle (ModuleConnection * con) : connection (con) {}

	/// Setter da conexão, pra poder iterar sobre conexões reutilizando o handle
	void set (ModuleConnection * con) {
		connection = con;
	}

	template<typename ...Args>
	ModuleHandle & operator() (const Args & ... args) {
		connection->call (args...);
		return *this;
	}

	void sync () {
		connection->fillResponse (respostas);
	}

	Arguments & resp () {
		connection->fillResponse (respostas);
		return respostas;
	}

private:
	ModuleConnection * connection;
	Arguments respostas;
};
