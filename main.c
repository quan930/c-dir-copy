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
void dir_copy_no_recursive(char *, char *);
int main(){
    dir_copy("/Users/daquan/Desktop/1","/Users/daquan/Desktop/qqq/2");
    return 0;
}
void mkdirs(char *path){
    if (access(path,F_OK)==0){}//文件存在
    else{//获取上级目录
        int len= (int)strlen(path);
        int position=-1;
        for (int i = len-1; i >= 0 ; --i) {
            if (path[i]=='/'){
                position = i;
                break;
            }
        }
        if (position==-1){mkdir(path,ACCESSPERMS);}//防止相对路径
        else{
            char *newPath = calloc((size_t)position, sizeof(char));
            mkdirs(strncpy(newPath,path,position));
            free(newPath);
            mkdir(path,ACCESSPERMS);
        }
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
    DIR *dir =  opendir(oldpath);
    if (dir == NULL){//不是目录
        file_copy(oldpath,newpath);
    } else{
        mkdirs(newpath);//创建目录
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL){
            if (strcmp(dp->d_name,".") == 0||strcmp(dp->d_name,"..") == 0)
                continue;
            char *oldFileName = calloc(strlen(oldpath)+strlen(dp->d_name)+2,1);
            char *newFileName = calloc(strlen(newpath)+strlen(dp->d_name)+2,1);
            dir_copy(strcat(strcat(strcat(oldFileName,oldpath),"/"),dp->d_name),strcat(strcat(strcat(newFileName,newpath),"/"),dp->d_name));
            free(oldFileName);
            free(newFileName);
        }
        closedir(dir);
    }
}
void print_dir(char *path,int depth){
    struct dirent *dp;
    DIR *dir =  opendir(path);
    if (dir == NULL){
        for (int i = 0; i < depth; ++i) {printf("%s","..");}
        printf("%s\n",path);
    } else{
        for (int i = 0; i < depth; ++i) {printf("%s","..");}
        printf("%s/\n",path);
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
void dir_copy_no_recursive(char *oldpath, char *newpath){
    DIR *dir =  opendir(oldpath);
    mkdirs(newpath);//创建目录
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name,".") == 0||strcmp(dp->d_name,"..") == 0)
            continue;
        char *oldFileName = calloc(strlen(oldpath)+strlen(dp->d_name)+2,1);
        char *newFileName = calloc(strlen(newpath)+strlen(dp->d_name)+2,1);
        file_copy(strcat(strcat(strcat(oldFileName,oldpath),"/"),dp->d_name),strcat(strcat(strcat(newFileName,newpath),"/"),dp->d_name));
        free(oldFileName);
        free(newFileName);
    }
    closedir(dir);
}