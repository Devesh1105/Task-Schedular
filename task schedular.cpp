#include <iostream>
#include <list>         // For linked list of tasks
#include <string>
#include <algorithm>    // For std::find_if
using namespace std;

// Task class representing individual tasks
class Task {
public:
    int taskID;
    string taskName;
    int priority;
    string deadline;
    string tag; // For single tagging
    string notes; // For task notes
    bool isCompleted;

    Task(int id, string name, int prio, string dl, string tg)
        : taskID(id), taskName(name), priority(prio), deadline(dl), tag(tg), isCompleted(false) {}

    void addNotes(const string& note) {
        notes = note;
    }

    void completeTask() {
        isCompleted = true;
    }

    void display() const {
        cout << "Task ID: " << taskID
             << ", Name: " << taskName
             << ", Priority: " << priority
             << ", Deadline: " << deadline
             << ", Completed: " << (isCompleted ? "Yes" : "No") << endl;
        if (!notes.empty()) {
            cout << "Notes: " << notes << endl;
        }
        if (!tag.empty()) {
            cout << "Tag: " << tag << endl;
        }
    }

    void displayDetails() const {
        cout << "\nTask Details: \n";
        cout << "Task ID: " << taskID << endl;
        cout << "Name: " << taskName << endl;
        cout << "Priority: " << priority << endl;
        cout << "Deadline: " << deadline << endl;
        cout << "Completed: " << (isCompleted ? "Yes" : "No") << endl;
        if (!notes.empty()) {
            cout << "Notes: " << notes << endl;
        }
        if (!tag.empty()) {
            cout << "Tag: " << tag << endl;
        }
        cout << "--------------------------------" << endl;
    }
};

// TreeNode for Task Prioritization (BST)
class TreeNode {
public:
    Task task;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Task& t) : task(t), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for Task Prioritization
class TaskPriorityTree {
public:
    TreeNode* root;

    TaskPriorityTree() : root(nullptr) {}

    // Insert task into BST based on priority
    void insertTask(const Task& t) {
        root = insert(root, t);
    }

    // Inorder traversal to display tasks based on priority
    void displayInPriorityOrder() {
        inorder(root);
    }

private:
    TreeNode* insert(TreeNode* node, const Task& t) {
        if (node == nullptr) {
            return new TreeNode(t);
        }
        if (t.priority < node->task.priority) {
            node->left = insert(node->left, t);
        } else {
            node->right = insert(node->right, t);
        }
        return node;
    }

    void inorder(TreeNode* node) {
        if (node == nullptr) return;
        inorder(node->left);
        node->task.display();
        inorder(node->right);
    }
};

// TaskScheduler class to manage tasks using linked list
class TaskScheduler {
private:
    list<Task> tasks;              // Linked list to store tasks
    list<Task> completedTasks;     // Linked list for completed tasks
    TaskPriorityTree priorityTree; // BST for prioritizing tasks

public:
    void addTask(int id) {
        string name, deadline, note, tag;
        int priority;

        cout << "Enter task name: ";
        cin.ignore(); // Ignore the newline character left in the buffer
        getline(cin, name);

        cout << "Enter priority (1-5): ";
        cin >> priority;

        cout << "Enter deadline (YYYY-MM-DD): ";
        cin >> deadline;

        // Get tag (Single input)
        cout << "Enter a tag for the task: ";
        cin >> tag;

        Task newTask(id, name, priority, deadline, tag);

        // Get notes
        cout << "Enter notes for the task: ";
        cin.ignore(); // Ignore the newline character
        getline(cin, note);
        newTask.addNotes(note);

        tasks.push_back(newTask);  // Add task to linked list
        priorityTree.insertTask(newTask); // Add task to BST based on priority

        cout << "Task added successfully!" << endl;
    }

    void completeTask(int taskID) {
        auto it = find_if(tasks.begin(), tasks.end(), [taskID](const Task& task) {
            return task.taskID == taskID;
        });

        if (it != tasks.end()) {
            Task task = *it; // Copy the task
            task.completeTask();
            completedTasks.push_back(task); // Add to completed tasks
            tasks.erase(it); // Remove from linked list of tasks
            cout << "Completed Task: " << task.taskName << endl;
        } else {
            cout << "Task ID not found!" << endl;
        }
    }

    void displayCompletedTasks() {
        if (completedTasks.empty()) {
            cout << "No completed tasks!" << endl;
            return;
        }

        cout << "Completed Tasks Details:" << endl;
        for (const auto& task : completedTasks) {
            task.displayDetails();
        }
    }

    void undoLastTask() {
        if (!completedTasks.empty()) {
            Task task = completedTasks.back();
            completedTasks.pop_back();
            task.isCompleted = false; // Mark the task as not completed
            tasks.push_back(task); // Re-add to task list
            cout << "Undid completion of Task: " << task.taskName << endl;
        } else {
            cout << "No tasks to undo!" << endl;
        }
    }

    void displayTasks() {
        cout << "Current Tasks:" << endl;
        for (const auto& task : tasks) {
            task.display();
        }
    }

    // Display tasks in priority order using the binary search tree
    void displayTasksByPriority() {
        cout << "\nTasks in Priority Order:" << endl;
        priorityTree.displayInPriorityOrder();
    }

    // Display only pending tasks (not completed)
    void displayPendingTasks() {
        cout << "\nPending Tasks:" << endl;
        bool foundPending = false;
        for (const auto& task : tasks) {
            if (!task.isCompleted) {
                task.display();
                foundPending = true;
            }
        }
        if (!foundPending) {
            cout << "No pending tasks!" << endl;
        }
    }
};

int main() {
    TaskScheduler scheduler;
    int choice;
    int taskCount = 1; // For unique task IDs

    while (true) {
        cout << "\nTask Scheduler Menu:\n";
        cout << "1. Add Task\n";
        cout << "2. Complete Task\n";
        cout << "3. Undo Last Task\n";
        cout << "4. Display All Tasks\n";
        cout << "5. Display Tasks by Priority\n";
        cout << "6. Display Completed Tasks\n";
        cout << "7. Display Pending Tasks\n";  // New menu option for pending tasks
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                scheduler.addTask(taskCount++);
                break;
            case 2: {
                int taskID;
                cout << "Enter Task ID to complete: ";
                cin >> taskID;
                scheduler.completeTask(taskID);
                break;
            }
            case 3:
                scheduler.undoLastTask();
                break;
            case 4:
                scheduler.displayTasks();
                break;
            case 5:
                scheduler.displayTasksByPriority();
                break;
            case 6:
                scheduler.displayCompletedTasks();
                break;
            case 7:
                scheduler.displayPendingTasks();  // Display pending tasks
                break;
            case 8:
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
