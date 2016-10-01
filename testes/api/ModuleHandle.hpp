/// Teste de um Module, que conversa com o módulo e recebe resposta (se precisar)
#pragma once

#include "ModuleConnection.hpp"
#include "Arguments.hpp"
#include <iostream>

using namespace std;

/// Handler de módulos, pra podermos usar polimorfismo =]
class ModuleHandle {
public:
	ModuleHandle (ModuleConnection * con) : connection (con) {}

	template<class...Args>
	ModuleHandle & operator() (Args... args) {
		connection->call (args...);
		return *this;
	}

	Arguments & resp () {
		connection->fillResponse (respostas);
		return respostas;
	}

private:
	ModuleConnection * connection;
	Arguments respostas;
};
