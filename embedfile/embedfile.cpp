//
// @file:   embedfile.cpp
// @author: FirePrincess
// @date:   2022-01-02
// @brief:  Converts a binary file into an array
//

#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <bit>

inline std::vector<char> read_file(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t const fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}


namespace fs = std::filesystem;

using namespace std;

int main(int const argc, char const * argv[])
{
	if (argc <= 3)
	{
		cout << "Usage: [VariableName] [InputFileName] [OutputFileName]" << endl;
		return 1;
	}

	try {
        std::string const variable_name = argv[1];
        std::string const input_file_name = argv[2];
        std::string const output_file_name = argv[3];

		auto const file = read_file(input_file_name);

		fs::path p = output_file_name;
		string filename = p.filename().string();

		ofstream out(output_file_name);


		if (!out.is_open()) {
			throw std::runtime_error("failed to create file!");
		}

		// calculate time
		// auto const now = std::chrono::system_clock::now();
		// std::time_t const now_time = std::chrono::system_clock::to_time_t(now);

		out << "///" << endl;
		out << "/// @file: " << filename << endl;
		std::time_t result = std::time(nullptr);
		char str[128];
		ctime_s(str, sizeof(str), &result);
		out << "/// @date: " << str;
		out << "/// @author: embedfile" << endl;
		out << "/// @brief: " << input_file_name << endl;
		out << "///" << endl;
		out << endl;
		out << "#pragma once" << endl;
		out << endl;
		out << "#include <array>" << endl;
		out << "#include <cstdint>" << endl;
		out << endl;
		out << "constexpr std::array<char, " << file.size() << "> " << variable_name << " = { " << endl;

		size_t i = 0;
		while(i < file.size())
		{
			//out << "  ";
			for (size_t j = 0; j < 16 && i+j < file.size(); ++j)
			{
				out << setw(4) << static_cast<int>(file[i+j]);
				if (i+j < file.size() - 1)
				{
					out << ",";
				}
			}

			out << " //";

			for (size_t j = 0; j < 16 && i+j < file.size(); ++j)
			{
				char const sign = file[i+j];
				if(sign >= ' ' && sign <= 'z' && sign != '\\') {
					out << " " << sign;
				}
				else {
					out << " .";
				}
			}

			out << endl;
			i += 16;
		}

		out << "};" << endl;

		out.close();
	}
	catch (exception const& e)
	{
		cout << "Exception: " << e.what() << endl;
		return 1;
	}

	return 0;
}







