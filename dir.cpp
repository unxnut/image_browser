#include <iostream>
#include <vector>
#include <string>
#include <string.h>

#ifdef _WIN32
#include "dirent.h"		// Use locally-supplied header file in Windows
#else
#if defined(__linux__) || defined(__APPLE__)
#include <dirent.h>		// Use system header file in Linux and Apple
#endif
#endif

#include "dir.hpp"

#if defined(__linux__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/******************************************************************************
 * \brief Find out if the specified file name refers to a directory
 *
 * In Linux and Apple, the comparison of file attribute to DT_DIR depends on
 * the underlying file system and is not guaranteed to work on all file
 * systems.  Therefore, we use the bulletproof function lstat to determine if
 * the file name refers to a directory.
 *
 * @param [in] filename refers to the name of file to check if it's a directory
 * \return true if the filename refers to a directory, false otherwise.
 *****************************************************************************/

bool is_directory ( const std::string& filename )
{
    struct stat buf;
    lstat ( filename.c_str(), &buf );
    return ( S_ISDIR ( buf.st_mode ) );
}
#endif

/******************************************************************************
 * \brief Create a list of files in directory
 *
 * Given a directory, scan it in depth-first order and produce a list of files
 * contained therein.  Traverse any subdirectory but do not save the name of
 * subdirectory.  All the files are relative to current working directory.
 * Throws an exception if directory cannot be opened.
 *
 * @param [in] dirname name of the directory
 * @param [in,out] files list of files
 * \return list of files as a vector of strings
 * \sa is_directory()
 *****************************************************************************/

std::vector<std::string> file_list(const std::string& dirname, std::vector<std::string>& files)
{
	DIR* dir;				//!< dir Variable to hold opened directory
	struct dirent* ent;		//!< ent Variable to hold an entry in directory

	// Open directory

	if ((dir = opendir(dirname.c_str())) == NULL)
	{
		throw (std::string("Unknown directory ") + dirname);
	}

	// Process each entry in the directory

	while ((ent = readdir(dir)) != NULL)
	{
		// Ignore directories . and ..

		if ( ! ( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) )
			continue;

		//!< file_name contains the complete path name of the file entry relative to
		//             current working directory; uses \ or / depending on Windows or
		//             Linux/Apple

#ifdef _WIN32
		std::string file_name = dirname + std::string("\\") + std::string(ent->d_name);
#else
		std::string file_name = dirname + std::string("/") + std::string(ent->d_name);
#endif

		// Make a recursive call in case of directory; otherwise, append file_name
		// to list of files

#ifdef _WIN32
		if (ent->d_type == DT_DIR)
#elif defined(__linux__) || defined(__APPLE__)
		if ( is_directory ( file_name ) )
#endif
		{
			files = file_list(file_name, files);
		}
		else
		{
			files.push_back(file_name);
		}
	}

	return (files);
}


