/// Argumentos, pra pegar no Module
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

template<typename T>
T fromMessage (message_t & msg);

template<>
string fromMessage (message_t & msg) {
	return move (string (msg.data<char> (), msg.size ()));
}
template<>
double fromMessage (message_t & msg) {
	return (double) *msg.data<double> ();
}
template<>
int fromMessage (message_t & msg) {
	return (int) *msg.data<int> ();
}
template<>
uintptr_t fromMessage (message_t & msg) {
	return (uintptr_t) *msg.data<uintptr_t> ();
}
template<>
bool fromMessage (message_t & msg) {
	return (bool) *msg.data<bool> ();
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
		return fromMessage<string> (comando);
	}
	/// Pega o ID de quem mandou, como string mesmo
	string getId () {
		return fromMessage<string> (id);
	}
	/// Pega mensagem idx como um T
	template<typename T>
	T as (int idx) {
		auto & msg = args.at (idx);
		return fromMessage<T> (msg);
	}
	/// Pega a mensagem idx como um T. Se não tiver, torna o padrão
	template<typename T>
	T asDefault (int idx, const T & dflt) {
		try {
			return as<T> (idx);
		}
		catch (...) {
			return dflt;
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

