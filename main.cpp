
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
string data;
bool ready = false;
bool processed = false;

void worker_thread() {

  //Wait until main send data
  unique_lock<mutex> lk(mtx);
  cv.wait(lk, []{return ready;});

  //After
  cout << "Worker thread is processing data\n";
  data += " data after processed";

  //Long task
  for (int i = 0; i < 10000; i++) {
    cout << i << endl;
  }

  //Send data back to main
  processed = true;
  cout << "Work thread signal data\n";

  lk.unlock();
  cv.notify_one();

}

int main() {

  thread worker(worker_thread);

  data = "Data to be processed,";

  {
      lock_guard<mutex> lk(mtx);
      ready = true;
      cout << "main signal data to the working thread\n";
  }
  cv.notify_one();

  {
    unique_lock<mutex> lk(mtx);
    cv.wait(lk, []{return processed;});
  }
  cout << "back to main, data:" << data << endl;

  worker.join();

  return 1;
}
