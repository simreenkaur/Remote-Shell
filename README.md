# Remote-Shell

Done by: Simreen Kaur and Rachit Sharma

Steps to run:
1. Enter 'make all' to compile all the files
2. Run the server first using the executable server file 
   followed by port number './s #' 
   Eg- ./s 7856
3. Run the client next using the executable client file 
   followed by hostname and port number './c name %' 
   Eg- ./c localhost 7654
4. Type any command and press enter. 
   Eg- ls -a -l
5. Repeat step 4 or type quit and press enter.
6. Repeat steps 3-5 for any more clients.

Resources used:
1. https://www.geeksforgeeks.org/fork-execute-processes-bottom-using-wait/
2. https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
3. https://www.geeksforgeeks.org/design-a-concurrent-server-for-handling-multiple-clients-using-fork/?ref=rp
4. Assignment Handout
5. server.c, client.c from class modules.
