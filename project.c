#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure definitions
typedef struct File
{
    char file_name[50];
    int file_size;
    struct File *next;
} File;

typedef struct Directory
{
    char fold_name[50];
    struct Directory *sub_dir;
    struct File *dir_file;
    struct Directory *next_dir;
} Directory;

typedef struct User
{
    char username[50];
    Directory *root_dir;
} User;

// Function prototypes for file operations
void createFile(const char *file_name);
void writeFile(const char *filename, const char *content);
void readFile(const char *filename);
void deleteFile(const char *filename);

// Function prototypes for directory operations
void add_file(Directory *dir, const char *file_name, int file_size);
void print_dir_struct(Directory *dir, int depth);
void del_directory(Directory *dir);
void add_sub_dir(Directory *parent_dir, Directory *child_dir);
Directory *create_dir(const char *dir_name);
void initialize_default_dir(Directory *root);
void directory_operations_menu(Directory *current_dir);

// Function prototypes for authentication
int authenticate(const char *username, const char *password);
int login(User *logged_in_user);
void register_user();

// File operation functions
void createFile(const char *file_name)
{
    FILE *file = fopen(file_name, "w");
    if (file == NULL)
    {
        printf("Error while creating file: %s\n", file_name);
        return;
    }
    printf("File created: %s\n", file_name);
    fclose(file);
}

void writeFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error While writing to the file: %s\n", filename);
        return;
    }
    fprintf(file, "%s", content);
    printf("Content written to file: %s\n", filename);
    fclose(file);
}

void readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error while reading the file: %s\n", filename);
        return;
    }
    char ch;
    printf("File content:\n");
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
        printf("File deleted successfully: %s\n", filename);
    }
    else
    {
        printf("Error in deleting the file: %s\n", filename);
    }
}

// Directory operation functions
Directory *create_dir(const char *dir_name)
{
    Directory *new_dir = (Directory *)malloc(sizeof(Directory));
    strcpy(new_dir->fold_name, dir_name);
    new_dir->sub_dir = NULL;
    new_dir->dir_file = NULL;
    new_dir->next_dir = NULL;
    return new_dir;
}

void add_sub_dir(Directory *parent_dir, Directory *child_dir)
{
    if (parent_dir->sub_dir == NULL)
    {
        parent_dir->sub_dir = child_dir;
        return;
    }

    Directory *temp = parent_dir->sub_dir;
    while (temp->next_dir != NULL)
    {
        temp = temp->next_dir;
    }
    temp->next_dir = child_dir;
}

void add_file(Directory *dir, const char *file_name, int file_size)
{
    File *new_file = (File *)malloc(sizeof(File));
    if (new_file == NULL)
    {
        printf("Memory allocation failed for file\n");
        return;
    }
    strcpy(new_file->file_name, file_name);
    new_file->file_size = file_size;
    new_file->next = NULL;

    if (dir->dir_file == NULL)
    {
        dir->dir_file = new_file;
    }
    else
    {
        File *temp = dir->dir_file;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_file;
    }
    createFile(file_name);
}

void print_dir_struct(Directory *dir, int depth)
{
    if (dir == NULL)
        return;
    for (int i = 0; i < depth; i++)
    {
        printf(" ");
    }
    printf("Directory: %s\n", dir->fold_name);

    File *file = dir->dir_file;
    while (file != NULL)
    {
        for (int i = 0; i < depth + 1; i++)
        {
            printf(" ");
        }
        printf("File: %s (Size: %d bytes)\n", file->file_name, file->file_size);
        file = file->next;
    }

    print_dir_struct(dir->sub_dir, depth + 1);
    print_dir_struct(dir->next_dir, depth);
}

void del_directory(Directory *dir)
{
    if (dir == NULL)
    {
        return;
    }
    File *file = dir->dir_file;
    while (file != NULL)
    {
        File *next_file = file->next;
        free(file);
        file = next_file;
    }
    del_directory(dir->sub_dir);
    del_directory(dir->next_dir);
    free(dir);
}

void initialize_default_dir(Directory *root)
{
    const char *default_dir[] = {"Images", "Docs", "More", "Bluetooth", "Lib", "Src"};
    int default_dir_num = sizeof(default_dir) / sizeof(default_dir[0]);
    for (int i = 0; i < default_dir_num; i++)
    {
        Directory *new_dir = create_dir(default_dir[i]);
        add_sub_dir(root, new_dir);
    }
}

// Menu function for file and directory operations
void directory_operations_menu(Directory *current_dir)
{
    char command[20];
    char filename[50];
    char content[100];
    while (1)
    {
        printf("\nEnter command (CreateFile, ReadFile, WriteFile, DeleteFile, CreateDir, OpenDir, DeleteDir, Back): ");
        scanf("%s", command);

        if (strcmp(command, "CreateFile") == 0)
        {
            printf("Enter filename to create: ");
            scanf("%s", filename);
            printf("Enter file size: ");
            int size;
            scanf("%d", &size);
            add_file(current_dir, filename, size);
        }
        else if (strcmp(command, "ReadFile") == 0)
        {
            printf("Enter the filename to read: ");
            scanf("%s", filename);
            readFile(filename);
        }
        else if (strcmp(command, "WriteFile") == 0)
        {
            printf("Enter the filename to write: ");
            scanf("%s", filename);
            printf("Enter the content to write: ");
            scanf(" %[^\n]", content); // Reads a string with spaces
            writeFile(filename, content);
        }
        else if (strcmp(command, "DeleteFile") == 0)
        {
            printf("Enter the filename to delete: ");
            scanf("%s", filename);
            deleteFile(filename);
        }
        else if (strcmp(command, "CreateDir") == 0)
        {
            printf("Enter directory name to create: ");
            scanf("%s", filename);
            Directory *new_dir = create_dir(filename);
            add_sub_dir(current_dir, new_dir);
            printf("Directory created: %s\n", filename);
        }
        else if (strcmp(command, "OpenDir") == 0)
        {
            printf("Enter the directory name to open: ");
            scanf("%s", filename);
            Directory *temp = current_dir->sub_dir;
            while (temp != NULL && strcmp(temp->fold_name, filename) != 0)
            {
                temp = temp->next_dir;
            }
            if (temp != NULL)
            {
                printf("Entering directory: %s\n", filename);
                directory_operations_menu(temp);
            }
            else
            {
                printf("Directory not found: %s\n", filename);
            }
        }
        else if (strcmp(command, "DeleteDir") == 0)
        {
            printf("Enter the sub-directory name to delete in current: ");
            scanf("%s", filename);
            Directory *temp = current_dir->sub_dir;
            while (temp != NULL)
            {
                if (strcmp(temp->fold_name, filename) == 0)
                {
                    break;
                }
                temp = temp->next_dir;
            }
            del_directory(temp);
        }
        else if (strcmp(command, "Back") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command. Try again.\n");
        }
    }
}

// Authentication functions
int authenticate(const char *username, const char *password)
{
    FILE *file = fopen("user_db.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open user database.\n");
        return 0;
    }
    char stored_user[50], stored_pass[50];
    while (fscanf(file, "%s %s", stored_user, stored_pass) != EOF)
    {
        if (strcmp(username, stored_user) == 0 && strcmp(password, stored_pass) == 0)
        {
            fclose(file);
            return 1; // Authentication successful
        }
    }
    fclose(file);
    return 0; // Authentication failed
}

void register_user()
{
    FILE *file = fopen("user_db.txt", "a");
    if (file == NULL)
    {
        printf("Error: Could not open user database.\n");
        return;
    }

    char username[50];
    char password[50];

    printf("Enter a username: ");
    scanf("%s", username);

    printf("Enter a password: ");
    scanf("%s", password);

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("User registered successfully!\n");
}

int login(User *logged_in_user)
{
    char username[50];
    char password[50];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    if (authenticate(username, password))
    {
        printf("Login Successful\n");
        strcpy(logged_in_user->username, username);

        // Creating a unique root directory for the user
        logged_in_user->root_dir = create_dir(username);

        // Initializing default directories under the user's root
        initialize_default_dir(logged_in_user->root_dir);

        return 1;
    }
    else
    {
        printf("Login failed. Please enter correct credentials.\n");
        return 0;
    }
}

int main()
{
    printf("\t *********** Welcome to File Manager *********** \t\n");

    int choice;
    User logged_in_user;

    while (1)
    {
        printf("1. Register\n2. Login\n3. Exit\nChoose an option: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            register_user();
        }
        else if (choice == 2)
        {
            if (login(&logged_in_user))
            {
                // Print the user's initial directory structure
                printf("\nInitial Directory Structure for %s:\n", logged_in_user.username);
                print_dir_struct(logged_in_user.root_dir, 0);

                // Open the directory operations menu starting from the user's root directory
                directory_operations_menu(logged_in_user.root_dir);

                // Clean up and delete the user's directory structure
                del_directory(logged_in_user.root_dir);

                printf("Exiting the file manager for user %s.\n", logged_in_user.username);
            }
        }
        else if (choice == 3)
        {
            printf("Exiting program.\n");
            break;
        }
        else
        {
            printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}
