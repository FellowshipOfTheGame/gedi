Projeto de API
==============
Quais funcionalidades podem ser proporcionadas para facilitar criação e uso de
módulos?


ModuleManager
-------------
Uma classe que gerencia a conexão com os módulos, contendo info sobre os
módulos conectados. Uma instância é suficiente por jogo, talvez compense fazer
singleton.

Métodos:
- __construtor__: inicializa podendo ler o arquivo de configuração, e
  conectando com os módulos. Pode usar `argc` e `argv` pra sacar umas coisa,
  incluindo caminho da configuração.
- __Module connect (string nome, string endereço)__: conecta com o módulo
  baseado no endereço `endereço`, salvando-o com o nome especificado. Note que
  esse método só realiza a conexão, então trate de já ter o módulo ligado e
  pronto (ao ler do arquivo de configuração, criação do processo do módulo vai
  ser feita automaticamente pelo __Gedi__, talvez métodos específicos como
  "connectIpc", ou "connectInproc")
- __Module get (string)__: pega uma instância de módulo baseado em seu nome, o
  mesmo usado no arquivo de configuração.
