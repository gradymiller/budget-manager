#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "cli/cmdTemplate.hpp"
#include "core/database.hpp"
#include "core/path.hpp"
#include "core/utils.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string progressBar(double percent, int width = 25) {
    percent = std::clamp(percent, 0.0, 100.0);

    int filled = static_cast<int>(
        percent / 100.0 * width
    );

    std::string bar;

    for (int i = 0; i < filled; ++i) {
        bar += "█";
    }

    for (int i = filled; i < width; ++i) {
        bar += "░";
    }

    return bar;
}

int cmdStatus() {
	return runCommand([&]() {
		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

		auto name = budget.getName();
		auto start = budget.getStartDate();
		auto end = budget.getEndDate();
		auto now = std::chrono::system_clock::now();
		auto limit = budget.getLimit();
		auto usage = budget.getUsage();

		json data = {
			{"name", name},
			{"start_date", dateToStr(start)},
			{"end_date", dateToStr(end)},
			{"limit", limit},
			{"usage", usage}
		};


		fs::create_directories(PATH / "reports");

		// Open the output file
		std::ofstream file(PATH / "reports/status.json");

		if (!file) {
			throw std::runtime_error("Could not open status.json");
		}

		file << data.dump(4);

		double time_percent =
			std::chrono::duration<double>(now - start).count() /
			std::chrono::duration<double>(end - start).count() * 100.0;

		double usage_percent = (usage / limit) * 100.0;

		std::cout << std::fixed << std::setprecision(2);
		std::cout << "Budget: " << name << '\n';
		std::cout << "Period: " 
				  << dateToStr(start) 
				  << " — " << dateToStr(end) << "\n\n";

		std::cout << "Limit:      $" << limit << '\n';
		std::cout << "Used:       $" << usage << '\n';
		std::cout << "Remaining:  $" << limit - usage << "\n\n";
		
		std::cout << "Time Progress: " 
				  << progressBar(time_percent) 
				  << " " << time_percent << "%\n\n";

		std::cout << "Budget Usage: " 
				  << progressBar(usage_percent)	
				  << " " << usage_percent << "%\n\n";
	});
}

int cmdReport() {
	return runCommand([&]() {
		Database db(PATH / "budget-data.db");
		Budget budget = db.loadBudget();

		json data = json::array();

		for (const auto& [id, category] : budget.getCategories()) {
			auto name = category.getName();
			auto limit = category.getLimit();
			auto usage = category.getUsage();

			
			double usage_percent = 0.0;
			if (limit != 0) {
				usage_percent = (usage / limit) * 100.0;
			}
			
			data.push_back({
				{"name", name},
				{"limit", limit},
				{"usage", usage}
			});	

			std::cout << std::fixed << std::setprecision(2);
			std::cout << "=== Categories ===\n\n";
			std::cout << name << '\n';
			std::cout << "Limit: " << limit << '\n';
			std::cout << "Usage: " << usage << '\n';
			std::cout << progressBar(usage_percent) << " " << usage_percent <<"%\n\n";
		}

		fs::create_directories(PATH / "reports");

		// Open the output file
		std::ofstream file(PATH / "reports/report.json");

		if (!file) {
			throw std::runtime_error("Could not open report.json");
		}

		file << data.dump(4);
	});
}
