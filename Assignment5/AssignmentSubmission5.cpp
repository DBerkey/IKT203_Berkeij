// Author: Douwe Berkeij
// Date: 04-11-2025
// Assignment 5: The Algorithmic Organizer
// AI use: For this assignment there was made use of github copilot with bug fixes.

#include <random>
#include <string>
#include <ctime>
#include <functional>
#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <cctype>

enum class EBankAccountType { Checking, Savings, Credit, Pension, Loan };

// Forward declaration for TBankAccount
class TBankAccount;

// Typedef for compare callback used in searches/sorts (part of this assignment)
// returns negative if a < b, zero if equal, positive if a > b
typedef int (*FCompareAccounts)(TBankAccount* a, TBankAccount* b);

// OperationSummary struct to track performance metrics
struct OperationSummary {
	long long comparisons;
	long long swaps;
	double timeSpentMs;
	OperationSummary() : comparisons(0), swaps(0), timeSpentMs(0.0) {}
};

class TBankAccount {
public:
	std::string accountNumber;
	EBankAccountType accountType;
	std::string ownerFirstName;
	std::string ownerLastName;
	time_t creationTimestamp;
	double balance;

	TBankAccount(const std::string& accountNumber,
				 EBankAccountType accountType,
				 const std::string& ownerFirstName,
				 const std::string& ownerLastName,
				 time_t creationTimestamp)
		: accountNumber(accountNumber),
		  accountType(accountType),
		  ownerFirstName(ownerFirstName),
		  ownerLastName(ownerLastName),
		  creationTimestamp(creationTimestamp)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		if (accountType == EBankAccountType::Loan) {
			std::uniform_real_distribution<> dis(-50000.0, -25000.0);
			balance = dis(gen);
		} else if (accountType == EBankAccountType::Credit) {
			std::uniform_real_distribution<> dis(-1000.0, 0.0);
			balance = dis(gen);
		} else if (accountType == EBankAccountType::Checking ||
				   accountType == EBankAccountType::Savings ||
				   accountType == EBankAccountType::Pension) {
			std::uniform_real_distribution<> dis(0.0, 1000.0);
			balance = dis(gen);
		} else {
			balance = 0.0;
		}
	}
};

// Simple singly-linked list template used across assignments
template<typename T>
class TLinkedList {
private:
	struct Node {
		T* data;
		Node* next;
		Node(T* dataPtr) : data(dataPtr), next(nullptr) {}
	};

	Node* head;
	Node* tail;
	bool ownsData;
	size_t size;

public:
	explicit TLinkedList(bool ownsDataFlag = true)
		: head(nullptr), tail(nullptr), ownsData(ownsDataFlag), size(0) {}

	~TLinkedList() { clear(); }
	TLinkedList(const TLinkedList&) = delete;
	TLinkedList& operator=(const TLinkedList&) = delete;

	void add(T* dataPtr) {
		if (!dataPtr) return;
		Node* newNode = new Node(dataPtr);
		if (!head) head = tail = newNode;
		else { tail->next = newNode; tail = newNode; }
		size++;
	}

	size_t getSize() const { return size; }
	bool isEmpty() const { return head == nullptr; }

	void clear() {
		while (head) {
			Node* nodeToDelete = head;
			head = head->next;
			if (ownsData) delete nodeToDelete->data;
			delete nodeToDelete;
		}
		tail = nullptr; size = 0;
	}

	// Iterator
	class Iterator {
	private:
		Node* current;
	public:
		Iterator(Node* node) : current(node) {}
		T* operator*() const { return current ? current->data : nullptr; }
		Iterator& operator++() { if (current) current = current->next; return *this; }
		bool operator!=(const Iterator& other) const { return current != other.current; }
	};

	Iterator begin() const { return Iterator(head); }
	Iterator end() const { return Iterator(nullptr); }
};

// Utility data from Assignment 4 reused here
std::vector<std::string> firstNames = {
	"James", "Mary", "John", "Patricia", "Robert", "Jennifer", "Michael", "Linda",
	"William", "Elizabeth", "David", "Barbara", "Richard", "Susan", "Joseph", "Jessica",
	"Thomas", "Sarah", "Christopher", "Karen", "Charles", "Nancy", "Daniel", "Lisa",
	"Matthew", "Betty", "Anthony", "Helen", "Mark", "Sandra", "Donald", "Donna",
	"Steven", "Carol", "Paul", "Ruth", "Andrew", "Sharon", "Joshua", "Michelle",
	"Kenneth", "Laura", "Kevin", "Sarah", "Brian", "Kimberly", "George", "Deborah",
	"Frank", "Dorothy", "Gregory", "Lisa", "Ronald", "Nancy", "Timothy", "Karen",
	"Jason", "Betty", "Edward", "Helen", "Jeffrey", "Sandra", "Ryan", "Donna",
	"Jacob", "Carol", "Gary", "Ruth", "Nicholas", "Sharon", "Eric", "Michelle",
	"Jonathan", "Laura", "Stephen", "Sarah", "Larry", "Kimberly", "Justin", "Deborah",
	"Scott", "Dorothy", "Brandon", "Amy", "Benjamin", "Angela", "Samuel", "Ashley",
	"Gregory", "Brenda", "Alexander", "Emma", "Patrick", "Olivia", "Alexander", "Cynthia"
};

std::vector<std::string> lastNames = {
	"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
	"Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas",
	"Taylor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson", "White",
	"Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker", "Young",
	"Allen", "King", "Wright", "Scott", "Torres", "Nguyen", "Hill", "Flores",
	"Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell",
	"Carter", "Roberts", "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker",
	"Cruz", "Edwards", "Collins", "Reyes", "Stewart", "Morris", "Morales", "Murphy",
	"Cook", "Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper", "Peterson", "Bailey",
	"Reed", "Kelly", "Howard", "Ramos", "Kim", "Cox", "Ward", "Richardson",
	"Watson", "Brooks", "Chavez", "Wood", "James", "Bennett", "Gray", "Mendoza",
	"Ruiz", "Hughes", "Price", "Alvarez", "Castillo", "Sanders", "Patel", "Myers",
	"Foster", "Ferguson", "Freeman", "Fletcher", "Franklin", "Fuller", "Francis", "Fowler"
};

// Generate random account number
std::string GenerateAccountNumber(std::mt19937& gen) {
	std::uniform_int_distribution<> dis(100000, 999999);
	return "ACC" + std::to_string(dis(gen));
}

// Generate random timestamp
time_t GenerateRandomTimestamp(std::mt19937& gen) {
	struct tm tm2025start = {};
	// tm_year is years since 1900; 125 -> 2025.
	tm2025start.tm_year = 125;
	tm2025start.tm_mon = 0;
	tm2025start.tm_mday = 1;
	time_t start2025 = mktime(&tm2025start);
	struct tm tm2025end = {};
	tm2025end.tm_year = 125;
	tm2025end.tm_mon = 11;
	tm2025end.tm_mday = 31;
	tm2025end.tm_hour = 23; tm2025end.tm_min = 59; tm2025end.tm_sec = 59;
	time_t end2025 = mktime(&tm2025end);
	std::uniform_int_distribution<time_t> dis(start2025, end2025);
	return dis(gen);
}

EBankAccountType GenerateRandomAccountType(std::mt19937& gen) {
	std::uniform_int_distribution<> dis(0, 4);
	return static_cast<EBankAccountType>(dis(gen));
}

// Comparator callbacks (two required)
int CompareByLastName(TBankAccount* a, TBankAccount* b) {
	// lexicographic compare on last name, tie-breaker on first name
	if (!a || !b) return (a ? 1 : (b ? -1 : 0));
	int cmp = a->ownerLastName.compare(b->ownerLastName);
	if (cmp != 0) return cmp;
	return a->ownerFirstName.compare(b->ownerFirstName);
}

int CompareByBalance(TBankAccount* a, TBankAccount* b) {
	if (!a || !b) return (a ? 1 : (b ? -1 : 0));
	if (a->balance < b->balance) return -1;
	if (a->balance > b->balance) return 1;
	return 0;
}

// TSort class: sorting engine that produces sorted arrays/lists of pointers
class TSort {
private:
	TLinkedList<TBankAccount>* originalList;
	TBankAccount** originalArray;
	int originalArraySize;

	// internal cached sorted array and flag
	TBankAccount** sortedArray;
	int sortedArraySize;
	bool isArraySorted;

	// Helpers for timing
	static double NowMs() {
		auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(now.time_since_epoch()).count();
	}

public:
	TSort(TLinkedList<TBankAccount>* aList, TBankAccount** aArray, int aArraySize)
		: originalList(aList), originalArray(aArray), originalArraySize(aArraySize),
		  sortedArray(nullptr), sortedArraySize(0), isArraySorted(false) {}

	~TSort() {
		if (sortedArray) delete[] sortedArray;
	}

	// Selection sort on array (returns new array of pointers)
	// Complexity: Best O(n^2), Average O(n^2), Worst O(n^2). Space O(n) for copy.
	TBankAccount** SelectionSortArray(FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		double start = NowMs();

		// Create a copy of pointers
		TBankAccount** arr = new TBankAccount*[originalArraySize];
		for (int i = 0; i < originalArraySize; ++i) arr[i] = originalArray[i];

		for (int i = 0; i < originalArraySize - 1; ++i) {
			int minIdx = i;
			for (int j = i + 1; j < originalArraySize; ++j) {
				summary.comparisons++;
				if (cmp(arr[j], arr[minIdx]) < 0) minIdx = j;
			}
			if (minIdx != i) {
				std::swap(arr[i], arr[minIdx]);
				summary.swaps++;
			}
		}

		double end = NowMs();
		summary.timeSpentMs = end - start;

		// cache sorted array for binary search (overwrite previous)
		if (sortedArray) delete[] sortedArray;
		sortedArraySize = originalArraySize;
		sortedArray = new TBankAccount*[sortedArraySize];
		for (int i = 0; i < sortedArraySize; ++i) sortedArray[i] = arr[i];
		isArraySorted = true;

		return arr; // caller must delete[] returned array
	}

	// Selection sort on linked list: build pointer vector then selection sort
	// Complexity: Best/Avg/Worst O(n^2). Space O(n).
	TLinkedList<TBankAccount>* SelectionSortList(FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		double start = NowMs();

		// gather pointers from list
		std::vector<TBankAccount*> vec;
		for (auto it = originalList->begin(); it != originalList->end(); ++it) vec.push_back(*it);

		int n = (int)vec.size();
		for (int i = 0; i < n - 1; ++i) {
			int minIdx = i;
			for (int j = i + 1; j < n; ++j) {
				summary.comparisons++;
				if (cmp(vec[j], vec[minIdx]) < 0) minIdx = j;
			}
			if (minIdx != i) {
				std::swap(vec[i], vec[minIdx]);
				summary.swaps++;
			}
		}

		TLinkedList<TBankAccount>* result = new TLinkedList<TBankAccount>(false);
		for (auto p : vec) result->add(p);

		double end = NowMs();
		summary.timeSpentMs = end - start;
		return result; // caller must delete result (ownsData=false)
	}

	// Bubble sort on array (pointer array)
	// Complexity: Best O(n) (already sorted), Avg/Worst O(n^2). Space O(n) for copy.
	TBankAccount** BubbleSortArray(FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		double start = NowMs();

		TBankAccount** arr = new TBankAccount*[originalArraySize];
		for (int i = 0; i < originalArraySize; ++i) arr[i] = originalArray[i];

		bool swapped;
		for (int pass = 0; pass < originalArraySize - 1; ++pass) {
			swapped = false;
			for (int j = 0; j < originalArraySize - 1 - pass; ++j) {
				summary.comparisons++;
				if (cmp(arr[j+1], arr[j]) < 0) {
					std::swap(arr[j], arr[j+1]);
					summary.swaps++;
					swapped = true;
				}
			}
			if (!swapped) break;
		}

		double end = NowMs();
		summary.timeSpentMs = end - start;

		// cache sorted array
		if (sortedArray) delete[] sortedArray;
		sortedArraySize = originalArraySize;
		sortedArray = new TBankAccount*[sortedArraySize];
		for (int i = 0; i < sortedArraySize; ++i) sortedArray[i] = arr[i];
		isArraySorted = true;

		return arr;
	}

	// QuickSort (array) public/private recursion with Partition helper
	// Complexity: Best O(n log n), Avg O(n log n), Worst O(n^2) (bad pivot). Space O(log n) stack.
	TBankAccount** QuickSortArray(FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		double start = NowMs();

		TBankAccount** arr = new TBankAccount*[originalArraySize];
		for (int i = 0; i < originalArraySize; ++i) arr[i] = originalArray[i];

		QuickSortRecursive(arr, 0, originalArraySize - 1, cmp, summary);

		double end = NowMs();
		summary.timeSpentMs = end - start;

		// cache sorted array
		if (sortedArray) delete[] sortedArray;
		sortedArraySize = originalArraySize;
		sortedArray = new TBankAccount*[sortedArraySize];
		for (int i = 0; i < sortedArraySize; ++i) sortedArray[i] = arr[i];
		isArraySorted = true;

		return arr;
	}

private:
	void QuickSortRecursive(TBankAccount** arr, int left, int right, FCompareAccounts cmp, OperationSummary& summary) {
		if (left >= right) return;
		int pivotIndex = Partition(arr, left, right, cmp, summary);
		QuickSortRecursive(arr, left, pivotIndex - 1, cmp, summary);
		QuickSortRecursive(arr, pivotIndex + 1, right, cmp, summary);
	}

	int Partition(TBankAccount** arr, int left, int right, FCompareAccounts cmp, OperationSummary& summary) {
		// Lomuto partition picking rightmost as pivot
		TBankAccount* pivot = arr[right];
		int i = left - 1;
		for (int j = left; j < right; ++j) {
			summary.comparisons++;
			if (cmp(arr[j], pivot) <= 0) {
				++i;
				if (i != j) { std::swap(arr[i], arr[j]); summary.swaps++; }
			}
		}
		if (i+1 != right) { std::swap(arr[i+1], arr[right]); summary.swaps++; }
		return i+1;
	}

public:
	// Merge sort on linked list. We'll implement via pointer array (stable merge) but use recursive public/private pattern.
	// Complexity: Best/Average/Worst O(n log n). Space O(n) for auxiliary arrays.
	TLinkedList<TBankAccount>* MergeSortList(FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		double start = NowMs();

		// collect pointers
		std::vector<TBankAccount*> vec;
		for (auto it = originalList->begin(); it != originalList->end(); ++it) vec.push_back(*it);

		MergeSortRecursive(vec, 0, (int)vec.size() - 1, cmp, summary);

		TLinkedList<TBankAccount>* result = new TLinkedList<TBankAccount>(false);
		for (auto p : vec) result->add(p);

		double end = NowMs();
		summary.timeSpentMs = end - start;
		return result;
	}

private:
	void MergeSortRecursive(std::vector<TBankAccount*>& vec, int left, int right, FCompareAccounts cmp, OperationSummary& summary) {
		if (left >= right) return;
		int mid = left + (right - left) / 2;
		MergeSortRecursive(vec, left, mid, cmp, summary);
		MergeSortRecursive(vec, mid+1, right, cmp, summary);
		Merge(vec, left, mid, right, cmp, summary);
	}

	void Merge(std::vector<TBankAccount*>& vec, int left, int mid, int right, FCompareAccounts cmp, OperationSummary& summary) {
		int n1 = mid - left + 1;
		int n2 = right - mid;
		std::vector<TBankAccount*> L(n1), R(n2);
		for (int i = 0; i < n1; ++i) L[i] = vec[left + i];
		for (int j = 0; j < n2; ++j) R[j] = vec[mid + 1 + j];

		int i = 0, j = 0, k = left;
		while (i < n1 && j < n2) {
			summary.comparisons++;
			if (cmp(L[i], R[j]) <= 0) { vec[k++] = L[i++]; }
			else { vec[k++] = R[j++]; }
		}
		while (i < n1) vec[k++] = L[i++];
		while (j < n2) vec[k++] = R[j++];
	}

public:
	// Binary search on cached sorted array. Public/private recursion pattern.
	// Requires that one of the array-sorting methods was called earlier (isArraySorted == true).
	TBankAccount* BinarySearch(TBankAccount* key, FCompareAccounts cmp, OperationSummary& summary) {
		summary = OperationSummary();
		if (!isArraySorted || !sortedArray) return nullptr; // not sorted
		double start = NowMs();
		TBankAccount* found = BinarySearchRecursive(0, sortedArraySize - 1, key, cmp, summary);
		double end = NowMs();
		summary.timeSpentMs = end - start;
		return found;
	}

private:
	TBankAccount* BinarySearchRecursive(int left, int right, TBankAccount* key, FCompareAccounts cmp, OperationSummary& summary) {
		if (left > right) return nullptr;
		int mid = left + (right - left) / 2;
		summary.comparisons++;
		int c = cmp(sortedArray[mid], key);
		if (c == 0) return sortedArray[mid];
		if (c < 0) return BinarySearchRecursive(mid + 1, right, key, cmp, summary);
		return BinarySearchRecursive(left, mid - 1, key, cmp, summary);
	}
};

// Linear search (from Assignment 4) to compare comparisons count
// Returns pointer and populates OperationSummary.comparisons
TBankAccount* LinearSearchArrayByLastName(TBankAccount** accountArray, int arraySize, const std::string& lastName, OperationSummary& summary) {
	summary = OperationSummary();
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < arraySize; ++i) {
		summary.comparisons++;
		if (accountArray[i] && accountArray[i]->ownerLastName == lastName) {
			auto end = std::chrono::high_resolution_clock::now();
			summary.timeSpentMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
			return accountArray[i];
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	summary.timeSpentMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
	return nullptr;
}

int main() {
	std::cout << "=== Assignment 5: Sorting Toolkit & Performance Battle ===\n";

	// Setup RNG and create data structures (reuse the same approach as assignment 4)
	std::random_device rd; std::mt19937 gen(rd());
	TLinkedList<TBankAccount> accountList(true);
	const int maxAccounts = 6000;
	TBankAccount** accountArray = new TBankAccount*[maxAccounts];
	int arraySize = 0;

	std::uniform_int_distribution<> nameFirstDis(0, (int)firstNames.size() - 1);
	std::uniform_int_distribution<> nameLastDis(0, (int)lastNames.size() - 1);
	std::uniform_int_distribution<> accountsPerNameDis(5, 10);

	for (int i = 0; i < 700 && arraySize < maxAccounts; ++i) {
		std::string firstName = firstNames[nameFirstDis(gen)];
		std::string lastName = lastNames[nameLastDis(gen)];
		int accountsForThisName = accountsPerNameDis(gen);
		for (int j = 0; j < accountsForThisName && arraySize < maxAccounts; ++j) {
			std::string accountNumber = GenerateAccountNumber(gen);
			EBankAccountType accountType = GenerateRandomAccountType(gen);
			time_t timestamp = GenerateRandomTimestamp(gen);
			TBankAccount* account = new TBankAccount(accountNumber, accountType, firstName, lastName, timestamp);
			accountList.add(account);
			accountArray[arraySize++] = account;
		}
	}

	std::cout << "Generated " << arraySize << " accounts." << std::endl;

	// Create TSort instance
	TSort sorter(&accountList, accountArray, arraySize);

	// We'll run all sorts by last name to compare
	std::cout << "\nRunning sorts by last name..." << std::endl;

	OperationSummary sSelectionArr, sSelectionList, sBubbleArr, sQuickArr, sMergeList;

	TBankAccount** selArr = sorter.SelectionSortArray(CompareByLastName, sSelectionArr);
	TLinkedList<TBankAccount>* selList = sorter.SelectionSortList(CompareByLastName, sSelectionList);
	TBankAccount** bubArr = sorter.BubbleSortArray(CompareByLastName, sBubbleArr);
	TBankAccount** quickArr = sorter.QuickSortArray(CompareByLastName, sQuickArr);
	TLinkedList<TBankAccount>* mergeList = sorter.MergeSortList(CompareByLastName, sMergeList);

	// Print a summary table
	std::cout << "\nSort\t\tComparisons\tSwaps\tTime(ms)\n";
	std::cout << "SelectionArray\t" << sSelectionArr.comparisons << "\t\t" << sSelectionArr.swaps << "\t" << sSelectionArr.timeSpentMs << "\n";
	std::cout << "SelectionList\t" << sSelectionList.comparisons << "\t\t" << sSelectionList.swaps << "\t" << sSelectionList.timeSpentMs << "\n";
	std::cout << "BubbleArray\t" << sBubbleArr.comparisons << "\t\t" << sBubbleArr.swaps << "\t" << sBubbleArr.timeSpentMs << "\n";
	std::cout << "QuickArray\t" << sQuickArr.comparisons << "\t\t" << sQuickArr.swaps << "\t" << sQuickArr.timeSpentMs << "\n";
	std::cout << "MergeList\t" << sMergeList.comparisons << "\t\t" << sMergeList.swaps << "\t" << sMergeList.timeSpentMs << "\n";

	// Demonstrate binary search vs linear search comparisons
	// Pick a target account from array (middle)
	TBankAccount* target = accountArray[arraySize/2];
	std::string targetLast = target->ownerLastName;

	// Linear search comparisons
	OperationSummary linSummary;
	TBankAccount* foundLin = LinearSearchArrayByLastName(accountArray, arraySize, targetLast, linSummary);

	// Ensure sorter has a sorted array (it does after previous sorts). We'll use the cached sorted array via sorter.BinarySearch
	OperationSummary binSummary;
	TBankAccount* foundBin = sorter.BinarySearch(target, CompareByLastName, binSummary);

	std::cout << "\nSearch comparisons to find last name '" << targetLast << "':\n";
	std::cout << "Linear search comparisons: " << linSummary.comparisons << ", time(ms): " << linSummary.timeSpentMs << "\n";
	std::cout << "Binary search comparisons: " << binSummary.comparisons << ", time(ms): " << binSummary.timeSpentMs << "\n";

	// Cleanup returned/allocated arrays and lists
	delete[] selArr; delete selList; delete[] bubArr; delete[] quickArr; delete mergeList;
	delete[] accountArray; // accountList owns data and will delete in destructor

	std::cout << "\nDone. Results show O(n^2) sorts cost far more comparisons/time than O(n log n) sorts." << std::endl;
	return 0;
}


