# Network routing simulation
## Stop and wait protocol idea, and no queue, then, less/not-aplicable in the real routing. But for study purposes it's nice.

## To compile, use: make no

## The file enlaces.config contains the link between routers. The format of file is:

"Id router" "Id router" "Weight"

So, to add new configurations just modify the file.

## The file roteador.config contains the router's configuration. The format of file is:

"Id router" "Port" "Ip address"

So, to include new routers, just add this info.

## To make a test easily, a script "test.sh" have been created. To include more routers just copy a line an change the id.

## To run:
 1 - run one by one: ./no "Id router". Or
 2 - run: sh test.sh


## To send messages between routers, just write in the format:
"Id destination" "message"
