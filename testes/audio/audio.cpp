#include "Module.hpp"
#include "Exception.hpp"
#include <unordered_map>
#include <SFML/Audio.hpp>

using namespace std;
using namespace zmq;

using MusicMap = unordered_map<string, sf::Music *>;
using SoundMap = unordered_map<string, sf::Sound>;
using BufferMap = unordered_map<string, sf::SoundBuffer>;

extern "C" void abre (context_t * ctx, const char * endereco);

int main () {
	context_t ctx;
	abre (&ctx, "ipc://gedi_audio");
	return 0;
}

extern "C" void abre (context_t * ctx, const char * endereco) {
	// módulo do audio
	Module M (*ctx, endereco);
	MusicMap musicas;
	SoundMap sons;
	BufferMap buffers;

	M.on ("open", [&] (Arguments & args) {
		M.sync ();
	});
	M.on ("quit", [&] (Arguments & args) {
		M.close ();
	});
	//--  Música  --//
	M.on ("music", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto arquivo = args.as<string> (1);

		auto novaMusica = new sf::Music;
		novaMusica->openFromFile (arquivo);
		musicas.emplace (move (nome), novaMusica);
	});
	M.on ("mplay", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		musicas.at (nome)->play ();
	});
	M.on ("mstop", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		musicas.at (nome)->stop ();
	});
	M.on ("mpause", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		musicas.at (nome)->pause ();
	});
	M.on ("setLoop", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto loop = args.as<bool> (1);
		musicas.at (nome)->setLoop (loop);
	});
	//--  Buffer pros sonzinhos  --//
	M.on ("buffer", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto arquivo = args.as<string> (1);

		sf::SoundBuffer novoBuffer;
		novoBuffer.loadFromFile (arquivo);
		buffers.emplace (move (nome), novoBuffer);
	});
	//--  Sonzinhos  --//
	M.on ("sound", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		auto buffer = args.as<string> (1);

		sf::Sound novoSom;
		novoSom.setBuffer (buffers.at (buffer));
		sons.emplace (move (nome), novoSom);
	});
	M.on ("splay", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		sons.at (nome).play ();
	});
	M.on ("sstop", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		sons.at (nome).stop ();
	});
	M.on ("spause", [&] (Arguments & args) {
		auto nome = args.as<string> (0);
		sons.at (nome).pause ();
	});

	while (M.isOpen ()) {
		M.handle ();
	}
	// apaga os rolê
	for (auto & it : musicas) {
		delete it.second;
	}
}
