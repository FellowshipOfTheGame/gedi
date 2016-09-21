#include "ModuleHandle.hpp"
#include "ModuleManager.hpp"

#include <iostream>
#include <thread>
using namespace std;

/// classe pra testar os template maluco
struct Oi {
	int x;
	Oi (int x) : x (x) {}
};
// Pra podermos mandar Ois pros módulos, basta overloadar a função `getBuffer`,
// lembrando que argumento seja const&
BuffNSize getBuffer (const Oi & o) {
	return make_pair ((void *) &o.x, sizeof (int));
}

int main () {
	ModuleManager mgr;
	auto & h = mgr.addConnection ("grafico", "ipc://teste");
	h ("window", 800, 600, "Minha janela pocoto");
	this_thread::sleep_for (5s);
	h ("quit");

	return 0;
}
