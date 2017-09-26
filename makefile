list:
	gcc test_list.c list.c
priorityq:
	gcc test_priorityq.c priorityq.c
no:
	gcc -Wall no.c dijkstra.c list.c priorityq.c utility.c -lpthread
clean:
	rm *.out
