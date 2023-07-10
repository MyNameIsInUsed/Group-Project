import socket
import datetime
import sys
import os

# Function to handle client requests
def handle_client(client_socket):
    # Receive data from the client
    data = client_socket.recv(1024).decode()
    if not data:
        return

    # Manipulate the incoming text
    current_time = datetime.datetime.now().strftime("%d %B %Y, %H:%M")
    combined_string = f"Text from client received at {current_time}: {data}"

    # Send the combined string back to the client
    client_socket.sendall(combined_string.encode())

    # Close the client socket
    client_socket.close()

# Server configuration
host = 'localhost'
port = 8484

# Create a TCP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Bind the socket to the host and port
    server_socket.bind((host, port))

    # Listen for incoming connections
    server_socket.listen(1)
    print(f"Server listening on {host}:{port}")

    while True:
        # Accept a client connection
        client_socket, addr = server_socket.accept()
        print(f"Accepted connection from {addr[0]}:{addr[1]}")

        # Fork a child process to handle the client request
        pid = os.fork()
        if pid == 0:
            # Child process
            server_socket.close()  # Close the server socket in the child process
            handle_client(client_socket)
            sys.exit(0)
        else:
            # Parent process
            client_socket.close()  # Close the client socket in the parent process
except KeyboardInterrupt:
    # Close the server socket on keyboard interrupt
    server_socket.close()
    sys.exit(0)
