#include "stdio.h"
#include "sys/stat.h"
#include "dirent.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
void mkdirs(char *);//创建多级目录
void file_copy(char *, char *);//文件拷贝
void dir_copy(char *, char *);//目录拷贝
void print_dir(char *,int);//打印目录结构
int main(){
    dir_copy("/Users/daquan/Desktop/1","/Users/daquan/Desktop/2");
    print_dir("/Users/daquan/Desktop",0);
    return 0;
}
void mkdirs(char *path){
    if (access(path,F_OK)==0){}//文件存在
    else{
        //获取上级目录
        int len= (int)strlen(path);
        int position=len;
        for (int i = len-1; i >= 0 ; --i) {
            if (path[i]=='/'){
                position = i;
                break;
            }
        }
        char *newPath = calloc((size_t)position, sizeof(char));
        strncpy(newPath,path,position);
        mkdirs(newPath);
        free(newPath);
        mkdir(path,ACCESSPERMS);
    }
}
void file_copy(char *old_file, char *new_file){//文件拷贝
    FILE *file = fopen(old_file,"r");
    FILE *fileb = fopen(new_file,"w");
    int c;
    while ((c = getc(file)) !=  EOF){//文件结束符（end of file）-1
        putc(c,fileb);
    }
    fclose(file);
    fclose(fileb);
}
void dir_copy(char *oldpath, char *newpath){//目录拷贝
    struct dirent *dp;
    DIR *dir =  opendir(oldpath);
    if (dir == NULL){//不是目录
        file_copy(oldpath,newpath);
    } else{
        chdir(oldpath);
        mkdirs(newpath);
        while ((dp = readdir(dir)) != NULL){
            if (strcmp(dp->d_name,".") == 0||strcmp(dp->d_name,"..") == 0)
                continue;
            char *oldFileName = calloc(1204,1);
            char *newFileName = calloc(1204,1);
            dir_copy(strcat(strcat(strcat(oldFileName,oldpath),"/"),dp->d_name),strcat(strcat(strcat(newFileName,newpath),"/"),dp->d_name));
            free(oldFileName);
            free(newFileName);
        }
        chdir("..");
        closedir(dir);
    }
}
void print_dir(char *path,int depth){
    struct dirent *dp;
    DIR *dir =  opendir(path);
    if (dir == NULL){
        printf("%*s%s\n",depth," ",path);
    } else{
        printf("%*s%s/\n",depth," ",path);
        chdir(path);
        while ((dp = readdir(dir)) != NULL){
            if (strcmp(dp->d_name,".") == 0||strcmp(dp->d_name,"..") == 0){
                continue;
            }
            print_dir(dp->d_name,depth+4);
        }
        chdir("..");
        closedir(dir);
    }
}