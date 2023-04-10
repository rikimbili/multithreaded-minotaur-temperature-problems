#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

constexpr int NUM_THREADS = 8;
constexpr int NUM_MINUTES = 60;
struct TempData {
  int time;
  int temperature;
};
std::mutex data_mutex;
std::vector<TempData> shared_temp_data;

int generate_random_temperature() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> distr(-100, 70);
  return distr(gen);
}

void sensor_thread(int thread_index) {
  for (int i = 0; i < NUM_MINUTES; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(
        100)); // Don't sleep for a full second to avoid long delays
    int temperature = generate_random_temperature();
    std::lock_guard<std::mutex> guard(data_mutex);
    shared_temp_data.push_back({i, temperature});
  }
}

void generate_report() {
  std::vector<TempData> sorted_data = shared_temp_data;
  std::sort(sorted_data.begin(), sorted_data.end(),
            [](const TempData &a, const TempData &b) {
              return a.temperature > b.temperature;
            });

  std::cout << "\nTop 5 highest temperatures:" << std::endl;

  for (int i = 0; i < 5 && i < sorted_data.size(); ++i) {
    std::cout << i + 1 << ": time " << sorted_data[i].time << " -> "
              << sorted_data[i].temperature << "F" << std::endl;
  }

  std::sort(sorted_data.begin(), sorted_data.end(),
            [](const TempData &a, const TempData &b) {
              return a.temperature < b.temperature;
            });

  std::cout << "\nTop 5 lowest temperatures:" << std::endl;

  for (int i = 0; i < 5 && i < sorted_data.size(); ++i) {
    std::cout << i + 1 << ": time " << sorted_data[i].time << " -> "
              << sorted_data[i].temperature << "F" << std::endl;
  }

  int start_interval = -1;
  int max_difference = 0;
  for (int i = 0; i < shared_temp_data.size() - 9; ++i) {
    int min_temperature = shared_temp_data[i].temperature;
    int max_temperature = shared_temp_data[i].temperature;
    for (int j = 1; j < 10; ++j) {
      min_temperature =
          std::min(min_temperature, shared_temp_data[i + j].temperature);
      max_temperature =
          std::max(max_temperature, shared_temp_data[i + j].temperature);
    }
    int difference = max_temperature - min_temperature;
    if (difference > max_difference) {
      max_difference = difference;
      start_interval = shared_temp_data[i].time;
    }
  }

  std::cout << "\n10-minute interval with largest temperature difference: "
            << start_interval << "-" << start_interval + 9 << std::endl;
}

int main() {
  std::vector<std::thread> sensor_threads;
  sensor_threads.reserve(NUM_THREADS);
  std::cout << "Starting " << NUM_THREADS << " sensor threads" << std::endl;
  for (int i = 0; i < NUM_THREADS; ++i) {
    sensor_threads.emplace_back(sensor_thread, i);
  }
  for (std::thread &t : sensor_threads) {
    t.join();
  }

  generate_report();

  return 0;
}
