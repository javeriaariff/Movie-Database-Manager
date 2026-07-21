#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

string cleanString(string str) {
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 32 && str[i] <= 126) {
            result += str[i];                     //using acii
        }
        // Also keep tabs
        else if (str[i] == '\t') {
            result += ' ';
        }
    }
    return result;
}

string trim(string str) {
    if (str.empty()) return str;

    int start = 0;
    int end = str.length() - 1;

    // Trim from start
    while (start <= end &&                        //asic function for removing quotes waghera
        (str[start] == ' ' || str[start] == '\t' ||
            str[start] == '\n' || str[start] == '\r' ||
            str[start] == '"')) {
        start++;
    }

    // Trim from end
    while (end >= start &&
        (str[end] == ' ' || str[end] == '\t' ||
            str[end] == '\n' || str[end] == '\r' ||
            str[end] == '"')) {
        end--;
    }

    if (start > end) return "";
    return str.substr(start, end - start + 1);              //return the trimmed string
}

char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

string toLowerString(string str) {
    string result = str;
    for (int i = 0; i < result.length(); i++) {
        result[i] = toLowerChar(result[i]);
    }
    return result;
}

string capitalizeFirst(string str) {
    if (str.empty()) return str;

    string result = str;

    if (result[0] >= 'a' && result[0] <= 'z') {
        result[0] = result[0] - 32;
    }

    for (int i = 1; i < result.length(); i++) {
        if (result[i - 1] == ' ') {
            if (result[i] >= 'a' && result[i] <= 'z') {
                result[i] = result[i] - 32;
            }
        }
        else if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32;
        }
    }

    return result;
}

int stringToInt(string str) {
    if (str.empty()) return 0;

    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        }
    }
    return result;
}

double stringToDouble(string str) {
    if (str.empty()) return 0.0;

    double result = 0.0;
    double decimal = 0.0;
    bool foundDecimal = false;
    double decimalMultiplier = 0.1;

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '.') {
            foundDecimal = true;
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            if (!foundDecimal) {
                result = result * 10 + (str[i] - '0');
            }
            else {
                decimal += (str[i] - '0') * decimalMultiplier;
                decimalMultiplier *= 0.1;
            }
        }
    }

    return result + decimal;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

struct MovieNode {
    string title;
    string genre;
    string director;
    string actor1;
    string actor2;
    double rating;
    int year;
    int duration;

    MovieNode* next;

    MovieNode() : rating(0.0), year(0), duration(0), next(nullptr) {}

    bool actorfound(const string& name) {
        string lowerActor = toLowerString(name);                 //if either actor 1 or 2 is found then trye
        return (toLowerString(actor1) == lowerActor || toLowerString(actor2) == lowerActor);
    }

    bool directorfound(const string& Name) {
        return toLowerString(director) == toLowerString(Name);
    }

    string getCleanTitle() {            //formatting
        return cleanString(trim(title));
    }

    bool sharesActorWith(MovieNode* other) {
        if (!other) return false;
        return (actorfound(other->actor1) || actorfound(other->actor2) ||
            other->actorfound(actor1) || other->actorfound(actor2));
    }

    bool sharesDirectorWith(MovieNode* other) {
        if (!other) return false;
        return directorfound(other->director);
    }

    bool sharesGenreWith(MovieNode* other) {
        if (!other || genre.empty() || other->genre.empty()) return false;

        // Parse genres from this movie
        string current = "";
        for (int i = 0; i < genre.length(); i++) {
            if (genre[i] == '|') {
                string thisGenre = toLowerString(trim(current));
                string second = "";                    //check all possible genres
                for (int j = 0; j < other->genre.length(); j++) {
                    if (other->genre[j] == '|') {
                        if (thisGenre == toLowerString(trim(second))) {
                            return true;
                        }
                        second = "";
                    }
                    else {
                        second += other->genre[j];
                    }
                }
                // Check last genre
                if (thisGenre == toLowerString(trim(second))) {
                    return true;
                }
                current = "";
            }
            else {
                current += genre[i];
            }
        }
        // Check last genre
        string thisGenre = toLowerString(trim(current));
        if (!thisGenre.empty()) {
            string second = "";
            for (int j = 0; j < other->genre.length(); j++) {
                if (other->genre[j] == '|') {
                    if (thisGenre == toLowerString(trim(second))) {
                        return true;
                    }
                    second = "";
                }
                else {
                    second += other->genre[j];
                }
            }
            if (thisGenre == toLowerString(trim(second))) {
                return true;
            }
        }
        return false;
    }
};

class MovieList {
private:
    MovieNode* head;
    int size;

public:
    MovieList() : head(nullptr), size(0) {}

    ~MovieList() {
        MovieNode* current = head;
        while (current) {
            MovieNode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(MovieNode* movie) {
        if (!head) {
            head = movie;
        }
        else {
            if (toLowerString(movie->getCleanTitle()) < toLowerString(head->getCleanTitle())) {                 //sorting alphabetically
                movie->next = head;
                head = movie;
            }
            else {
                MovieNode* current = head;
                while (current->next &&
                    toLowerString(current->next->getCleanTitle()) < toLowerString(movie->getCleanTitle())) {
                    current = current->next;
                }
                movie->next = current->next;
                current->next = movie;
            }
        }
        size++;
    }

    MovieNode* search(const string& title) {
        MovieNode* current = head;
        string searchTitle = toLowerString(cleanString(trim(title)));

        while (current) {
            if (toLowerString(current->getCleanTitle()) == searchTitle) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void display(int limit = 15) {
        if (size == 0) {
            cout << "No movies found." << endl;
            return;
        }

        cout << "\n" << string(60, '=') << endl;
        cout << "MOVIE LIST (" << min(limit, size) << " of " << size << " movies)" << endl;                 //basic formatting
        cout << string(60, '=') << endl;

        MovieNode* current = head;
        int count = 0;

        while (current && count < limit) {
            cout << setw(3) << count + 1 << ". " << setw(40) << left << current->getCleanTitle().substr(0, 39)
                << " Year: " << setw(4) << current->year
                << " Rating: " << fixed << setprecision(1) << current->rating << endl;
            current = current->next;
            count++;
        }
    }

    int getSize() { return size; }
    MovieNode* getHead() { return head; }

    MovieNode** getarr(int& count) {
        if (size == 0) {
            count = 0;
            return nullptr;
        }

        MovieNode** array = new MovieNode * [size];
        MovieNode* current = head;
        count = 0;

        while (current && count < size) {
            array[count] = current;
            current = current->next;
            count++;
        }

        return array;
    }

    // Find movies by actor
    MovieNode** actorwise(const string& actor, int& resultCount) {
        MovieNode** results = new MovieNode * [size];
        resultCount = 0;
        MovieNode* current = head;

        while (current) {
            if (current->actorfound(actor)) {
                results[resultCount++] = current;
            }
            current = current->next;
        }

        return results;
    }

    // Find movies by director
    MovieNode** directorwise(const string& director, int& Count) {
        MovieNode** results = new MovieNode * [size];
        Count = 0;
        MovieNode* current = head;
        string search = toLowerString(director);

        while (current) {
            if (toLowerString(current->director) == search) {
                results[Count++] = current;
            }
            current = current->next;
        }

        return results;
    }
};

struct titlenode {
    string title;
    titlenode* next;

    titlenode(const string& t) : title(t), next(nullptr) {}
};

class titlelist {
private:
    titlenode* head;
    int size;

public:
    titlelist() : head(nullptr), size(0) {}

    ~titlelist() {
        titlenode* current = head;
        while (current) {
            titlenode* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(const string& title) {
        titlenode* newNode = new titlenode(title);

        if (!head || title < head->title) {
            newNode->next = head;
            head = newNode;
        }
        else {
            titlenode* current = head;
            while (current->next && current->next->title < title) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }

    void display(int limit = 15) {
        if (size == 0) {
            cout << "No movies " << endl;
            return;
        }

        cout << "\n" << string(60, '=') << endl;
        cout << "found (" << size << " total)" << endl;
        cout << string(60, '=') << endl;

        titlenode* current = head;
        int count = 0;

        while (current && count < limit) {
            cout << "  " << count + 1 << ". " << current->title << endl;
            current = current->next;
            count++;
        }

    }

    int getSize() { return size; }
    titlenode* getHead() { return head; }

    string* titlearr(int& count) {
        if (size == 0) {
            count = 0;
            return nullptr;
        }

        string* array = new string[size];
        titlenode* current = head;
        count = 0;

        while (current && count < size) {
            array[count] = current->title;
            current = current->next;
            count++;
        }

        return array;
    }
};

class ActorHashTable {
private:
    static const int TABLE_SIZE = 200;

    struct ActorNode {
        string name;
        titlelist* movies;
        ActorNode* next;

        ActorNode(const string& name) : name(name), next(nullptr) {
            movies = new titlelist();
        }

        ~ActorNode() {
            delete movies;
        }
    };

    ActorNode* hashtable[TABLE_SIZE];

    int hashFunction(const string& key) {
        string lowerKey = toLowerString(key);
        int hash = 0;
        for (int i = 0; i < lowerKey.length(); i++) {                   //hash function for storing name
            hash = (hash * 31 + lowerKey[i]) % TABLE_SIZE;
        }
        return hash;
    }

    bool compareNames(const string& name1, const string& name2) {                                  //comparing both actor names
        return toLowerString(name1) == toLowerString(name2);
    }

public:
    ActorHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashtable[i] = nullptr;
        }
    }

    ~ActorHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            ActorNode* current = hashtable[i];
            while (current) {
                ActorNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const string& name, const string& movieTitle) {
        if (name.empty()) return;

        int index = hashFunction(name);
        ActorNode* current = hashtable[index];

        // Check if actor alread exists
        while (current) {
            if (compareNames(current->name, name)) {
                current->movies->insert(movieTitle);
                return;
            }
            current = current->next;
        }


        ActorNode* newNode = new ActorNode(capitalizeFirst(name));
        newNode->movies->insert(movieTitle);
        newNode->next = hashtable[index];                     //new
        hashtable[index] = newNode;
    }

    titlelist* search(const string& name) {
        if (name.empty()) return nullptr;

        // Try exact match first
        int index = hashFunction(name);
        ActorNode* current = hashtable[index];

        while (current) {
            if (compareNames(current->name, name)) {
                return current->movies;
            }
            current = current->next;
        }
        string properCase = capitalizeFirst(name);                 //check if capitaliztion is problem
        if (properCase != name) {
            index = hashFunction(properCase);
            current = hashtable[index];

            while (current) {
                if (compareNames(current->name, properCase)) {
                    return current->movies;
                }
                current = current->next;
            }
        }

        string trimmed = trim(name);
        if (trimmed != name) {
            index = hashFunction(trimmed);                   //removal of spaces// smae search
            current = hashtable[index];

            while (current) {
                if (compareNames(current->name, trimmed)) {
                    return current->movies;
                }
                current = current->next;
            }
        }

        return nullptr;
    }

    bool actorfound(const string& name) {
        return search(name) != nullptr;
    }
};

class DirectorHashTable {
private:
    static const int TABLE_SIZE = 200;

    struct DirectorNode {
        string directorName;
        titlelist* movies;
        DirectorNode* next;

        DirectorNode(const string& name) : directorName(name), next(nullptr) {
            movies = new titlelist();
        }

        ~DirectorNode() {
            delete movies;
        }
    };

    DirectorNode* hashtable[TABLE_SIZE];

    int hashFunction(const string& key) {
        string lowerKey = toLowerString(key);
        int hash = 0;
        for (int i = 0; i < lowerKey.length(); i++) {                //same as actor
            hash = (hash * 31 + lowerKey[i]) % TABLE_SIZE;
        }
        return hash;
    }

    bool compareNames(const string& name1, const string& name2) {
        return toLowerString(name1) == toLowerString(name2);
    }

public:
    DirectorHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            hashtable[i] = nullptr;
        }
    }

    ~DirectorHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            DirectorNode* current = hashtable[i];
            while (current) {
                DirectorNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const string& name, const string& title) {
        if (name.empty()) return;

        int i = hashFunction(name);            //basic inseruon
        DirectorNode* current = hashtable[i];

        while (current) {
            if (compareNames(current->directorName, name)) {
                current->movies->insert(title);
                return;
            }
            current = current->next;
        }

        DirectorNode* NN = new DirectorNode(capitalizeFirst(name));
        NN->movies->insert(title);
        NN->next = hashtable[i];
        hashtable[i] = NN;
    }

    titlelist* search(const string& name) {
        if (name.empty()) return nullptr;

        int i = hashFunction(name);
        DirectorNode* current = hashtable[i];

        while (current) {
            if (compareNames(current->directorName, name)) {
                return current->movies;                                //compare with every dirextor
            }
            current = current->next;
        }
        return nullptr;
    }

    bool directorfound(const string& directorName) {
        return search(directorName) != nullptr;
    }
};

class GenreHashTable {
private:
    static const int TABLE_SIZE = 100;

    struct GenreNode {
        string genreName;
        titlelist* movies;
        GenreNode* next;

        GenreNode(const string& name) : genreName(name), next(nullptr) {
            movies = new titlelist();
        }

        ~GenreNode() {
            delete movies;
        }
    };

    GenreNode* table[TABLE_SIZE];

    int hashFunction(const string& key) {
        string lowerKey = toLowerString(key);
        int hash = 0;
        for (int i = 0; i < lowerKey.length(); i++) {
            hash = (hash * 31 + lowerKey[i]) % TABLE_SIZE;
        }                                        //using 31 as m
        return hash;
    }

    bool compareGenres(const string& genre1, const string& genre2) {
        return toLowerString(genre1) == toLowerString(genre2);
    }

public:
    GenreHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~GenreHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            GenreNode* current = table[i];
            while (current) {
                GenreNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const string& genre, const string& title) {
        if (genre.empty()) return;

        string currentGenre = "";
        for (int i = 0; i < genre.length(); i++) {
            if (genre[i] == '|') {                           //checking if its spearated by |
                if (!currentGenre.empty()) {
                    insertSingleGenre(trim(currentGenre), title);
                }
                currentGenre = "";
            }
            else {
                currentGenre += genre[i];
            }
        }

        if (!currentGenre.empty()) {
            insertSingleGenre(trim(currentGenre), title);
        }
    }

private:
    void insertSingleGenre(const string& genre, const string& title) {
        int i = hashFunction(genre);
        GenreNode* current = table[i];

        while (current) {
            if (compareGenres(current->genreName, genre)) {
                current->movies->insert(title);
                return;
            }
            current = current->next;
        }

        GenreNode* newNode = new GenreNode(genre);
        newNode->movies->insert(title);
        newNode->next = table[i];
        table[i] = newNode;
    }

public:
    titlelist* search(const string& genre) {
        if (genre.empty()) return nullptr;

        int index = hashFunction(genre);
        GenreNode* current = table[index];

        while (current) {
            if (compareGenres(current->genreName, genre)) {
                return current->movies;
            }
            current = current->next;
        }
        return nullptr;
    }
};

class findshortestpath {
private:
    MovieNode** allMovies;
    int movieCount;

    struct PathNode {
        MovieNode* movie;
        PathNode* prev;
        int distance;

        PathNode(MovieNode* m, PathNode* p, int d) : movie(m), prev(p), distance(d) {}
    };

    struct QueueNode {
        PathNode* pathNode;
        QueueNode* next;

        QueueNode(PathNode* pn) : pathNode(pn), next(nullptr) {}
    };

    class SimpleQueue {
    private:
        QueueNode* front;
        QueueNode* rear;

    public:
        SimpleQueue() : front(nullptr), rear(nullptr) {}

        ~SimpleQueue() {
            while (front) {
                QueueNode* temp = front;
                front = front->next;
                delete temp;
            }
        }

        void enqueue(PathNode* pathNode) {
            QueueNode* newNode = new QueueNode(pathNode);
            if (!rear) {
                front = rear = newNode;
            }
            else {
                rear->next = newNode;
                rear = newNode;
            }
        }

        PathNode* dequeue() {
            if (!front) return nullptr;
            QueueNode* temp = front;
            PathNode* result = temp->pathNode;
            front = front->next;
            if (!front) rear = nullptr;
            delete temp;
            return result;
        }

        bool isEmpty() {
            return front == nullptr;
        }
    };

    int findMovieIndex(MovieNode* movie) {
        for (int i = 0; i < movieCount; i++) {
            if (allMovies[i] == movie) {
                return i;
            }
        }
        return -1;
    }

    bool areConnected(MovieNode* movie1, MovieNode* movie2) {
        return (movie1->sharesActorWith(movie2) ||
            movie1->sharesDirectorWith(movie2) ||
            movie1->sharesGenreWith(movie2));
    }

public:
    findshortestpath(MovieNode** movies, int count) : allMovies(movies), movieCount(count) {}

    // Find shortest path between two movies
    void movietomovie(const string& movie1, const string& movie2, MovieList& movieList) {
        MovieNode* startMovie = movieList.search(movie1);
        MovieNode* endMovie = movieList.search(movie2);

        if (!startMovie) {
            cout << "Movie not found: " << movie1 << endl;
            return;
        }
        if (!endMovie) {
            cout << "Movie not found: " << movie2 << endl;
            return;
        }
        if (startMovie == endMovie) {
            cout << "Both movies are the same!" << endl;
            return;
        }

        // BFS to find shortest path
        bool* visited = new bool[movieCount];
        PathNode** pathn = new PathNode * [movieCount];
        for (int i = 0; i < movieCount; i++) {
            visited[i] = false;
            pathn[i] = nullptr;
        }

        SimpleQueue queue;
        int start = findMovieIndex(startMovie);
        visited[start] = true;                        //start saving the path in queue
        queue.enqueue(new PathNode(startMovie, nullptr, 0));
        pathn[start] = new PathNode(startMovie, nullptr, 0);

        bool found = false;
        PathNode* end = nullptr;

        while (!queue.isEmpty() && !found) {                       //by the end the queue is showing the shortest path
            PathNode* current = queue.dequeue();

            // Check all movies for connections
            for (int i = 0; i < movieCount && !found; i++) {
                if (!visited[i] && areConnected(current->movie, allMovies[i])) {
                    visited[i] = true;
                    PathNode* newPath = new PathNode(allMovies[i], current, current->distance + 1);
                    pathn[i] = newPath;
                    queue.enqueue(newPath);
                    //keep adding to queue the visited nodes
                    if (allMovies[i] == endMovie) {
                        found = true;
                        end = newPath;
                    }
                }
            }
        }

        if (found && end) {
            cout << "\n" << string(60, '=') << endl;
            cout << "shortest path:" << endl;
            cout << "From: " << startMovie->title << endl;
            cout << "To: " << endMovie->title << endl;
            cout << "Distance: " << end->distance << " connections" << endl;

            // Reconstruct and display path
            PathNode* current = end;
            string* path = new string[end->distance + 1];                                 //shows teh reconstructed meaning smallest path, removes all duplicates
            int pathIndex = end->distance;

            while (current) {
                path[pathIndex--] = current->movie->title;
                current = current->prev;
            }

            for (int i = 0; i <= end->distance; i++) {
                cout << i + 1 << ". " << path[i];
                if (i < end->distance) {
                    // Show connection type
                    MovieNode* currentMovie = nullptr;
                    MovieNode* nextMovie = nullptr;

                    // Find current and next movies
                    for (int j = 0; j < movieCount; j++) {
                        if (allMovies[j]->title == path[i]) currentMovie = allMovies[j];
                        if (allMovies[j]->title == path[i + 1]) nextMovie = allMovies[j];
                        if (currentMovie && nextMovie) break;
                    }

                    if (currentMovie && nextMovie) {
                        if (currentMovie->sharesActorWith(nextMovie)) {
                            cout << " --[Shared Actor]--> ";
                        }
                        else if (currentMovie->sharesDirectorWith(nextMovie)) {
                            cout << " --[Shared Director]--> ";
                        }
                        else if (currentMovie->sharesGenreWith(nextMovie)) {
                            cout << " --[Shared Genre]--> ";
                        }
                        else {
                            cout << " --> ";
                        }
                    }
                    else {
                        cout << " --> ";
                    }
                }
            }
            cout << endl;

            delete[] path;
        }
        else {
            cout << "\nNo path found between " << movie1 << " and " << movie2 << endl;
        }

        // Cleanup
        delete[] visited;
        for (int i = 0; i < movieCount; i++) {
            if (pathn[i]) delete pathn[i];
        }
        delete[] pathn;
    }

    // Find shortest path between two actors
    void findActorToActorPath(const string& actor1, const string& actor2,
        MovieList& movieList, ActorHashTable& actorTable) {
        // Get movies for each actor
        int count1, count2;
        MovieNode** movies1 = movieList.actorwise(actor1, count1);
        MovieNode** movies2 = movieList.actorwise(actor2, count2);
        //same way but use actors using 2d arrays
        if (count1 == 0) {
            cout << "Actor not found: " << actor1 << endl;
            delete[] movies1;
            return;
        }
        if (count2 == 0) {
            cout << "Actor not found: " << actor2 << endl;
            delete[] movies1;
            delete[] movies2;
            return;
        }

        // Check if actors are in same movie
        for (int i = 0; i < count1; i++) {
            for (int j = 0; j < count2; j++) {
                if (movies1[i] == movies2[j]) {
                    cout << "shortets path between actors" << endl;
                    cout << string(60, '=') << endl;
                    cout << "From: " << actor1 << endl;
                    cout << "To: " << actor2 << endl;
                    cout << "Distance: 1 (appeared in same movie)" << endl;
                    cout << "1. " << actor1 << " --[Appeared in]--> " << movies1[i]->title
                        << " --[Appeared in]--> " << actor2 << endl;
                    delete[] movies1;                          //freeing up memory
                    delete[] movies2;
                    return;
                }
            }
        }

        // Find shortest path through movie connections
        bool* visited = new bool[movieCount];
        for (int i = 0; i < movieCount; i++) visited[i] = false;

        SimpleQueue queue;

        // Add all movies of actor1 to queue
        for (int i = 0; i < count1; i++) {
            int idx = findMovieIndex(movies1[i]);
            if (idx != -1) {
                visited[idx] = true;
                queue.enqueue(new PathNode(movies1[i], nullptr, 0));
            }
        }

        bool found = false;
        PathNode* end = nullptr;
        MovieNode* connection = nullptr;

        while (!queue.isEmpty() && !found) {
            PathNode* current = queue.dequeue();

            // Check if this movie has actor2
            if (current->movie->actorfound(actor2)) {
                found = true;
                end = current;
                connection = current->movie;
                break;
            }

            for (int i = 0; i < movieCount && !found; i++) {                         //checking for other movies
                if (!visited[i] && areConnected(current->movie, allMovies[i])) {
                    visited[i] = true;
                    queue.enqueue(new PathNode(allMovies[i], current, current->distance + 1));                  ///inset in queue again
                }
            }
        }

        if (found && end) {
            cout << "From: " << actor1 << endl;
            cout << "To: " << actor2 << endl;
            cout << "Distance: " << end->distance + 1 << " connections" << endl;

            // Reconstruct path
            string* path = new string[end->distance + 2];
            PathNode* current = end;
            int pathIndex = end->distance;

            while (current) {
                path[pathIndex--] = current->movie->title;
                current = current->prev;
            }

            // Display path
            cout << "1. " << actor1 << " --[Appeared in]--> " << path[0];
            for (int i = 0; i <= end->distance; i++) {
                if (i > 0) {
                    cout << " --Shared connection--> " << path[i];
                }
                if (i == end->distance) {
                    cout << " --[Appeared in]--> " << actor2;
                }
            }
            cout << endl;

            delete[] path;
        }
        else {
            cout << "\nNo path found between " << actor1 << " and " << actor2 << endl;
        }

        // Cleanup
        delete[] visited;
        delete[] movies1;
        delete[] movies2;
    }

    // Find shortest path between two directors
    void directortodirector(const string& dir1, const string& dir2,
        MovieList& movieList) {
        // Get movies for each director
        int count1, count2;
        MovieNode** movies1 = movieList.directorwise(dir1, count1);
        MovieNode** movies2 = movieList.directorwise(dir2, count2);

        if (count1 == 0) {
            cout << "Director not found: " << dir1 << endl;
            delete[] movies1;
            return;
        }
        if (count2 == 0) {
            cout << "Director not found: " << dir2 << endl;
            delete[] movies1;
            delete[] movies2;
            return;
        }

        // Check if directors worked with same actor or shared genre
        cout << "connextion between directors" << endl;
        cout << "From: " << dir1 << endl;
        cout << "To: " << dir2 << endl;

        // Find common actors
        bool found = false;
        for (int i = 0; i < count1 && !found; i++) {
            for (int j = 0; j < count2 && !found; j++) {
                // Check for shared actors
                if (movies1[i]->sharesActorWith(movies2[j])) {
                    cout << "\nConnection found through shared actor!" << endl;
                    cout << string(40, '-') << endl;
                    cout << "1. " << dir1 << " directed '" << movies1[i]->title << "'" << endl;
                    cout << "2. '" << movies1[i]->title << "' and '" << movies2[j]->title
                        << "' share an actor" << endl;
                    cout << "3. " << dir2 << " directed '" << movies2[j]->title << "'" << endl;
                    found = true;
                }
                // shared genres
                else if (movies1[i]->sharesGenreWith(movies2[j])) {
                    cout << "\nConnection found through shared genre!" << endl;
                    cout << string(40, '-') << endl;
                    cout << "1. " << dir1 << " directed '" << movies1[i]->title << "'" << endl;
                    cout << "2. '" << movies1[i]->title << "' and '" << movies2[j]->title
                        << "' share a genre" << endl;
                    cout << "3. " << dir2 << " directed '" << movies2[j]->title << "'" << endl;
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "\nNo direct connection ." << endl;
        }

        delete[] movies1;
        delete[] movies2;
    }
};


struct GraphNode {
    string movieTitle;
    string cleanTitle;
    int* connections;
    int count;
    int capacity;

    GraphNode() : connections(nullptr), count(0), capacity(10) {
        connections = new int[capacity];
    }

    ~GraphNode() {
        delete[] connections;
    }

    void addConnection(int index) {

        for (int i = 0; i < count; i++) {
            if (connections[i] == index) return;
        }

        if (count >= capacity) {
            int newCapacity = capacity * 2;
            int* newConnections = new int[newCapacity];                            //make new dynamic connection
            for (int i = 0; i < count; i++) {
                newConnections[i] = connections[i];
            }
            delete[] connections;
            connections = newConnections;
            capacity = newCapacity;
        }

        connections[count++] = index;
    }
};

class MovieGraph {
private:
    GraphNode* nodes;
    MovieNode** allMovies;
    int nodeCount;
    int maxNodes;

    int findMovieIndex(const string& title) {
        string searchTitle = toLowerString(cleanString(trim(title)));
        for (int i = 0; i < nodeCount; i++) {
            if (toLowerString(nodes[i].cleanTitle) == searchTitle) {
                return i;
            }
        }
        return -1;
    }

    bool commonactor(MovieNode* movie1, MovieNode* movie2) {
        if (!movie1->actor1.empty() && !movie2->actor1.empty()) {
            if (toLowerString(movie1->actor1) == toLowerString(movie2->actor1) ||
                toLowerString(movie1->actor1) == toLowerString(movie2->actor2) ||
                toLowerString(movie1->actor2) == toLowerString(movie2->actor1) ||
                toLowerString(movie1->actor2) == toLowerString(movie2->actor2)) {
                return true;
            }
        }
        return false;
    }

    bool commongenre(MovieNode* movie1, MovieNode* movie2) {
        if (movie1->genre.empty() || movie2->genre.empty()) return false;

        int first = 0;
        string genres1[10];
        string current1 = "";
        for (int i = 0; i < movie1->genre.length() && first < 10; i++) {
            if (movie1->genre[i] == '|') {
                if (!current1.empty()) {
                    genres1[first++] = toLowerString(trim(current1));
                }
                current1 = "";
            }
            else {                                     //chcking for common genres or actors to make connections
                //different linked lists/ graphs for both of the
                current1 += movie1->genre[i];
            }
        }
        if (!current1.empty() && first < 10) {
            genres1[first++] = toLowerString(trim(current1));
        }

        int second = 0;
        string genres2[10];
        string current2 = "";
        for (int i = 0; i < movie2->genre.length() && second < 10; i++) {
            if (movie2->genre[i] == '|') {
                if (!current2.empty()) {
                    genres2[second++] = toLowerString(trim(current2));
                }                                   //genre list as an array for commons
                current2 = "";
            }
            else {
                current2 += movie2->genre[i];
            }
        }
        if (!current2.empty() && second < 10) {
            genres2[second++] = toLowerString(trim(current2));
        }

        //common genres
        for (int i = 0; i < first; i++) {
            for (int j = 0; j < second; j++) {
                if (genres1[i] == genres2[j]) {
                    return true;
                }
            }
        }

        return false;
    }

public:
    MovieGraph(int maxSize) : nodeCount(0), maxNodes(maxSize) {
        nodes = new GraphNode[maxSize];
        allMovies = new MovieNode * [maxSize];
    }

    ~MovieGraph() {
        delete[] nodes;
        delete[] allMovies;
    }

    void buildGraph(MovieList& movieList) {
        //all movies
        int movieCount;
        allMovies = movieList.getarr(movieCount);
        nodeCount = movieCount;

        cout << "\nBuilding movie graph " << endl;

        // graph nodes
        for (int i = 0; i < nodeCount; i++) {
            nodes[i].movieTitle = allMovies[i]->title;
            nodes[i].cleanTitle = allMovies[i]->getCleanTitle();
        }
        //connections
        int count = 0;
        for (int i = 0; i < nodeCount; i++) {
            for (int j = i + 1; j < nodeCount; j++) {
                if (commonactor(allMovies[i], allMovies[j]) || commongenre(allMovies[i], allMovies[j])) {
                    nodes[i].addConnection(j);
                    nodes[j].addConnection(i);
                    count++;
                }
            }
        }

        cout << "Graph built with " << count << " connections." << endl;
    }

    void getRecommendations(const string& movieTitle, int maxResults) {
        string searchTitle = cleanString(trim(movieTitle));
        int start = findMovieIndex(searchTitle);

        if (start == -1) {
            if (toLowerString(searchTitle) == "avatar") {
                start = findMovieIndex("Avatar");
                if (start == -1) start = findMovieIndex("Avatar ");
            }
        }

        if (start == -1) {
            cout << "\nMovie not found: '" << movieTitle << "'" << endl;
            return;
        }

        bool* visited = new bool[nodeCount];
        int* queue = new int[nodeCount];
        int front = 0, rear = 0;
        int resultCount = 0;

        for (int i = 0; i < nodeCount; i++) visited[i] = false;

        visited[start] = true;
        queue[rear++] = start;

        cout << "recommendations for: " << nodes[start].movieTitle << endl;

        while (front < rear && resultCount < maxResults) {
            int current = queue[front++];

            if (current != start) {
                cout << "  " << ++resultCount << ". " << nodes[current].movieTitle << endl;
            }

            for (int i = 0; i < nodes[current].count; i++) {
                int neighbor = nodes[current].connections[i];
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue[rear++] = neighbor;
                }
            }
        }

        delete[] visited;
        delete[] queue;

        if (resultCount == 0) {
            cout << "No recommendations found." << endl;
        }
    }
};

class MovieManager {
private:
    MovieList movieList;
    ActorHashTable actorTable;
    DirectorHashTable directorTable;
    GenreHashTable genreTable;
    MovieGraph* movieGraph;
    findshortestpath* pathFinder;
    int total;

    void parseCSVLine(string line, MovieNode* movie) {
        int commaCount = 0;
        int start = 0;
        bool inQuotes = false;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '"') {
                inQuotes = !inQuotes;
            }
            else if (line[i] == ',' && !inQuotes) {
                commaCount++;
                string field = line.substr(start, i - start);
                start = i + 1;

                switch (commaCount) {
                case 1:
                    break;
                case 2:
                    movie->director = cleanString(trim(field));
                    break;
                case 3:
                    break;
                case 4:
                    movie->duration = stringToInt(field);
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    movie->actor2 = cleanString(trim(field));
                    break;
                case 8:
                    break;
                case 9:
                    break;
                case 10:
                    movie->genre = cleanString(trim(field));
                    break;
                case 11:
                    movie->actor1 = cleanString(trim(field));
                    break;
                case 12:
                    movie->title = cleanString(trim(field));
                    break;
                case 24:
                    movie->year = stringToInt(field);
                    break;
                case 26:
                    movie->rating = stringToDouble(field);
                    break;
                }
            }
        }


        string lastField = line.substr(start);
        if (commaCount == 25) {
            movie->rating = stringToDouble(lastField);
        }
    }

    void display(MovieNode* movie) {
        if (!movie) return;


        cout << endl << "Title:     " << movie->title << endl;
        cout << "Year:      " << movie->year << endl;
        cout << "Genre:     " << movie->genre << endl;
        cout << "Director:  " << movie->director << endl;
        cout << "Actors:    " << movie->actor1;
        if (!movie->actor2.empty()) {
            cout << ", " << movie->actor2;
        }
        cout << endl;
        cout << "Rating:    " << fixed << setprecision(1) << movie->rating << "/10" << endl;
        cout << "Duration:  " << movie->duration << " minutes" << endl;
    }

public:
    MovieManager() : total(0), movieGraph(nullptr), pathFinder(nullptr) {
        movieGraph = new MovieGraph(5000);
    }

    ~MovieManager() {
        delete movieGraph;
        delete pathFinder;
    }

    void loadDataset(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file '" << filename << "'" << endl;
            return;
        }

        string line;
        getline(file, line);

        int loaded = 0;
        int skipped = 0;
        while (getline(file, line) && loaded < 5000) { //
            if (line.empty()) continue;

            MovieNode* movie = new MovieNode();
            parseCSVLine(line, movie);

            if (!movie->title.empty() && movie->title.find("            ") == string::npos) {
                movieList.insert(movie);

                // Add to actor table
                if (!movie->actor1.empty()) {
                    actorTable.insert(movie->actor1, movie->title);
                }
                if (!movie->actor2.empty()) {
                    actorTable.insert(movie->actor2, movie->title);
                }

                // Add to director table
                if (!movie->director.empty()) {
                    directorTable.insert(movie->director, movie->title);
                }

                //insertion
                if (!movie->genre.empty()) {
                    genreTable.insert(movie->genre, movie->title);
                }

                loaded++;


            }
            //else {
              //  delete movie;
                //skipped++;
            //}
        }

        file.close();
        total = loaded;

        cout << "\nDataset loaded successfully!" << endl;
        cout << "Total movies: " << total << endl;


        // for graph
        movieGraph->buildGraph(movieList);

        // to fnid shortest path
        int movieCount;
        MovieNode** allMovies = movieList.getarr(movieCount);
        pathFinder = new findshortestpath(allMovies, movieCount);
        delete[] allMovies;


    }

    void searchByTitle() {
        string title;
        cout << "\nEnter movie title: ";
        getline(cin, title);



        // Clean the search input
        title = cleanString(trim(title));

        // Search for exact match first
        MovieNode* movie = movieList.search(title);

        if (movie) {
            display(movie);
            return;
        }

        // If still not found, search for partial matches
        cout << "\nSearching for movies containing: '" << title << "'" << endl;

        string searchTerm = toLowerString(title);
        MovieNode* current = movieList.getHead();
        int foundCount = 0;

        cout << "SEARCH RESULTS" << endl;

        while (current && foundCount < 20) {
            string currentTitle = toLowerString(current->getCleanTitle());
            if (currentTitle.find(searchTerm) != string::npos) {
                cout << "  " << ++foundCount << ". " << current->title
                    << " (" << current->year << ")" << endl;
            }
            current = current->next;
        }

        if (foundCount == 0) {
            cout << "No movies found with: '" << title << "'" << endl;
            cout << "\nTry these popular movies:" << endl;
            cout << "- Caroline" << endl;
            cout << "- Titanic (1997)" << endl;
            cout << "- The Avengers (2012)" << endl;
            cout << "- shrek (2015)" << endl;
            cout << "- The Dark Knight (2008)" << endl;
        }
        else if (foundCount == 1) {
            // If only one result found, show its details
            current = movieList.getHead();
            while (current) {
                string currentTitle = toLowerString(current->getCleanTitle());
                if (currentTitle.find(searchTerm) != string::npos) {
                    display(current);
                    break;
                }
                current = current->next;
            }
        }
    }

    void searchByActor() {
        string actor;
        cout << "\nEnter actor name: ";
        getline(cin, actor);

        if (actor.empty()) {
            cout << "Please enter an actor name." << endl;
            return;
        }

        // Clean  actor name
        actor = cleanString(trim(actor));

        // Try to search using the hash table 
        titlelist* movies = actorTable.search(actor);

        if (movies && movies->getSize() > 0) {
            cout << "MOVIES FEATURING: " << actor << " (" << movies->getSize() << " movies)" << endl;
            movies->display(10);
        }
        else {
            cout << "\nNo movies found for actor: " << actor << endl;

        }
    }
    //filtering by genres
    void searchByGenre() {
        string genre;
        cout << "\nEnter genre: ";
        getline(cin, genre);

        if (genre.empty()) {
            cout << "Please enter a genre." << endl;
            return;
        }

        titlelist* movies = genreTable.search(genre);
        if (movies && movies->getSize() > 0) {
            cout << "MOVIES IN GENRE: " << genre << endl;
            movies->display(20);
        }
        else {
            cout << "No movies found in genre: " << genre << endl;

        }
    }

    void searchByYear() {
        string yearInput;
        cout << "\nEnter year: ";
        getline(cin, yearInput);

        if (yearInput.empty()) {
            cout << "Please enter a year." << endl;
            return;
        }

        int year = stringToInt(yearInput);
        if (year < 1900 || year > 2100) {
            cout << "Please enter a valid year (1900-2100)." << endl;
            return;
        }

        // filtering by year
        int nomovies;
        MovieNode** allMovies = movieList.getarr(nomovies);

        cout << "movies from : " << year << endl;

        int count = 0;
        for (int i = 0; i < nomovies && count < 20; i++) {
            if (allMovies[i]->year == year) {
                cout << "  " << ++count << ". " << allMovies[i]->title
                    << " (Rating: " << allMovies[i]->rating << ")" << endl;
            }
        }

        if (count == 0) {
            cout << "No movies found for year " << year << endl;
        }
        else {
            cout << "\nTotal found: " << count << " movies" << endl;
        }

        delete[] allMovies;
    }

    void searchByRating() {
        string min, max;
        cout << "\nEnter minimum rating : ";
        getline(cin, min);
        cout << "Enter maximum rating: ";
        getline(cin, max);


        double minRating = stringToDouble(min);
        double maxRating = stringToDouble(max);

        // movies and then counting
        int movieCount;
        MovieNode** allMovies = movieList.getarr(movieCount);

        // Sotring by bubble
        for (int i = 0; i < movieCount - 1; i++) {
            for (int j = 0; j < movieCount - i - 1; j++) {
                if (allMovies[j]->rating < allMovies[j + 1]->rating) {
                    MovieNode* temp = allMovies[j];
                    allMovies[j] = allMovies[j + 1];
                    allMovies[j + 1] = temp;
                }
            }
        }

        cout << "MOVIES WITH RATING " << minRating << " - " << maxRating << endl;

        int count = 0;
        for (int i = 0; i < movieCount && count < 20; i++) {
            if (allMovies[i]->rating >= minRating && allMovies[i]->rating <= maxRating) {
                cout << "  " << ++count << ". " << allMovies[i]->title
                    << " (Rating: " << fixed << setprecision(1) << allMovies[i]->rating << ")" << endl;
            }
        }

        if (count == 0) {
            cout << "No movies found in this rating range." << endl;
        }
        else {
            cout << "\nTotal found: " << count << " movies" << endl;
        }

        delete[] allMovies;
    }

    void getRecommendations() {
        string movieTitle;
        cout << "\nEnter a movie you like: ";
        getline(cin, movieTitle);



        movieGraph->getRecommendations(movieTitle, 10);
    }

    void findShortestPath() {
        cout << "\n" << string(60, '=') << endl;
        cout << "Shortest Path" << endl;
        cout << "1. Find path between two movies" << endl;
        cout << "2. Find path between two actors" << endl;
        cout << "3. Find path between two directors" << endl;
        cout << "4. Back to main menu" << endl;
        cout << string(60, '=') << endl;
        cout << "Enter your choice (1-4): ";

        string choiceInput;
        getline(cin, choiceInput);
        int choice = stringToInt(choiceInput);

        switch (choice) {
        case 1: {
            string movie1, movie2;
            cout << "\nEnter first movie title: ";
            getline(cin, movie1);
            cout << "Enter second movie title: ";
            getline(cin, movie2);



            pathFinder->movietomovie(movie1, movie2, movieList);
            break;
        }
        case 2: {
            string actor1, actor2;
            cout << "\nEnter first actor name: ";
            getline(cin, actor1);
            cout << "Enter second actor name: ";
            getline(cin, actor2);


            pathFinder->findActorToActorPath(actor1, actor2, movieList, actorTable);
            break;
        }
        case 3: {
            string first, second;
            cout << "\nEnter first director name: ";
            getline(cin, first);
            cout << "Enter second director name: ";
            getline(cin, second);



            pathFinder->directortodirector(first, second, movieList);
            break;
        }
        case 4:
            return;
        default:
            cout << "Invalid choice." << endl;
        }
    }

    void displayAllMovies() {
        movieList.display(20);
    }

    void displayStatistics() {
        int movieCount;
        MovieNode** allMovies = movieList.getarr(movieCount);

        if (movieCount == 0) {
            cout << "No movies in database." << endl;
            delete[] allMovies;
            return;
        }

        // Calculate statistics
        double totalRating = 0;
        int oldest = 2100, newest = 1900;

        // Count genres
        const int MAX_GENRES = 50;
        string genres[MAX_GENRES];
        int genreCounts[MAX_GENRES] = { 0 };
        int genreIndex = 0;

        for (int i = 0; i < movieCount; i++) {
            totalRating += allMovies[i]->rating;

            if (allMovies[i]->year > 0) {
                if (allMovies[i]->year < oldest) oldest = allMovies[i]->year;
                if (allMovies[i]->year > newest) newest = allMovies[i]->year;
            }

            // iterate the names and count genres
            if (!allMovies[i]->genre.empty()) {
                string current = "";
                for (int j = 0; j < allMovies[i]->genre.length(); j++) {
                    if (allMovies[i]->genre[j] == '|') {
                        if (!current.empty()) {
                            // Add to genre counts
                            bool found = false;
                            for (int k = 0; k < genreIndex; k++) {
                                if (genres[k] == current) {
                                    genreCounts[k]++;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found && genreIndex < MAX_GENRES) {
                                genres[genreIndex] = current;
                                genreCounts[genreIndex]++;
                                genreIndex++;
                            }
                        }
                        current = "";
                    }
                    else {
                        current += allMovies[i]->genre[j];
                    }
                }


                if (!current.empty()) {
                    bool found = false;
                    for (int k = 0; k < genreIndex; k++) {
                        if (genres[k] == current) {
                            genreCounts[k]++;
                            found = true;
                            break;
                        }
                    }
                    if (!found && genreIndex < MAX_GENRES) {
                        genres[genreIndex] = current;
                        genreCounts[genreIndex]++;
                        genreIndex++;
                    }
                }
            }
        }

        double avgRating = totalRating / movieCount;

        // Find most common genre by calculating the count
        string topGenre = "N/A";
        int topGenreCount = 0;
        for (int i = 0; i < genreIndex; i++) {
            if (genreCounts[i] > topGenreCount) {
                topGenre = genres[i];
                topGenreCount = genreCounts[i];
            }
        }

        cout << "Stats:" << endl;
        cout << "Total Movies:      " << movieCount << endl;
        cout << "Year Range:        " << oldest << " - " << newest << endl;
        cout << "Average Rating:    " << fixed << setprecision(2) << avgRating << "/10" << endl;
        cout << "Most Common Genre: " << topGenre << " (" << topGenreCount << " movies)" << endl;
        cout << string(60, '=') << endl;

        // top 5
        cout << "\nTOP 5 RATED MOVIES:" << endl;
        cout << string(40, '-') << endl;

        // sorting
        for (int i = 0; i < min(5, movieCount); i++) {
            for (int j = i + 1; j < movieCount; j++) {
                if (allMovies[j]->rating > allMovies[i]->rating) {
                    MovieNode* temp = allMovies[i];
                    allMovies[i] = allMovies[j];
                    allMovies[j] = temp;
                }
            }
        }

        for (int i = 0; i < min(5, movieCount); i++) {
            cout << i + 1 << ". " << allMovies[i]->title << " (" << allMovies[i]->year
                << ") - Rating: " << fixed << setprecision(1) << allMovies[i]->rating << endl;
        }

        delete[] allMovies;
    }

    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "MOVIE DATABASE MANAGER" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Search movie by title" << endl;
        cout << "2. Search movies by actor" << endl;
        cout << "3. Search movies by genre" << endl;
        cout << "4. Search movies by year" << endl;
        cout << "5. Search movies by rating range" << endl;
        cout << "6. Get movie recommendations" << endl;
        cout << "7. Find shortest connection path" << endl;
        cout << "8. Display all movies" << endl;
        cout << "9. Display statistics" << endl;
        cout << "10. Exit" << endl;
        cout << string(60, '=') << endl;
        cout << "Enter your choice (1-10): ";
    }
};

int main() {
    MovieManager manager;

    cout << "\n" << string(60, '=') << endl;
    cout << "WELCOME TO MOVIE DATABASE MANAGER" << endl;
    cout << string(60, '=') << endl;

    manager.loadDataset("movie_metadata.csv");

    int choice = 0;
    string input;

    do {
        manager.displayMenu();

        // ipnut
        getline(cin, input);

        // Convert input to integer
        choice = 0;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] >= '0' && input[i] <= '9') {
                choice = choice * 10 + (input[i] - '0');
            }
        }
        cin.clear();

        switch (choice) {
        case 1:
            manager.searchByTitle();
            break;
        case 2:
            manager.searchByActor();
            break;
        case 3:
            manager.searchByGenre();
            break;
        case 4:
            manager.searchByYear();
            break;
        case 5:
            manager.searchByRating();
            break;
        case 6:
            manager.getRecommendations();
            break;
        case 7:
            manager.findShortestPath();
            break;
        case 8:
            manager.displayAllMovies();
            break;
        case 9:
            manager.displayStatistics();
            break;
        case 10:
            cout << "\nThank you for using Movie Database Manager!" << endl;
            break;
        default:
            cout << "\nInvalid choice. Please enter a number between 1 and 10." << endl;
        }

        if (choice != 10) {
            cout << "\nPress Enter to continue";
            getline(cin, input);
        }

    } while (choice != 10);

    return 0;
}