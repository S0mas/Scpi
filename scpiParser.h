#pragma once
#include "buVariant.h"
#include "typedefs.h"

#include <string>
#include <map>
#include <vector>

class ScpiArgsParser {
	struct ScpiArgsRecognizer {
		static inline std::map<std::string, ScpiArg> mnemonics;	
		static bool isHexFormat(const std::string& str) noexcept;
		static bool isOctFormat(const std::string& str) noexcept;
		static bool isBinFormat(const std::string& str) noexcept;
		static bool isUnsignedLL(const std::string& str) noexcept;
		static bool isSignedLL(const std::string& str) noexcept;
		static bool isString(const std::string& str) noexcept;
		static bool isList(const std::string& str) noexcept;
		static bool isMnemonic(const std::string& str) noexcept;
	};
	static ScpiArg parseHex(const std::string& str);
	static ScpiArg parseOct(const std::string& str);
	static ScpiArg parseBin(const std::string& str);
	static ScpiArg parseSignedLL(const std::string& str);
	static ScpiArg parseDouble(const std::string& str);
	static ScpiArg parseString(const std::string& str);
	static ScpiArg parseList(const std::string& str);
	static ScpiArg parseMnemonic(const std::string& str);
public:
	static void addMnemonic(std::string const& str, ScpiArg const& mappedArg) noexcept;
	static ScpiArg parseArg(const std::string& str);
	class EmptyArgDataException : public std::exception {};
	class InvalidArgDataException : public std::exception {};
};

class ScpiParser {
	static std::vector<ScpiArg> parseArgs(const std::vector<std::string>& args);
	static std::string getCmdPart(const std::string& data);
	static std::string getArgsPart(const std::string& data);
	static std::string removeLeadingColon(const std::string& data);
	static std::vector<unsigned long long> getInCmdNameNumberParams(const std::vector<std::string>& cmds);
	static ScpiArg parseArgsFromCmdNames(const std::vector<unsigned long long>& inNameParams);
public:
	static ParsedScpiData parse(const std::string& data);
};

