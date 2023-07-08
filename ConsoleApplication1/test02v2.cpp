#include <process.h>
#include <Windows.h>

class Test {
	int              counter    ;
	HANDLE           hStartEvent;
	CRITICAL_SECTION cs         ;

	void f() noexcept
	{
		if (WaitForSingleObject(hStartEvent, INFINITE) == WAIT_OBJECT_0) {
			for (int i = 0; i < 1000000; ++i) {
				//�N���e�B�J���Z�N�V�����ɓ���
				EnterCriticalSection(&cs);

				++counter;

				//�N���e�B�J���Z�N�V��������o��
				LeaveCriticalSection(&cs);
			}
		}
	}
	Test(const Test&) = delete;
	Test& operator=(const Test&) = delete;
public:
	Test(HANDLE hStartEvent) noexcept
		: counter()//counter ������� 0 �ɂȂ�
		, hStartEvent(hStartEvent)
	{
		//�N���e�B�J���Z�N�V������p�ӂ���
		InitializeCriticalSection(&cs);
	}
	~Test()
	{
		//�N���e�B�J���Z�N�V������j������
		DeleteCriticalSection(&cs);
	}
	static unsigned CALLBACK ThreadProc(void* p) noexcept
	{
		static_cast<Test*>(p)->f();
		return 0;
	}
	int value() const noexcept
	{
		return counter;
	}
};

#include <stdio.h>
int main()
{
	if (const auto hStartEvent = CreateEventW(nullptr, true, false, nullptr)) {
		Test t(hStartEvent);
		if (const auto hThread1 = HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
		( /*_In_opt_  void*                    _Security    */nullptr
		, /*_In_      unsigned                 _StackSize   */0
		, /*_In_      _beginthreadex_proc_type _StartAddress*/Test::ThreadProc
		, /*_In_opt_  void*                    _ArgList     */&t
		, /*_In_      unsigned                 _InitFlag    */0
		, /*_Out_opt_ unsigned*                _ThrdAddr    */nullptr
		))) {
			if (const auto hThread2 = HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
			( /*_In_opt_  void*                    _Security    */nullptr
			, /*_In_      unsigned                 _StackSize   */0
			, /*_In_      _beginthreadex_proc_type _StartAddress*/Test::ThreadProc
			, /*_In_opt_  void*                    _ArgList     */&t
			, /*_In_      unsigned                 _InitFlag    */0
			, /*_Out_opt_ unsigned*                _ThrdAddr    */nullptr
			))) {
				//�X���b�h�̃X�^�[�g�������ł���ɂ́c
				(void)getchar();
				(void)SetEvent(hStartEvent);

				//�����ŃX���b�h2�{�������ɓ����`�ƂȂ�

				(void)WaitForSingleObject(hThread2, INFINITE);
				(void)CloseHandle(hThread2);
			}
			(void)WaitForSingleObject(hThread1, INFINITE);
			(void)CloseHandle(hThread1);
		}
		printf("%d\n", t.value());

		(void)CloseHandle(hStartEvent);
	}




}
