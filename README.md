# Redes

##To compile use: make no

##The file enlaces.config contains the link between routers. The format of file is:

"Id router" "Id router" "Weight"

So, to add new configurations just modify the archive.

##The file roteador.config contains the router's configuration. The format of file is:

"Id router" "Port" "Ip address"

So, to include new routers, just modify this info.

##To make easily for test, a archive "test.sh" have been created. In

##To run:
 1 - execute one by one ./no "Id router", or
 2 - execute sh test.sh
