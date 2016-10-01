/// Conexão com módulos, tenha um só por módulo pliz
#pragma once

#include "Exception.hpp"
#include "Connection.hpp"
#include "Arguments.hpp"
#include <iostream>
#include <tuple>
#include <zmq.hpp>
#include <cstdlib>
#include <cstring>
#include <glib.h>

using namespace std;
using namespace zmq;

/// Conexão com o módulo de fato, sendo extendida para inicialização automática e talz
class ModuleConnection {
public:
	/// Ctor, com contexto zmq, endereço de conexão
	ModuleConnection (context_t & ctx, const string & endereco) : conn (ctx, ZMQ_DEALER) {
		conn.skt.connect (endereco);
	}

	/// Chamar operação sem argumentos
	void call (const string & func) {
		conn.send (func);
	}
	/// Chamar operação com argumentos, transformando-os usando `getBuffer`
	template<class...Args>
	void call (const string & func, Args... args) {
		conn.send (func, args...);
	}

	void fillResponse (Arguments & args) {
		args.recebeMensagens (conn.skt);
	}

private:
	/// O socket pra comunicar, sempre DEALER
	Connection conn;
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
