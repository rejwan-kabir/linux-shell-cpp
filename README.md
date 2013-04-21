linux-shell-cpp
===============

Linux Shell from scratch using C++ and Basic Linux Socket Library.

That was actually a university assignment. We will implement a linux command with basic command. Command list is available on heklp file.

We need to open a server with a port first. Port can be arbritrary for server side, but we must use that port for client side.

./myserver 1000 # 1000 is port number

The server will run on localhost:1000 port. It will be listening untill we press null character, in our shell Ctrl-C.

./myclient localhost 1000

The client shell will start. We can use the supplied command.

=================

There is some reading attached to the project. You should read them If you want to fully understand how Socket programming is done with C++ and how Linux handles it.

For Further information, please contact to rejwan.shuvo@gmail.com. Thanks a lot.