#include <stdio.h>
#include <windows.h> 
HANDLE ReadMutex; 
HANDLE WriteMutex;
int readercount = 0;
int data = 0;

DWORD WINAPI Reader(LPVOID); //读者线程 
DWORD WINAPI Writer(LPVOID);    //写者线程

DWORD WINAPI Reader(LPVOID lpPara) 
{   
    int i;
    for(i = 0; i < 3;i ++)
    {   

        WaitForSingleObject(ReadMutex,INFINITE); //p 互斥
        readercount ++;
        if (readercount == 1)
        {
            WaitForSingleObject(WriteMutex,INFINITE);
        }
        ReleaseMutex(ReadMutex); //v;  

        printf("reader %d is reading\n",GetCurrentThreadId()); 
        printf("get data = %d\n\n",data);       
        Sleep(1500);

        WaitForSingleObject(ReadMutex,INFINITE); //p 互斥
        readercount --;
        if (readercount == 0)
        {
            ReleaseMutex(WriteMutex);
        }
        ReleaseMutex(ReadMutex); //v;
    }

    return 0; 
}

DWORD  WINAPI Writer(LPVOID lpPara) 
{   
    int i;
    for(i = 0; i < 3;i ++)
    {          
        WaitForSingleObject(WriteMutex,INFINITE);  //P(mutex);         
        printf("writer %d is writing\n",GetCurrentThreadId());
        data ++;
        printf("set data = %d\n\n",data);       
        Sleep(1500);         
        ReleaseMutex(WriteMutex);  //V    
    }     
    return 0; 
}

int main()
{
    ReadMutex = CreateMutex(NULL,FALSE,NULL);
    WriteMutex = CreateMutex(NULL,FALSE,NULL);

    const unsigned short READERS_COUNT = 4;  //读者的个数     
    const unsigned short WRITERS_COUNT = 4;  //写者的个数

    //总的线程数     
    const unsigned short THREADS_COUNT = READERS_COUNT + WRITERS_COUNT;

    HANDLE hThreads[THREADS_COUNT]; // 各线程的 handle     
    DWORD readerID[READERS_COUNT]; // 读者线程的id     
    DWORD writerID[WRITERS_COUNT]; // 写者线程的id

    int i;
    //创建写者线程     
    for (i = 0;i< WRITERS_COUNT;i ++)
    {         
        hThreads[i] = CreateThread(NULL,0,Writer,NULL,0,&writerID[i]);         
        if (hThreads[i]==NULL) 
            return -1;     
    }

    //创建读者线程
    for (i = 0;i < READERS_COUNT;i ++)
    {         
        hThreads[WRITERS_COUNT + i] = CreateThread(NULL,0,Reader,NULL,0,&readerID[i]);         
        if (hThreads[WRITERS_COUNT + i] == NULL)
            return -1;     
    }

    getchar();

    CloseHandle(ReadMutex); 
    CloseHandle(WriteMutex);
    for (i = 0; i < THREADS_COUNT; i++)
    {
        CloseHandle(hThreads[i]);
    }    
    return 0; 
}