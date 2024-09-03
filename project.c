#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void createFile(const char *file_name)
{                                      // Passing the filename as string
    FILE *file = fopen(file_name, "w"); // This line will try to open the file with write access
    // if file will not exist it will create a new file
    if (file == NULL)
    {
        printf("Error while creating file: %s\n", file_name);
        return;
    }
    printf("File created: %s\n", file_name);
    fclose(file); // Closes the file after creation.
}
    void writeFile(const char *filename, const char *content)
    {
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Error While writing to the file: %s\n ", filename);
            return;
        }
        fprintf(file, "%s", content);
        printf("Content written to file: %s\n", filename);
        fclose(file);
    }
    void readFile(const char *filename)
    {
        FILE *file = fopen(filename, "r"); // will open the file in reading mode
        if (file == NULL)
        {
            printf("Error while reading the file: %s\n", filename);
            return;
        }
        char ch;
        printf("File content  \n");
        while ((ch = fgetc(file)) != EOF)
        {
            putchar(ch);
        }
        printf("\n");
        fclose(file);
    }
    void deleteFile(const char *filename)
    {
        if (remove(filename) == 0)
        {
            printf("File deleted successfully : %s\n", filename);
        }
        else
        {
            printf("Error in deleting the file :%s\n", filename);
        }
    }

    //Implementing user authentication
    typedef struct User{
        char user_name[50];
        char password[50];
    }User;
    int authenticate(const char* username,const char* password){
        FILE* file=fopen("user_db.txt","r");
        if(file==NULL){
            printf("Error: Could not open user database \n");
            return 0;
        }
        char stored_user[50],stored_pass[50];
        while(fscanf(file,"%s %s",stored_user,stored_pass)!=EOF){
            if((strcmp(username,stored_user)==0  && strcmp(password,stored_pass)==0)){
                fclose(file);
                return 1;//Authentication successful
            }
        }
        //authentication fail
        fclose(file);
        return 0;
    }
    int login() {
    printf("Hello User, Please login\n");

    char username[50];
    char password[50];

    // Get username input
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline character if present

    // Get password input
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';  // Remove newline character if present

    // Authentication
    if (authenticate(username, password)) {
        printf("Login Successful\n");
        return 1;
    }

    printf("Login failed, please enter correct credentials\n");
    return 0;
}

    //Simulating directory structure
    //Defining a structure to represent a file
    typedef struct File{
        char file_name[50];//Name of the file
        int file_size;
        struct File *next;//Given pointer store the address of next file in same directory
    }File;
    typedef struct Directory{
        char fold_name[50];//Name of directory
        struct Directory* sub_dir;//reference to sub-directory within current directory
        struct File* dir_file;//file in directory
        struct Directory *next_dir;//directory next to given directory
    }Directory;
    //Function to create Directory
    Directory* create_dir(const char* dir_name){//will take reference of file name
        Directory *new_dir=(Directory *)malloc(sizeof(Directory));
        strcpy(new_dir->fold_name,dir_name);
        new_dir->sub_dir=NULL;
        new_dir->dir_file=NULL;
        new_dir->next_dir=NULL;
        return new_dir;
    }
    void add_sub_dir(Directory *parent_dir,Directory *child_dir){
        if(parent_dir->sub_dir==NULL){
            //is empty
            parent_dir->sub_dir=child_dir;
        }
        else{
            //parent directory has already some child directories
            Directory *temp=parent_dir->sub_dir;
            while(temp->next_dir!=NULL){
                //came on last sub_dir
                temp=temp->next_dir;
            }
            temp->next_dir=child_dir;
            //New directory added 
        }
    }
    void add_file(Directory *dir,const char* file_name,int file_size){
        File *new_file=(File*)malloc(sizeof(File));
        if(new_file==NULL){
            printf("Memory allocation failed for file\n");
           

        }
        strcpy(new_file->file_name,file_name);
        new_file->file_size=file_size;
        new_file->next=NULL;
        if(dir->dir_file==NULL){
            dir->dir_file=new_file;
        }
        else{
            File* temp=dir->dir_file;
            while(temp->next!=NULL){
                temp=temp->next;
            }
            temp->next=new_file;//New File added
        }
        //Actually creating file
        createFile(file_name);
    }
  
    
    //Function to intialize default directories
    void intialize_default_dir(Directory* root){
        //default directories
        const char* default_dir[]={"Images","Docs","More","Bluetooth","Lib","Src"};
        //Array of pointers Each element of the array is a pointer to a string literal like "More", "src", etc.
        int default_dir_num=sizeof(default_dir)/sizeof(default_dir[0]);
        //creating and adding default directories to root directories
        for(int i=0;i<default_dir_num;i++){
            Directory* new_dir=create_dir(default_dir[i]);
            add_sub_dir(root,new_dir);
        }
    }
    //Function to print a directory structure
    void print_dir_struct(Directory *dir,int depth){
        if(dir==NULL)return;//basically we can visualize the directory as tree
        for(int i=0;i<depth;i++){
            printf(" ");
        }
        printf("Directory: %s\n",dir->fold_name);
        //files in current directory
        File *file=dir->dir_file;
        while(file!=NULL){
            for(int i=0;i<depth+1;i++){
                printf(" ");
            }
            printf("File: %s (Size: %d bytes)\n",file->file_name,file->file_size);
            file=file->next;
        }
        //print subdirectories
       print_dir_struct(dir->sub_dir, depth + 1);
        // Print sibling directories at the same level
        if(dir->next_dir==NULL){
            printf("\n---You are in directory: %s---\n",dir->fold_name);
            file_operations_menu(dir);//show options to perform operations
        }
         print_dir_struct(dir->next_dir, depth);
         


    }
    //Function to delete directory
    void del_directory(Directory* dir){
        if(dir==NULL){
            return;
        }
        File *file=dir->dir_file;
        while(file!=NULL){
            File *next_file=file->next;
            free(file);
            file=next_file;

        }
         // Free subdirectories recursively
        free_directory(dir->sub_dir);
        free_directory(dir->next_dir);
        free(dir);
    }
    void directory_operations_menu(Directory *current_dir) {
    char command[10];
    char filename[50];
    char content[100];
    while (1) {
        printf("\n Enter command (CreateFile, ReadFile, WriteFile, DeleteFile, CreateDir, DeleteDir, OpenDir, Back): ");
        scanf("%s", command);
        
        if (strcmp(command, "CreateFile") == 0) {
            printf("Enter filename to create: ");
            scanf("%s", filename);
            printf("Enter file size: ");
            int size;
            scanf("%d", &size);
            add_file(current_dir, filename, size);
        } else if (strcmp(command, "ReadFile") == 0) {
            printf("Enter the filename to read: ");
            scanf("%s", filename);
            readFile(filename);
        } else if (strcmp(command, "WriteFile") == 0) {
            printf("Enter the filename to write: ");
            scanf("%s", filename);
            printf("Enter the content to write: ");
            scanf("%s", content);
            writeFile(filename, content);
        } else if (strcmp(command, "DeleteFile") == 0) {
            printf("Enter the filename to delete: ");
            scanf("%s", filename);
            deleteFile(filename);
        } else if (strcmp(command, "CreateDir") == 0) {
            printf("Enter directory name to create: ");
            scanf("%s", filename);
            Directory *new_dir = create_dir(filename);
            add_sub_dir(current_dir, new_dir);
            printf("Directory created: %s\n", filename);
        } else if (strcmp(command, "DeleteDir") == 0) {
            // Implement directory deletion logic here
        } else if (strcmp(command, "OpenDir") == 0) {
            printf("Enter the directory name to open: ");
            scanf("%s", filename);
            Directory *temp = current_dir->sub_dir;
            while (temp != NULL && strcmp(temp->fold_name, filename) != 0) {
                temp = temp->next_dir;
            }
            if (temp != NULL) {
                printf("Entering directory: %s\n", filename);
                directory_operations_menu(temp);
            } else {
                printf("Directory not found: %s\n", filename);
            }
        } else if (strcmp(command, "Back") == 0) {
            break;
        } else {
            printf("Invalid command. Try again.\n");
        }
    }
}
    
    void welcome(){
        printf("\t ***********Welcome to file manager********** \t\n");
       
       if(login()){
        Directory *root=create_dir("root");
            add_file(root, "example.txt", 1024);
    Directory *images = create_dir("Images");
    add_sub_dir(root, images);
    add_file(images, "image.jpg", 2048);
    Directory *docs = create_dir("Docs");
    add_sub_dir(root, docs);
        print_dir_struct(root,0);
        del_directory(root);
       }
        
    } 
   int main() {
    // Display welcome message
    printf("\t*********** Welcome to the File Manager **********\t\n");

    // Perform user login
    if (login()) {
        // After successful login, create the root directory
        Directory *root = create_dir("root");

        // Initialize default directories under the root directory
        initialize_default_dir(root);

        // Display the directory structure
        printf("\nInitial Directory Structure:\n");
        print_dir_struct(root, 0);

        // Start operations on the directory structure
        directory_operations_menu(root);

        // Clean up and free memory
        del_directory(root);
    } else {
        printf("Login failed. Exiting...\n");
    }

    return 0;
}

