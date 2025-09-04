// Author: Douwe Berkeij
// Date: 04-09-2025

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

const int MAX_SIZE = 100;

// Simple stack for int values
class TStack {
private:
    int data[MAX_SIZE];
    int top;
public:
    TStack() : top(-1) {}
    // Pushes an item onto the stack
    // Time: O(1), Space: O(1)
    bool Push(int item) {
        if (top >= MAX_SIZE - 1) return false; // Stack overflow
        data[++top] = item;
        return true;
    }
    // Pops the top item from the stack
    // Time: O(1), Space: O(1)
    int Pop() {
        if (IsEmpty()) throw std::out_of_range("Stack underflow");
        return data[top--];
    }
    // Returns the top item without removing it
    // Time: O(1), Space: O(1)
    int Peek() const {
        if (IsEmpty()) throw std::out_of_range("Stack is empty");
        return data[top];
    }
    // Checks if the stack is empty
    // Time: O(1), Space: O(1)
    bool IsEmpty() const { return top == -1; }
    // Checks if the stack is full
    // Time: O(1), Space: O(1)
    bool IsFull() const { return top == MAX_SIZE - 1; }
};

// Simple queue for int values
class TQueue {
private:
    int data[MAX_SIZE];
    int front, rear, count;
public:
    TQueue() : front(0), rear(0), count(0) {}
    // Enqueues an item
    // Time: O(1), Space: O(1)
    bool Enqueue(int item) {
        if (count >= MAX_SIZE) return false; // Queue overflow
        data[rear] = item;
        rear = (rear + 1) % MAX_SIZE;
        ++count;
        return true;
    }
    // Dequeues an item
    // Time: O(1), Space: O(1)
    int Dequeue() {
        if (IsEmpty()) throw std::out_of_range("Queue underflow");
        int item = data[front];
        front = (front + 1) % MAX_SIZE;
        --count;
        return item;
    }
    // Returns the front item without removing it
    // Time: O(1), Space: O(1)
    int Peek() const {
        if (IsEmpty()) throw std::out_of_range("Queue is empty");
        return data[front];
    }
    // Checks if the queue is empty
    // Time: O(1), Space: O(1)
    bool IsEmpty() const { return count == 0; }
    // Checks if the queue is full
    // Time: O(1), Space: O(1)
    bool IsFull() const { return count == MAX_SIZE; }
};

// Reverses a string using TStack
// Time: O(n), Space: O(n)
std::string ReverseString(const std::string& str) {
    TStack stack;
    for (char ch : str) {
        stack.Push(ch);
    }
    std::string reversed;
    while (!stack.IsEmpty()) {
        reversed += (char)stack.Pop();
    }
    // Stack is perfect for reversal because it retrieves elements in reverse order (LIFO)
    return reversed;
}

// Iterative factorial using TStack
// Time: O(n), Space: O(n)
int IterativeFactorial(int n) {
    TStack stack;
    int result = 1;
    for (int i = n; i > 1; --i) {
        stack.Push(i);
    }
    while (!stack.IsEmpty()) {
        result *= stack.Pop();
    }
    return result;
}

// Simulates a wait line using TQueue
// Time: O(n), Space: O(n)
void SimulateWaitLine() {
    TQueue queue;
    std::cout << "Enqueue IDs 1, 2, 3, 4, 5\n";
    for (int i = 1; i <= 5; ++i) queue.Enqueue(i);
    while (!queue.IsEmpty()) {
        std::cout << "Serving ID: " << queue.Dequeue() << "\n";
    }
}

// DFS on a 100x100 grid using TStack
// Time: O(n^2), Space: O(n^2)
struct Cell { int row, col; };

bool DFSFindZero(int grid[100][100], bool visited[100][100], int startRow, int startCol) {
    TStack stack;
    stack.Push(startRow * 100 + startCol); // Encode cell as int
    while (!stack.IsEmpty()) {
        int code = stack.Pop();
        int row = code / 100, col = code % 100;
        if (row < 0 || row >= 100 || col < 0 || col >= 100 || visited[row][col]) continue;
        visited[row][col] = true;
        if (grid[row][col] == 0) {
            std::cout << "DFS found 0 at (" << row << ", " << col << ")\n";
            return true;
        }
        // LIFO: explores as deep as possible before backtracking
        stack.Push((row - 1) * 100 + col); // up
        stack.Push((row + 1) * 100 + col); // down
        stack.Push(row * 100 + (col - 1)); // left
        stack.Push(row * 100 + (col + 1)); // right
    }
    return false;
}

// BFS on a 100x100 grid using TQueue
// Time: O(n^2), Space: O(n^2)
bool BFSFindZero(int grid[100][100], bool visited[100][100], int startRow, int startCol) {
    TQueue queue;
    queue.Enqueue(startRow * 100 + startCol);
    while (!queue.IsEmpty()) {
        int code = queue.Dequeue();
        int row = code / 100, col = code % 100;
        if (row < 0 || row >= 100 || col < 0 || col >= 100 || visited[row][col]) continue;
        visited[row][col] = true;
        if (grid[row][col] == 0) {
            std::cout << "BFS found 0 at (" << row << ", " << col << ")\n";
            return true;
        }
        // FIFO: explores level by level
        queue.Enqueue((row - 1) * 100 + col); // up
        queue.Enqueue((row + 1) * 100 + col); // down
        queue.Enqueue(row * 100 + (col - 1)); // left
        queue.Enqueue(row * 100 + (col + 1)); // right
    }
    return false;
}

int main() {
    // Test TStack
    TStack stack;
    std::cout << "Testing TStack boundary cases:\n";
    for (int i = 0; i < MAX_SIZE; ++i) stack.Push(i);
    std::cout << "IsFull: " << stack.IsFull() << "\n";
    for (int i = 0; i < MAX_SIZE; ++i) stack.Pop();
    std::cout << "IsEmpty: " << stack.IsEmpty() << "\n";

    // Test TQueue
    TQueue queue;
    std::cout << "Testing TQueue boundary cases:\n";
    for (int i = 0; i < MAX_SIZE; ++i) queue.Enqueue(i);
    std::cout << "IsFull: " << queue.IsFull() << "\n";
    for (int i = 0; i < MAX_SIZE; ++i) queue.Dequeue();
    std::cout << "IsEmpty: " << queue.IsEmpty() << "\n";

    // String reversal
    std::string s = "StackQueue";
    std::cout << "ReverseString: " << ReverseString(s) << "\n";

    // Iterative factorial
    std::cout << "IterativeFactorial(5): " << IterativeFactorial(5) << "\n";

    // Wait line simulation
    SimulateWaitLine();

    // Grid setup
    int grid[100][100];
    bool visited[100][100] = {false};
    std::srand((unsigned)std::time(0));
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            grid[i][j] = std::rand() % 10;
    int startRow = std::rand() % 100, startCol = std::rand() % 100;
    std::cout << "Random start cell: (" << startRow << ", " << startCol << ")\n";

    // DFS
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            visited[i][j] = false;
    DFSFindZero(grid, visited, startRow, startCol);

    // BFS
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            visited[i][j] = false;
    BFSFindZero(grid, visited, startRow, startCol);

    // Compare and contrast
    std::cout << "\n";
    std::cout << "differences between DFS and BFS:\n";
    std::cout << "DFS explores deeply along one path before backtracking, guided by the LIFO nature of the stack.\n";
    std::cout << "BFS explores the grid level by level, guided by the FIFO nature of the queue.\n";
    std::cout << "Comparison of DFS and BFS:\n";
    std::cout << "DFS: Good for exploring all possible paths, not guaranteed to find the shortest path.\n";
    std::cout << "BFS: Good for finding the shortest path, explores all neighbors before going deeper.\n";

    return 0;
}
