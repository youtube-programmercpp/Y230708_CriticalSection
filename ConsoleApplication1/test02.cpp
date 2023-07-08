#include <process.h>
#include <Windows.h>
int counter;
CRITICAL_SECTION cs;
void f()
{
	for (int i = 0; i < 1000000; ++i) {
		//クリティカルセクションに入る
		EnterCriticalSection(&cs);
		//クリティカルセクションから出る
		LeaveCriticalSection(&cs);
	}
}
#include <stdio.h>
int main()
{
	//クリティカルセクションを用意する
	InitializeCriticalSection(&cs);

	if (const auto hThread1 = HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
	( /*_In_opt_  void*                    _Security    */nullptr
	, /*_In_      unsigned                 _StackSize   */0
	, /*_In_      _beginthreadex_proc_type _StartAddress*/[](void*){f();return 0U;}
	, /*_In_opt_  void*                    _ArgList     */nullptr
	, /*_In_      unsigned                 _InitFlag    */0
	, /*_Out_opt_ unsigned*                _ThrdAddr    */nullptr
	))) {
		if (const auto hThread2 = HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
		( /*_In_opt_  void*                    _Security    */nullptr
		, /*_In_      unsigned                 _StackSize   */0
		, /*_In_      _beginthreadex_proc_type _StartAddress*/[](void*){f();return 0U;}
		, /*_In_opt_  void*                    _ArgList     */nullptr
		, /*_In_      unsigned                 _InitFlag    */0
		, /*_Out_opt_ unsigned*                _ThrdAddr    */nullptr
		))) {

			//ここでスレッド2本が同時に動く形となる

			(void)WaitForSingleObject(hThread2, INFINITE);
			(void)CloseHandle(hThread2);
		}
		(void)WaitForSingleObject(hThread1, INFINITE);
		(void)CloseHandle(hThread1);
	}
	printf("%d\n", counter);


	//クリティカルセクションを破棄する
	DeleteCriticalSection(&cs);
}
