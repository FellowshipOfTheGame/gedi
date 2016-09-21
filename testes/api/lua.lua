#!/usr/bin/env lua
zmq = require 'lzmq'

local endereco = 'ipc://teste'
--local endereco = 'tcp://localhost:5555'

local ctx = zmq.context ()
local skt = assert (ctx:socket { zmq.ROUTER, bind = endereco })

msgs = skt:recv_all ()
msgs[3] = string.unpack ('i', msgs[3])
msgs[4] = string.unpack ('i', msgs[4])
for i, pedaco in ipairs (msgs) do print (i, pedaco) end

skt:close ()
ctx:destroy ()
