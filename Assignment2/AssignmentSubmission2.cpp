// Author: Douwe Berkeij
// Date: 03-09-2025

#include <iostream>
#include <string>

// Prints all natural numbers from 1 to n using recursion
// Time: O(n), Space: O(n) due to call stack
void printNaturalNumbers(int n) {
	if (n < 1) return; // Base case
	printNaturalNumbers(n - 1); // Recursive call
	std::cout << n << " ";
}

// Calculates factorial of n using recursion
// Time: O(n), Space: O(n) due to call stack
int calculateFactorial(int n) {
    if (n <= 1) return 1; // Base case
    return n * calculateFactorial(n - 1); // Recursive step
}

// Calculates base raised to exponent using recursion (efficient)
// Time: O(log n), Space: O(log n)
int power(int base, int exponent) {
    if (exponent == 0) return 1; // Base case
    if (exponent % 2 == 0) {
        int half = power(base, exponent / 2);
        return half * half;
    } else {
        return base * power(base, exponent - 1);
    }
}

// Returns the n-th Fibonacci number using naive recursion
// Time: O(2^n), Space: O(n) due to call stack
int fibonacci(int n) {
    if (n <= 1) return n; // Base cases: fib(0)=0, fib(1)=1
    return fibonacci(n - 1) + fibonacci(n - 2); // Multiple recursion
}
// Note: This naive recursive solution is inefficient because it recalculates the same subproblems many times. Using memoization or iteration would improve performance to O(n).

// Counts occurrences of character c in string s using recursion
// Time: O(n), Space: O(n) due to call stack
int countOccurrences(const std::string& s, char c) {
    if (s.empty()) return 0; // Base case
    int count = (s[0] == c) ? 1 : 0;
    return count + countOccurrences(s.substr(1), c); // Recursive step
}

// Finds the largest element in an array using divide and conquer recursion
// Time: O(n), Space: O(log n) due to call stack (for balanced splits)
int findLargestElement(int arr[], int left, int right) {
    if (left == right) return arr[left]; // Base case: one element
    int mid = left + (right - left) / 2;
    int maxLeft = findLargestElement(arr, left, mid);
    int maxRight = findLargestElement(arr, mid + 1, right);
    return (maxLeft > maxRight) ? maxLeft : maxRight;
}

// Recursively prints ASCII characters from start to end
// Time: O(n), Space: O(n) due to call stack
void traverseAsciiTable(char start, char end) {
    if (start > end) return; // Base case
    std::cout << start << " "; // Print before recursion (building stack)
    traverseAsciiTable(start + 1, end); // Recursive call
    std::cout << start << " "; // Print after recursion (unwinding stack)
}


int main() {
    std::cout << "printNaturalNumbers(5 and 25): ";
    printNaturalNumbers(5);
    std::cout << "\n";
    printNaturalNumbers(25);
    std::cout << "\n";

    std::cout << "calculateFactorial(5 and 10): ";
    std::cout << calculateFactorial(5) << "\n";
    std::cout << calculateFactorial(10) << "\n";

    std::cout << "power(2, 8 and 3, 5): ";
    std::cout << power(2, 8) << "\n";
    std::cout << power(3, 5) << "\n";

    std::cout << "fibonacci(10 and 8): ";
    std::cout << fibonacci(10) << "\n";
    std::cout << fibonacci(8) << "\n";

    std::string testStr = "recursion is cool!";
    char testChar = 'o';
    std::cout << "countOccurrences(\"" << testStr << "\", '" << testChar << "'): ";
    std::cout << countOccurrences(testStr, testChar) << "\n";
    char testChar2 = 'i';
    std::cout << "countOccurrences(\"" << testStr << "\", '" << testChar2 << "'): ";
    std::cout << countOccurrences(testStr, testChar2) << "\n";

    int arr[] = {3, 7, 2, 9, 4, 1};
    int arrSize = sizeof(arr) / sizeof(arr[0]);
    std::cout << "findLargestElement(arr, 0, " << arrSize - 1 << "): ";
    std::cout << findLargestElement(arr, 0, arrSize - 1) << "\n";
    std::cout << findLargestElement(arr, 2, 4) << "\n";

    std::cout << "traverseAsciiTable('A', 'E' and 'X', 'Z'): ";
    traverseAsciiTable('A', 'E');
    std::cout << "\n";
    traverseAsciiTable('X', 'Z');

    return 0;
}
