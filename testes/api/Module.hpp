/// Teste do módulo, um dos jeitos de implementar a bagaça
#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <iostream>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

string fromMessage (message_t & msg) {
	return move (string (msg.data<char> (), msg.size ()));
}

class Arguments {
public:
	/// Ctor
	Arguments () {}
	/// Lê mensagens
	void recebeMensagens (socket_t & skt) {
		args.clear ();
		skt.recv (&id);
		skt.recv (&comando);
		while (skt.getsockopt<int> (ZMQ_RCVMORE)) {
			message_t msg;
			skt.recv (&msg);
			args.push_back (move (msg));
		}
	}
	/// Pega o comando
	string getComando () {
		return fromMessage (comando);
	}
	/// Pega o ID de quem mandou, como string mesmo
	string getId () {
		return fromMessage (id);
	}
	/// Pega mensagem idx como uma string
	string asString (int idx) {
		auto & msg = args.at (idx);
		return fromMessage (msg);
	}
	string defaultString (int idx, const string & str) {
		try {
			return asString (idx);
		}
		catch (...) {
			return str;
		}
	}
	/// Pega mensagem idx como um int
	int asInt (int idx) {
		auto & msg = args.at (idx);
		return (int) *msg.data<int> ();
	}
	int defaultInt (int idx, int I) {
		try {
			return asInt (idx);
		}
		catch (...) {
			return I;
		}
	}

	size_t size () {
		return args.size ();
	}
private:
	message_t comando;
	message_t id;
	vector<message_t> args;
};

using comando = function<void (Arguments &)>;
using mapaComando = unordered_map<string, comando>;

class Module {
public:
	/// Ctor
	Module (const string & endereco) : skt (ctx, ZMQ_ROUTER), taberto (true) {
		skt.bind (endereco);
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
		args.recebeMensagens (skt);
		auto it = comandosConhecidos.find (args.getComando ());
		if (it == comandosConhecidos.end ()) {
			throw runtime_error ("Comando desconhecido!");
		}
		it->second (args);
	}

	
private:
	/// Contexto, pro socket funfar
	context_t ctx;
	/// O socket pra comunicar, sempre ROUTER
	socket_t skt;

	/// Tá aberto o módulo?
	bool taberto;

	/// Nossos argumentos =]
	Arguments args;
	/// E o mapa de comandos, pra chamar hora que receber um comando
	mapaComando comandosConhecidos;
};
