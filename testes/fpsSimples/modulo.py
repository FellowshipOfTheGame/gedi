#!/usr/bin/env python

import sdl2 as sdl
import sdl2.ext as sdlx
import zmq
import time

class Conexao:
    def __init__ (self, título, tamanho):
        self.título = título
        self.win = sdlx.Window (título, tamanho)
        self.win.show ()
        self.time = time.perf_counter ()
        self.tempoTotal = 0
        self.i = 0

    def update (self):
        último = self.time
        self.time = time.perf_counter ()
        # desenha cinza
        sdlx.fill (self.win.get_surface (), sdlx.string_to_color ('#555'))
        sdl.SDL_UpdateWindowSurface (self.win.window)
        # e soma o tempo total
        self.tempoTotal += self.time - último
        self.i += 1

    def getFps (self):
        return self.i / self.tempoTotal


class ConnectionManager:
    """Um gerenciador de conexões pra esse módulo =P"""
    def __init__ (self):
        self.conexões = {}
        self.ctx = zmq.Context ()
        self.skt = self.ctx.socket (zmq.ROUTER)
        self.skt.bind ('tcp://*:5555')


    def create (self, id, nome = b'MeuModuloTeste', largura = 640, altura = 480):
        # se alguém pedir pra abrir uma janela nova, mata
        if self.conexões.get (id):
            del self.conexões[id]
        self.conexões[id] = Conexao (nome.decode ('utf-8'), (int (largura), int (altura)))
        print (id, 'conectou')

    def loop (self, id):
        # processa eventos
        events = sdlx.get_events ()
        for ev in events:
            if ev.type == sdl.SDL_QUIT:
                self.close (id)
                return
        # atualiza os fps e tela
        self.conexões[id].update ()

    def close (self, id):
        self.skt.send_multipart ([id, b'flws'])
        print (id, 'quitou. Média de FPS:', self.conexões[id].getFps ())
        del self.conexões[id]

def main ():
    cabou = False
    clientes = ConnectionManager ()

    while not cabou:
        msgs = clientes.skt.recv_multipart ()

        if msgs[1] == b'quit':
            cabou = True
        elif msgs[1] == b'create':
            clientes.create (msgs[0], *msgs[2:])
        elif clientes.conexões.get (msgs[0]):
            getattr (clientes, msgs[1].decode ('utf-8')) (msgs[0], *msgs[2:])
            
    sdlx.quit ()

if __name__ == '__main__':
    main ()
