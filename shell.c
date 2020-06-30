#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    while (1)
    {
        printf("$ ");
        char command[50] = {""};
        gets(command);
        // printf("command=%s\n",command);

        // exit
        if (strcmp(command,"bye") == 0 || strcmp(command,"quit") == 0 || strcmp(command,"exit") == 0)
        {
            break;
        }

        // help
        if (strcmp(command,"help") == 0)
        {
            printf("command             description\n"
                   "exit/bye/quit       exit shell\n"
                   "cd dir              Change the current directory to dir\n"
                   "pwd                 print current working directory\n"
                   "ls                  list files and directories in current directory\n"
                   "jobs                show all child processes of this shell\n"
                   "echo<content>       print content\n"
                   "environ             show environments in this shell\n"
                   "echo<content>       print content\n");
        }

        // cd
        else if (command[0] == 'c' && command[1] == 'd' && command[2] == ' ')
        {
            char* path = strtok(command, " ");
            path = strtok(NULL, " ");
            /*printf("path = ");
            puts(path);*/
            if (chdir(path) == -1)
            {
                printf("cd: %s: No such file or directory\n",path);
            }
        }
        
        // pwd
        else if (strcmp("pwd",command) == 0)
        {
            char dir[80];
            getcwd(dir,sizeof(dir));
            printf("current working directory: %s\n", dir);
        }

        // ls
        else if (command[0] == 'l' && command[1] == 's' && command[2] == '\0')
        {
            int pid;
            while((pid = fork()) == -1);
            if (pid == 0)
            {
                /*char buf[80];
                getcwd(buf,sizeof(buf));
                printf("current working directory: %s\n", buf);*/
                execlp("/bin/ls","ls",NULL);
            }
            else
            {
                wait(NULL);
            }
        }

        // ps
        else if (strcmp(command,"jobs") == 0)
        {
            int result,parentId;
            parentId = getpid();

            while((result = fork()) == -1);
            if (result == 0)
            {   char args[10];
                sprintf(args, "%d", parentId);
                execlp("/usr/bin/ps","ps","--ppid",args,NULL);
            }
            else
            {
                wait(NULL);
            }
        }

        // echo
        else if (command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o' && command[4] == '<' && command[strlen(command) - 1] == '>')
        {
            char* args = strtok(command, "<");
            args = strtok(NULL, "<");
            args[strlen(args) - 1] = '\0';
            puts(args);
        }

        // environ
        else if (strcmp(command,"environ") == 0)
        {
            FILE *fp;
            char ch;
            if((fp = fopen("/usr/C_progaram/test2/environment","r"))==NULL){
                printf("Cannot open file!\n");
            }
            // 使用fgetc函数后，该位置指针将向后移动一个字节
            ch = fgetc(fp);
            while(ch!=EOF)
            {
                putchar(ch);
                ch = fgetc(fp);
            }
            fclose(fp);
        }

        // java
        else if (command[0] == 'j' && command[1] == 'a' && command[2] == 'v' && command[3] == 'a')
        {
            char* args = strtok(command, " ");
            char* cmd = args; // 获取命令
            args = strtok(NULL, " "); // 获取命令的参数
            //printf("cmd = ");
            //puts(cmd);
            //printf("args = ");
            //puts(args);

            // 获取外部命令的路径
            char line[1000] = {0};
            char* path;
            FILE *fp = fopen("/usr/C_progaram/test2/environment", "r");
            if(NULL == fp)
            {
                printf("Cannot open file!\n");
            }
            while(!feof(fp))
            {
                memset(line, 0, sizeof(line));
                fgets(line, sizeof(line) - 1, fp); // 包含了换行符
                line[strlen(line) - 1] = '\0'; // 去除行尾多余的换行符
                //printf("%s\n", line);
                path = strtok(line, "=");
                //printf("path1 = ");
                //puts(path);
                if (strcmp(cmd,path) == 0)
                {
                    path = strtok(NULL, "=");
                    /*printf("path = ");
                    puts(path);*/
                    break;
                }
            }

            // 执行外部命令
            int pid;
            while((pid = fork()) == -1);
            if (pid == 0)
            {
                execlp(path,cmd,args,NULL);
            }
            else
            {
                wait(NULL);
            }


        }

        else
        {
            printf("%s: command not found or error command\n"
                   "you can type `help' to get help\n",command);
        }

    }

    return 0;
}
