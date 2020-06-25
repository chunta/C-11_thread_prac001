
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

mutex mtx;
vector<int> taskList;

void worker_thread() {

    //Wait until main send data
    bool working = false;
    int total = 0;
    while(true) {
        ////////////////////////////////////////
        if (working) {
            total--;
            cout << this_thread::get_id() << "----" << total << endl;
            if (total <= 0) {
                total = 0;
                working = false;
              }
        } else {
            unique_lock<mutex> lk(mtx);
            if (taskList.size()) {
                total = taskList.front();
                taskList.erase(taskList.begin());
                working = true;
            }
        }
        ////////////////////////////////////////
    }
}

int main() {

  for (int i = 0; i < 10; i++) {
      taskList.push_back(10 * (i + 1));
      taskList.push_back(100 * 2);
      taskList.push_back(100 * 5);
  }
  thread worker_01(worker_thread);
  thread worker_02(worker_thread);
  thread worker_03(worker_thread);

  while(1) {}
  /*
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
  */


  return 1;
}
