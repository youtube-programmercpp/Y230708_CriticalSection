#include <stdio.h>
int counter;
void f()
{
	for (int i = 0; i < 1000000; ++i)
		++counter;
}
#include <Windows.h>
int main()
{
	if (const auto hThread = /*WINBASEAPI _Ret_maybenull_ HANDLE WINAPI*/CreateThread
	( /*_In_opt_                  LPSECURITY_ATTRIBUTES  lpThreadAttributes*/nullptr
	, /*_In_                      SIZE_T                 dwStackSize       */0
	, /*_In_                      LPTHREAD_START_ROUTINE lpStartAddress    */[](void*){f();return 0UL;}
	, /*_In_opt_ __drv_aliasesMem LPVOID                 lpParameter       */nullptr
	, /*_In_                      DWORD                  dwCreationFlags   */0
	, /*_Out_opt_                 LPDWORD                lpThreadId        */nullptr
	)) {
		(void)WaitForSingleObject(hThread, INFINITE);//スレッドが終わるまで待つ
		printf("%d\n", counter);
		(void)CloseHandle(hThread);
	}
}
