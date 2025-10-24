#include <windows.h>
#include <psapi.h>
#include <stdio.h>

int main() {
    printf("Waiting for left mouse click...\n");
    while (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        Sleep(10);
    }

    POINT pt;
    GetCursorPos(&pt);
    HWND hwnd = WindowFromPoint(pt);

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess) {
        WCHAR windowTitle[256];
        GetWindowTextW(hwnd, windowTitle, 256);

        if (TerminateProcess(hProcess, 0)) {
            wprintf(L"Killed process %lu owning window: %s\n", pid, windowTitle);
        } else {
            wprintf(L"Failed to kill process %lu\n", pid);
        }

        CloseHandle(hProcess);
    } else {
        printf("Unable to open process with PID %lu\n", pid);
    }

    return 0;
}