#pragma once

#ifdef __linux__
bool is_directory(const std::string& filename);
#endif

std::vector<std::string> file_list(const std::string& dirname, std::vector<std::string>& files);
