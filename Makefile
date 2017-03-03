all:
	gcc server.c socket/socket.c numbers/numbers.c -o server
	gcc client.c socket/socket.c numbers/numbers.c -o client
	gcc trusted_center.c numbers/numbers.c -o trust
