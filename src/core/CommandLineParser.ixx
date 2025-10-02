module;

#include <boost/program_options.hpp>
#include <optional>
#include <span>
#include <string>
#include <vector>

export module druid.core.commandlineparser;

export namespace druid::core
{
	namespace po = boost::program_options;

	class CommandLineParser
	{
	public:
		explicit CommandLineParser(std::string program_name, std::optional<std::string> program_description)
			: options_description_("Options"), program_name_(std::move(program_name)), program_description_(std::move(program_description))
		{
			// should always have help option
			options_description_.add_options()("help,h", "Display this help message");
		}

		auto add_flag(const std::string& name, const std::string& description) -> CommandLineParser&
		{
			options_description_.add_options()(name.c_str(), description.c_str());
			return *this;
		}

		template <typename T>
		auto add_option(const std::string& name, const std::string& description) -> CommandLineParser&
		{
			options_description_.add_options()(name.c_str(), po::value<T>(), description.c_str());
			return *this;
		}

		template <typename T>
		auto add_option(const std::string& name, const T& default_value, const std::string& description) -> CommandLineParser&
		{
			options_description_.add_options()(name.c_str(), po::value<T>()->default_value(default_value), description.c_str());
			return *this;
		}

		auto add_positional(const std::string& name, int max_count = -1) -> CommandLineParser&
		{
			positional_description_.add(name.c_str(), max_count);
			positional_names_.emplace_back(name);
			return *this;
		}

		auto set_parsed(bool parsed) -> void
		{
			if(parsed_ != parsed)
			{
				parsed_ = parsed;
			}
		}

		[[nodiscard]] auto get_parsed() const -> bool
		{
			return parsed_;
		}

		auto set_error_message(const std::string& error_message) -> void
		{
			if(error_message_ != error_message)
			{
				error_message_ = error_message;
			}
		}

		[[nodiscard]] auto get_error_message() const -> std::string
		{
			return error_message_;
		}


		[[nodiscard]] auto parse(int argc, std::span<char*> argv) -> bool
		{
			try
			{
				po::store(po::command_line_parser(argc, argv.data()).options(options_description_).positional(positional_description_).run(),
						  variables_map_);
				po::notify(variables_map_);

				if(variables_map_.contains("help") == true)
				{
					// this->printHelp();
					return this->get_parsed();
				}

				this->set_parsed(true);
				return this->get_parsed();
			}
			catch(const po::error& e)
			{
				this->set_error_message(e.what());
				return this->get_parsed();
			}
		}

	private:
		po::options_description options_description_;
		po::variables_map variables_map_;
		po::positional_options_description positional_description_;
		std::vector<std::string> positional_names_;
		std::string program_name_;
		std::optional<std::string> program_description_;
		std::string error_message_;
		bool parsed_{false};
	};
}