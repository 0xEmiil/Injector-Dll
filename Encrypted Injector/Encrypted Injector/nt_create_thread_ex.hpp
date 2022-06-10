#pragma once
#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include "design_ui.hpp"
#include <iostream>
namespace Encrypted
{
	class CreateThread {
	public:
		
		
		
		std::string class_target = "grcWindow";
		// Nt create thread header def ref -> https://undocumented.ntinternals.net/
		typedef NTSTATUS(WINAPI* lpNtCreateThreadEx)(

			OUT		PHANDLE				hThread,
			IN		ACCESS_MASK			DesiredAccess,
			IN		LPVOID				ObjectAttributes,
			IN		HANDLE				ProcessHandle,
			IN		LPVOID				lpStartAddress,
			IN		LPVOID				lpParameter,
			IN		ULONG				CreateSuspended,
			IN		SIZE_T				StackZeroBits,
			IN		SIZE_T				SizeOfStackCommit,
			IN		SIZE_T				SizeOfStackReserve,
			OUT		LPVOID				lpBytesBuffer
			);
		// sacado de la api de Nt create thread
		struct NtCreateThreadExBuffer
		{
			SIZE_T	Size;
			SIZE_T	Unknown1;
			SIZE_T	Unknown2;
			PULONG	Unknown3;
			SIZE_T	Unknown4;
			SIZE_T	Unknown5;
			SIZE_T	Unknown6;
			PULONG	Unknown7;
			SIZE_T	Unknown8;
		};
		/*
		* Redireccionar el path del archivo a un modulo en especifico
		* in - hProcess
		* in, optional - hModule
		* out - lpFilename
		* in - nSize
		*/
		HMODULE hm_get_module_path()
		{
			return HMODULE();
		}
		/*
         * Se escalan los privilegios del sistema
        */
		int iescalar_privilegios()
		{
			HANDLE doToken;
			if (!OpenThreadToken(
				GetCurrentThread(),
				TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, /* TOKEN_ADJUST_PRIVILEGES : se requiere cambiar privilegios del token especifico | * TOKEN_QUERY : requiere request al token */
				0,
				&doToken))
			{
				if (!ImpersonateSelf(SecurityImpersonation)) return 13; /* 13 = error return*/
			}
			if (!bget_privilegios(
				doToken,
				SE_DEBUG_NAME,
				1))
			{
				CloseHandle(doToken);
				return 13;
			}

			return 0;
		}
		/*
		* Se ponen los privilegios del sistema
		*/
		bool bget_privilegios(HANDLE hToken, LPCTSTR Privilege, BOOL bEnablePrivilege)
		{
			TOKEN_PRIVILEGES token, token_old; LUID luid; DWORD dwPrev = sizeof(TOKEN_PRIVILEGES);
			if (!LookupPrivilegeValue(
				NULL,
				Privilege,
				&luid))
				return 0;
			token.PrivilegeCount = 1, token.Privileges[0].Luid = luid, token.Privileges[0].Attributes = 0;

			AdjustTokenPrivileges(
				hToken,
				0,
				&token,
				sizeof(TOKEN_PRIVILEGES),
				&token_old,
				&dwPrev);

			token_old.PrivilegeCount = 1, token_old.Privileges[0].Luid = luid;

			if (bEnablePrivilege)
				token_old.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
			else
				token_old.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & token_old.Privileges[0].Attributes);

			AdjustTokenPrivileges(
				hToken,
				0,
				&token_old,
				dwPrev,
				NULL,
				NULL);

			return 1;
		}
		/*
		* Creacion de operacion para injectar el dll
		*/
		bool nt_create_thread(LPCSTR dllpath, HANDLE hProcess)
		{
			LPVOID get_kernel = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
			LPVOID dll_mem_process = VirtualAllocEx(hProcess, 0, strlen(dllpath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			BOOL write_mem = WriteProcessMemory(hProcess, dll_mem_process, (LPVOID)dllpath, strlen(dllpath), NULL); // posible crashh mem aqui | arg : 3
			HMODULE ntdll = GetModuleHandle("ntdll.dll"); // se arreglo un errorsito cambiando de "unicode" a "multibyte"
			lpNtCreateThreadEx fNtCreateThreadEx = (lpNtCreateThreadEx)GetProcAddress(ntdll, "NtCreateThreadEx");
			HANDLE hThread = nullptr;
			if (!get_kernel)
			{
				/* no se puede obtener la direccion de kernel32*/
				return 0;
			}
			if (!dll_mem_process)
			{
				/* no se puede obtener la memoria del dll*/
				return 0;
			}
			if (!write_mem)
			{
				/* no se puede alocar memoria*/
				return 0;
			}
			if (!ntdll)
			{
				/* no se puede obtener el modulo de ntdll.dll*/
				return 0;
			}
			else {
				if (!fNtCreateThreadEx)
				{
					/* no se puede obtener la direccion de ntdll.dll*/
					return 0;
				}
			}
			NtCreateThreadExBuffer ntbuffer = { 0 };
			ULONG temp1 = { 0 };
			ULONG temp2 = { 0 };
			// ref de valores -> https://github.com/nettitude/DLLInjection/blob/master/Nettitude/Injection/InjectNTCreateThreadEx.cpp
			ntbuffer.Size = sizeof(NtCreateThreadExBuffer), ntbuffer.Unknown1 = 0x10003, ntbuffer.Unknown2 = 0x8, ntbuffer.Unknown3 = (DWORD*)&temp2, ntbuffer.Unknown4 = 0, ntbuffer.Unknown5 = 0x10004,
			ntbuffer.Unknown6 = 4, ntbuffer.Unknown7 = (DWORD*)&temp1, ntbuffer.Unknown8 = 0;

			NTSTATUS status = fNtCreateThreadEx(
				&hThread,
				THREAD_ALL_ACCESS,
				nullptr,
				hProcess,
				(LPTHREAD_START_ROUTINE)get_kernel,
				dll_mem_process,
				NULL, //start instantly
				0,
				0,
				0,
				nullptr
			);
			if (!hThread)
			{
				/* nt create thread fallo :c*/
				if (VirtualFreeEx(
					hProcess,
					dll_mem_process,
					0,
					MEM_RELEASE
				))
				/* se libero la memoria del processo*/

				
				return 0;
			}
			/* se espera a que el thread se haya completado*/
			WaitForSingleObject(hThread, INFINITE);

			if (VirtualFreeEx(
				hProcess,
				dll_mem_process,
				0,
				MEM_RELEASE
			))
			{
				Sleep(1000);
			}
			CloseHandle(hThread);
			CloseHandle(hProcess);

			return 1;
		}
		/*
		* inject
		*/
		int inject()
		{
			
			iescalar_privilegios();
			DWORD pId;
			PROCESSENTRY32 ProcEntry{ sizeof(PROCESSENTRY32) };
			ProcEntry.dwSize = sizeof(ProcEntry);
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), hProcess;
			BOOL bRet = Process32First(hSnap, &ProcEntry);
			while (bRet)
			{
				// se convierte de string a lpcstr
				//LPTSTR x = new TCHAR[proceso.size() + 1];
				//std::strcpy(x, proceso.c_str());
				//strcpy_s(x, proceso.c_str());
				if (!std::strcmp((LPCSTR)proceso.c_str(), ProcEntry.szExeFile)) // se encontro 
				{
					pId = ProcEntry.th32ProcessID;
					Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "PID: " << pId << std::endl;
				}
				bRet = Process32Next(hSnap, &ProcEntry);
			}
			bRet = Process32Next(hSnap, &ProcEntry);

			CloseHandle(hSnap);

			hProcess = OpenProcess( // no pues no existe o no esta corriendo el procceso
				PROCESS_QUERY_INFORMATION |
				PROCESS_CREATE_THREAD |
				PROCESS_VM_OPERATION |
				PROCESS_VM_WRITE,
				FALSE, pId);
			if (!hProcess)
			{
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "No se puede Abrir el proceso" << std::endl;
			}
			bget_privilegios(hToken, SE_DEBUG_NAME, 0);

			CloseHandle(hToken);

			Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Injectado" << std::endl;

			nt_create_thread(dll_path, hProcess);

			CloseHandle(hProcess);// me da un probelita/ ARREGLARLO -> YA

			if (!TerminateProcess(hProcess, 0xffffffff))
				
			return 0;
		}
		void thread_sub()
		{
			
			do {
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Escribe el Proceso > "; std::cin >> proceso;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Proceso: " << proceso << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "1. Injectar" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "2. Atras" << std::endl;
				Encrypted::d->set_coord_text(15, Encrypted::current_coords.y); std::cout << "Seleciona una opcion > "; std::cin >> iIndexThread;
				switch (iIndexThread)
				{
				case 1: inject(); break;
				case 2: Encrypted::current_coords.y = 0; system("cls"); break;
				}
			} while (iIndexThread != 2);
		}
		private: 
			int iIndexThread;
			std::string proceso = "";
			//std::string dll_path = "";
			LPCSTR dll_path = "D:\\Emiliano Rios\\Documents\\ProyectosCode\\!Encrypted Menu\\bin\\Debug\\BigBaseV2.dll"; // se predefine la ruta
			HANDLE hToken;
	};
	CreateThread g_thread;
}

