import socket
import struct
import msvcrt
if __name__ == '__main__':
    running = True
    while(running):
        y = 0
        x = 0
        input_char = msvcrt.getch()
        if input_char.upper() == b'\x1b':
            running = False
        if input_char.upper() == b'D':
            x = 1
        if input_char.upper() == b'A':
            x = -1
        if input_char.upper() == b'W':
            y = -1
        if input_char.upper() == b'S':
            y = 1

        inputs = struct.pack('2i', x, y)
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM,
                          socket.IPPROTO_UDP)
        s.sendto(inputs, ('127.0.0.1', 9999))
