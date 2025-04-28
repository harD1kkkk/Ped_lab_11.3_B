#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <cstdio>
#include "../LAB_11.3_B/lab_11.3_B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestsB {
    TEST_CLASS(UnitTestB) {
public:
    TEST_METHOD(TestSortAndSearchB) {
        const char* f = "tB.bin";
        ofstream o(f, ios::binary);
        StudentB a[] = {
          {"A",1,CS,5,5,{5}},
          {"B",1,Inf,4,4,{6}},
          {"C",1,CS,5,4,{4}}
        };
        for (auto& s : a) o.write((char*)&s, sizeof(s));
        o.close();

        SortFileB(f);
        ifstream i(f, ios::binary);
        StudentB first; i.read((char*)&first, sizeof(first));
        Assert::AreEqual(string("A"), string(first.surname));

        StudentB key = { "B",1,Inf,4,4,{6} };
        Assert::IsTrue(BinarySearchB(f, key));
        remove(f);
    }

    TEST_METHOD(TestBuildIndexB) {
        const char* f = "tB2.bin", * ix = "iB2.bin";
        ofstream o(f, ios::binary);
        StudentB a[] = {
          {"X",1,Inf,4,4,{4}},
          {"Y",1,CS,5,5,{5}},
          {"Z",1,CS,3,3,{3}}
        };
        for (auto& s : a) o.write((char*)&s, sizeof(s));
        o.close();

        BuildIndexB(f, ix);
        ifstream in(ix, ios::binary);
        int idx; in.read((char*)&idx, sizeof(idx));
        Assert::AreEqual(1, idx);
        remove(f); remove(ix);
    }
    };
}
