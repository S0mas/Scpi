#pragma once

#include "typedefs.h"
#include <functional>
#include <vector>
#include <string>
#include <iostream>

template<typename T>
void addType(std::vector<size_t>& types) {
	types.push_back(typeid(T).hash_code());
}

template<typename T = void, typename... Rest>
void addTypes(std::vector<size_t>& types) {
	addType<T>(types);
	addTypes<Rest...>(types);
}

template<>
inline void addTypes<>(std::vector<size_t>& types) {}

class ScpiFunction {
	bool success;
	int error;
	std::function<int(const std::vector<ScpiArg>&)> function;
	std::vector<size_t> argsTypes;

	bool checkArgsTypes(const std::vector<ScpiArg>& args) const {
		for (auto i = 0u; i < args.size(); ++i)
			if (args[i].getTypeId() != argsTypes[i]) {
				//ERROR HANDLE
				return false;
			}
		return true;
	}

	ScpiFunction() {};
public:
	template <typename... Types>
	static ScpiFunction getScpiFunction(const std::function<int(const std::vector<ScpiArg>&)>& function = nullptr) {
		ScpiFunction sf;
		sf.function = function;
		addTypes<Types...>(sf.argsTypes);
		return sf;
	}
	
	bool wasSuccessful() const {
		return success;
	}

	bool errorType() const {
		return error;
	}

	bool isRunable() const {
		return function != nullptr;
	}

	int run(const std::vector<ScpiArg>& args) {
		if (args.size() > argsTypes.size())
			return ERR_100_PARAM_NOT_ALLOWED;
		else if (args.size() < argsTypes.size())
			return ERR_100_MISSING_PARAM;

		if (checkArgsTypes(args))
			return function(args);
		else
			return ERR_100_DATA_TYPE;
	}
};
