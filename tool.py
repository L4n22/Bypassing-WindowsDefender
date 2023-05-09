import base64

with open('payload.txt', 'r') as f:
    payload = f.read().split(",")

xor_value = 0x45
encode = [int(element, 16) ^ xor_value for element in payload]
print()
print("Calculation of the value to subtract to decipher:")
print()
sum_encode = sum(encode)
length_encode = len(encode)
print("xor_value = ", xor_value)
print("sum = ", sum_encode)
print("total = ", length_encode)
subtract = int(sum_encode/length_encode) - xor_value
print("subtract = ", subtract)
print("sum / total - substract = xor_value")
print("\n")

byte_string = bytes(encode)
base64_bytes = base64.b64encode(byte_string)


formatted_payload = ''
for i in range(0, len(base64_bytes), 60):
    formatted_payload += '"' + \
        base64_bytes[i:i+60].decode('utf-8') + '" \\\n\t'

codeb64 = 'const std::string codeb64 = {}";'.format(formatted_payload[:-5])

with open('payload.b64', 'w') as f:
    f.write(codeb64)
