import base64

FLAG = "crypto{dummy_text}"
KEY = 0x42


def encrypt_flag(flag, key):
    encrypted = bytes([ord(c) ^ key for c in flag])
    return base64.b64encode(encrypted).decode()


encrypted_flag = encrypt_flag(FLAG, KEY)
print(f"Encrypted flag: {encrypted_flag[::-1]}")
