#pragma once
#include <iostream>
#include <string>
#include <vector>

class ScpiArgVisitor {
public:
	void operator()(const std::string& var) const noexcept { std::cout << var << " "; }
	void operator()(const unsigned long long& var) const noexcept { std::cout << var << " "; }
	void operator()(const signed long long& var) const noexcept { std::cout << var << " "; }
	void operator()(const double& var) const noexcept { std::cout << var << " "; }
	void operator()(const std::vector<unsigned long long>& var) const noexcept { 
		for(auto const& e : var)
			std::cout << e << " "; 
	}
};