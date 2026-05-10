#include <windows.h>
#include <stdio.h>

int main()
{
    PROCESS_INFORMATION pi_hijo;
    STARTUPINFO si_hijo;

    ZeroMemory(&si_hijo, sizeof(si_hijo));
    si_hijo.cb = sizeof(si_hijo);

    ZeroMemory(&pi_hijo, sizeof(pi_hijo));

    printf("Proceso padre ID: %lu\n", GetCurrentProcessId());

    // Creacion del primer hijo
    if(CreateProcess(
        NULL,
        "cmd /c echo Proceso hijo ID: %PROCESSID%",
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si_hijo,
        &pi_hijo))
    {
        printf("Hijo creado con ID: %lu\n", pi_hijo.dwProcessId);

        for(int i = 0; i < 5; i++)
        {
            PROCESS_INFORMATION pi2;
            STARTUPINFO si2;

            ZeroMemory(&si2, sizeof(si2));
            si2.cb = sizeof(si2);

            ZeroMemory(&pi2, sizeof(pi2));

            if(CreateProcess(
                NULL,
                "notepad.exe",
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si2,
                &pi2))
            {
                printf("Proceso nivel 2 ID: %lu\n", pi2.dwProcessId);

                for(int j = 0; j < 3; j++)
                {
                    PROCESS_INFORMATION pi3;
                    STARTUPINFO si3;

                    ZeroMemory(&si3, sizeof(si3));
                    si3.cb = sizeof(si3);

                    ZeroMemory(&pi3, sizeof(pi3));

                    if(CreateProcess(
                        NULL,
                        "calc.exe",
                        NULL,
                        NULL,
                        FALSE,
                        0,
                        NULL,
                        NULL,
                        &si3,
                        &pi3))
                    {
                        printf("Proceso nivel 3 ID: %lu\n", pi3.dwProcessId);
                    }
                }
            }
        }

        WaitForSingleObject(pi_hijo.hProcess, INFINITE);

        CloseHandle(pi_hijo.hProcess);
        CloseHandle(pi_hijo.hThread);
    }
    else
    {
        printf("Error al crear proceso (%lu)\n", GetLastError());
    }

    return 0;
}
