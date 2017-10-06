# Redes

## To compile use: make no

## The file enlaces.config contains the link between routers. The format of file is:

"Id router" "Id router" "Weight"

So, to add new configurations just modify the file.

## The file roteador.config contains the router's configuration. The format of file is:

"Id router" "Port" "Ip address"

So, to include new routers, just add this info.

## To make easily for test, a script "test.sh" have been created. In

## To run:
 1 - execute one by one ./no "Id router". Or
 2 - execute sh test.sh


## The format of messages is:
"Id destination" "message"
