
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
string data;
bool ready01 = false;
bool ready02 = false;
bool processed01 = false;
bool processed02 = false;

void worker_thread_01() {

  //Wait until main send data
  unique_lock<mutex> lk(mtx);
  cv.wait(lk, []{return ready01;});

  //After
  data += "[Worker thread_01] data after processed |_";

  //Long task
  for (int i = 0; i < 10000; i++) {
    cout << i << endl;
  }

  //Send data back to main
  processed01 = true;
  cout << "Work thread signal data\n";

  lk.unlock();
  cv.notify_one();

}

void worker_thread_02() {

    //Wait until main send data
    unique_lock<mutex> lk(mtx);
    cv.wait(lk, []{return ready02;});

    //After
    data += "[Worker thread_02] data after processed |_";

    //Long task
    for (int i = 0; i < 10000; i++) {
      cout << i << endl;
    }

    //Send data back to main
    processed02 = true;
    cout << "Work thread signal data\n";

    lk.unlock();
    cv.notify_one();
}

int main() {

  thread worker_01(worker_thread_01);

  thread worker_02(worker_thread_02);

  data = "Data to be processed,";

  {
      lock_guard<mutex> lk(mtx);
      ready01 = true;
      cout << "main signal data to the working thread 01\n";
  }
  cv.notify_one();

  {
      lock_guard<mutex> lk(mtx);
      ready02 = true;
      cout << "main signal data to the working thread 02\n";
  }
  cv.notify_one();

  {
    unique_lock<mutex> lk(mtx);
    cv.wait(lk, []{return processed01;});
  }
  cout << "back to main, data:" << data << endl;
  worker_01.join();

  {
    unique_lock<mutex> lk(mtx);
    cv.wait(lk, []{return processed02;});
  }
  cout << "back to main, data:" << data << endl;
  worker_02.join();



  return 1;
}
