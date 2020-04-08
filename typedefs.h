#pragma once
#include "buVariant.h"

#include <string>
#include <vector>

using ScpiArg = buVariant<std::string, unsigned long long, signed long long, double, std::vector<unsigned long long>>;

struct ParsedScpiData {
	std::vector<std::string> nodesNames;
	std::vector<ScpiArg> args;
};
// QUERY ERRORS [-499,400]
const int NO_ERROR = 0;
// COMMAND ERRORS [-199,-100]
const int ERR_100_COMMAND_ERROR = -100;
const int ERR_100_INVALID_CHAR = -101;
const int ERR_100_SYNTAX_ERROR = -102;
const int ERR_100_INV_SEPARATOR = -103;
const int ERR_100_DATA_TYPE = -104;
const int ERR_100_GET_NOT_ALLOWED = -105;
const int ERR_100_PARAM_NOT_ALLOWED = -108;
const int ERR_100_MISSING_PARAM = -109;
const int ERR_100_NUM_DATA_ERR = -120;

// EXECUTION ERROR [-299,-200]
const int ERR_200_PARAM_ERR = -220;
const int ERR_200_INIT_IGNORED = -213;
const int ERR_200_SETTINGS_CONFLICT = -221;
const int ERR_200_TOO_MUCH_DATA = -223;
const int ERR_200_ILLEGAL_PAR_VALUE = -224;
const int ERR_200_OUT_OF_MEMORY = -225;
const int ERR_200_DATA_QUESTIONABLE = -231;
const int ERR_200_INVALID_FORMAT = -232;
const int ERR_200_HARDWARE_ERR = -240;
const int ERR_200_HARDWARE_MISSING = -241;
const int ERR_200_STORAGE_ERR = -250;
const int ERR_200_FILE_NOT_FOUND = -251;
const int ERR_200_FILE_NAME_ERR = -257;

// DEVICE SPECIFIC ERRORS [-399,-300]
const int ERR_300_DEVICE_SPEC = -300;
const int ERR_300_SYSTEM_ERROR = -310;
const int ERR_300_OUT_OF_MEMORY = -321;
const int ERR_300_SELF_TEST_FAILED = -330;
const int ERR_300_QUEUE_OVERFLOW = -350;

// QUERY ERRORS [-499,400]
const int ERR_400_QUERY_ERROR = -400;

// ProDAQ 6716 SPECIFIC ERRORS
const int BU6716_ERR_SCU_ERROR = 100;

static const struct
{
	int code;
	const char* msg;
} errorCodes[] =
{
	{ERR_100_COMMAND_ERROR,		"Command Error"				},
	{ERR_100_INVALID_CHAR,		"Invalid Character"			},
	{ERR_100_SYNTAX_ERROR,		"Syntax Error"				},
	{ERR_100_DATA_TYPE,			"Data type error"			},
	{ERR_100_PARAM_NOT_ALLOWED,	"Param not allowed"			},
	{ERR_100_MISSING_PARAM,		"Missing parameter"			},
	{ERR_100_NUM_DATA_ERR,		"Numeric data error"		},

	{ERR_200_OUT_OF_MEMORY,		"Out of memory"				},
	{ERR_200_HARDWARE_ERR,		"Hardware error"			},
	{ERR_200_HARDWARE_MISSING,	"Hardware missing"			},
	{ERR_200_STORAGE_ERR,		"Mass storage error"		},
	{ERR_200_FILE_NOT_FOUND,	"File name not found"		},
	{ERR_200_INIT_IGNORED,		"Initialize ignored"		},
	{ERR_200_SETTINGS_CONFLICT,	"Settings conflict"			},
	{ERR_200_PARAM_ERR,			"Unknown parameter error"	},
	{ERR_200_TOO_MUCH_DATA,		"Too much data"				},
	{ERR_200_ILLEGAL_PAR_VALUE,	"Illegal parameter value"	},
	{ERR_200_DATA_QUESTIONABLE,	"Data questionable"			},
	{ERR_200_INVALID_FORMAT,	"Invalid format"			},
	{ERR_200_FILE_NAME_ERR,		"File name error"			},

	{ERR_300_DEVICE_SPEC,		"Device specific error"		},
	{ERR_300_SYSTEM_ERROR,		"System error"				},
	{ERR_300_OUT_OF_MEMORY,		"Out of memory"				},
	{ERR_300_SELF_TEST_FAILED,	"Self-test failed"			},
	{ERR_300_QUEUE_OVERFLOW,	"Queue overflow"			},

	{ERR_400_QUERY_ERROR,		"Query error"				},

	{NO_ERROR,					"No Error"					},
};