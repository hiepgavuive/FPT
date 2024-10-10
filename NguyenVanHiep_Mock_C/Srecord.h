
#ifndef __SRECORD_H__
#define __SRECORD_H__


/*******************************************************************************
* Definitions
******************************************************************************/
#define MAX 255U
#define MAX_SIZE_S5 65535U
#define MAX_SIZE_S6 1677215U
#define MAX_LINE_LENGTH 1024

/* All possible error codes the S - Record checking functions may return */ 
enum SRecordErrors
{
	NOT_ERROR = 0,
	ERROR_S = -1,
	ERROR_TYPE = -2,
	ERROR_HEX = -3,
	ERROR_BYTE_COUNT = -4,
	ERROR_SUM = -5,
	ERROR_LINE_COUNT = -6,
	ERROR_TERMINATE = -7
};

/*******************************************************************************
* API
* ******************************************************************************/

/*******************************************************************************
 * @brief Reads a line of text from a file.
 *
 * This function reads characters from the given file until a newline character,
 * end-of-file (EOF), or the maximum line length is reached. The read characters
 * are stored in the provided line buffer and the length of the line is updated.
 * The line is null-terminated. If EOF is reached before any characters are read,
 * the length is set to 0.
 *
 * @param line A pointer to a buffer where the read line will be stored.
 * @param file A pointer to the file from which to read the line.
 * @param length A pointer to a variable where the length of the read line will be stored.
******************************************************************************/
void readLine(int8_t* line, FILE* file, uint8_t* pos);

/*******************************************************************************
 * @brief Checks if the provided character is 'S', indicating the start of an S-record.
 *
 * This function verifies that the first character of an S-record line is 'S',
 * which is a standard indicator for the beginning of an S-record line in Motorola
 * S-record format files. If the character is not 'S', it sets an error flag.
 *
 * @param temp The character to check, typically the first character of a line.
 * @return An enum value representing either `NOT_ERROR` if the character is 'S',
 * or `ERROR_S` if the character is not 'S'.
******************************************************************************/
enum SRecordErrors checkS(uint8_t temp);

/*******************************************************************************
 * @brief Checks if the provided type character is a valid S-record type.
 *
 * The function verifies that the S-record type character falls within the
 * valid range of '0' to '9'. These characters correspond to the allowable
 * S-record types in Motorola S-record files (S0, S1, S2, ..., S9).
 *
 * @param type The type character from the S-record line.
 * @return An enum value representing either `NOT_ERROR` if the type is valid,
 * or `ERROR_TYPE` if the type is invalid.
******************************************************************************/
enum SRecordErrors checkType(uint8_t temp);

/*******************************************************************************
 * @brief Validates that all characters in the S-record line are valid hexadecimal digits.
 *
 * This function iterates through the characters in the provided S-record line,
 * starting from the second character, and checks if each character represents
 * a valid hexadecimal digit (0-9, A-F). The `hexToDec` function is used to
 * convert a character to its decimal equivalent; if the result is not in the
 * range 0-15, the character is not a valid hexadecimal digit.
 *
 * @param temp The S-record line as a string.
 * @return An enum value representing either `NOT_ERROR` if all characters are
 * valid hexadecimal digits, or `ERROR_HEX` if any invalid character is found.
******************************************************************************/
enum SRecordErrors checkHex(int8_t* temp);

/*******************************************************************************
 * @brief Checks if the byte count specified in the S-record line matches the actual data length.
 *
 * This function calculates the expected number of bytes in the S-record line
 * by converting the first two characters after the 'S' and type character to an
 * integer. It then compares this byte count with the actual number of data
 * bytes present in the record, calculated by taking the length of the string
 * minus the non-data characters (initial 'S', type, and checksum).
 *
 * @param str The S-record line as a string.
 * @return An enum value representing either `NOT_ERROR` if the byte count is
 * correct, or `ERROR_BYTE_COUNT` if there is a discrepancy.
******************************************************************************/
enum SRecordErrors checkByteCount(int8_t* str);

/*******************************************************************************
 * @brief Verifies the checksum of the S-record line.
 *
 * This function calculates the checksum for an S-record line by summing the
 * hexadecimal values of all bytes in the record, excluding the initial 'S'
 * character and the checksum itself. It then compares the calculated checksum
 * with the checksum present in the record (the last byte), ensuring they match.
 *
 * @param str The S-record line as a string.
 * @return An enum value representing either `NOT_ERROR` if the checksums match,
 * or `ERROR_SUM` if they do not match.
******************************************************************************/
enum SRecordErrors checkSum(int8_t* str);

/*******************************************************************************
 * @brief Verifies if the line count in the S-record matches the expected count.
 *
 * This function checks the line count encoded in the S-record against the actual
 * count of data lines processed. It extracts the line count from the S-record
 * (located at positions 4 to 7), converts it from hexadecimal to decimal, and
 * compares it with the `count` parameter. If the counts do not match, it returns
 * an error.
 *
 * @param str The S-record line as a string.
 * @param count The expected line count.
 * @return An enum value representing either `NOT_ERROR` if the line count matches,
 * or `ERROR_LINE_COUNT` if it does not match.
******************************************************************************/
enum SRecordErrors checkLineCount(int8_t* str, uint16_t count);

/*******************************************************************************
 * @brief Checks if the S-record line has the correct termination type.
 *
 * This function compares the termination type specified in the S-record line with
 * the expected termination type for the record type. If the termination type does
 * not match the expected type, it returns an error indicating an incorrect
 * termination type.
 *
 * @param temp The termination type found in the S-record line.
 * @param type The expected termination type for the record type.
 * @return An enum value representing either `NOT_ERROR` if the termination type is
 * correct, or `ERROR_TERMINATE` if it is incorrect.
******************************************************************************/
enum SRecordErrors checkTerminate(int8_t temp, uint8_t type);

/*******************************************************************************
 * @brief Prints data from an S-record line to the output file and console.
 *
 * This function processes an S-record line, extracting the relevant data portion
 * based on the record type (S1, S2, or S3). It prints the data to the output file
 * and console, formatting it with spaces at appropriate positions depending on the
 * address length associated with the record type.
 *
 * @param str Pointer to the S-record line.
 * @param length Length of the S-record line.
 * @param file Output file pointer where the data will be written.
 * @param type The type of the S-record (e.g., '1' for S1, '2' for S2, '3' for S3).
******************************************************************************/
void printData(int8_t* str, uint8_t length, FILE* file, int8_t type);

#endif /* __SRECORD_H__ */

/*******************************************************************************
* End of file
******************************************************************************/