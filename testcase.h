// testcase.h
#ifndef __TESTCASE_H__
#define __TESTCASE_H__

#include "VectorStore.h"  // kéo theo main.h và utils.h (iostream, sstream, string, stdexcept, cmath)

// ===== Mini assertion helpers (không dùng thư viện ngoài) =====
class TestCase {
protected:
    int passed = 0, failed = 0, asserts = 0;

    void printLine(const std::string& s) { std::cout << s << std::endl; }

    void ok(const std::string& name)   { std::cout << "  [OK]   " << name << std::endl; }
    void ng(const std::string& name, const std::string& why) {
        std::cout << "  [FAIL] " << name << " -> " << why << std::endl;
    }

    void assertTrue(bool cond, const char* msg, const std::string& tn, const std::string& tc) {
        ++asserts;
        if (!cond) throw std::logic_error(std::string("ASSERT_TRUE: ") + (msg?msg:""));
        (void)tn; (void)tc;
    }
    template<typename T, typename U>
    void assertEqual(const T& a, const U& b, const char* msg, const std::string& tn, const std::string& tc) {
        ++asserts;
        if (!(a == b)) {
            std::stringstream ss;
            ss << "ASSERT_EQ: expected " << b << ", got " << a;
            if (msg) ss << " | " << msg;
            throw std::logic_error(ss.str());
        }
        (void)tn; (void)tc;
    }
    // Kiểm tra có ném ngoại lệ
    template<typename Fn>
    void assertThrows(Fn&& fn, const char* msg, const std::string& tn, const std::string& tc) {
        ++asserts;
        try {
            fn();
        } catch (const std::exception&) { (void)tn; (void)tc; return; }
        catch (const char*) { (void)tn; (void)tc; return; }
        catch (...) { (void)tn; (void)tc; return; }
        throw std::logic_error(std::string("ASSERT_THROWS: expected exception | ") + (msg?msg:""));
    }

    // Summary mỗi suite (kèm số testcase)
    void suiteSummary(const std::string& name, int tcCount) {
        std::cout << "== Result " << name << ": "
                  << passed << " passed, " << failed << " failed, "
                  << asserts << " assertions, "
                  << tcCount << " testcases ==" << std::endl << std::endl;
    }

public:
    virtual ~TestCase() = default;
    virtual void run() = 0;

    int getPassed()  const { return passed; }
    int getFailed()  const { return failed; }
    int getAsserts() const { return asserts; }
};

// ===== ArrayList Test Suite =====
class ArrayListTC : public TestCase {
public:
    void run() override {
        std::cout << "== Running ArrayListTC ==" << std::endl;
        int tc = 0;

        // (1) add/get/size
        try {
            ++tc;
            ArrayList<int> a(2);
            a.add(10); a.add(20);
            assertEqual(a.size(), 2, "size", "ArrayListTC", "tc1");
            assertEqual(a.get(0), 10, "get(0)", "ArrayListTC", "tc1");
            assertEqual(a.get(1), 20, "get(1)", "ArrayListTC", "tc1");
            ok("(1) ArrayList: add/get/size");
            ++passed;
        } catch (const std::exception& e) { ng("(1) ArrayList: add/get/size", e.what()); ++failed; }

        // (2) add(index) shift-right
        try {
            ++tc;
            ArrayList<int> a(2);
            a.add(10); a.add(30);
            a.add(1, 20); // -> 10,20,30
            assertEqual(a.size(), 3, 0, "ArrayListTC", "tc2");
            assertEqual(a.get(0), 10, 0, "ArrayListTC", "tc2");
            assertEqual(a.get(1), 20, 0, "ArrayListTC", "tc2");
            assertEqual(a.get(2), 30, 0, "ArrayListTC", "tc2");
            ok("(2) ArrayList: add(index) shift-right");
            ++passed;
        } catch (const std::exception& e) { ng("(2) ArrayList: add(index) shift-right", e.what()); ++failed; }

        // (3) removeAt shift-left
        try {
            ++tc;
            ArrayList<int> a(4);
            a.add(1); a.add(2); a.add(3); a.add(4);
            int v = a.removeAt(1); // remove 2 -> [1,3,4]
            assertEqual(v, 2, 0, "ArrayListTC", "tc3");
            assertEqual(a.size(), 3, 0, "ArrayListTC", "tc3");
            assertEqual(a.get(0), 1, 0, "ArrayListTC", "tc3");
            assertEqual(a.get(1), 3, 0, "ArrayListTC", "tc3");
            assertEqual(a.get(2), 4, 0, "ArrayListTC", "tc3");
            ok("(3) ArrayList: removeAt shift-left");
            ++passed;
        } catch (const std::exception& e) { ng("(3) ArrayList: removeAt shift-left", e.what()); ++failed; }

        // (4) iterator traversal
        try {
            ++tc;
            ArrayList<int> a(3); a.add(5); a.add(6); a.add(7);
            int sum = 0;
            for (auto it = a.begin(); it != a.end(); ++it) sum += *it;
            assertEqual(sum, 18, 0, "ArrayListTC", "tc4");
            ok("(4) ArrayList: iterator traversal");
            ++passed;
        } catch (const std::exception& e) { ng("(4) ArrayList: iterator traversal", e.what()); ++failed; }

        // (5) contains/indexOf
        try {
            ++tc;
            ArrayList<std::string> a(2);
            a.add("a"); a.add("b"); a.add("c");
            assertTrue(a.contains("a"), "contains a", "ArrayListTC", "tc5");
            assertTrue(!a.contains("x"), "not contains x", "ArrayListTC", "tc5");
            assertEqual(a.indexOf("b"), 1, 0, "ArrayListTC", "tc5");
            assertEqual(a.indexOf("x"), -1, 0, "ArrayListTC", "tc5");
            ok("(5) ArrayList: contains/indexOf");
            ++passed;
        } catch (const std::exception& e) { ng("(5) ArrayList: contains/indexOf", e.what()); ++failed; }

        // (6) clear() rồi dùng lại
        try {
            ++tc;
            ArrayList<int> a(1);
            a.add(9);
            a.clear();
            assertEqual(a.size(), 0, "after clear size=0", "ArrayListTC", "tc6");
            a.add(1); a.add(2);
            assertEqual(a.size(), 2, "reuse after clear", "ArrayListTC", "tc6");
            ok("(6) ArrayList: clear & reuse");
            ++passed;
        } catch (const std::exception& e) { ng("(6) ArrayList: clear & reuse", e.what()); ++failed; }

        // (7) copy constructor
        try {
            ++tc;
            ArrayList<int> a(2); a.add(1); a.add(2);
            ArrayList<int> b(a); // copy
            a.set(0, 99);
            assertEqual(b.get(0), 1, "copy unaffected", "ArrayListTC", "tc7");
            ok("(7) ArrayList: copy constructor");
            ++passed;
        } catch (const std::exception& e) { ng("(7) ArrayList: copy constructor", e.what()); ++failed; }

        // (8) assignment operator
        try {
            ++tc;
            ArrayList<int> a(2); a.add(4); a.add(5);
            ArrayList<int> b(1); b = a;
            a.set(1, 42);
            assertEqual(b.get(0), 4, 0, "ArrayListTC", "tc8");
            assertEqual(b.get(1), 5, 0, "ArrayListTC", "tc8");
            ok("(8) ArrayList: operator=");
            ++passed;
        } catch (const std::exception& e) { ng("(8) ArrayList: operator=", e.what()); ++failed; }

        // (9) exceptions: get / add(index) / removeAt out_of_range
        try {
            ++tc;
            ArrayList<int> a(1);
            a.add(7);
            assertThrows([&]{ (void)a.get(-1); }, "get(-1)", "ArrayListTC", "tc9");
            assertThrows([&]{ (void)a.get(1); }, "get(1)", "ArrayListTC", "tc9");
            assertThrows([&]{ a.add(3, 10); }, "add(3,10)", "ArrayListTC", "tc9");
            assertThrows([&]{ (void)a.removeAt(5); }, "removeAt(5)", "ArrayListTC", "tc9");
            ok("(9) ArrayList: exceptions");
            ++passed;
        } catch (const std::exception& e) { ng("(9) ArrayList: exceptions", e.what()); ++failed; }

        // (10) ensureCapacity gián tiếp: push nhiều phần tử
        try {
            ++tc;
            ArrayList<int> a(2);
            for (int i = 0; i < 100; ++i) a.add(i);
            assertEqual(a.size(), 100, "bulk add size=100", "ArrayListTC", "tc10");
            assertEqual(a.get(0), 0, 0, "ArrayListTC", "tc10");
            assertEqual(a.get(99), 99, 0, "ArrayListTC", "tc10");
            ok("(10) ArrayList: bulk add (ensureCapacity)");
            ++passed;
        } catch (const std::exception& e) { ng("(10) ArrayList: bulk add (ensureCapacity)", e.what()); ++failed; }

        // (11) iterator ++/-- và giới hạn
        try {
            ++tc;
            ArrayList<int> a(3); a.add(1); a.add(2); a.add(3);
            auto it = a.begin();
            assertEqual(*it, 1, 0, "ArrayListTC", "tc11");
            it++; // post++
            assertEqual(*it, 2, 0, "ArrayListTC", "tc11");
            ++it; // pre++
            assertEqual(*it, 3, 0, "ArrayListTC", "tc11");
            assertThrows([&]{ ++it; }, "advance past end throws", "ArrayListTC", "tc11");
            ok("(11) ArrayList: iterator ++ bounds");
            ++passed;
        } catch (const std::exception& e) { ng("(11) ArrayList: iterator ++ bounds", e.what()); ++failed; }

        suiteSummary("ArrayListTC", tc);
    }
};

// ===== SinglyLinkedList Test Suite =====
class SLinkedListTC : public TestCase {
public:
    void run() override {
        std::cout << "== Running SLinkedListTC ==" << std::endl;
        int tc = 0;

        // (1) add tail & iteration
        try {
            ++tc;
            SinglyLinkedList<int> s;
            s.add(1); s.add(2); s.add(3);
            int sum=0; for (auto it=s.begin(); it!=s.end(); ++it) sum += *it;
            assertEqual(sum, 6, 0, "SLinkedListTC", "tc1");
            assertEqual(s.size(), 3, 0, "SLinkedListTC", "tc1");
            ok("(1) SLL: add tail & iterate");
            ++passed;
        } catch (const std::exception& e) { ng("(1) SLL: add tail & iterate", e.what()); ++failed; }

        // (2) add_at_head & remove_first_item
        try {
            ++tc;
            SinglyLinkedList<int> s;
            s.add_at_head(2); s.add_at_head(1); // [1,2]
            assertEqual(s.size(), 2, 0, "SLinkedListTC", "tc2");
            assertEqual(s.get(0), 1, 0, "SLinkedListTC", "tc2");
            s.remove_first_item(); // [2]
            assertEqual(s.size(), 1, 0, "SLinkedListTC", "tc2");
            assertEqual(s.get(0), 2, 0, "SLinkedListTC", "tc2");
            ok("(2) SLL: add_at_head & remove_first_item");
            ++passed;
        } catch (const std::exception& e) { ng("(2) SLL: add_at_head & remove_first_item", e.what()); ++failed; }

        // (3) add(index) & removeAt
        try {
            ++tc;
            SinglyLinkedList<int> s;
            s.add(1); s.add(3);
            s.add(1, 2); // [1,2,3]
            assertEqual(s.size(), 3, 0, "SLinkedListTC", "tc3");
            assertEqual(s.get(1), 2, 0, "SLinkedListTC", "tc3");
            int x = s.removeAt(1); // remove 2 -> [1,3]
            assertEqual(x, 2, 0, "SLinkedListTC", "tc3");
            assertEqual(s.size(), 2, 0, "SLinkedListTC", "tc3");
            assertEqual(s.get(1), 3, 0, "SLinkedListTC", "tc3");
            ok("(3) SLL: add(index) & removeAt");
            ++passed;
        } catch (const std::exception& e) { ng("(3) SLL: add(index) & removeAt", e.what()); ++failed; }

        // (4) removeItem & contains
        try {
            ++tc;
            SinglyLinkedList<std::string> s;
            s.add("a"); s.add("b"); s.add("c");
            assertTrue(s.contains(std::string("b")), "contains b", "SLinkedListTC", "tc4");
            assertTrue(s.removeItem(std::string("b")), "remove b", "SLinkedListTC", "tc4");
            assertTrue(!s.contains(std::string("b")), "not contains b", "SLinkedListTC", "tc4");
            assertTrue(!s.removeItem(std::string("x")), "remove x false", "SLinkedListTC", "tc4");
            ok("(4) SLL: removeItem & contains");
            ++passed;
        } catch (const std::exception& e) { ng("(4) SLL: removeItem & contains", e.what()); ++failed; }

        // (5) exceptions & empty behaviors
        try {
            ++tc;
            SinglyLinkedList<int> s;
            assertThrows([&]{ s.remove_first_item(); }, "remove_first_item on empty", "SLinkedListTC", "tc5");
            assertThrows([&]{ (void)s.removeAt(0); }, "removeAt(0) on empty", "SLinkedListTC", "tc5");
            s.add(9);
            assertThrows([&]{ (void)s.get(1); }, "get(1) out of range", "SLinkedListTC", "tc5");
            s.clear();
            int sum=0; for (auto it=s.begin(); it!=s.end(); ++it) sum += *it; // duyệt 0 lần
            assertEqual(sum, 0, "iterate empty => sum 0", "SLinkedListTC", "tc5");
            ok("(5) SLL: exceptions & empty");
            ++passed;
        } catch (const std::exception& e) { ng("(5) SLL: exceptions & empty", e.what()); ++failed; }

        // (6) remove tail bằng removeItem + tính toàn vẹn thứ tự
        try {
            ++tc;
            SinglyLinkedList<int> s;
            s.add(1); s.add(2); s.add(3);
            assertTrue(s.removeItem(3), "remove tail=3", "SLinkedListTC", "tc6");
            assertEqual(s.size(), 2, 0, "SLinkedListTC", "tc6");
            assertEqual(s.get(0), 1, 0, "SLinkedListTC", "tc6");
            assertEqual(s.get(1), 2, 0, "SLinkedListTC", "tc6");
            ok("(6) SLL: remove tail & order intact");
            ++passed;
        } catch (const std::exception& e) { ng("(6) SLL: remove tail & order intact", e.what()); ++failed; }

        // (7) clear() rồi dùng lại
        try {
            ++tc;
            SinglyLinkedList<int> s;
            s.add(7); s.add(8);
            s.clear();
            assertEqual(s.size(), 0, "after clear size=0", "SLinkedListTC", "tc7");
            s.add(42);
            assertEqual(s.get(0), 42, 0, "SLinkedListTC", "tc7");
            ok("(7) SLL: clear & reuse");
            ++passed;
        } catch (const std::exception& e) { ng("(7) SLL: clear & reuse", e.what()); ++failed; }

        suiteSummary("SLinkedListTC", tc);
    }
};

// ===== VectorStore Test Suite =====
class VectorStoreTC : public TestCase {
    // Dummy embedding: tạo vector 4 chiều ổn định từ chuỗi
    static SinglyLinkedList<float>* dummyEmbedding(const std::string& s) {
        SinglyLinkedList<float>* v = new SinglyLinkedList<float>();
        float acc0=0, acc1=0, acc2=0, acc3=0;
        int i=0;
        for (size_t k=0; k<s.size(); ++k, ++i) {
            unsigned char c = static_cast<unsigned char>(s[k]);
            switch (i & 3) {
                case 0: acc0 += float(c); break;
                case 1: acc1 += float(c); break;
                case 2: acc2 += float(c); break;
                default: acc3 += float(c); break;
            }
        }
        v->add(acc0); v->add(acc1); v->add(acc2); v->add(acc3);
        return v;
    }
public:
    // HELPER FUNCTION - DO NOT TOUCH IF U DONT KNOW WHAT IT IS BABI
    static int& tc9_seen() { static int v = 0; return v; }
    static void tc9_cb(SinglyLinkedList<float>& vec, int id, std::string& raw) {
        (void)vec;
        if (!(id == 0 || id == 1)) throw std::logic_error("id must be 0 or 1");
        if (raw.size() != 1)      throw std::logic_error("raw len must be 1");
        ++tc9_seen();
    }
    // bộ đếm lượt visit (dùng static trong hàm để tránh global variable thật sự)
    static int& vs_tc_seen() { static int v = 0; return v; }

    // validate id & raw cho forEach
    static void vs_tc_forEach_validate(SinglyLinkedList<float>& vec, int id, std::string& raw) {
        (void)vec;
        (void)raw;
        if (!(id == 0 || id == 1)) throw std::logic_error("bad id in forEach");
    }

    // callback đếm lượt gọi
    static void vs_tc_forEach_count(SinglyLinkedList<float>& vec, int id, std::string& raw) {
        vs_tc_forEach_validate(vec, id, raw);
        ++vs_tc_seen();
    }
    // HELPER FUNCTION - DO NOT TOUCH IF U DONT KNOW WHAT IT IS BABI

    void run() override {
        std::cout << "== Running VectorStoreTC ==" << std::endl;
        int tc = 0;

        // (1) addText/size/getId/getVector basics
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("hello");
            store.addText("world");
            assertEqual(store.size(), 2, "size=2", "VectorStoreTC", "tc1");
            assertEqual(store.getId(0), 0, 0, "VectorStoreTC", "tc1");
            assertEqual(store.getId(1), 1, 0, "VectorStoreTC", "tc1");
            double l0 = store.vector_length(store.getVector(0));
            double l1 = store.vector_length(store.getVector(1));
            assertTrue(l0 > 0.0 && l1 > 0.0, "non-zero lengths", "VectorStoreTC", "tc1");
            ok("(1) VectorStore: addText/size/get");
            ++passed;
        } catch (const std::exception& e) { ng("(1) VectorStore: addText/size/get", e.what()); ++failed; }

        // (2) cosine nearest (minimize 1 - cos)
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("aaa");     // id=0
            store.addText("aaaa");    // id=1 (gần 0)
            store.addText("zzzzzz");  // id=2 (xa)
            SinglyLinkedList<float>* q = dummyEmbedding("aaa");
            int nn = store.findNearest(*q, "cosine"); // đang dùng độ đo càng thấp càng gần
            assertEqual(nn, 0, "nearest should be id 0", "VectorStoreTC", "tc2");
            delete q;
            ok("(2) VectorStore: cosine nearest");
            ++passed;
        } catch (const std::exception& e) { ng("(2) VectorStore: cosine nearest", e.what()); ++failed; }

        // (3) L1/L2 monotonicity
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("abc");     // id=0
            store.addText("abcX");    // id=1 (khác ít)
            store.addText("abcXXXX"); // id=2 (khác nhiều)

            const SinglyLinkedList<float>& v0 = store.getVector(0);
            const SinglyLinkedList<float>& v1 = store.getVector(1);
            const SinglyLinkedList<float>& v2 = store.getVector(2);

            double d01 = store.l2Distance(v0, v1);
            double d02 = store.l2Distance(v0, v2);
            assertTrue(d01 <= d02, "L2 monotonic", "VectorStoreTC", "tc3");

            double m01 = store.l1Distance(v0, v1);
            double m02 = store.l1Distance(v0, v2);
            assertTrue(m01 <= m02, "L1 monotonic", "VectorStoreTC", "tc3");

            ok("(3) VectorStore: L1/L2 distances");
            ++passed;
        } catch (const std::exception& e) { ng("(3) VectorStore: L1/L2 distances", e.what()); ++failed; }

        // (4) topKNearest (cosine)
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("x");      // 0
            store.addText("xx");     // 1
            store.addText("xxx");    // 2
            store.addText("xxxx");   // 3

            SinglyLinkedList<float>* q = dummyEmbedding("xxx");
            int k = 3;
            int* ids = store.topKNearest(*q, k, "cosine");
            assertEqual(ids[0], 2, "top-1 is id 2", "VectorStoreTC", "tc4");
            delete[] ids;
            delete q;
            ok("(4) VectorStore: topKNearest");
            ++passed;
        } catch (const std::exception& e) { ng("(4) VectorStore: topKNearest", e.what()); ++failed; }

        // (5) updateText & removeAt
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("a"); // 0
            store.addText("b"); // 1
            assertEqual(store.size(), 2, 0, "VectorStoreTC", "tc5");
            store.updateText(1, "aaaa");
            assertEqual(store.getRawText(1), std::string("aaaa"), 0, "VectorStoreTC", "tc5");
            bool okRemove = store.removeAt(0);
            assertTrue(okRemove, "removeAt(0) true", "VectorStoreTC", "tc5");
            assertEqual(store.size(), 1, 0, "VectorStoreTC", "tc5");
            assertEqual(store.getId(0), 1, "remaining id is 1", "VectorStoreTC", "tc5");
            ok("(5) VectorStore: updateText & removeAt");
            ++passed;
        } catch (const std::exception& e) { ng("(5) VectorStore: updateText & removeAt", e.what()); ++failed; }

        // (6) invalid metric throws
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("a"); store.addText("b");
            SinglyLinkedList<float>* q = dummyEmbedding("a");
            assertThrows([&]{ (void)store.findNearest(*q, "chebyshev"); }, "invalid metric", "VectorStoreTC", "tc6");
            delete q;
            ok("(6) VectorStore: invalid metric throws");
            ++passed;
        } catch (const std::exception& e) { ng("(6) VectorStore: invalid metric throws", e.what()); ++failed; }

        // (7) empty store behaviors
        try {
            ++tc;
            VectorStore emptyStore(4, dummyEmbedding);
            assertEqual(emptyStore.size(), 0, "empty size==0", "VectorStoreTC", "tc7");
            SinglyLinkedList<float>* q = dummyEmbedding("x");
            int id = emptyStore.findNearest(*q, "euclidean");
            assertEqual(id, -1, "nearest id on empty == -1", "VectorStoreTC", "tc7");
            delete q;
            ok("(7) VectorStore: empty store");
            ++passed;
        } catch (const std::exception& e) { ng("(7) VectorStore: empty store", e.what()); ++failed; }

        // (8) exceptions on bad indices
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("a");
            assertThrows([&]{ (void)store.getId(5); }, "getId out of range", "VectorStoreTC", "tc8");
            assertThrows([&]{ (void)store.getRawText(2); }, "getRawText out of range", "VectorStoreTC", "tc8");
            assertThrows([&]{ (void)store.getVector(-1); }, "getVector out of range", "VectorStoreTC", "tc8");
            assertThrows([&]{ (void)store.updateText(3, "x"); }, "updateText out of range", "VectorStoreTC", "tc8");
            assertThrows([&]{ (void)store.removeAt(9); }, "removeAt out of range", "VectorStoreTC", "tc8");
            ok("(8) VectorStore: index exceptions");
            ++passed;
        } catch (const std::exception& e) { ng("(8) VectorStore: index exceptions", e.what()); ++failed; }

        // (9) forEach callback được gọi đủ & thứ tự id hợp lý
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("p"); // id=0
            store.addText("q"); // id=1

            tc9_seen() = 0;                             // reset bộ đếm
            store.forEach(&tc9_cb);                     // truyền function pointer
            assertEqual(tc9_seen(), 2, "forEach visited 2", "VectorStoreTC", "tc9");

            ok("(9) VectorStore: forEach visit count");
            ++passed;
        } catch (const std::exception& e) {
            ng("(9) VectorStore: forEach visit count", e.what());
            ++failed;
        }

        // (10) topKNearest với k==size (bao phủ hết)
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("A"); // 0
            store.addText("AA"); // 1
            store.addText("AAA"); // 2
            SinglyLinkedList<float>* q = dummyEmbedding("AA");
            int k = store.size();
            int* ids = store.topKNearest(*q, k, "euclidean");
            // Không giả định thứ tự tuyệt đối, chỉ kiểm tra độ dài & tính hợp lệ id
            assertTrue(k == 3, "k==size==3", "VectorStoreTC", "tc10");
            assertTrue(ids[0] == 1 || ids[1] == 1 || ids[2] == 1, "id 1 xuất hiện", "VectorStoreTC", "tc10");
            delete[] ids; delete q;
            ok("(10) VectorStore: topK == size");
            ++passed;
        } catch (const std::exception& e) { ng("(10) VectorStore: topK == size", e.what()); ++failed; }
        // (11) removeAt ở giữa: id còn lại phải hợp lý
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("a"); // id=0
            store.addText("b"); // id=1
            store.addText("c"); // id=2
            bool okRm = store.removeAt(1); // bỏ id=1
            assertTrue(okRm, "removeAt(1) returns true", "VectorStoreTC", "tc11");
            assertEqual(store.size(), 2, "size==2 after remove", "VectorStoreTC", "tc11");
            assertEqual(store.getId(0), 0, "getId(0)==0", "VectorStoreTC", "tc11");
            assertEqual(store.getId(1), 2, "getId(1)==2", "VectorStoreTC", "tc11");
            ok("(11) VectorStore: removeAt middle keeps ids");
            ++passed;
        } catch (const std::exception& e) { ng("(11) VectorStore: removeAt middle keeps ids", e.what()); ++failed; }

        // (12) preprocessing: truncate (> dim) & pad (< dim)
        try {
            ++tc;
            // embed6: trả 6 chiều -> truncate khi dim=4
            VectorStore::EmbedFn embed6 = [](const std::string&)->SinglyLinkedList<float>* {
                auto* v = new SinglyLinkedList<float>();
                for (int i = 0; i < 6; ++i) v->add(float((i+1)*10));
                return v;
            };
            VectorStore sTrunc(4, embed6);
            sTrunc.addText("x");
            const auto& vt = sTrunc.getVector(0);
            assertEqual(vt.size(), 4, "truncate to 4", "VectorStoreTC", "tc12");

            // embed2: trả 2 chiều -> pad 0 đến dim=4
            VectorStore::EmbedFn embed2 = [](const std::string&)->SinglyLinkedList<float>* {
                auto* v = new SinglyLinkedList<float>();
                v->add(1.0f); v->add(2.0f);
                return v;
            };
            VectorStore sPad(4, embed2);
            sPad.addText("y");
            const auto& vp = sPad.getVector(0);
            assertEqual(vp.size(), 4, "pad to 4", "VectorStoreTC", "tc12");
            int idx = 0; float last2[2] = {1.f,1.f};
            for (auto it = vp.const_begin(); it != vp.const_end(); ++it, ++idx) {
                if (idx == 2) last2[0] = *it;
                if (idx == 3) last2[1] = *it;
            }
            assertTrue(last2[0] == 0.0f && last2[1] == 0.0f, "padded zeros", "VectorStoreTC", "tc12");

            ok("(12) VectorStore: preprocessing truncate & pad");
            ++passed;
        } catch (const std::exception& e) { ng("(12) VectorStore: preprocessing truncate & pad", e.what()); ++failed; }

        // (13) tie-breaking: vector trùng nhau -> trả id nhỏ hơn (đầu tiên)
        try {
            ++tc;
            VectorStore::EmbedFn embedConst = [](const std::string&)->SinglyLinkedList<float>* {
                auto* v = new SinglyLinkedList<float>();
                v->add(1.0f); v->add(0.0f); v->add(0.0f); v->add(0.0f);
                return v;
            };
            VectorStore store(4, embedConst);
            store.addText("p"); // id=0
            store.addText("q"); // id=1
            auto* q = embedConst("x");
            int id = store.findNearest(*q, "euclidean"); // cả hai cost=0
            assertEqual(id, 0, "first id chosen on tie", "VectorStoreTC", "tc13");
            delete q;
            ok("(13) VectorStore: tie-breaking keeps first");
            ++passed;
        } catch (const std::exception& e) { ng("(13) VectorStore: tie-breaking keeps first", e.what()); ++failed; }

        // (14) updateText thay đổi vector nhưng giữ kích thước đúng
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("short"); // id=0
            double len0 = store.vector_length(store.getVector(0));
            store.updateText(0, "a-much-longer-text");
            double len1 = store.vector_length(store.getVector(0));
            assertTrue(len1 != len0, "length changed after update", "VectorStoreTC", "tc14");
            assertEqual(store.getVector(0).size(), 4, "dim preserved", "VectorStoreTC", "tc14");
            ok("(14) VectorStore: updateText re-embeds");
            ++passed;
        } catch (const std::exception& e) { ng("(14) VectorStore: updateText re-embeds", e.what()); ++failed; }

        // (15) clear() rồi dùng lại
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("a"); store.addText("b");
            store.clear();
            assertEqual(store.size(), 0, "size=0 after clear", "VectorStoreTC", "tc15");
            store.addText("c");
            assertEqual(store.size(), 1, "size=1 after reuse", "VectorStoreTC", "tc15");
            assertEqual(store.getId(0), 0, "ids restart at 0", "VectorStoreTC", "tc15");
            ok("(15) VectorStore: clear & reuse");
            ++passed;
        } catch (const std::exception& e) { ng("(15) VectorStore: clear & reuse", e.what()); ++failed; }

        // (16) setEmbeddingFunction rồi updateText để áp dụng embed mới
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("seed"); // id=0
            double lenOld = store.vector_length(store.getVector(0));

            VectorStore::EmbedFn embedOnes = [](const std::string&)->SinglyLinkedList<float>*{
                auto* v = new SinglyLinkedList<float>();
                v->add(1); v->add(1); v->add(1); v->add(1);
                return v;
            };
            store.setEmbeddingFunction(embedOnes);
            store.updateText(0, "anything");
            double lenNew = store.vector_length(store.getVector(0));
            assertTrue(lenNew != lenOld, "length changed due to new embedFn", "VectorStoreTC", "tc16");
            ok("(16) VectorStore: setEmbeddingFunction + updateText");
            ++passed;
        } catch (const std::exception& e) { ng("(16) VectorStore: setEmbeddingFunction + updateText", e.what()); ++failed; }

        // (17) findNearest với metric 'manhattan'
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("aaaa");   // 0
            store.addText("aaaaaaaa"); // 1 xa hơn theo L1
            auto* q = dummyEmbedding("aaa");
            int id = store.findNearest(*q, "manhattan");
            assertEqual(id, 0, "L1 nearest is id 0", "VectorStoreTC", "tc17");
            delete q;
            ok("(17) VectorStore: L1 nearest");
            ++passed;
        } catch (const std::exception& e) { ng("(17) VectorStore: L1 nearest", e.what()); ++failed; }

        // (18) L2 đối xứng & zero self-distance
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("abc"); // 0
            const auto& v = store.getVector(0);
            double dSelf = store.l2Distance(v, v);
            assertEqual(dSelf, 0.0, "d(v,v)==0", "VectorStoreTC", "tc18");
            store.addText("abcd"); // 1
            const auto& u = store.getVector(1);
            double dUV = store.l2Distance(u, v);
            double dVU = store.l2Distance(v, u);
            assertEqual(dUV, dVU, "symmetry", "VectorStoreTC", "tc18");
            ok("(18) VectorStore: L2 symmetry & zero");
            ++passed;
        } catch (const std::exception& e) { ng("(18) VectorStore: L2 symmetry & zero", e.what()); ++failed; }

        // (19) Cosine bất biến theo scale (1 - cos == 0 nếu song song)
        try {
            ++tc;
            VectorStore::EmbedFn embedLen = [](const std::string& s)->SinglyLinkedList<float>*{
                auto* v = new SinglyLinkedList<float>();
                float L = float(s.size());
                v->add(L); v->add(0); v->add(0); v->add(0);
                return v;
            };
            VectorStore store(4, embedLen);
            store.addText("xx");      // id=0, vector [2,0,0,0]
            store.addText("xxxx");    // id=1, vector [4,0,0,0] (cùng hướng)
            auto* q = embedLen("xxxxxxxx"); // [8,0,0,0]
            int id = store.findNearest(*q, "cosine");
            // cả hai cos=1 => cost=0; tie -> id nhỏ hơn
            assertEqual(id, 0, "cosine tie -> first id", "VectorStoreTC", "tc19");
            delete q;
            ok("(19) VectorStore: cosine scale invariance");
            ++passed;
        } catch (const std::exception& e) { ng("(19) VectorStore: cosine scale invariance", e.what()); ++failed; }

        // (20) forEach với function pointer (không capture)
        try {
            ++tc;
            VectorStore store(4, dummyEmbedding);
            store.addText("p"); // 0
            store.addText("q"); // 1

            vs_tc_seen() = 0;  // reset đếm
            store.forEach(&vs_tc_forEach_count);  // truyền function pointer tự do
            assertEqual(vs_tc_seen(), 2, "visited 2", "VectorStoreTC", "tc20");

            ok("(20) VectorStore: forEach with function pointer");
            ++passed;
        } catch (const std::exception& e) {
            ng("(20) VectorStore: forEach with function pointer", e.what());
            ++failed;
        }
        suiteSummary("VectorStoreTC", tc);
    }
    
};

// ===== Runner tổng =====
inline void run_all_tests() {
    int TP=0, TF=0, TA=0;

    {
        ArrayListTC tc; tc.run();
        TP += tc.getPassed(); TF += tc.getFailed(); TA += tc.getAsserts();
    }
    {
        SLinkedListTC tc; tc.run();
        TP += tc.getPassed(); TF += tc.getFailed(); TA += tc.getAsserts();
    }
    {
        VectorStoreTC tc; tc.run();
        TP += tc.getPassed(); TF += tc.getFailed(); TA += tc.getAsserts();
    }

    std::cout << "==== SUMMARY: " << TP << " passed, "
              << TF << " failed, " << TA << " assertions ====" << std::endl;
}

#endif // __TESTCASE_H__
