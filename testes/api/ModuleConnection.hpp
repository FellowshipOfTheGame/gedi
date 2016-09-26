/// Conexão com módulos, tenha um só por módulo pliz
#pragma once

#include "Exception.hpp"
#include <iostream>
#include <tuple>
#include <zmq.hpp>
#include <cstdlib>
#include <cstring>
#include <glib.h>

using namespace std;
using namespace zmq;

using BuffNSize = pair<void *, size_t>;

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

/// Conexão com o módulo de fato, sendo extendida para inicialização automática e talz
class ModuleConnection {
public:
	/// Ctor, com contexto zmq, endereço de conexão
	ModuleConnection (context_t & ctx, const string & endereco) : skt (ctx, ZMQ_DEALER) {
		skt.connect (endereco);
	}

	/// Chamar operação sem argumentos
	void call (const string & func) {
		sendToModule (func);
	}
	/// Chamar operação com argumentos, transformando-os usando `getBuffer`
	template<class...Args>
	void call (const string & func, Args... args) {
		sendToModule (func, args...);
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


/// Conexão com módulo que spawna o processo
class ModuleConnectionProcess : public ModuleConnection {
public:
	ModuleConnectionProcess (context_t & ctx, const string & endereco
			, const string & comando) : ModuleConnection (ctx, endereco) {
		GError * err = NULL;
		gchar * cmd = strdup (comando.data ());
		gchar * argv[] = { cmd, NULL };
		if (!g_spawn_async (NULL, argv, NULL, G_SPAWN_DEFAULT
				, NULL, NULL, &pid, &err)) {
			throw GEDI_API_EXCEPTION ("ModuleConnectionProcess", err->message);
		}
		free (cmd);
	}

	~ModuleConnectionProcess () {
		g_spawn_close_pid (pid);
	}
private:
	/// PID do processo, usado pra fechá-lo depois
	GPid pid;
};
