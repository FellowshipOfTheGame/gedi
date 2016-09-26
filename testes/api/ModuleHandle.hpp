/// Teste de um Module, que conversa com o módulo e recebe resposta (se precisar)
#pragma once

#include "ModuleConnection.hpp"
#include <iostream>

using namespace std;

/// Handler de módulos, pra podermos usar polimorfismo =]
class ModuleHandle {
public:
	ModuleHandle (ModuleConnection * con) : connection (con) {}

	template<class...Args>
	void operator() (Args... args) {
		connection->call (args...);
	}

private:
	ModuleConnection * connection;
};
