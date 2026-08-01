#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;
class Task{
    protected:
        int id;
        string name;
        int priority;
        int execTime;

    public:
        Task(int id, string name, int priority, int execTime) : 
            id(id), name(name), priority(priority), execTime(execTime){}
        int getTaskId() const{
            return id;
        }
        int getPriority() const{
            return priority;
        }
        int getExecutionTime() const{
            return execTime;
        }
        virtual void display() const = 0;
        virtual void execute() = 0;
        virtual ~Task() = default;
};
class CPUTask : public Task{
    private:
        int threads, affinity;
    public:
        CPUTask(int id, string name, int priority, int execTime, int threads, int affinity) :
        Task(id,name,priority,execTime), threads(threads), affinity(affinity){}

        void display () const override{
            cout << "CPU TASK\n";
            cout << "Task Name : " << name << "\n";
            cout << "Task ID : " << id << "\n";
            cout << "Task priority : " << priority << "\n";
            cout << "Execution Time : " << execTime << "\n";
            cout << "No. of threads : " << threads << "\n";
            cout << "Affinity : " << affinity << "\n";
        }
        void execute() override{
            cout << "Executing CPU Task :" << id << '\n';
        }
};
class IOTask : public Task{
    private:
        string deviceName;
        int dataSize;
    public:
        IOTask(int id, string name, int priority, int execTime, string deviceName, int dataSize) :
        Task(id,name,priority,execTime), deviceName(deviceName), dataSize(dataSize){}
        void display () const override{
            cout << "IO TASK\n";
            cout << "Task Name : " << name << "\n";
            cout << "Task ID : " << id << "\n";
            cout << "Task priority : " << priority << "\n";
            cout << "Execution Time : " << execTime << "\n";
            cout << "Device Name : " << deviceName << "\n";
            cout << "DataSize : " << dataSize << "\n";
        }
        void execute() override{
            cout << "Executing IO Task :" << id << '\n';
        }
};
class NetworkTask : public Task{
    private:
        string destinationIp;
        int port;
    public:
        NetworkTask(int id, string name, int priority, int execTime, string destinationIp, int port) :
        Task(id,name,priority,execTime), destinationIp(destinationIp), port(port){}
        void display () const override{
            cout << "NETWORK TASK\n";
            cout << "Task Name : " << name << "\n";
            cout << "Task ID : " << id << "\n";
            cout << "Task priority : " << priority << "\n";
            cout << "Execution Time : " << execTime << "\n";
            cout << "Destination IP : " << destinationIp << "\n";
            cout << "Port : " << port << "\n";
        }
        void execute() override{
            cout << "Executing Network Task :" << id << '\n';
        }
};
class Scheduler{
    private:
        vector<unique_ptr<Task>> tasks;
    public:
        void addTask(unique_ptr<Task> p){
            tasks.push_back(move(p));
        }
        void displayAllTasks() const{
            if(tasks.empty()){
                cout << "No tasks to display \n";
                return;
            }
            for(const auto &task : tasks){
                task->display();
            }
        }
        void executeTask(int taskId){
            for(auto &task : tasks){
                if(task->getTaskId()==taskId){
                    task->execute();
                    return;
                }
            }
            cout << "Task not found\n";
        }
        void executeAll(){
            if(tasks.empty()){
                cout << "No tasks found\n";
                return;
            }
            for(auto &task : tasks){
                task->execute();
            }
        }
        void removeTask(int taskId){
            for(auto it = tasks.begin(); it!=tasks.end(); it++){
                if((*it)->getTaskId()==taskId){
                    tasks.erase(it);
                    return;
                }
            }
            cout << "Task not found\n" << "\n";
        }
        void highestPriorityTask() const{
            if(tasks.empty()){
                cout << "No tasks found\n";
                return;
            }
            int currPriority = -1;
            int id = -1;
            for(auto &task : tasks){
                if(task->getPriority()>currPriority){
                    currPriority = task->getPriority();
                    id = task->getTaskId();
                }
            }
            cout << "Highest priority task id : " << id << '\n';
        }
        double averageExecutionTime() const{
            if(tasks.size()==0) return 0.0;
            double total = 0.0;
            for(auto &task : tasks){
                total += task->getExecutionTime();
            }
            return ((double)total/(double)tasks.size());
        }
};
int main() {

    Scheduler scheduler;

    scheduler.addTask(
        make_unique<CPUTask>(
            101,
            "Physics Simulation",
            8,
            120,
            12,
            4));

    scheduler.addTask(
        make_unique<IOTask>(
            102,
            "Disk Backup",
            4,
            450,
            "SSD",
            2048));

    scheduler.addTask(
        make_unique<NetworkTask>(
            103,
            "Send Market Data",
            10,
            30,
            "239.10.10.5",
            9000));

    cout << "======= TASKS =======\n";
    scheduler.displayAllTasks();

    cout << "\nExecuting Task 103\n";
    scheduler.executeTask(103);

    cout << "\nExecuting All Tasks\n";
    scheduler.executeAll();

    cout << "\nHighest Priority Task\n";
    scheduler.highestPriorityTask();

    cout << "\nAverage Execution Time : "
         << scheduler.averageExecutionTime()
         << " ms\n";

    cout << "\nRemoving Task 102\n";
    scheduler.removeTask(102);

    cout << "\nRemaining Tasks\n";
    scheduler.displayAllTasks();

    return 0;
}