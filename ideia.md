Game Engine DIstribuído - GEDI
==============================

O quê?
------
Um motor de jogos que usa técnicas de computação distribuída para conversar o
núcleo com os módulos. Módulos podem usar computação distribuída dentro de si
também, mas não é necessário.


Por quê?
--------
Flexibilidade.

A comunicação entre o núcleo e os módulos feita via _sockets_ abstrai o modo
como as coisas são implementadas, forçando somente especificação de protocolo
de chamada. Desse modo, módulos (ou mesmo o núcleo) podem ser implementados em
qualquer linguagem de programação, podem diferir entre plataformas (SO,
processador), podem usar quaisquer tecnologias que convier, podem residir em
diferentes máquinas, etc...

Além disso, módulos podem ser facilmente ligados ou desligados ao núcleo,
diminuindo as dependências de cada jogo e facilitando a criação,
compartilhamento e testes de diferentes módulos, ou mesmo diferentes versões de
um mesmo módulo. Um sistema gerenciador de pacotes para módulos pode/será
feito, para facilitar a instalação de módulos e não precisar de várias cópias
do mesmo módulo no mesmo ambiente.


Pesquisa
--------
A flexibilidade proporcionada pela arquitetura distribuída possibilita uma
facilidade na pesquisa de algoritmos novos ou implementações diferentes de tais
algoritmos.


Performance
-----------
Chamadas de procedimento remoto (RPC) são mais demoradas e imprevisíveis. Pra
que o __Gedi__ funcione, algum paranauê vai ter que rolar pra não ficar muito
lento, talvez assincronia e talz.
