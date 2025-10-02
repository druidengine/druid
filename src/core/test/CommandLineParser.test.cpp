#include <gtest/gtest.h>
#include <string>
#include <vector>

import druid.core.commandlineparser;

namespace dc = druid::core;

namespace
{
	class ArgvBuilder
	{
	public:
		auto add(const std::string& arg) -> ArgvBuilder&
		{
			args_.emplace_back(std::move(arg));
			return *this;
		}

		auto argc() const -> int
		{
			return static_cast<int>(args_.size());
		}

		auto argv() -> char**
		{
			ptrs_.clear();
			for(auto& arg : args_)
			{
				ptrs_.emplace_back(arg.data());
			}
			return ptrs_.data();
		}

	private:
		std::vector<std::string> args_;
		std::vector<char*> ptrs_;
	};
}

TEST(CommandLineParser, constructorNoDescription)
{
	const dc::CommandLineParser parser("test", std::nullopt);
}

TEST(CommandLineParser, constructorWithDescription)
{
	const dc::CommandLineParser parser("test", "test application");
}

// TEST(CommandLineParser, flagsAreFalseByDefault)
// {
// 	dc::CommandLineParser parser("test", std::nullopt);
// 	parser.addFlag("verbose,v", "Verbose Output").addFlag("debug,d", "Debug mode");
//
// 	ArgvBuilder args;
// 	args.add("test");
// }