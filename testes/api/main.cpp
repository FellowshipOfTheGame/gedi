#include "ModuleHandle.hpp"
#include "ModuleManager.hpp"
#include "GameObject.hpp"

#include <iostream>
#include <chrono>
using namespace std;

constexpr int raio = 100;

int main () {
	// conta os tempo dos frames
	chrono::time_point<chrono::system_clock> inicio, fim;

	ModuleManager mgr;
	mgr.readConfig ("gedimods.yml");
	// auto h = mgr.addConnection ("grafico", "ipc://teste");
	auto gfx = mgr.get ("grafico");
	gfx ("window", 800, 600, "Minha janela pocoto");

	// primeiro círculo
	GameObject primeiro;
	gfx ("texture", "flango", "flango.png");
	gfx ("sprite", primeiro, "flango");

	bool fechou = false;
	chrono::duration<double> timeDiff;
	while (!fechou) {
		inicio = chrono::system_clock::now ();
		gfx ("move", primeiro, 0.1, 0.1);
		gfx ("draw");
		fechou = gfx ("didClose").resp ().as<bool> (0);
		// verifica o tempo
		fim = chrono::system_clock::now ();
		timeDiff = fim - inicio;
		cout << timeDiff.count () << endl;
	}
	gfx ("quit");

	return 0;
}
