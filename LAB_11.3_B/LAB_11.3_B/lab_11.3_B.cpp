#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

enum SpecialtyB { CS, Inf, MathEcon, PhysInf, LaborTrain };
union ThirdGrade { double prog; double numMeth; double pedag; };

struct StudentB {
    char surname[30];
    int course;
    SpecialtyB spec;
    double phys;
    double math;
    ThirdGrade third;
};

void CreateFileB(const char* fname)
{
    ofstream f(fname, ios::binary);
    if (!f)
    {
        cout << "Cannot open file\n";
        return;
    }
    char c;
    do
    {
        StudentB s;
        cout << "Surname: ";
        cin >> s.surname;
        cout << "Course: ";
        cin >> s.course;
        int t;
        cout << "Specialty (0 - CS, 1 - Inf, 2 - ME, 3 - PI, 4 - LT): ";
        cin >> t;
        if (t < 0 || t > 4)
        {
            cout << "Invalid specialty\n";
            continue;
        }
        s.spec = static_cast<SpecialtyB>(t);
        cout << "Physics: ";
        cin >> s.phys;
        cout << "Math: ";
        cin >> s.math;
        if (s.spec == CS)
        {
            cout << "Programming grade: ";
            cin >> s.third.prog;
        }
        else if (s.spec == Inf)
        {
            cout << "Numeric methods grade: ";
            cin >> s.third.numMeth;
        }
        else
        {
            cout << "Pedagogy grade: ";
            cin >> s.third.pedag;
        }
        f.write((char*)&s, sizeof(s));
        cout << "Continue? (y/n): ";
        cin >> c;
    } while (c == 'y' || c == 'Y');
}

int RecordCountB(const char* fname)
{
    ifstream f(fname, ios::binary);
    if (!f)
    {
        return 0;
    }
    f.seekg(0, ios::end);
    int n = static_cast<int>(f.tellg()) / sizeof(StudentB);
    return n;
}

void ReadRecordB(fstream& f, int idx, StudentB& s)
{
    f.seekg(idx * sizeof(StudentB), ios::beg);
    f.read((char*)&s, sizeof(s));
}

void WriteRecordB(fstream& f, int idx, const StudentB& s)
{
    f.seekp(idx * sizeof(StudentB), ios::beg);
    f.write((char*)&s, sizeof(s));
}

double AverageB(const StudentB& s)
{
    double third;
    if (s.spec == CS)
    {
        third = s.third.prog;
    }
    else if (s.spec == Inf)
    {
        third = s.third.numMeth;
    }
    else
    {
        third = s.third.pedag;
    }
    return (s.phys + s.math + third) / 3.0;
}

int CompareB(const StudentB& a, const StudentB& b)
{
    double av = AverageB(a);
    double bv = AverageB(b);
    if (av > bv)
    {
        return 1;
    }
    else if (av < bv)
    {
        return -1;
    }
    double ath, bth;
    if (a.spec == CS)
    {
        ath = a.third.prog;
    }
    else if (a.spec == Inf)
    {
        ath = a.third.numMeth;
    }
    else
    {
        ath = a.third.pedag;
    }
    if (b.spec == CS)
    {
        bth = b.third.prog;
    }
    else if (b.spec == Inf)
    {
        bth = b.third.numMeth;
    }
    else
    {
        bth = b.third.pedag;
    }
    if (ath > bth)
    {
        return 1;
    }
    else if (ath < bth)
    {
        return -1;
    }
    int cmp = strcmp(a.surname, b.surname);
    if (cmp < 0)
    {
        return 1;
    }
    else if (cmp > 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void SortFileB(const char* fname)
{
    fstream f(fname, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cout << "Cannot open file for sorting\n";
        return;
    }
    int n = RecordCountB(fname);
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            StudentB s1;
            StudentB s2;
            ReadRecordB(f, j, s1);
            ReadRecordB(f, j + 1, s2);
            if (CompareB(s1, s2) < 0)
            {
                WriteRecordB(f, j, s2);
                WriteRecordB(f, j + 1, s1);
            }
        }
    }
}

void BuildIndexB(const char* dataFile, const char* idxFile)
{
    int n = RecordCountB(dataFile);
    if (n <= 0) return;

    fstream df(dataFile, ios::binary | ios::in | ios::out);
    if (!df)
    {
        cout << "Cannot open data file for indexing\n";
        return;
    }

    ofstream ix(idxFile, ios::binary | ios::trunc);
    if (!ix)
    {
        cout << "Cannot open index file for writing\n";
        df.close();
        return;
    }

    for (int range = n; range > 0; --range)
    {
        StudentB bestRec;
        ReadRecordB(df, 0, bestRec);
        int bestIdx = 0;

        for (int i = 1; i < range; ++i)
        {
            StudentB cur;
            ReadRecordB(df, i, cur);
            if (CompareB(cur, bestRec) > 0)
            {
                bestRec = cur;
                bestIdx = i;
            }
        }

        ix.write((char*)&bestIdx, sizeof(bestIdx));

        if (bestIdx != range - 1)
        {
            StudentB tail;
            ReadRecordB(df, range - 1, tail);

            WriteRecordB(df, bestIdx, tail);
            WriteRecordB(df, range - 1, bestRec);
        }
    }

    df.close();
    ix.close();
}


bool BinarySearchB(const char* fname, const StudentB& key)
{
    fstream f(fname, ios::binary | ios::in);
    if (!f)
    {
        cout << "Cannot open file for searching\n";
        return false;
    }
    int n = RecordCountB(fname);
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        StudentB cur;
        ReadRecordB(f, mid, cur);
        int cmp = CompareB(cur, key);
        if (cmp == 0)
        {
            return true;
        }
        else if (cmp > 0)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return false;
}

void DisplayB(const char* fname)
{
    ifstream f(fname, ios::binary);
    if (!f)
    {
        cout << "Cannot open file for display\n";
        return;
    }
    cout << left
        << setw(4) << "No"
        << setw(15) << "Surname"
        << setw(7) << "Course"
        << setw(12) << "Specialty"
        << setw(6) << "Phys"
        << setw(6) << "Math"
        << setw(8) << "Prog"
        << setw(12) << "NumMeth"
        << setw(10) << "Pedagogy"
        << "\n";
    StudentB s;
    int i = 1;
    const char* names[] = { "CS", "Inf", "MathEco", "PhysInf", "LabTrain" };
    while (f.read((char*)&s, sizeof(s)))
    {
        cout << setw(4) << i
            << setw(15) << s.surname
            << setw(7) << s.course
            << setw(12) << names[s.spec]
            << setw(6) << s.phys
            << setw(6) << s.math;
        if (s.spec == CS)
        {
            cout << setw(8) << s.third.prog
                << setw(12) << ""
                << setw(10) << "";
        }
        else if (s.spec == Inf)
        {
            cout << setw(8) << ""
                << setw(12) << s.third.numMeth
                << setw(10) << "";
        }
        else
        {
            cout << setw(8) << ""
                << setw(12) << ""
                << setw(10) << s.third.pedag;
        }
        cout << "\n";
        i++;
    }
}

void PrintIndexFile(const char* idxFile)
{
    ifstream f(idxFile, ios::binary);
    if (!f)
    {
        cout << "Cannot open index file\n";
        return;
    }
    cout << "Index file contents: ";
    int idx;
    while (f.read((char*)&idx, sizeof(idx)))
    {
        cout << idx << ' ';
    }
    cout << endl;
}

int main()
{
    char fname[100];
    char idxf[100];
    int choice = -1;
    do
    {
        cout << "\nMenu:\n"
            << "1 - Create file\n"
            << "2 - Display file\n"
            << "3 - Sort file\n"
            << "4 - Build index file\n"
            << "5 - Search record\n"
            << "0 - Exit\n"
            << "> ";
        cin >> choice;
        if (choice == 1)
        {
            cout << "Data file name: ";
            cin >> fname;
            CreateFileB(fname);
        }
        else if (choice == 2)
        {
            cout << "Data file name: ";
            cin >> fname;
            DisplayB(fname);
        }
        else if (choice == 3)
        {
            cout << "Data file name: ";
            cin >> fname;
            SortFileB(fname);
        }
        else if (choice == 4)
        {
            cout << "Data file name: ";
            cin >> fname;
            cout << "Index file name: ";
            cin >> idxf;
            BuildIndexB(fname, idxf);
            PrintIndexFile(idxf);
        }
        else if (choice == 5)
        {
            SortFileB(fname);
            cout << "Data file name: ";
            cin >> fname;
            StudentB key;
            cout << "Surname: ";
            cin >> key.surname;
            cout << "Course: ";
            cin >> key.course;
            int t;
            cout << "Specialty (0-CS..4-LT): ";
            cin >> t;
            key.spec = static_cast<SpecialtyB>(t);
            cout << "Physics: ";
            cin >> key.phys;
            cout << "Math: ";
            cin >> key.math;
            if (key.spec == CS)
            {
                cout << "Programming grade: ";
                cin >> key.third.prog;
            }
            else if (key.spec == Inf)
            {
                cout << "Numeric methods grade: ";
                cin >> key.third.numMeth;
            }
            else
            {
                cout << "Pedagogy grade: ";
                cin >> key.third.pedag;
            }
            bool found = BinarySearchB(fname, key);
            if (found)
            {
                cout << "Record found\n";
            }
            else
            {
                cout << "Record not found\n";
            }
        }
    } while (choice != 0);
    return 0;
}
