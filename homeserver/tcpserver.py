import socket

# Create a socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to an address
server_address = ('localhost', 8080)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()

    try:
        print('connection from', client_address)

        # Receive and send data
        while True:
            data = connection.recv(16)
            if data:
                print('received {!r}'.format(data))
                connection.sendall(data)
            else:
                break

    finally:
        # Clean up the connection
        connection.close()

