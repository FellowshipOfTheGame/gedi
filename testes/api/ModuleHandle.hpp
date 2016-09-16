/// Teste de um Module, que conversa com o módulo e recebe resposta (se precisar)
#pragma once

#include <iostream>
#include <tuple>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

using BuffNSize = pair<void *, size_t>;

BuffNSize getBuffer (const string & str) {
	return make_pair ((void *) str.c_str (), str.size ());
}

BuffNSize getBuffer (const double & x) {
	return make_pair ((void *) &x, sizeof (double));
}

BuffNSize getBuffer (const int & x) {
	return make_pair ((void *) &x, sizeof (int));
}

template<typename T>
BuffNSize getBuffer (const pair<size_t, T *> & P) {
	return make_pair (P.second, P.first * sizeof (T));
}

class ModuleHandle {
public:
	/// Ctor, com contexto zmq, endereço de conexão e 
	ModuleHandle (context_t & ctx, string endereco) : skt (ctx, ZMQ_DEALER) {
		skt.connect (endereco);
	}

	/// Chamar operação sem argumentos
	void operator() (const string & func) {
		skt.send (func.data (), func.size ());
	}
	/// Chamar operação com argumentos, transformando-os usando `getBuffer`
	template<class...Args>
	void operator() (const string & func, Args... args) {
		skt.send (func.data (), func.size (), ZMQ_SNDMORE);
		sendToModule (args...);
	}

private:
	/// O socket pra comunicar, sempre DEALER
	socket_t skt;

	/// Caso base, mandando último argumento, sem mandar ZMQ_SNDMORE, fechando
	// mensagem
	template<class T>
	void sendToModule (const T & arg) {
		size_t size;
		void * buf;

		tie (buf, size) = getBuffer (arg);
		skt.send (buf, size);
	}

	/// Manda argumentos para o módulo, usando `getBuffer` pra mandar argumentos
	// direitim
	template<typename T, typename...Args>
	void sendToModule (const T & head, Args... tail) {
		size_t size;
		void * buf;

		tie (buf, size) = getBuffer (head);
		skt.send (buf, size, ZMQ_SNDMORE);

		sendToModule (tail...);
	}
};
