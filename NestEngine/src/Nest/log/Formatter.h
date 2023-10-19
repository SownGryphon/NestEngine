#pragma once

#include <string>

#include <geometry/VectorN.h>

namespace Nest
{
	class Formatter
	{
		unsigned int formatIndex = 0;

	public:
		Formatter() {}

		std::string format(const std::string &string)
		{
			return string;
		}

		template <typename T>
		void format(std::string &string, T arg)
		{
			std::string argString = toString(arg);
			size_t genericFmt = string.find("{}", 0);
			if (genericFmt != std::string::npos)
			{
				string.erase(genericFmt, 2);
				string.insert(genericFmt, argString);
			}

			std::string explicitToken = "{" + std::to_string(formatIndex) + "}";
			size_t explicitIndex = string.find(explicitToken);
			while (explicitIndex != std::string::npos)
			{
				string.erase(explicitIndex, explicitToken.length());
				string.insert(explicitIndex, argString);
				explicitIndex = string.find(explicitToken, explicitIndex);
			}
		}

		template <typename T, typename ...Args>
		void format(std::string &string, T arg, Args... args)
		{
			format(string, arg);
			++formatIndex;
			format(string, args...);
			--formatIndex;
		}

		template <typename ...Args>
		std::string format(const std::string &string, Args... args)
		{
			std::string result = string;
			format(result, args...);
			return result;
		}

	private:
		template <typename T>
		std::string toString(T arg)
		{
			return std::to_string(arg);
		}

		template <>
		std::string toString(std::string string)
		{
			return string;
		}

		template <unsigned int size, typename T>
		std::string toString(chcl::VectorN<size, T> vec)
		{
			std::string out = "[";
			for (unsigned int i = 0; i < size; ++i)
			{
				if (i)
					out += ", ";

				out += toString(vec[i]);
			}
			out += "]";
			return out;
		}
	};
}