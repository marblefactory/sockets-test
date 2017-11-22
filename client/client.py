import socket


class Connect(object):
    sock: socket

    def connect(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print('Connecting to server')
        sock.connect(('127.0.0.1', 1024))
        return sock

    def send(self, message: str):
        sock = self.connect()

        print('sending: ' + message)
        sock.sendall(message.encode())

        sock.close()


connect = Connect()

connect.send("Hello World")
connect.send("Another message")
