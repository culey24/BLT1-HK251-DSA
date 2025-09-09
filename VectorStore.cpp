#include "VectorStore.h"

// ----------------- ArrayList Implementation -----------------

template <class T>
ArrayList<T>::ArrayList(int initCapacity = 10) {
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
    int new_capacity = ceil(capacity * 1.5);
    T* new_data = new T[capacity];
    for (int i = 0; i < capacity; i++) {
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
    for (int i = index; i < count; i++) {
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
string ArrayList<T>::toString(string (*item2str)(T&) = 0) const {
    if (!item2str) return "";
    stringstream ss;
    ss << '['
    for (int i = 0; i < count; i++) {
        ss << item2str(data[i]);
        if (i != count - 1) ss << ", ";
    }
    ss << ']'
    return ss.str();
}

template <class T>
ArrayList<T>::Iterator ArrayList<T>::begin() {
    return Iterator(this, 0);
}

template <class T>
ArrayList<T>::Iterator ArrayList<T>::end() {
    return Iterator(this, this->count);
}

// ----------------- Iterator of ArrayList Implementation -----------------
template <class T>
ArrayList<T>::Iterator::Iterator(ArrayList<T>* pList, int index) {
    if (index < 0 || index >= pList->count) {
        throw out_of_range("Iterator is out of range!");
    }
    this->pList = pList;
    this->cursor = index;
}

template <class T>
ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator=(const Iterator& other) {
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
ArrayList<T>::Iterator &ArrayList<T>::Iterator::operator++() {
    if (cursor == count) throw out_of_range("Iterator cannot advance past end!");
    this->cursor++;
    return *this;
}

template <class T>
ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
    if (cursor == count) throw out_of_range("Iterator cannot advance past end!");
    Iterator previous_value = *this;
    ++*this;
    return previous_value;
}

template <class T>
ArrayList<T>::Iterator &ArrayList<T>::Iterator::operator--() {
    if (cursor == 0) throw out_of_range("Iterator cannot move before begin!");
    this->cursor--;
    return *this;
}

template <class T>
ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
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
}   

template <class T>
void SinglyLinkedList<T>::add(T e) {
    Node* new_node = new Node(e, nullptr);
    tail->next = new_node;
    tail = new_node;
}

template <class T>
void SinglyLinkedList<T>::add(int index, T e) {
    if (index < 0 || index >= count) throw out_of_range("Index is invalid!");
    Node* new_node = new Node(e);
    
}

// ----------------- Iterator of SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::Iterator::Iterator(Node* node) {
    // TODO
}   

// TODO: implement other methods of SinglyLinkedList::Iterator



// ----------------- VectorStore Implementation -----------------

VectorStore::VectorStore(int dimension = 512, EmbedFn embeddingFunction = nullptr) {
    // TODO
}

VectorStore::~VectorStore() {
    // TODO
}

// TODO: implement other methods of VectorStore


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
