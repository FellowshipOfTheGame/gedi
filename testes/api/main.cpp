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

	auto audio = mgr.get ("audio");
	// audio ("music", "fundo", "musica.ogg");
	// audio ("mplay", "fundo");
	audio ("buffer", "shot", "shotgun.ogg");
	audio ("sound", "shot", "shot");
	audio ("splay", "shot");

	auto gfx = mgr.get ("grafico");
	gfx ("window", (int) largura, (int) altura, "Minha janela pocoto");

	// primeiro círculo
	GameObject primeiro;
	gfx ("texture", "flango", "flango.png");
	gfx ("sprite", primeiro, "flango");
	gfx ("shader", "meuShader", "f.glsl", "fragment");
	gfx ("attach", primeiro, "meuShader");
	gfx ("move", primeiro, largura / 2, altura / 2);
	gfx ("setOrigin", primeiro, 71, 105);

	bool fechou = false;
	chrono::duration<double> timeDiff;
	double ang = 0, delta = 0;
	while (!fechou) {
		inicio = chrono::system_clock::now ();
		double sc = 1.5 + sin (ang) / 2; ang += 0.001;
		gfx ("setScale", primeiro, sc, sc);
		gfx ("rotate", primeiro, 10 * delta);
		gfx ("draw");
		fechou = gfx ("didClose").resp ().as<bool> (0);
		// verifica o tempo
		fim = chrono::system_clock::now ();
		timeDiff = fim - inicio;
		delta = timeDiff.count ();
		cout << delta << endl;
	}

	return 0;
}
