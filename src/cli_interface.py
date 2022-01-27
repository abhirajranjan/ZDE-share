from network.main import NetworkManager
import typing

intro = "Alora Python Shell\nuse nm to access NetworkManager class"

def start_server(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm = NetworkManager(ip=ip, debug=debug)
    nm.run()
    return nm

def run(ip: typing.Union[str('ipv4'), str('ipv6')] = 'ipv4', debug: bool = False):
    nm = start_server(ip=ip, debug=debug)
    print(intro)
    execute(nm)

def execute(nm: NetworkManager):
    while a:= input("\n>>> "):
        a = a.strip()
        if a == chr(4):
            break
        try:
            print(eval(a))
        except Exception as e:
            print(f"[Exception]: {e}")

if __name__ == "__main__":
    run()