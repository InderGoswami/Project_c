#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
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
void print_dir_struct(const char *dir_name, int depth);
void del_directory(Directory *dir);
void add_sub_dir(Directory *parent_dir, Directory *child_dir);
Directory *create_dir(const char *dir_name);
void initialize_default_dir(Directory *root, const char *base_path);
void directory_operations_menu(Directory *current_dir, const char *base_path);

// Function prototypes for authentication
int authenticate(const char *username, const char *password);
int login(User *logged_in_user);
void register_user();

// Function to create a directory using mkdir command
int create_directory_on_disk(const char *dir_name)
{
    // Attempt to create the directory
    if (mkdir(dir_name, 0777) == -1)
    {
        // Check if the error is because the directory already exists
        if (errno == EEXIST)
        {
            // Directory already exists, no need to print an error
            return 0;
        }
        else
        {
            // Print an error message for other errors
            fprintf(stderr, "Error: Unable to create directory %s: %s\n", dir_name, strerror(errno));
            return -1;
        }
    }
    return 0;
}

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
        printf("Error while writing to the file: %s\n", filename);
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

void print_dir_struct(const char *dir_name, int depth)
{
    DIR *dir = opendir(dir_name);
    if (dir == NULL)
    {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char path[1024];

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip the current and parent directory entries (i.e., "." and "..")
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Print indentation for depth level
        for (int i = 0; i < depth; i++)
        {
            printf("  ");
        }

        // Construct the full path of the entry
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        // Check if the entry is a directory or a file
        if (stat(path, &file_stat) == 0)
        {
            if (S_ISDIR(file_stat.st_mode))
            {
                printf("Directory: %s\n", entry->d_name);
                // Recursively print the contents of the subdirectory
                print_dir_struct(path, depth + 1);
            }
            else if (S_ISREG(file_stat.st_mode))
            {
                printf("File: %s (Size: %lld bytes)\n", entry->d_name, (long long)file_stat.st_size);
            }
        }
    }

    closedir(dir);
}

void del_directory(Directory *dir)
{
    if (dir == NULL)
    {
        return;
    }

    // Recursively delete subdirectories
    del_directory(dir->sub_dir);

    // Recursively delete next directories at the same level
    del_directory(dir->next_dir);

    // Delete files in the current directory
    File *file = dir->dir_file;
    while (file != NULL)
    {
        File *temp_file = file;
        file = file->next;
        free(temp_file); // Free the file structure
    }

    // Free the current directory
    free(dir);
}
void initialize_default_dir(Directory *root, const char *base_path)
{
    // Ensure the user's base directory is created first
    create_directory_on_disk(base_path);

    const char *default_dir[] = {"Images", "Docs", "More", "Bluetooth", "Lib", "Src"};
    int default_dir_num = sizeof(default_dir) / sizeof(default_dir[0]);
    for (int i = 0; i < default_dir_num; i++)
    {
        Directory *new_dir = create_dir(default_dir[i]);
        add_sub_dir(root, new_dir);

        // Create the directory on disk
        char full_path[100];
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, default_dir[i]);
        create_directory_on_disk(full_path);
    }
}

// Menu function for file and directory operations
void directory_operations_menu(Directory *current_dir, const char *base_path)
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

            // Create the file on disk in the target directory
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            createFile(full_path);
        }
        else if (strcmp(command, "ReadFile") == 0)
        {
            printf("Enter the filename to read: ");
            scanf("%s", filename);

            // Read the file from disk in the target directory
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            readFile(full_path);
        }
        else if (strcmp(command, "WriteFile") == 0)
        {
            printf("Enter the filename to write to: ");
            scanf("%s", filename);
            printf("Enter the content to write: ");
            scanf(" %[^\n]", content); // Reads a string with spaces

            // Write to the file on disk in the target directory
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            writeFile(full_path, content);
        }
        else if (strcmp(command, "DeleteFile") == 0)
        {
            printf("Enter the filename to delete: ");
            scanf("%s", filename);

            // Delete the file from disk in the target directory
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            deleteFile(full_path);
        }
        else if (strcmp(command, "CreateDir") == 0)
        {
            printf("Enter directory name to create: ");
            scanf("%s", filename);
            Directory *new_dir = create_dir(filename);
            add_sub_dir(current_dir, new_dir);

            // Create the directory on disk in the target directory
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            create_directory_on_disk(full_path);
        }
        else if (strcmp(command, "OpenDir") == 0)
        {
            // Print the current directory structure
            printf("\nCurrent Directory: %s\n", base_path);
            print_dir_struct(current_dir->fold_name, 0);

            // Prompt the user to enter the name of the directory to open
            printf("Enter directory name to open: ");
            scanf("%s", filename);
            if (strcmp(filename, "stop") == 0)
            {
                break;
            }
            Directory *temp = current_dir->sub_dir;
            while (temp != NULL)
            {
                if (strcmp(temp->fold_name, filename) == 0)
                {
                    char full_path[100];
                    snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);

                    // Call the directory operations menu for the selected sub-directory
                    printf("\nCurrent Directory: %s\n", full_path);
                    directory_operations_menu(temp, full_path);
                    break;
                }
                temp = temp->next_dir;
            }
            if (temp == NULL)
            {
                printf("Directory not found.\n");
            }
        }
        else if (strcmp(command, "DeleteDir") == 0)
        {
            printf("Enter the directory name to delete: ");
            scanf("%s", filename);

            // Find and delete the target directory and its contents
            Directory *prev_dir = NULL, *temp = current_dir->sub_dir;
            while (temp != NULL)
            {
                if (strcmp(temp->fold_name, filename) == 0)
                {
                    if (prev_dir == NULL)
                    {
                        current_dir->sub_dir = temp->next_dir;
                    }
                    else
                    {
                        prev_dir->next_dir = temp->next_dir;
                    }
                    del_directory(temp);
                    break;
                }
                prev_dir = temp;
                temp = temp->next_dir;
            }

            // Remove the directory from disk
            char full_path[100];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, filename);
            if (rmdir(full_path) == -1)
            {
                printf("Error: Unable to delete directory %s\n", full_path);
            }
            else
            {
                printf("Directory deleted: %s\n", full_path);
            }
        }
        else if (strcmp(command, "Back") == 0)
        {
            // Print the updated directory structure before going back
            printf("\nUpdated Directory: %s\n", base_path);
            print_dir_struct(current_dir->fold_name, 0);
            break;
        }
        else
        {
            printf("Invalid command.\n");
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
int is_username_taken(const char *username)
{
    FILE *file = fopen("user_db.txt", "r");
    if (file == NULL)
    {
        return 0; // If the file doesn't exist, no usernames are taken
    }

    char existing_username[50];
    char existing_password[50];

    while (fscanf(file, "%s %s", existing_username, existing_password) != EOF)
    {
        if (strcmp(existing_username, username) == 0)
        {
            fclose(file);
            return 1; // Username is already taken
        }
    }

    fclose(file);
    return 0; // Username is not taken
}

void register_user()
{
    FILE *file = fopen("user_db.txt", "a+"); // Open in a+ mode to allow both reading and appending
    if (file == NULL)
    {
        printf("Error: Could not open user database.\n");
        return;
    }

    char username[50];
    char password[50];

    printf("Enter a username: ");
    scanf("%s", username);

    // Check if username is already taken
    if (is_username_taken(username))
    {
        printf("Error: Username is already taken. Please choose a different username.\n");
        fclose(file);
        return;
    }

    printf("Enter a password: ");
    scanf("%s", password);

    // Register the new user
    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    // Create user directory on disk
    create_directory_on_disk(username);

    printf("User registered successfully!\n");
}
void delete_user()
{
    char username[50];
    char password[50];
    char existing_username[50];
    char existing_password[50];
    int user_found = 0;

    // Prompt user for credentials
    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    // Open the user database file in read mode
    FILE *file = fopen("user_db.txt", "r");
    if (file == NULL)
    {
        printf("Error: Could not open user database.\n");
        return;
    }

    // Temporary file to store remaining users
    FILE *temp_file = fopen("temp_user_db.txt", "w");
    if (temp_file == NULL)
    {
        printf("Error: Could not create temporary file.\n");
        fclose(file);
        return;
    }

    // Read the database line by line
    while (fscanf(file, "%s %s", existing_username, existing_password) != EOF)
    {
        // Check if the entered credentials match
        if (strcmp(existing_username, username) == 0 && strcmp(existing_password, password) == 0)
        {
            user_found = 1;
            // User matched, don't write this user to the temporary file
            printf("User %s has been deleted successfully.\n", username);
        }
        else
        {
            // Write non-matching users to the temporary file
            fprintf(temp_file, "%s %s\n", existing_username, existing_password);
        }
    }

    fclose(file);
    fclose(temp_file);

    // If the user was found and deleted, replace the old database file with the new one
    if (user_found)
    {
        remove("user_db.txt");
        rename("temp_user_db.txt", "user_db.txt");

        // Optionally, delete the user's directory
        char command[100];
        snprintf(command, sizeof(command), "rm -rf %s", username);
        system(command);
        printf("User directory %s deleted.\n", username);
    }
    else
    {
        // If user not found, delete the temporary file
        remove("temp_user_db.txt");
        printf("Error: Username or password is incorrect.\n");
    }
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

        // Create user's root directory structure in memory and on disk
        logged_in_user->root_dir = create_dir(username);
        initialize_default_dir(logged_in_user->root_dir, username);

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
    // ANSI escape codes for colors
    const char *RED_COLOR = "\033[1;31m";
    const char *YELLOW_COLOR = "\033[1;33m";
    const char *RESET_COLOR = "\033[0m";

    printf("\n");
    printf("%s\t*************************************************\n", YELLOW_COLOR);
    printf("%s\t***********   WELCOME TO FILE MANAGER   *********\n", RED_COLOR);
    printf("%s\t*************************************************\n", YELLOW_COLOR);
    printf("\n");

    int choice;
    User logged_in_user;

    while (1)
    {
        printf("1. Register\n2. Login\n3. Exit\nChoose an option (1-3): ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            register_user();
        }
        else if (choice == 2)
        {
            if (login(&logged_in_user))
            {
                while (1)
                {
                    // After logging in, provide options for the logged-in user
                    printf("\nWelcome, %s!\n", logged_in_user.username);
                    printf("1. View Directory Structure\n2. Delete Account\n3. Logout\nChoose an option (1-3): ");
                    scanf("%d", &choice);

                    if (choice == 1)
                    {
                        // Print the user's initial directory structure
                        printf("\nInitial Directory Structure for %s:\n", logged_in_user.username);
                        print_dir_struct(logged_in_user.username, 0);

                        // Open the directory operations menu starting from the user's root directory
                        directory_operations_menu(logged_in_user.root_dir, logged_in_user.username);
                    }
                    else if (choice == 2)
                    {
                        // Delete the user from the database and their directory
                        delete_user();
                        printf("Exiting the file manager after deleting the user.\n");
                        return 0; // Exit the program after deleting the user
                    }
                    else if (choice == 3)
                    {
                        printf("Logging out...\n");
                        break; // Break out of the logged-in menu and return to the main menu
                    }
                    else
                    {
                        printf("Invalid option. Try again.\n");
                    }
                }
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
    // Reset color back to default
    printf("%s", RESET_COLOR);

    return 0;
}
