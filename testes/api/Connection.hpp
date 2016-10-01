#pragma once

#include <tuple>
#include <zmq.hpp>

using namespace zmq;

using BuffNSize = pair<void *, size_t>;

BuffNSize getBuffer (const bool & b) {
	return make_pair ((void *) &b, 1);
}

BuffNSize getBuffer (const char * const & str) {
	return make_pair ((void *) str, strlen (str));
}

BuffNSize getBuffer (const string & str) {
	return make_pair ((void *) str.data (), str.size ());
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


/// Wrapper pros sockets zmq, pra mandar/receber coisas mais fácil
class Connection {
public:
	/// Ctor, com contexto e tipo do socket ZMQ
	Connection (context_t & ctx, int type) : skt (ctx, type) {}

	/// Caso base, mandando último argumento, sem mandar ZMQ_SNDMORE, fechando
	// mensagem
	template<class T>
	void send (const T & arg) {
		size_t size;
		void * buf;

		tie (buf, size) = getBuffer (arg);
		skt.send (buf, size);
	}

	/// Manda argumentos para o módulo, usando `getBuffer` pra mandar argumentos
	// direitim
	template<typename T, typename...Args>
	void send (const T & head, Args... tail) {
		size_t size;
		void * buf;

		tie (buf, size) = getBuffer (head);
		skt.send (buf, size, ZMQ_SNDMORE);

		send (tail...);
	}

	socket_t skt;
};
