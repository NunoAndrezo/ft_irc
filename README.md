# ft_irc

Lisbon, 9th of February 2026 

Hello dear user,
This small readme functions as way for you to discover us, our project, being able to use it, and check the ressources we used to achieve it.

## Who are we?

We are Nuno, Fabio, and Tomaz. 3 students currently studying at school 42 Lisbon.

## The project.

ft_IRC is our own rudimentary Internet Relay Chat server! yada yada...

## How-to

To start the using our small server youll need a few things. A UNIX system (debian, ubuntu, etc...), a C++ compiler, CMAKE, a IRC client (specifically irssi in this case), and a bit of patience (just kidding, it shouldn't take you more than 15min untill you can start sending your own messages)!

Dependencies to install

	sudo apt install g++
	sudo apt install make
	sudo apt install irssi

To use

After cloning our project, do make. Then execute the program with the following template :

PORT is the port you want to use.
PASSWORD is the password you will want for your clients to connect to the server with.
--debug is optional, it serves mostly for a greater understanding of the messages going to and from the server.

	./ircserver PORT PASSWORD [--debug]

Then, either launch irssi with the following command

PORT same as the one you chose for the server
YOUR_NICKNAME any nickname you choose, can only be used once per server
PASSWORD same as the one you chose for the server

	irssi -p PORT -n YOUR_NICKNAME -w PASSWORD

or once your in irssi

If running on the same machine (server and client) you can use localhost (127.0.0.1) or (0.0.0.0). Otherwise 
PORT same as the one you chose for the server
YOUR_NICKNAME any nickname you choose, can only be used once per server
PASSWORD same as the one you chose for the server

	/CONNECT IP PORT PASSWORD YOUR_NICKNAME

<!-- KEEP EXPLAING ABOUT THE COMMANDS YOU CAN USE TO CREATE CHANNELS, CHANGE PERMISSIONS, SEND MSGS, ETC... -->

## Sources

Man Pages:<br>
	https://linux.die.net/man/3/socket <br>
	https://linux.die.net/man/3/close <br>
	https://man7.org/linux/man-pages/man3/setsockopt.3p.html<br>
	https://man7.org/linux/man-pages/man3/setsockopt.3p.html<br>
	https://linux.die.net/man/3/getprotobyname<br>
	https://linux.die.net/man/3/gethostbyname<br>
	https://man7.org/linux/man-pages/man3/getaddrinfo.3.html<br>
	https://linux.die.net/man/3/freeaddrinfo<br>
	https://linux.die.net/man/3/bind<br>
	https://linux.die.net/man/3/connect<br>
	https://linux.die.net/man/3/listen<br>
	https://linux.die.net/man/3/accept<br>
	https://linux.die.net/man/3/htons<br>
	https://linux.die.net/man/3/inet_addr<br>
	https://linux.die.net/man/3/send<br>

IRC replies (all the codes our server sends back to the IRC client):<br>
	https://www.alien.net.au/irc/irc2numerics.html

The Client:<br>
	https://irssi.org/

Full IRC protocol documentation<br>
	https://modern.ircdocs.horse/
