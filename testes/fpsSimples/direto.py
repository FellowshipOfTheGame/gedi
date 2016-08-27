#!/usr/bin/env python

import sdl2 as sdl
import sdl2.ext as sdlx
import time

win = sdlx.Window ('Direto', (800, 600))
win.show ()
tempo = time.perf_counter ()

done = False
tempoTotal = 0
i = 0

while not done:
    events = sdlx.get_events ()
    for ev in events:
        if ev.type == sdl.SDL_QUIT:
            done = True
            break;
    # desenha =P
    sdlx.fill (win.get_surface (), sdlx.string_to_color ('#555'))
    sdl.SDL_UpdateWindowSurface (win.window)

    último = tempo
    tempo = time.perf_counter ()
    tempoTotal += tempo - último
    i += 1

print ('Média de FPS:', 1 / (tempoTotal / i))
