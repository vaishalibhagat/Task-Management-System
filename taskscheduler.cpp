#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <limits>

using namespace std;

// Structure to define a Task
struct Task
{
    int id;
    string name;
    int priority;
    int executionTime;
    int originalTime;
    string deadline;

    Task(int _id, string _name, int _priority, int _executionTime, string _deadline)
        : id(_id), name(_name), priority(_priority), executionTime(_executionTime), deadline(_deadline) {}

    bool operator<(const Task &other) const
    {
        return priority > other.priority;
    }
};

struct CompletedTask
{
    string algorithm; // Stores the scheduling algorithm name
    Task task;
};

vector<CompletedTask> completedTasks; // Stores completed tasks with their scheduling type

// Class for Task Scheduler
class TaskScheduler
{
private:
    queue<Task> fcfsQueue;
    priority_queue<Task> priorityQueue;
    vector<Task> taskList;
    queue<Task> roundRobinQueue;
    int taskCount = 0;

public:
    void addTask(string name, int priority, int executionTime, string deadline)
    {
        taskCount++;
        Task newTask(taskCount, name, priority, executionTime, deadline);
        newTask.originalTime = executionTime;
        fcfsQueue.push(newTask);
        priorityQueue.push(newTask);
        taskList.push_back(newTask);
        roundRobinQueue.push(newTask);
        cout << "\nTask Added: " << name << " (ID: " << taskCount << ")\n";
    }

    void executeFCFS()
    {
        cout << "\n[INFO] Executing Tasks (FCFS Order)\n";
        cout << "---------------------------------------\n";
        while (!fcfsQueue.empty())
        {
            Task task = fcfsQueue.front();
            fcfsQueue.pop();
            completedTasks.push_back({"FCFS", task});
            cout << "[DONE] Completed: " << task.name << " | Time: " << task.executionTime << " | Deadline: " << task.deadline << endl;
        }
    }

    void executeSJF()
    {
        cout << "\n[INFO] Executing Tasks (Shortest Job First)\n";
        cout << "---------------------------------------\n";
        sort(taskList.begin(), taskList.end(), [](const Task &a, const Task &b)
             {
                 return a.executionTime < b.executionTime; // Sort by shortest execution time
             });
        for (Task &task : taskList)
        {
            completedTasks.push_back({"SJF", task});
            cout << "[DONE] Completed: " << task.name << " | Time: " << task.executionTime << " | Deadline: " << task.deadline << endl;
        }
    }

    void executePriorityScheduling()
    {
        cout << "\n[INFO] Executing Tasks (Priority Order)\n";
        cout << "------------------------------------------\n";
        while (!priorityQueue.empty())
        {
            Task task = priorityQueue.top();
            priorityQueue.pop();
            completedTasks.push_back({"Priority Scheduling", task});
            cout << "[DONE] Completed: " << task.name << " | Priority: " << task.priority << " | Deadline: " << task.deadline << endl;
        }
    }

    void executeRoundRobin(int quantum)
    {
        cout << "\n[INFO] Executing Tasks (Round Robin, Time Quantum = " << quantum << ")\n";
        cout << "------------------------------------------------------\n";

        queue<Task> tempQueue = roundRobinQueue;

        while (!tempQueue.empty())
        {
            Task task = tempQueue.front();
            tempQueue.pop();

            if (task.executionTime > quantum)
            {
                cout << "[IN-PROGRESS] Executing Task: " << task.name << " for " << quantum << " units\n";
                task.executionTime -= quantum;
                tempQueue.push(task); // Re-add to queue if execution time is left
            }
            else
            {
                cout << "[DONE] Completed: " << task.name << " | Execution Time: " << task.executionTime << " | Deadline: " << task.deadline << endl;
                completedTasks.push_back({"Round Robin", task}); // Store completed task with algorithm name
            }
        }
    }

    void showCompletedTasks()
    {
        cout << "\n[INFO] Completed Tasks (Grouped by Algorithm)\n";
        cout << "------------------------------------------------\n";

        map<string, vector<Task>> groupedTasks;

        // Group tasks by algorithm
        for (const auto &entry : completedTasks)
        {
            groupedTasks[entry.algorithm].push_back(entry.task);
        }

        // Display tasks by their respective scheduling algorithm
        for (const auto &[algorithm, tasks] : groupedTasks)
        {
            cout << "\n"
                 << algorithm << " Scheduling\n";
            cout << "--------------------------------\n";
            for (const Task &task : tasks)
            {
                cout << "ID: " << task.id << " | Name: " << task.name
                     << " | Time: " << task.originalTime << " | Priority: "
                     << task.priority << " | Deadline: " << task.deadline << endl;
            }
        }
    }
};

// Function to display a stylish menu
void displayMenu()
{
    cout << "\n======================================\n";
    cout << "|        TASK SCHEDULER SYSTEM       |\n";
    cout << "======================================\n";
    cout << "| 1. Add Task                        |\n";
    cout << "| 2. Execute FCFS Scheduling         |\n";
    cout << "| 3. Execute Shortest Job First (SJF)|\n";
    cout << "| 4. Execute Priority Scheduling     |\n";
    cout << "| 5. Execute Round Robin Scheduling  |\n";
    cout << "| 6. Show Completed Tasks            |\n";
    cout << "| 7. Exit                            |\n";
    cout << "======================================\n";
}

int main()
{
    TaskScheduler scheduler;
    int choice, timeQuantum;

    while (true)
    {
        displayMenu();
        cout << "[INPUT] Enter your choice: ";

        if (!(cin >> choice)) // Check if input is a valid number
        {
            cout << "[ERROR] Invalid input! Please enter a number between 1 and 7.\n";
            cin.clear();                                         // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            continue;                                            // Restart loop
        }

        if (choice == 1)
        {
            string name, deadline;
            int priority, executionTime;
            cout << "[INPUT] Enter Task Name: ";
            cin >> name;

            cout << "[INPUT] Enter Priority (Lower number = Higher priority): ";
            while (!(cin >> priority))
            { // Validate integer input
                cout << "[ERROR] Invalid input! Enter a valid priority number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "[INPUT] Enter Execution Time: ";
            while (!(cin >> executionTime))
            {
                cout << "[ERROR] Invalid input! Enter a valid execution time: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "[INPUT] Enter Deadline (YYYY-MM-DD): ";
            cin >> deadline;
            scheduler.addTask(name, priority, executionTime, deadline);
        }
        else if (choice == 2)
        {
            scheduler.executeFCFS();
        }
        else if (choice == 3)
        {
            scheduler.executeSJF();
        }
        else if (choice == 4)
        {
            scheduler.executePriorityScheduling();
        }
        else if (choice == 5)
        {
            cout << "[INPUT] Enter Time Quantum for Round Robin: ";
            while (!(cin >> timeQuantum))
            {
                cout << "[ERROR] Invalid input! Enter a valid time quantum: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            scheduler.executeRoundRobin(timeQuantum);
        }
        else if (choice == 6)
        {
            scheduler.showCompletedTasks();
        }
        else if (choice == 7)
        {
            cout << "[EXIT] Task Scheduler is shutting down...\n";
            break;
        }
        else
        {
            cout << "[ERROR] Invalid choice! Enter a number between 1 and 7.\n";
        }
    }

    return 0;
}
