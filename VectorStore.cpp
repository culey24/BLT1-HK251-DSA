#include "VectorStore.h"

// ----------------- ArrayList Implementation -----------------

template <class T>
ArrayList<T>::ArrayList(int initCapacity) {
    if (initCapacity < 0) initCapacity = 10;
    data = new T[initCapacity];
    capacity = initCapacity;
    count = 0;
}

template <class T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) {
    this->data = new T[other.capacity];
    this->capacity = other.capacity;
    this->count = other.count;
    for (int i = 0; i < this->count; i++) {
        this->data[i] = other.data[i];
    }
}   

template <class T>
ArrayList<T>::~ArrayList() {
    delete[] data;
}

template <class T>
void ArrayList<T>::ensureCapacity(int cap) {
    if (cap <= capacity) return;
    int new_capacity = capacity * 1.5;
    T* new_data = new T[new_capacity];
    for (int i = 0; i < count; i++) {
        new_data[i] = data[i];
    }
    delete[] data;
    capacity = new_capacity;
    data = new_data;
}

template <class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
    if (this == &other) return *this;
    delete[] this->data;
    this->data = new T[other.capacity];
    this->count = other.count;
    this->capacity = other.capacity;
    for (int i = 0; i < this->count; i++) {
        this->data[i] = other.data[i];
    }
    return *this;
}

template <class T>
void ArrayList<T>::add(T e) {
    ensureCapacity(count + 1);
    data[count] = e;
    count++;
    return;
}

template <class T>
void ArrayList<T>::add(int index, T e) {
    if (index < 0 || index > count) {
        throw out_of_range("Index is invalid!");
    }
    ensureCapacity(count + 1);
    T* new_data = new T[capacity];
    for (int i = 0; i < index; i++) {
        new_data[i] = data[i];
    }
    new_data[index] = e;
    for (int i = index; i < count; i++) {
        new_data[i + 1] = data[i];
    }
    delete[] data;
    data = new_data;
    count++;
    return;
} 

template <class T>
T ArrayList<T>::removeAt (int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    T* new_data = new T[capacity];
    for (int i = 0; i < index; i++) {
        new_data[i] = data[i];
    }
    T result = data[index];
    for (int i = index + 1; i < count; i++) {
        new_data[i - 1] = data[i];
    }
    delete[] data;
    data = new_data;
    count--;
    return result;
}

template <class T>
bool ArrayList<T>::empty() const {
    if (count == 0) return true;
    else return false;
}

template <class T>
int ArrayList<T>::size() const {
    return count;
}

template <class T>
void ArrayList<T>::clear() {
    delete[] data;
    data = new T[10];
    count = 0;
    capacity = 10;
    return;
}

template <class T>
T& ArrayList<T>::get(int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    return *(data + index);
}

template <class T>
T ArrayList<T>::get_copy(int index) const {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    return data[index];
}

template <class T>
T* ArrayList<T>::get_raw_data() {
    return data;
}

template <class T>
void ArrayList<T>::set(int index, T e) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    data[index] = e;
    return;
}

template <class T>
int ArrayList<T>::indexOf(T item) const {
    for (int i = 0; i < count; i++) {
        if (data[i] == item) return i;
    }
    return -1;
}

template <class T>
bool ArrayList<T>::contains(T item) const {
    if (indexOf(item) == -1) return false;
    else return true;
}

template <class T>
string ArrayList<T>::toString(string (*item2str)(T&)) const {
    stringstream ss;
    ss << '[';
    for (int i = 0; i < count; i++) {
        if (item2str) {
            ss << item2str(data[i]);
        }
        else {
            ss << data[i];
        }
        if (i != count - 1) ss << ", ";
    }
    ss << ']';
    return ss.str();
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::begin() {
    return Iterator(this, 0);
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::end() {
    return Iterator(this, this->count);
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::const_begin() const {
    return Iterator(const_cast<ArrayList<T>*>(this), 0);
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::const_end() const {
    return Iterator(const_cast<ArrayList<T>*>(this), this->count);
}
// ----------------- Iterator of ArrayList Implementation -----------------
template <class T>
ArrayList<T>::Iterator::Iterator(ArrayList<T>* pList, int index) {
    if (index < 0 || index > pList->count) {
        throw out_of_range("Iterator is out of range!");
    }
    this->pList = pList;
    this->cursor = index;
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator=(const Iterator& other) {
    if (this == &other) return *this;
    this->pList = other.pList;
    this->cursor = other.cursor;
    return *this;
}

template <class T>
T& ArrayList<T>::Iterator::operator*() {
    if (cursor < 0 || cursor >= pList->count) {
        throw out_of_range("Iterator is out of range!");
    }
    return this->pList->data[cursor];
}   

template <class T>
bool ArrayList<T>::Iterator::operator!=(const ArrayList<T>::Iterator &other) const {
    if ((this->pList != other.pList) || 
    (this->cursor != other.cursor)) return true;
    else return false;
}

template <class T>
typename ArrayList<T>::Iterator &ArrayList<T>::Iterator::operator++() {
    if (cursor == pList->count) throw out_of_range("Iterator cannot advance past end!");
    this->cursor++;
    return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
    if (cursor == pList->count) throw out_of_range("Iterator cannot advance past end!");
    Iterator previous_value = *this;
    ++*this;
    return previous_value;
}

template <class T>
typename ArrayList<T>::Iterator &ArrayList<T>::Iterator::operator--() {
    if (cursor == 0) throw out_of_range("Iterator cannot move before begin!");
    this->cursor--;
    return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
    if (cursor == 0) throw out_of_range("Iterator cannot move before begin!");
    Iterator previous_value = *this;
    --*this;
    return previous_value;

}

// ----------------- SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::SinglyLinkedList() {
    head = nullptr;
    tail = nullptr;
    count = 0;
}   

template <class T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    Node* cursor = head;
    while (cursor != nullptr) {
        Node* temp = cursor;
        cursor = cursor->next;
        delete temp;
    }
    count = 0;
}   

template <class T>
void SinglyLinkedList<T>::add(T e) {
    Node* new_node = new Node(e, nullptr);
    if (count == 0) {
        head = new_node;
        tail = new_node;
        count++;
        return;
    }
    tail->next = new_node;
    tail = new_node;
    count++;
}

template <class T> 
void SinglyLinkedList<T>::add_at_head(T e) {
    Node* new_node = new Node(e, nullptr);
    new_node->next = head;
    head = new_node;
    if (tail == nullptr) tail = new_node;
    count++;
}

template <class T>
void SinglyLinkedList<T>::add(int index, T e) {
    if (index < 0 || index > count) throw out_of_range("Index is invalid!");
    if (index == 0) {
        add_at_head(e);
        return;
    }
    if (index == count) {
        add(e);
        return;    
    }
    Node* new_node = new Node(e);
    Node* cursor = head;
    for (int i = 0; i < index - 1; i++) {
        cursor = cursor->next;
    }
    new_node->next = cursor->next;
    cursor->next = new_node;
    count++;
}

template <class T>
void SinglyLinkedList<T>::remove_first_item() {
    // DEBUG ===
    if (count == 0) throw out_of_range("EMPTY_LIST: CANNOT REMOVE HEAD OF EMPTY LIST");
    // DEBUG ===
    Node* temp = head;
    head = head->next;
    delete temp;
    count--;
    if (count == 0) tail = nullptr;
}

template <class T>
T SinglyLinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    if (index == 0) {
        T result = head->data;
        remove_first_item();
        return result;
    }
    Node* cursor = head;
    for (int i = 0; i < index - 1; i++) {
        cursor = cursor->next;
    }
    Node* temp = cursor->next;
    if (temp == tail) tail = cursor;
    T result = temp->data;
    cursor->next = cursor->next->next;
    delete temp;
    count--;
    return result;
}

template <class T>
bool SinglyLinkedList<T>::removeItem(T item) {
    if (head != nullptr && head->data == item) {
        remove_first_item();
        return true;
    }
    Node* cursor = head;
    while (cursor->next != nullptr) {
        if (cursor->next->data == item) {
            Node* temp = cursor->next;
            if (temp == tail) tail = cursor;
            cursor->next = temp->next;
            delete temp;
            count--;
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

template <class T>
bool SinglyLinkedList<T>::empty() const {
    return (count == 0);
}

template <class T>
int SinglyLinkedList<T>::size() const {
    return count;
}

template <class T>
void SinglyLinkedList<T>::clear() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}

template <class T>
T& SinglyLinkedList<T>::get(int index) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    Node* cursor = head;
    for (int i = 0; i < index; i++) {
        cursor = cursor->next;
    }    
    return cursor->data;
}

template <class T>
int SinglyLinkedList<T>::indexOf(T item) const {
    Node* cursor = head;
    for (int i = 0; i < count; i++) {
        if (cursor->data == item) return i;
        cursor = cursor->next;
    }
    return -1;
}

template <class T>
bool SinglyLinkedList<T>::contains(T item) const {
    for (Node* cursor = head; cursor != nullptr; cursor = cursor->next) {
        if (cursor->data == item) return true;
    }
    return false;
}

template <class T>
string SinglyLinkedList<T>::toString(string (*item2str)(T&)) const {
    stringstream ss;
    for (Node* cursor = head; cursor != nullptr; cursor = cursor->next) {
        if (item2str) ss << '[' << item2str(cursor->data) << ']';
        else ss << '[' << cursor->data << ']';
        if (cursor != tail) ss<<"->";
    }
    return ss.str();
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin() {
    return Iterator(head);
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end() {
    return Iterator(nullptr);
}

template<class T> 
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::const_begin() const {
    return Iterator(head);
}

template<class T> 
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::const_end() const {
    return Iterator(nullptr);
}

// ----------------- Iterator of SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::Iterator::Iterator(Node* node) {
    current = node;
}   

template <class T>
typename SinglyLinkedList<T>::Iterator& SinglyLinkedList<T>::Iterator::operator=(const Iterator& other) {
    if (this == &other) return *this;
    this->current = other.current;
    return *this;
}

template <class T>
T& SinglyLinkedList<T>::Iterator::operator*() {
    return this->current->data;
}

template <class T>
bool SinglyLinkedList<T>::Iterator::operator!=(const Iterator& other) const {
    return this->current != other.current;
}

template <class T>
typename SinglyLinkedList<T>::Iterator& SinglyLinkedList<T>::Iterator::operator++() {
    if (this->current == nullptr) throw out_of_range("Iterator cannot advance past end!");
    this->current = this->current->next;
    return *this;
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Iterator::operator++(int) {
    if (this->current == nullptr) throw out_of_range("Iterator cannot advance past end!");
    Iterator previous_value = *this;
    ++*this;
    return previous_value;
}


// ----------------- VectorRecord of VectorStore Implementation -----------------

VectorStore::VectorRecord::VectorRecord(int id, const std::string &rawText, SinglyLinkedList<float> *vector) {
    this->id = id;
    this->rawText = rawText;
    this->rawLength = rawText.size();
    this->vector = vector;
}

// ----------------- VectorStore Implementation -----------------

VectorStore::VectorStore(int dimension, EmbedFn embeddingFunction) {
    this->dimension = dimension;
    this->count = 0;
    this->embeddingFunction = embeddingFunction;
}

VectorStore::~VectorStore() {
    clear();
}

int VectorStore::size() const {
    return count;
}

bool VectorStore::empty() const {
    return (count == 0);
}

void VectorStore::clear() {
    for (int i = 0; i < records.size(); i++) {
        VectorRecord* record = records.get(i);
        if (record != nullptr) {
            delete record->vector;
            delete record;
        }
    }
    records.clear();
    count = 0;
}

SinglyLinkedList<float>* VectorStore::preprocessing(string rawText) {
    if (!embeddingFunction) {
        return nullptr;
    }
    SinglyLinkedList<float>* preprocessed_vector = embeddingFunction(rawText);
    int new_dimension = preprocessed_vector->size();
    if (new_dimension == dimension) return preprocessed_vector;
    else if (new_dimension > dimension) {
        SinglyLinkedList<float>* adjusted_vector = new SinglyLinkedList<float>();
        int copied = 0;
        for (auto iterator = preprocessed_vector->begin(); iterator != preprocessed_vector->end() && copied < dimension; iterator++, copied++) {
            adjusted_vector->add(*iterator);
        }
        delete preprocessed_vector;
        return adjusted_vector;
    }
    else {
        for (int i = 0; i < dimension - new_dimension; i++) {
            preprocessed_vector->add(0);
        }
        return preprocessed_vector;
    }
}

void VectorStore::addText(string rawText) {
    SinglyLinkedList<float>* new_vector = preprocessing(rawText);
    VectorRecord* new_record = new VectorRecord(get_biggest_id() + 1, rawText, new_vector);
    records.add(new_record);
    count++;
}

SinglyLinkedList<float>& VectorStore::getVector(int index) {
    if (index < 0 || index >= records.size()) throw out_of_range("Index is invalid!");
    VectorRecord* record = records.get(index);
    return *record->vector;
}

string VectorStore::getRawText(int index) const {
    if (index < 0 || index >= records.size()) throw out_of_range("Index is invalid!");
    VectorRecord* record = records.get_copy(index); // FIXME
    return record->rawText;
}

int VectorStore::getId(int index) const {
    if (index < 0 || index >= records.size()) throw out_of_range("Index is invalid!");   
    VectorRecord* record = records.get_copy(index);
    return record->id;
}

bool VectorStore::removeAt(int index) {
    if (index < 0 || index >= records.size()) throw out_of_range("Index is invalid!");
    VectorRecord* record = records.get(index);
    delete record->vector;
    delete record;
    records.removeAt(index);    
    count--;
    return true;                                          
}

bool VectorStore::updateText(int index, string newRawText) {
    if (index < 0 || index >= records.size()) throw out_of_range("Index is invalid!");
    VectorRecord* record = records.get(index);
    record->rawText = newRawText;
    record->rawLength = newRawText.size();
    SinglyLinkedList<float>* new_vector = preprocessing(newRawText);
    delete record->vector;
    record->vector = new_vector;
    return true;
}

void VectorStore::setEmbeddingFunction(EmbedFn newEmbeddingFunction) {
    this->embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEach(void (*action)(SinglyLinkedList<float>&, int, string&)) {
    for (auto it = records.begin(); it != records.end(); it++) {
        VectorRecord* record = *it;
        action(*record->vector, record->rawLength, record->rawText);
    }
}

double VectorStore::vector_length(const SinglyLinkedList<float> &vector) const{
    double result = 0;
    for (auto it = vector.const_begin(); it != vector.const_end(); it++) {
        result += *it * *it;
    }
    result = sqrt(result);
    return result;
}

double VectorStore::cosineSimilarity(const SinglyLinkedList<float>& v1, const SinglyLinkedList<float>& v2) const {
    double v1_length = vector_length(v1);
    double v2_length = vector_length(v2);
    double result = 0;
    for (
    auto it1 = v1.const_begin(), it2 = v2.const_begin();
    it1 != v1.const_end() && it2 != v2.const_end();
    it1++, it2++
    ) {
        result += *it1 * *it2;
    }
    result = result / (v1_length * v2_length);
    return result;
}

double VectorStore::l1Distance(const SinglyLinkedList<float>& v1, const SinglyLinkedList<float>& v2) const {
    double result = 0;
    for (
        auto it1 = v1.const_begin(), it2 = v2.const_begin();
        it1 != v1.const_end() && it2 != v2.const_end();
        it1++, it2++
    ) {
        result += abs(*it1 - *it2);
    }
    return result;
}

double VectorStore::l2Distance(const SinglyLinkedList<float>& v1, const SinglyLinkedList<float>& v2) const {
    double result = 0;
    for (
        auto it1 = v1.const_begin(), it2 = v2.const_begin();
        it1 != v1.const_end() && it2 != v2.const_end();
        it1++, it2++
    ) {
        result += pow(*it1 - *it2, 2);
    }
    result = sqrt(result);
    return result;
}    

int VectorStore::findNearest(const SinglyLinkedList<float>& query, const string& metric) const {
    if (!(metric == "cosine" || metric == "euclidean" || metric == "manhattan")) throw invalid_metric();
    double min_cost = __INT_MAX__;
    int result_id = -1;
    for (auto it = records.const_begin(); it != records.const_end(); it++) {
        VectorRecord* record = *it;
        double cost = __INT_MAX__;
        if (metric == "cosine") {
            cost = cosineSimilarity(*record->vector, query); 
            cost = 1 - cost;  
        }
        else if (metric == "euclidean") {
            cost = l2Distance(*record->vector, query);
        }
        else {
            cost = l1Distance(*record->vector, query);
        }
        if (cost < min_cost) {
            min_cost = cost;
            result_id = record->id;
        }
    }
    return result_id;
}

int *VectorStore::topKNearest(const SinglyLinkedList<float> &query, int k, const std::string &metric) const {
    if (!(metric == "cosine" || metric == "euclidean" || metric == "manhattan")) throw invalid_metric();
    if (k <= 0) throw invalid_k_value();
    ArrayList<RankedItem> ranking_board;
    for (auto it = records.const_begin(); it != records.const_end(); it++) {
        VectorRecord* record = *it;
        double cost = __INT_MAX__;
        if (metric == "cosine") {
            cost = cosineSimilarity(*record->vector, query);    
            cost = 1 - cost;
        }
        else if (metric == "euclidean") {
            cost = l2Distance(*record->vector, query);
        }
        else {
            cost = l1Distance(*record->vector, query);
        }
        RankedItem item = RankedItem(record->id, cost);
        ranking_board.add(item);
    }

    Sorter<RankedItem>* item_sorter = new Sorter<RankedItem>(ranking_board.get_raw_data(), ranking_board.size());
    item_sorter->sort();
    delete item_sorter;
    int* result = new int[k];
    for (int i = 0; i < k; i++) {
        RankedItem ranked_item = ranking_board.get(i);
        result[i] = ranked_item.id;
    }
    return result;
}

int VectorStore::get_biggest_id() const {
    if (records.empty()) return 0;
	VectorRecord* record = records.get_copy(count - 1);
	return record->id;
}
// RANKEDITEM IMPLEMENT

VectorStore::RankedItem::RankedItem(int id, double score) : id(id), score(score) {}

VectorStore::RankedItem::RankedItem() {
    this->id = -1;
    this->score = 0;
}

bool VectorStore::RankedItem::operator<(const RankedItem& other) const {
    if (this->score < other.score) return true;
    else if (this->score == other.score) {
        if (this->id < other.id) return true;
        else return false;
    }
    else return false;
}

// SORTeR CLASS IMPLEMENT
template <class T>
Sorter<T>::Sorter(T* array, int size) {
    this->array = array;
    this->size = size;
}

template <class T>
void Sorter<T>::quick_sort(int left_index, int right_index) {
    T thresh = array[(left_index + right_index)/2];
    int i = left_index;
    int j = right_index;
    while (i < j) {
        while (array[i] < thresh) {
            i++;
        }
        while (thresh < array[j]) {
            j--;
        }
        if (i <= j) {
            swap(array[i], array[j]);
            i++;
            j--;
        }   
    }
    if (i < right_index) {
        quick_sort(i, right_index);
    }
    if (left_index < j) {
        quick_sort(left_index, j);
    }
}

template <class T>
void Sorter<T>::sort() {
    quick_sort(0, size - 1);
}

// Explicit template instantiation for char, string, int, double, float, and Point

template class ArrayList<char>;
template class ArrayList<string>;
template class ArrayList<int>;
template class ArrayList<double>;
template class ArrayList<float>;                                                                                        
template class ArrayList<Point>;

template class SinglyLinkedList<char>;
template class SinglyLinkedList<string>;
template class SinglyLinkedList<int>;                                                       
template class SinglyLinkedList<double>;
template class SinglyLinkedList<float>;                                                                                         
template class SinglyLinkedList<Point>;

// TEMPLE OS
