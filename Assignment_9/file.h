#ifndef __FILE_H__
#define __FILE_H__

#include <stdbool.h>


/*******************************************************************************
* API
******************************************************************************/

/*******************************************************************************
 * @brief Opens a file and checks if it exists. If not, creates a new file.
 * @param path Path of the file to open or create.
 * @return True if the file exists or is created successfully, otherwise false.
******************************************************************************/
bool openOrCreateFile(const char* path);

/*******************************************************************************
 * @brief Displays the content of the file.
 * @param path Path of the file to display.
******************************************************************************/
void displayFileContent(const char* path);

/*******************************************************************************
 * @brief Appends a string to the end of the file.
 * @param path Path of the file to append to.
 * @param str The string to append.
******************************************************************************/
void appendStringToFile(const char* path, const char* str);

/*******************************************************************************
 * @brief Searches for a string in the file and returns its coordinates.
 * @param path Path of the file to search in.
 * @param str The string to search for.
 * @return True if the string is found, otherwise false.
******************************************************************************/
bool findStringInFile(const char* path, const char* str);

#endif /* __FILE_H__ */ 
