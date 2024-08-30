#include <stdio.h>
#include <stdlib.h>
void createFile(const char *filename)
{                                      // Passing the filename as string
    FILE *file = fopen(filename, "w"); // This line will try to open the file with write access
    // if file will not exist it will create a new file
    if (file == NULL)
    {
        printf("Error while creating file: %s\n", filename);
        return;
    }
    printf("File created: %s\n", filename);
    fclose(file); // Closes the file after creation.
    void writeFile(const char *filename, const char *content)
    {
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            printf("Erro While writing to the file: %s\n ", filename);
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
    int main()
    {
        const char *filename = "example.txt";
        createFile(filename);
        writeFile(filename, "Hello , This project is by Inder Goswami");
        readFile(filename);
        deleteFile(filename);
        return 0;
    }
}
