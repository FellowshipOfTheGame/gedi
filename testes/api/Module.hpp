/// Teste do módulo, um dos jeitos de implementar a bagaça
#pragma once

#include "Arguments.hpp"
#include "Connection.hpp"
#include <unordered_map>
#include <functional>
#include <iostream>
#include <zmq.hpp>

using namespace std;
using namespace zmq;


using comando = function<void (Arguments &)>;
using mapaComando = unordered_map<string, comando>;

class Module {
public:
	/// Ctor
	Module (context_t & ctx, const string & endereco) : conn (ctx, ZMQ_ROUTER)
			, endereco (endereco), taberto (true) {
		conn.skt.bind (endereco);
	}

	/// Dtor, manda mensagem confirmando que acabou sussa
	~Module () {
		sync ();
	}
	
	/// Fecha módulo
	void close () {
		taberto = false;
	}

	/// Módulo tá aberto?
	bool isOpen () {
		return taberto;
	}

	void on (const string & nome, comando cmd) {
		comandosConhecidos[nome] = cmd;
	}

	void handle () {
		args.recebeComando (conn.skt);
		auto cmd = args.getComando ();
		auto it = comandosConhecidos.find (cmd);
		if (it == comandosConhecidos.end ()) {
			throw runtime_error ("[" + endereco + "] Comando \"" + cmd + "\" desconhecido!");
		}
		it->second (args);
	}

	/// Manda mensagem vazia, pra sincronização
	void sync () {
		conn.send (args.getId (), VAZIO);
	}

	template<typename...Args>
	void respond (Args... arguments) {
		conn.send (args.getId (), arguments...);
	}


private:
	/// O socket pra comunicar, sempre ROUTER
	Connection conn;

	string endereco;

	/// Tá aberto o módulo?
	bool taberto;

	/// Nossos argumentos =]
	Arguments args;
	/// E o mapa de comandos, pra chamar hora que receber um comando
	mapaComando comandosConhecidos;
};
