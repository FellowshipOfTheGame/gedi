/// Argumentos, pra pegar no Module
#pragma once

#include <string>
#include <vector>
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
	void recebeComando (socket_t & skt) {
		args.clear ();
		skt.recv (&id);
		skt.recv (&comando);
		while (skt.getsockopt<int> (ZMQ_RCVMORE)) {
			message_t msg;
			skt.recv (&msg);
			args.push_back (move (msg));
		}
	}
	void recebeMensagens (socket_t & skt) {
		args.clear ();
		do {
			message_t msg;
			skt.recv (&msg);
			args.push_back (move (msg));
		} while (skt.getsockopt<int> (ZMQ_RCVMORE));
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
	/// Pega mensagem idx como um bool
	bool asBool (int idx) {
		auto & msg = args.at (idx);
		return (bool) *msg.data<bool> ();
	}

	size_t size () {
		return args.size ();
	}
private:
	message_t comando;
	message_t id;
	vector<message_t> args;
};

