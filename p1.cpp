#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

class ConcurrentLinkedList {
public:
  ConcurrentLinkedList() {}

  void addPresent(int tag) {
    std::lock_guard<std::mutex> lock(list_mutex);
    presents.push_back(tag);
    presents.sort();
    present_count++;
  }

  bool writeThankYouCard() {
    std::lock_guard<std::mutex> lock(list_mutex);
    if (presents.empty()) {
      return false;
    }

    int presentTag = presents.front();
    presents.pop_front();
    card_count++;
    return true;
  }

  int getPresentCount() const { return present_count.load(); }
  int getCardCount() const { return card_count.load(); }

private:
  std::list<int> presents;
  std::mutex list_mutex;
  std::atomic<int> present_count{0};
  std::atomic<int> card_count{0};
};

constexpr int num_servants = 4;
constexpr int num_presents = 500000;

void servantTask(ConcurrentLinkedList &list, int num) {
  int presentsProcessed = 0;

  while (presentsProcessed < num) {
    int presentTag = rand() % num_presents;
    list.addPresent(presentTag);
    list.writeThankYouCard();
    presentsProcessed++;
  }
}

int main() {
  ConcurrentLinkedList list;
  std::vector<std::thread> servants;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_servants; i++) {
    servants.emplace_back(servantTask, std::ref(list), num_presents);
  }

  for (auto &servant : servants) {
    servant.join();
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  std::cout << "All servants have finished after " << duration
            << " milliseconds" << std::endl;

  std::cout << "Total presents processed: " << list.getPresentCount()
            << std::endl;
  std::cout << "Total \"Thank you\" cards written: " << list.getCardCount()
            << std::endl;

  return 0;
}
