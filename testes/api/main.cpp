#include "ModuleHandle.hpp"
#include "ModuleManager.hpp"

#include <iostream>
#include <chrono>
using namespace std;

constexpr int raio = 100;

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
	// conta os tempo de tudo!
	chrono::time_point<chrono::system_clock> inicio, fim;

	ModuleManager mgr;
	mgr.readConfig ("gedimods.yml");
	// auto h = mgr.addConnection ("grafico", "ipc://teste");
	auto gfx = mgr.get ("grafico");
	gfx ("window", 800, 600, "Minha janela pocoto");
	gfx ("circle", 0, raio);
	gfx ("setOrigin", 0, raio, raio);
	gfx ("circle", 1, raio / 2);
	gfx ("setFillColor", 1, "amarelo");

	bool fechou = false;
	chrono::duration<double> timeDiff;
	while (!fechou) {
		inicio = chrono::system_clock::now ();
		gfx ("draw");
		fechou = gfx ("didClose").resp ().asBool (0);
		// verifica o tempo
		fim = chrono::system_clock::now ();
		timeDiff = fim - inicio;
		cout << timeDiff.count () << endl;
	}
	gfx ("quit");

	return 0;
}
