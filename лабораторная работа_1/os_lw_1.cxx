#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;


std::mutex m;
std::condition_variable cv;
bool ready = false;

class something 
{
	vector <int> someth;

public:
     void Add(int el)
    {
		 someth.push_back(el+1);
	}
	void Output()
	{
		for(long int i=0; i<someth.size(); i++)
		    cout << someth[i] << " ";
		cout << endl;
	}	
};

something s;

void provide ()
{
 std::unique_lock<std::mutex> lk(m);
 if (ready)
 {
  m.unlock();
  return;
 }
 ready = true;
 s.Add(1), s.Add(2);
 cout<<"provided\n";
 cv.notify_one();
 //lk.unlock();
}

void consume ()
{
 std::unique_lock<std::mutex> lk(m);
 while (!ready)
 {   
  cv.wait(lk, [] {return ready;});
  cout<<"awoke\n";
 }
 ready = false;
 s.Output();
 cout<<endl;
 cout<<"consumed\n";
 //lk.unlock();
}

int main()
{
 std::thread provider(provide);
 std::thread consumer(consume);
 
 consumer.join();
 provider.join();
 
 
}
