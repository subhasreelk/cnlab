COMPUTER NETWORKS LAB - C SOCKET PROGRAMS
==========================================

CONTENTS
--------
01_TCP_Echo/
    echo_server.c
    echo_client.c

02_TCP_Chat/
    chat_server.c
    chat_client.c

03_DNS_UDP/
    dns_server.c
    dns_client.c

04_Stop_and_Wait/
    sw_server.c
    sw_client.c

05_Go_Back_N/
    gbn_server.c
    gbn_client.c

06_Selective_Repeat/
    sr_server.c
    sr_client.c

REQUIREMENTS
------------
Linux / Ubuntu / WSL / college Linux lab with GCC.
These programs use standard Linux socket headers.

HOW TO COMPILE
--------------
Open Terminal and enter the required folder using cd.

Example for TCP Echo:
    cd 01_TCP_Echo
    gcc echo_server.c -o server
    gcc echo_client.c -o client

HOW TO RUN CLIENT-SERVER PROGRAMS
---------------------------------
Use TWO terminals.

Terminal 1 (server):
    ./server

Terminal 2 (client):
    ./client

Always start the server before the client.
For the same program, keep server and client in the same folder.
Use a new terminal for the client.

COMPILE COMMANDS FOR EACH PROGRAM
---------------------------------
TCP Echo:
    gcc echo_server.c -o server
    gcc echo_client.c -o client

TCP Chat:
    gcc chat_server.c -o server
    gcc chat_client.c -o client

DNS using UDP:
    gcc dns_server.c -o server
    gcc dns_client.c -o client

Stop-and-Wait:
    gcc sw_server.c -o server
    gcc sw_client.c -o client

Go-Back-N:
    gcc gbn_server.c -o server
    gcc gbn_client.c -o client

Selective Repeat:
    gcc sr_server.c -o server
    gcc sr_client.c -o client

PORT NUMBERS
------------
TCP Echo          8080
TCP Chat          8081
DNS UDP           8082
Stop-and-Wait     8083
Go-Back-N         8084
Selective Repeat  8085

IMPORTANT NOTES
---------------
1. These are educational lab demonstrations, not production networking code.
2. Stop-and-Wait / GBN / Selective Repeat simulate loss at the application level.
   TCP itself provides reliable delivery, so these programs simulate frame/ACK
   loss rather than causing actual TCP packet loss.
3. For GBN and Selective Repeat, the code demonstrates the key retransmission
   difference. It is a simplified sequential lab implementation, not a complete
   pipelined sliding-window transport implementation.
4. If "Address already in use" appears, wait a few seconds and run again.
5. If "Connection refused" appears, start the server first.
6. On a Windows lab PC, use WSL/Ubuntu or the lab's Linux environment. These
   source files are written for Linux sockets.
