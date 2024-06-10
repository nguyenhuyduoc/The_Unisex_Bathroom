#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <ctime>
#include <fstream>
#define THREADCOUNT 10

HANDLE empty, maleCount, femaleCount, turnstile, male, female, mutex;
int mcount = 0, fmcount = 0;
int Order = 1;
int maxPeople ;
std::fstream f;

void MalesProcess(){
	int time;
	int orderNumber = Order++;
			
	WaitForSingleObject(turnstile, INFINITE);
		WaitForSingleObject(male, INFINITE);
			mcount++;
			if(mcount == 1 && fmcount > 0) {
				WaitForSingleObject(mutex, INFINITE);
					f << "Boy " << orderNumber << " comes and waits because the bathroom is being used by Girl, ";
				ReleaseSemaphore(mutex, 1,  NULL);
				WaitForSingleObject(empty, INFINITE);
			}else if(mcount == 1 && fmcount == 0){
				WaitForSingleObject(empty, INFINITE);
			}
		ReleaseSemaphore(male, 1, NULL);
	ReleaseSemaphore(turnstile, 1, NULL);
	if(mcount > maxPeople)
	{
		WaitForSingleObject(mutex, INFINITE);
			f << "Boy " << orderNumber << " comes and waits because the bathroom is full, ";
		ReleaseSemaphore(mutex, 1, NULL);
	}
	WaitForSingleObject(maleCount, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
			f << "Boy " << orderNumber << " gets in, ";
		ReleaseSemaphore(mutex, 1, NULL);
		time=rand()%2000;
		Sleep(time);
		WaitForSingleObject(mutex, INFINITE);
			f << "Boy " << orderNumber << " gets out, ";
		ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(maleCount, 1, NULL);
	
	WaitForSingleObject(male, INFINITE);
		mcount--;
		if(mcount == 0){
			f << "\n\n";
			ReleaseSemaphore(empty, 1, NULL);
		}
	ReleaseSemaphore(male, 1, NULL);
}
void FemalesProcess(){
	int time;
	int orderNumber = Order++;

	WaitForSingleObject(turnstile, INFINITE);
		WaitForSingleObject(female, INFINITE);
			fmcount++;
			if(fmcount == 1 && mcount > 0) {
				WaitForSingleObject(mutex , INFINITE);
					f << "Girl " << orderNumber << " comes and waits because the bathroom is being used by Boy, ";
				ReleaseSemaphore(mutex, 1, NULL);
				WaitForSingleObject(empty, INFINITE);
			}else if(fmcount == 1 && mcount == 0){
				WaitForSingleObject(empty , INFINITE);
			}
		ReleaseSemaphore(female, 1, NULL);
	ReleaseSemaphore(turnstile, 1 , NULL);

	if(fmcount > maxPeople)
	{
		WaitForSingleObject(mutex , INFINITE);
			f << "Girl " << orderNumber << " comes and waits because the bathroom is full, ";
		ReleaseSemaphore(mutex, 1, NULL);
	}

	WaitForSingleObject(femaleCount, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
			f << "Girl " << orderNumber << " gets in, ";
		ReleaseSemaphore(mutex, 1, NULL);
		time=rand()%2000;
		Sleep(time);
		WaitForSingleObject(mutex, INFINITE);
			f << "Girl " << orderNumber << " gets out, ";
		ReleaseSemaphore(mutex, 1, NULL);
	ReleaseSemaphore(femaleCount, 1, NULL);
	
	WaitForSingleObject(female, INFINITE);
		fmcount--;
		if(fmcount == 0){
			f << "\n\n";
			ReleaseSemaphore(empty, 1, NULL);
		}
	ReleaseSemaphore(female, 1, NULL);
}
