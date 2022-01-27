from network.main import NetworkManager

print("Alora Python Shell")
nm = NetworkManager()
nm.run()
print("started threads")
print(f"server running at {nm}")


while a:= input("\n>>> "):
    a = a.strip()
    if a == chr(4):
        break
    try:
        print(eval(a))
    except Exception as e:
        print(e)