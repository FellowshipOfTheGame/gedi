#include "ModuleHandle.hpp"
#include "ModuleManager.hpp"
#include "GameObject.hpp"

#include <iostream>
#include <chrono>
using namespace std;

constexpr double largura = 800;
constexpr double altura = 600;

int main () {
	// conta os tempo dos frames
	chrono::time_point<chrono::system_clock> inicio, fim;

	ModuleManager mgr;
	mgr.readConfig ("gedimods.yml");
	// auto h = mgr.addConnection ("grafico", "ipc://teste");
	auto gfx = mgr.get ("grafico");
	gfx ("window", (int) largura, (int) altura, "Minha janela pocoto");

	// primeiro círculo
	GameObject primeiro;
	gfx ("texture", "flango", "flango.png");
	gfx ("sprite", primeiro, "flango");
	gfx ("move", primeiro, largura / 2, altura / 2);
	gfx ("setOrigin", primeiro, 71, 105);

	bool fechou = false;
	chrono::duration<double> timeDiff;
	double ang = 0;
	while (!fechou) {
		inicio = chrono::system_clock::now ();
		double sc = 1.5 + sin (ang) / 2; ang += 0.001;
		gfx ("setScale", primeiro, sc, sc);
		gfx ("rotate", primeiro, 0.01);
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
