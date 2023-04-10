# Multithreaded Minotaur and Temperature Problems

###### Coded by Raciel Antela Pardo for COP4520

## Problem 1: The Birthday Presents Party 

The Minotaur’s birthday party was a success. The Minotaur received a lot of presents
from his guests. The next day he decided to sort all of his presents and start writing
“Thank you” cards. Every present had a tag with a unique number that was associated
with the guest who gave it. Initially all of the presents were thrown into a large bag with
no particular order. The Minotaur wanted to take the presents from this unordered bag
and create a chain of presents hooked to each other with special links (similar to storing
elements in a linked-list). In this chain (linked-list) all of the presents had to be ordered
according to their tag numbers in increasing order. The Minotaur asked 4 of his servants
to help him with creating the chain of presents and writing the cards to his guests. Each
servant would do one of three actions in no particular order: 1. Take a present from the
unordered bag and add it to the chain in the correct location by hooking it to the
predecessor’s link. The servant also had to make sure that the newly added present is
also linked with the next present in the chain. 2. Write a “Thank you” card to a guest and
remove the present from the chain. To do so, a servant had to unlink the gift from its
predecessor and make sure to connect the predecessor’s link with the next gift in the
chain. 3. Per the Minotaur’s request, check whether a gift with a particular tag was
present in the chain or not; without adding or removing a new gift, a servant would scan
through the chain and check whether a gift with a particular tag is already added to the
ordered chain of gifts or not. As the Minotaur was impatient to get this task done
quickly, he instructed his servants not to wait until all of the presents from the
unordered bag are placed in the chain of linked and ordered presents. Instead, every
servant was asked to alternate adding gifts to the ordered chain and writing “Thank you”
cards. The servants were asked not to stop or even take a break until the task of writing
cards to all of the Minotaur’s guests was complete. After spending an entire day on this
task the bag of unordered presents and the chain of ordered presents were both finally
empty! Unfortunately, the servants realized at the end of the day that they had more
presents than “Thank you” notes. What could have gone wrong? Can we help the
Minotaur and his servants improve their strategy for writing “Thank you” notes? Design
and implement a concurrent linked-list that can help the Minotaur’s 4 servants with this
task. In your test, simulate this concurrent “Thank you” card writing scenario by
dedicating 1 thread per servant and assuming that the Minotaur received 500,000
presents from his guests.

### Approach and Proof of Correctness

The main problem in the original approach was creating unpredictability (race conditions) while adding gifts, removing gifts, and writing "Thank you" cards without waiting for the bag of unordered presents to be empty. This caused inconsistency between the number of presents and the number of "Thank you" cards written.

The new approach introduces a concurrent linked list to ensure a proper sequence of add and remove gift operations, which helps maintain consistency between presents and cards written. We use the mutex lock to ensure that no two servants access the list simultaneous, synchronizing their operations on the list. This enforces orderly access to shared data, preventing race conditions.

### Experimental Evaluation and Results

The output should indicate that all the servants have finished their tasks in a certain duration of time (measured in milliseconds), the total number of presents processed, and the total number of "Thank you" cards written.

Sample output for 500,000 presents and 4 servants:
```
All servants have finished after 6295 milliseconds
Total presents processed: 2000000
Total "Thank you" cards written: 2000000
```
The running time will vary between different systems, but the key takeaway is that the implementation achieves consistency between the number of presents processed and the number of "Thank you" cards written. This code might not be the most optimized, and the running time may be further improved by using more efficient data structures and parallel approaches.

### How to run

1. `cd` to this project folder
2. Run `g++ -std=c++20 -pthread p1.cpp && ./a.out` in the terminal
3. That's it!

## Problem 2: Atmospheric Temperature Reading Module 

You are tasked with the design of the module responsible for measuring the atmospheric
temperature of the next generation Mars Rover, equipped with a multicore CPU and 8
temperature sensors. The sensors are responsible for collecting temperature readings at
regular intervals and storing them in shared memory space. The atmospheric
temperature module has to compile a report at the end of every hour, comprising the top
5 highest temperatures recorded for that hour, the top 5 lowest temperatures recorded
for that hour, and the 10-minute interval of time when the largest temperature
difference was observed. The data storage and retrieval of the shared memory region
must be carefully handled, as we do not want to delay a sensor and miss the interval of
time when it is supposed to conduct temperature reading. Design and implement a
solution using 8 threads that will offer a solution for this task. Assume that the
temperature readings are taken every 1 minute. In your solution, simulate the operation
of the temperature reading sensor by generating a random number from -100F to 70F at
every reading. In your report, discuss the efficiency, correctness, and progress guarantee
of your program.

### Approach and Proof of Correctness

The implemented solution uses 8 sensor threads for collecting temperature data at regular intervals (every minute for a total of 60 minutes). These temperature readings are stored in a shared_temp_data vector, which is a shared memory space. To ensure thread-safe access to this shared memory, the data_mutex is used to make sure that only one thread writes to it at a time.

Temperature readings are simulated using a random number generator function generate_random_temperature() that generates a random number between -100°F and 70°F, representing the temperature readings.

After all the sensor threads have completed their operation, the generate_report() function is called to process the data and output the needed report. The shared_temperature_data vector is sorted in descending order and then the top 5 highest temperatures are displayed with their respective time. Next, the sorted data is sorted in ascending order to display the top 5 lowest temperatures. Lastly, a loop is used to find the 10-minute interval with the largest temperature difference.

The efficiency of the program is mainly determined by the work performed within each thread and the sorting operations. Overall, the performance is largely maintained due to each thread being able to execute concurrently, ensuring real-time temperature data collection.

The use of std::lock_guard to lock the shared memory ensures that only one thread can write to the shared memory at a time, preventing any race conditions. The use of std::mutex and lock_guard ensures that the data accesses are atomic and the storage of data happens in a mutually exclusive manner between threads, ensuring the integrity of the temperature readings.

### Experimental Evaluation and Results

The output will include a report containing the top 5 highest and lowest temperatures, as well as the time interval with the largest temperature difference. The temperature values and time intervals reported will vary due to the random number generator simulating temperature readings.

Sample output:
```
Starting 8 sensor threads

Top 5 highest temperatures:
1: time 24 -> 70F
2: time 9  -> 70F
3: time 25 -> 69F
4: time 51 -> 69F
5: time 32 -> 68F

Top 5 lowest temperatures:
1: time 59 -> -100F
2: time 13 -> -99F
3: time 34 -> -99F
4: time 16 -> -99F
5: time 45 -> -98F

10-minute interval with largest temperature difference: 58-67
```
The total program execution time should ideally be 60 minutes but the time was purposely reduced to 6 seconds for testing purposes.

### How to run

1. `cd` to this project folder
2. Run `g++ -std=c++20 -pthread p2.cpp && ./a.out` in the terminal
3. That's it!