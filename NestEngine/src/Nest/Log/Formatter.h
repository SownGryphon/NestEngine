#pragma once

#include <iomanip>
#include <string>
#include <sstream>
#include <regex>
#include <vector>

#include <geometry/VectorN.h>

namespace Nest
{
	class Formatter
	{
		/**
		 * @brief Regex to find format fields
		 * 
		 * Matches any substrings in one of the forms:
		 * {} - auto-allocated format field
		 * {[index]} - where [index] is the index of the format field
		 * {:[args]} - where [args] is additional format info
		 * {[index]:[args]}
		 * 
		 * Contains two additional capture groups:
		 * 1 - format field index, always present (matches a number)
		 * 2 - additional format specifiers (only sometimes present
		 */
		inline const static std::regex s_fmtRegex{ R"(\{(\d*)(?::([^}]*))?\})" };

		struct FormatField
		{
			size_t strIndex = 0, length = 0;
			unsigned int formatIndex = 0;
			std::string argSpecs;
			std::string formatResult;
		};

		std::vector<FormatField> m_formatFields;

	public:
		Formatter() {}

		/**
		 * @brief Formats a string with the given arguments
		 * 
		 * Format fields are in the form {} or {[index]:[args]}.
		 * The [index] must be an integer specifying the index of the argument to format the field with (or omitted entirely)
		 * The [args] field does nothing :) (for now)
		 * 
		 * @param fmtString String containing format fields to format
		 * @param args Arguments to use to format the string
		 * @return Formatted string
		 */
		template <typename ...Args>
		std::string format(std::string fmtString, Args ...args)
		{
			std::vector<size_t> bracketPositions;
			// Replace all escaped brackets with a temporary # to prevent the regex from matching them
			while (size_t i = fmtString.find("\\{") != std::string::npos)
			{
				fmtString.replace(i, 2, "#");
				bracketPositions.push_back(i);
			}

			// Format field index to auto-assign to unindexed fields
			unsigned int autoFmtIndex = 0;

			m_formatFields.clear();

			auto fmtBegin = std::sregex_iterator(fmtString.begin(), fmtString.end(), s_fmtRegex);
			auto fmtEnd = std::sregex_iterator();
			for (std::sregex_iterator i = fmtBegin; i != fmtEnd; ++i)
			{
				std::smatch match = *i;
				FormatField newField;
				
				// If the format field is explicitly indexed, use that, otherwise auto-assign one
				if (match.length(1))
				{
					newField.formatIndex = std::stoi(match.str(1));
				}
				else
				{
					newField.formatIndex = autoFmtIndex;
					++autoFmtIndex;
				}

				// Save the extra arguments. If they did not match, this should be blank
				newField.argSpecs = match.str(2);

				newField.strIndex = match.position(0);
				newField.length = match.length(0);

				m_formatFields.push_back(newField);
			}

			// Put back escaped brackets
			for (size_t i : bracketPositions)
				fmtString.replace(i, 1, "{");

			processArgs(fmtString, 0, args...);

			// Iterate over format fields in reverse, as this way the field position stays in place for all future fields
			for (auto i = m_formatFields.rbegin(); i != m_formatFields.rend(); ++i)
			{
				fmtString.replace(i->strIndex, i->length, i->formatResult);
			}

			return fmtString;
		}

	private:

		template <typename T, typename ...Args>
		void processArgs(std::string &string, unsigned int argIndex, T arg, Args ...args)
		{
			applyArg(string, argIndex, arg);
			processArgs(string, argIndex + 1, args...);
		}

		std::string processArgs(const std::string &string, unsigned int)
		{
			return string;
		}

		template <typename T>
		void applyArg(std::string &string, unsigned int argIndex, T arg)
		{
			for (FormatField &f : m_formatFields)
			{
				if (f.formatIndex == argIndex)
				{
					f.formatResult = toString(arg, f.argSpecs);
				}
			}
		}

		template <typename T>
		std::string toString(T arg, const std::string &specs)
		{
			if (specs.empty())
				return std::to_string(arg);

			std::ostringstream fStream;

			std::regex precisionRegex{ R"(\.(\d+))" };
			std::smatch precisionRes;
			if (std::regex_search(specs, precisionRes, precisionRegex))
			{
				fStream << std::fixed << std::setprecision(std::stoi(precisionRes.str(1)));
			}

			fStream << arg;

			return fStream.str();
		}

		template <>
		std::string toString(std::string string, const std::string &specs)
		{
			return string;
		}

		template <>
		std::string toString(bool val, const std::string &specs)
		{
			return val ? "true" : "false";
		}

		template <>
		std::string toString(const char *chars, const std::string &specs)
		{
			return std::string(chars);
		}

		template <unsigned int size, typename T>
		std::string toString(chcl::VectorN<size, T> vec, const std::string &specs)
		{
			std::string out = "[";
			for (unsigned int i = 0; i < size; ++i)
			{
				if (i)
					out += ", ";

				out += toString(vec[i], specs);
			}
			out += "]";
			return out;
		}
	};
}