from network.main import NetworkManager

intro = "Alora Python Shell\nuse nm to access NetworkManager class"

def start_server():
    nm = NetworkManager()
    nm.run()
    return nm

def run():
    print(intro)
    nm = start_server()
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