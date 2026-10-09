#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector {
private:
    T* _data;
    int count;
    int cap;

    void rsz(int newCap) {
        T* newData = new T[newCap];
        for (int i = 0; i < count; i++) newData[i] = _data[i];
        delete[] _data;
        _data = newData;
        cap = newCap;
    }

public:
    Vector(int initialCap = 4) : _data(new T[initialCap > 0 ? initialCap : 4]), count(0), cap(initialCap > 0 ? initialCap : 4) {}
    ~Vector() { delete[] _data; }

    Vector(const Vector<T>& o) : _data(new T[o.cap]), count(o.count), cap(o.cap) {
        for (int i = 0; i < count; i++) _data[i] = o._data[i];
    }

    Vector<T>& operator=(const Vector<T>& o) {
        if (this == &o) return *this;
        delete[] _data;
        cap = o.cap;
        count = o.count;
        _data = new T[cap];
        for (int i = 0; i < count; i++) _data[i] = o._data[i];
        return *this;
    }

    void pshbk(const T& val) {
        if (count == cap) rsz(cap * 2);
        _data[count++] = val;
    }

    void ppbk() {
        if (count > 0) count--;
    }

    T& operator[](int i) { return _data[i]; }
    const T& operator[](int i) const { return _data[i]; }

    int sz() const { return count; }
    bool emp() const { return count == 0; }


    void clr() { count = 0; }

    void ers(int idx) {
        if (idx < 0 || idx >= count) return;
        for (int i = idx; i < count - 1; i++) {
            _data[i] = _data[i+1];
        }
        count--;
    }

    T* dtptr() { return _data; }
    const T* dtptr() const { return _data; }
};

#endif
