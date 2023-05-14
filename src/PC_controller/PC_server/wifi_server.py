import socket
from threading import Thread

def get_ip():
    answer = 0
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        answer = s.getsockname()[0]
        s.close()
    except Exception:
        return socket.gethostbyname(socket.gethostname())
    return answer

class wifi_server_device_lisen(Thread):

    def __init__(self,device):
        Thread.__init__(self)
        self.device = device 
        self.flag = True
        self.line = ""

    def stop(self):
        self.flag = False

    def run(self):
        self.device.settimeout(1.0)
        while self.flag:
            try: self.line += self.device.recv(1024).decode("utf-8")
            except socket.timeout: pass

    def get(self):
        a = self.line
        self.line = ""
        return a

class wifi_server_device:

    def __init__(self,port=1234):
        self.ip = get_ip()
        self.port = port
        self.device = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def start(self):
        self.device.bind((self.ip, self.port))   
        self.device.listen(5) # Now wait for client connection.

        self.wifi_device, addr = self.device.accept()
        print ('Got connection from', addr)

        self.wifi_device_potok = wifi_server_device_lisen(self.wifi_device)
        self.wifi_device_potok.start()

    def write(self,data):
        self.wifi_device.send(str(data+"\n").encode('utf-8'))

    def get(self):
        return self.wifi_device_potok.get()

    def close(self):
        #print("!")
        self.device.close()
        try: self.wifi_device_potok.stop()
        except AttributeError: pass # инициализация не прошла полностью

if __name__=="__main__":
    from time import sleep
    test = wifi_server_device()
    test.start()
    for i in range(10):
        test.write(str(i))
        sleep(0.1)
    print(test.get())
    test.close()
    sleep(10)


