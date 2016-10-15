/// Conexão com módulos, tenha um só por módulo pliz
#pragma once

#include "Exception.hpp"
#include "Connection.hpp"
#include "Arguments.hpp"
#include <iostream>
#include <tuple>
#include <thread>
#include <zmq.hpp>
#include <cstdlib>
#include <cstring>
#include <glib.h>
#include <gmodule.h>

using namespace std;
using namespace zmq;

/// Conexão com o módulo de fato, sendo extendida para inicialização automática e talz
class ModuleConnection {
public:
	/// Ctor, com contexto zmq, endereço de conexão
	ModuleConnection (context_t & ctx, const string & endereco) : conn (ctx, ZMQ_DEALER) {
		conn.skt.connect (endereco);
	}

	virtual ~ModuleConnection () {}

	/// Chamar operação sem argumentos
	void call (const string & func) {
		conn.send (func);
	}
	/// Chamar operação com argumentos, transformando-os usando `getBuffer`
	template<typename ...Args>
	void call (const string & func, const Args & ... args) {
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

	virtual ~ModuleConnectionProcess () {
		g_spawn_close_pid (pid);
	}
private:
	/// PID do processo, usado pra fechá-lo depois
	GPid pid;
};

/// Conexão com módulo que spawna um thread
class ModuleConnectionThread : public ModuleConnection {
	using gediopenFunc = void (*) (context_t *, const char *);
public:
	ModuleConnectionThread (context_t & ctx, const string & endereco
			, const string & nomeLib, const string & simbolo) : ModuleConnection (ctx, endereco) {
		lib = g_module_open (nomeLib.c_str (), G_MODULE_BIND_LAZY);
		// tenta pegar a função lá
		gediopenFunc openFunc;
		if (!g_module_symbol (lib, simbolo.c_str (), (gpointer *) &openFunc)) {
			auto ex = GEDI_API_EXCEPTION ("ModuleConnectionThread", g_module_error ());
			g_module_close (lib);
			throw ex;
		}

		T = thread (openFunc, &ctx, endereco.c_str ());
	}

	virtual ~ModuleConnectionThread () {
		const char * nomeLib = g_module_name (lib);
		cout << "Esperando módulo \"" << nomeLib << "\" terminar" << endl;
		T.join ();
		if (!g_module_close (lib)) {
			g_warning ("%s: %s\n", nomeLib, g_module_error ());
		}
	}
private:
	/// Módulo do glib da lib dinâmica
	GModule * lib;

	/// Thread onde tá rodando
	thread T;
};
