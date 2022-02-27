# Buffer overflow examples


# buff_1
Basic example



# buff_2

tcpclient [-s server] [-p server port] [-c] [-v] [-h] filename


tcpserver [-p port] [-v] [-h]


==========



./tcpclient -v aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa<file_name_here>

Number of chars calculated according to the pointers (see code).


./tcpserver -v

