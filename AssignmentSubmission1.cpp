
// ...existing code...



#include <string>
#include <iostream>
#include <functional>

// Enum for movie genres using bitwise flags
enum EMovieGenreType {
	ACTION = 1 << 0,
	COMEDY = 1 << 1,
	SCIFI  = 1 << 2,
	HORROR = 1 << 3,
	DRAMA  = 1 << 4
};

// Movie class
class TMovie {
private:
	std::string title;
	std::string director;
	int year;
	EMovieGenreType genre;
	float score;
public:
	TMovie(const std::string& title, const std::string& director, int year, EMovieGenreType genre, float score)
		: title(title), director(director), year(year), genre(genre), score(score) {}

	std::string GetTitle() const { return title; }
	std::string GetDirector() const { return director; }
	int GetYear() const { return year; }
	EMovieGenreType GetGenre() const { return genre; }
	float GetScore() const { return score; }
};

// Node class for doubly linked list
class TMovieNode {
private:
	TMovie* movie;
	TMovieNode* next;
	TMovieNode* prev;
public:
	TMovieNode(TMovie* movie) : movie(movie), next(nullptr), prev(nullptr) {}
	~TMovieNode() { delete movie; }

	TMovie* GetMovie() const { return movie; }
	TMovieNode* GetNext() const { return next; }
	TMovieNode* GetPrev() const { return prev; }
	void SetNext(TMovieNode* node) { next = node; }
	void SetPrev(TMovieNode* node) { prev = node; }
};

// Typedef for search callback
typedef bool (*FCheckMovie)(const TMovie*);

// Movie list class (doubly linked list with dummy head and tail)
class TMovieList {
private:
	TMovieNode* head; // Dummy node
	TMovieNode* tail; // Points to last real node, or head if empty
public:
	TMovieList() {
		head = new TMovieNode(nullptr); // Dummy node
		tail = head;
	}
	~TMovieList() {
		TMovieNode* current = head;
		while (current) {
			TMovieNode* next = current->GetNext();
			delete current;
			current = next;
		}
	}

	// Append a movie to the end (O(1))
	void Append(TMovie* movie) {
		TMovieNode* node = new TMovieNode(movie);
		node->SetPrev(tail);
		tail->SetNext(node);
		tail = node;
	}

	// Prepend a movie after the dummy node (O(1))
	void Prepend(TMovie* movie) {
		TMovieNode* node = new TMovieNode(movie);
		TMovieNode* first = head->GetNext();
		node->SetNext(first);
		node->SetPrev(head);
		head->SetNext(node);
		if (first) {
			first->SetPrev(node);
		} else {
			tail = node;
		}
	}

	// Get movie at index (0-based, not counting dummy)
	TMovie* GetAtIndex(int index) const {
		TMovieNode* current = head->GetNext();
		int i = 0;
		while (current) {
			if (i == index) return current->GetMovie();
			current = current->GetNext();
			++i;
		}
		return nullptr;
	}

	// Remove node at index (0-based, not counting dummy)
	void Remove(int index) {
		TMovieNode* current = head->GetNext();
		int i = 0;
		while (current) {
			if (i == index) {
				TMovieNode* prev = current->GetPrev();
				TMovieNode* next = current->GetNext();
				if (prev) prev->SetNext(next);
				if (next) next->SetPrev(prev);
				if (current == tail) tail = prev;
				current->SetNext(nullptr);
				current->SetPrev(nullptr);
				delete current;
				return;
			}
			current = current->GetNext();
			++i;
		}
	}

	// Reverse the list in-place (excluding dummy)
	void Reverse() {
		TMovieNode* current = head->GetNext();
		TMovieNode* prevNode = nullptr;
		tail = current ? current : head;
		while (current) {
			TMovieNode* next = current->GetNext();
			current->SetNext(prevNode);
			current->SetPrev(next);
			prevNode = current;
			current = next;
		}
		head->SetNext(prevNode);
		if (prevNode) prevNode->SetPrev(head);
	}

	// Search for a movie using a callback
	TMovie* SearchFor(FCheckMovie check) const {
		TMovieNode* current = head->GetNext();
		while (current) {
			TMovie* movie = current->GetMovie();
			if (movie && check(movie)) {
				return movie;
			}
			current = current->GetNext();
		}
		return nullptr;
	}
};

// Global search functions
bool SearchByTitle(const TMovie* movie) {
	// Example: search for title "Inception"
	return movie->GetTitle() == "Inception";
}

bool SearchByDirector(const TMovie* movie) {
	// Example: search for director "Nolan"
	return movie->GetDirector() == "Nolan";
}

bool SearchByGenre(const TMovie* movie) {
	// Example: search for ACTION genre
	return (movie->GetGenre() & ACTION) != 0;
}

// Example usage and test code
int main() {
	// Create a movie list
	TMovieList movieList;

	// Add movies
	movieList.Append(new TMovie("Inception", "Nolan", 2010, (EMovieGenreType)(ACTION | SCIFI), 8.8f));
	movieList.Append(new TMovie("The Godfather", "Coppola", 1972, DRAMA, 9.2f));
	movieList.Prepend(new TMovie("Ghostbusters", "Reitman", 1984, (EMovieGenreType)(COMEDY | SCIFI), 7.8f));

	// Print all movies
	std::cout << "All movies in list:" << std::endl;
	for (int i = 0; ; ++i) {
		TMovie* m = movieList.GetAtIndex(i);
		if (!m) break;
		std::cout << i << ": " << m->GetTitle() << " (" << m->GetDirector() << ", " << m->GetYear() << ") Score: " << m->GetScore() << std::endl;
	}

	// Remove the second movie
	movieList.Remove(1);
	std::cout << "\nAfter removing index 1:" << std::endl;
	for (int i = 0; ; ++i) {
		TMovie* m = movieList.GetAtIndex(i);
		if (!m) break;
		std::cout << i << ": " << m->GetTitle() << std::endl;
	}

	// Reverse the list
	movieList.Reverse();
	std::cout << "\nAfter reversing:" << std::endl;
	for (int i = 0; ; ++i) {
		TMovie* m = movieList.GetAtIndex(i);
		if (!m) break;
		std::cout << i << ": " << m->GetTitle() << std::endl;
	}

	// Search for a movie by title
	TMovie* found = movieList.SearchFor(SearchByTitle);
	if (found) {
		std::cout << "\nFound by title: " << found->GetTitle() << std::endl;
	}

	// Search for a movie by director
	found = movieList.SearchFor(SearchByDirector);
	if (found) {
		std::cout << "Found by director: " << found->GetTitle() << std::endl;
	}

	// Search for a movie by genre
	found = movieList.SearchFor(SearchByGenre);
	if (found) {
		std::cout << "Found by genre: " << found->GetTitle() << std::endl;
	}

	return 0;
}

